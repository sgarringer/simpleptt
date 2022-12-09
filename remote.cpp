// This file is part of simpleptt.
//
// simpleptt is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// simpleptt is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with simpleptt; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"
#include "simpleptt.h"
#include "simplepttDlg.h"

extern CsimplepttDlg dlg;
extern bool endServerThread;
extern CString servHost;
extern CString servPass;
extern int servPort;
extern bool sendPTTON;
extern bool sendPTTOFF;

int readSckLine( SOCKET clientSocket, char* inBuf, int inBufLen, int& inpos )
{
	char in;
	int res;

	res = recv( clientSocket, &in, 1, 0 );
	if( res > 0 )
	{
		inBuf[inpos++] = in;
	}

	if( res == 0 ) // res == 0 -> client disconnected
		return -1;
	if( in == 10 || inpos == inBufLen - 1 )
	{
		inBuf[inpos] = 0;
		inpos = 0;
		return 1;
	}

	return 0;
}

UINT remoteThread( LPVOID pParam )
{
	SOCKET clientSocket;
	WSADATA wsaData;

	int wsaret = WSAStartup( 0x101, &wsaData );
	if( wsaret != 0 )
	{
		dlg.ShowError( wsaret, "WinSock init error" );
		dlg.remoteStopped();
		return 0;
	}

	clientSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( clientSocket == INVALID_SOCKET )
	{
		dlg.ShowError( servPort, "Can't open the given port!" );
		dlg.remoteStopped();
		return 0;
	}

	struct hostent* hp;
	if( inet_addr( (LPCTSTR)servHost ) == INADDR_NONE )
	{
		hp = gethostbyname( (LPCTSTR)servHost );
	}
	else
	{
		unsigned int addr = inet_addr( (LPCTSTR)servHost );
		hp = gethostbyaddr( (char*)&addr, sizeof(addr), AF_INET );
	}
	if( hp == NULL )
	{
		closesocket( clientSocket );
		MessageBox( 0, "Unable to resolve hostname!", 0, 0 );
		dlg.remoteStopped();
		return 0;
	}

	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons((u_short)servPort);
	local.sin_addr.s_addr = *((unsigned long*)hp->h_addr);

	if( connect( clientSocket, (struct sockaddr*)&local, sizeof(local) ) )
	{
		closesocket( clientSocket );
		MessageBox( 0, "Can't connect to the given server!", 0, 0 );
		dlg.remoteStopped();
		return 0;
	}

	char temp[50];
	sprintf( temp, "PASS %s\r\n", (LPCTSTR)servPass );
	if( send( clientSocket, temp, strlen(temp), 0 ) <= 0 )
	{
		closesocket( clientSocket );
		MessageBox( 0, "Can't connect to the given server!", 0, 0 );
		dlg.remoteStopped();
		return 0;
	}

	u_long nonBlock = 1;
	ioctlsocket( clientSocket, FIONBIO, &nonBlock );

	int res;
	bool loggedin = false;
	int tmppos = 0;

	while( !endServerThread )
	{
		Sleep( 1 );
		res = readSckLine( clientSocket, temp, sizeof(temp), tmppos );
		if( res == -1 )
		{
			dlg.remoteStopped();
			return 0;
		}

		if( res == 1 )
		{
			if( !loggedin )
			{
				if( strncmp( temp, "err\r\n", 5 ) == 0 )
				{
					closesocket( clientSocket );
					MessageBox( 0, "Invalid pass!", 0, 0 );
					dlg.remoteStopped();
					return 0;
				}
				if( strncmp( temp, "ok\r\n", 4 ) == 0 )
				{
					dlg.remoteConnected();
					loggedin = true;
					continue;
				}
			}
			else
			{
				if( strncmp( temp, "P0\r\n", 4 ) == 0 )
				{
					dlg.remoteTogglePTT( false );
					continue;
				}
				if( strncmp( temp, "P1\r\n", 4 ) == 0 )
				{
					dlg.remoteTogglePTT( true );
					continue;
				}
			}

			closesocket( clientSocket );
			CString errs;
			errs.Format( "Invalid server response: %s", temp );
			MessageBox( 0, errs, 0, 0 );
		}

		if( sendPTTON )
		{
			send( clientSocket, "P1\r\n", 4, 0 );
			sendPTTON = false;
		}
		if( sendPTTOFF )
		{
			send( clientSocket, "P0\r\n", 4, 0 );
			sendPTTOFF = false;
		}
	}

	closesocket( clientSocket );
	WSACleanup();
	endServerThread = false;
	return 0;
}
