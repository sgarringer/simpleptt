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
#include "remote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsimplepttDlg dialog



CsimplepttDlg::CsimplepttDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsimplepttDlg::IDD, pParent)
{
}

void CsimplepttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, PTTButton);
	DDX_Control(pDX, IDC_EDIT1, EditBoxServer);
	DDX_Control(pDX, IDC_EDIT2, EditBoxPort);
	DDX_Control(pDX, IDC_COMBO1, ComboBoxPort);
	DDX_Control(pDX, IDC_COMBO2, ComboBoxPin);
	DDX_Control(pDX, IDC_CHECK2, ChkBoxEnable);
	DDX_Control(pDX, IDC_EDIT3, EditBoxPass);
}

BEGIN_MESSAGE_MAP(CsimplepttDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_MESSAGE( MYWM_NOTIFYICON, onTrayNotify )
	ON_COMMAND( ID__SHOW, onSysTrayMenuShow )
	ON_COMMAND( ID__CLOSE, onSysTrayMenuClose )
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
END_MESSAGE_MAP()


// CsimplepttDlg message handlers

void CsimplepttDlg::OnClose()
{
	PostMessage( WM_SIZE, SIZE_MINIMIZED, 0 );
}

void CsimplepttDlg::OnSize( UINT nType, int cx, int cy )
{
	if( nType == SIZE_MINIMIZED )
	{
		Shell_NotifyIcon( NIM_ADD, &m_hSysTrayIcon );
		ShowWindow( SW_HIDE );
	}
}

BOOL CsimplepttDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString sTip(_T("simpleptt"));
	m_hSysTrayIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_hSysTrayIcon.hWnd = this->GetSafeHwnd();;
	m_hSysTrayIcon.uID = IDI_ICON1;
	m_hSysTrayIcon.uFlags = NIF_MESSAGE|NIF_ICON;
	m_hSysTrayIcon.uCallbackMessage = MYWM_NOTIFYICON;
	m_hSysTrayIcon.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP; 
	m_hSysTrayIcon.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON1));
	lstrcpyn( m_hSysTrayIcon.szTip, (LPCTSTR)sTip, sizeof(m_hSysTrayIcon.szTip) );

	m_trayMenu.LoadMenu( IDR_MENU1 );
	Shell_NotifyIcon( NIM_ADD, &m_hSysTrayIcon );

	ShowWindow( SW_HIDE );

	m_iniFile.SetPath( "simpleptt.ini" );
	m_iniFile.ReadFile();
	m_sPort = m_iniFile.GetValue( "simpleptt", "comport", "COM1" ).c_str();
	m_nPTTPin = m_iniFile.GetValueI( "simpleptt", "pttpin", 0 );

	if( CSerial::CheckPort( "COM1" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM1" );
	if( CSerial::CheckPort( "COM2" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM2" );
	if( CSerial::CheckPort( "COM3" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM3" );
	if( CSerial::CheckPort( "COM4" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM4" );
	if( CSerial::CheckPort( "COM5" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM5" );
	if( CSerial::CheckPort( "COM6" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM6" );
	if( CSerial::CheckPort( "COM7" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM7" );
	if( CSerial::CheckPort( "COM8" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM8" );
	if( CSerial::CheckPort( "COM9" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM9" );
	if( CSerial::CheckPort( "COM10" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "COM10" );
	if( CSerial::CheckPort( "\\\\.\\CNCA0" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "CNCA0" );
	if( CSerial::CheckPort( "\\\\.\\CNCB0" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "CNCB0" );
	if( CSerial::CheckPort( "\\\\.\\CNCA1" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "CNCA1" );
	if( CSerial::CheckPort( "\\\\.\\CNCB1" ) == CSerial::EPortAvailable )
		ComboBoxPort.AddString( "CNCB1" );
	ComboBoxPort.AddString( "Remote" );
	ComboBoxPort.SetCurSel( ComboBoxPort.FindString( 0, m_sPort ) );

	ComboBoxPin.AddString( "DTR" );
	ComboBoxPin.AddString( "RTS" );
	ComboBoxPin.AddString( "DTR+RTS" );
	ComboBoxPin.SetCurSel( m_nPTTPin );

	PTTButton.EnableWindow( false );

	EditBoxServer.SetWindowText( m_iniFile.GetValue( "simpleptt", "host", "localhost" ).c_str() );
	EditBoxPort.SetWindowText( m_iniFile.GetValue( "simpleptt", "port", "5454" ).c_str() );
	EditBoxPass.SetWindowText( m_iniFile.GetValue( "simpleptt", "pass", "ptt2dtr" ).c_str() );

	if( m_sPort == "Remote" )
	{
		EditBoxPort.EnableWindow( true );
		EditBoxServer.EnableWindow( true );
		ComboBoxPin.EnableWindow( false );
	}
	else
	{
		EditBoxServer.EnableWindow( false );
		EditBoxPort.EnableWindow( false );
		ComboBoxPin.EnableWindow( true );
	}

	m_hRemoteThread = INVALID_HANDLE_VALUE;
	toggleEnable( m_iniFile.GetValueI( "simpleptt", "enable", 0 ) );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CsimplepttDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CsimplepttDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsimplepttDlg::onSysTrayMenuClose()
{
	// saving settings
	m_iniFile.SetValue( "simpleptt", "comport", (LPCSTR)m_sPort );
	m_iniFile.SetValueI( "simpleptt", "pttpin", m_nPTTPin );
	m_iniFile.SetValueI( "simpleptt", "enable", ChkBoxEnable.GetCheck() );
	CString tmp;
	EditBoxServer.GetWindowText( tmp );
	m_iniFile.SetValue( "simpleptt", "host", (LPCTSTR)tmp );
	EditBoxPort.GetWindowText( tmp );
	m_iniFile.SetValue( "simpleptt", "port", (LPCTSTR)tmp );
	EditBoxPass.GetWindowText( tmp );
	m_iniFile.SetValue( "simpleptt", "pass", (LPCTSTR)tmp );
	m_iniFile.WriteFile();

	Shell_NotifyIcon( NIM_DELETE, &m_hSysTrayIcon );
	DestroyWindow();
}

void CsimplepttDlg::onSysTrayMenuShow()
{
	onTrayNotify( 0, WM_LBUTTONDBLCLK );
}

LRESULT CsimplepttDlg::onTrayNotify( WPARAM wParam, LPARAM lParam )
{
	if( lParam == WM_LBUTTONDBLCLK )
	{
		Shell_NotifyIcon( NIM_DELETE, &m_hSysTrayIcon );
		ShowWindow( SW_NORMAL );
		SetForegroundWindow();
		SetFocus();
	}

	if( lParam == WM_RBUTTONUP )
	{
		CPoint pt;
		GetCursorPos( &pt );
		m_trayMenu.GetSubMenu(0)->TrackPopupMenu( TPM_RIGHTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, this );
	} 

	return TRUE;
}

int CsimplepttDlg::ShowError( LONG lError, LPCTSTR lptszMessage )
{
	// Generate a message text
	TCHAR tszMessage[256];
	wsprintf(tszMessage,_T("%s\n(error code %d)"), lptszMessage, lError);

	// Display message-box and return with an error-code
	::MessageBox( 0, tszMessage, "Error", MB_ICONERROR|MB_OK );
	return 1;
}

bool endServerThread = false;
int servPort = 0;
CString servHost;
CString servPass;
bool sendPTTON = false;
bool sendPTTOFF = false;

void CsimplepttDlg::toggleEnable( bool fState )
{
	if( fState )
	{
		m_serialPort.Close();

		if( m_sPort == "Remote" )
		{
			ChkBoxEnable.SetCheck( false );
			if( m_hRemoteThread != INVALID_HANDLE_VALUE )
				return;
			EditBoxServer.GetWindowText( servHost );
			CString tmp;
			EditBoxPort.GetWindowText( tmp );
			servPort = atoi( (LPCTSTR)tmp );
			EditBoxPass.GetWindowText( servPass );
			PTTButton.SetCheck( false );

			m_hRemoteThread = AfxBeginThread( remoteThread, 0 );
		}
		else
		{
			LONG lLastError = m_serialPort.Open( "\\\\.\\" + m_sPort, this->m_hWnd, WU_EVENT_SERIALPORT );
			if ( lLastError != ERROR_SUCCESS )
			{
				ShowError( m_serialPort.GetLastError(), _T("Unable to open COM port") );
				ChkBoxEnable.SetCheck( 0 );
				return;
			}
			lLastError = m_serialPort.Setup( CSerial::EBaud9600, CSerial::EData8, CSerial::EParNone, CSerial::EStop1 );
			if (lLastError != ERROR_SUCCESS)
			{
				ShowError( m_serialPort.GetLastError(), _T("Unable to set COM port setting") );
				ChkBoxEnable.SetCheck( 0 );
				return;
			}
			lLastError = m_serialPort.SetupHandshaking( CSerial::EHandshakeOff );
			if (lLastError != ERROR_SUCCESS)
			{
				ShowError( m_serialPort.GetLastError(), _T("Unable to set COM port handshaking") );
				ChkBoxEnable.SetCheck( 0 );
				return;
			}

			m_nPTTPin = ComboBoxPin.GetCurSel();
			ComboBoxPin.EnableWindow( false );
			ComboBoxPort.EnableWindow( false );
			PTTButton.EnableWindow( true );
			ChkBoxEnable.SetCheck( true );
		}
	}
	else
	{
		if( m_hRemoteThread != INVALID_HANDLE_VALUE )
		{
			endServerThread = true;
			while( endServerThread ) { Sleep( 1 ); } // waiting for thread to exit
		}
		m_hRemoteThread = INVALID_HANDLE_VALUE;

		m_serialPort.Close();
		if( m_sPort != "Remote" )
		{
			ComboBoxPin.EnableWindow( true );
			EditBoxServer.EnableWindow( false );
			EditBoxPort.EnableWindow( false );
			EditBoxPass.EnableWindow( false );
		}
		else
		{
			EditBoxServer.EnableWindow( true );
			EditBoxPort.EnableWindow( true );
			EditBoxPass.EnableWindow( true );
		}
		ComboBoxPort.EnableWindow( true );
		PTTButton.EnableWindow( false );
		ChkBoxEnable.SetCheck( false );
		PTTButton.SetCheck( false );
	}
}

void CsimplepttDlg::OnBnClickedCheck2()
{
	toggleEnable( ChkBoxEnable.GetCheck() );
}

void CsimplepttDlg::OnCbnSelchangeCombo1()
{
	ComboBoxPort.GetLBText( ComboBoxPort.GetCurSel(), m_sPort );
	if( m_sPort == "Remote" )
	{
		EditBoxPort.EnableWindow( true );
		EditBoxServer.EnableWindow( true );
		EditBoxPass.EnableWindow( true );
		ComboBoxPin.EnableWindow( false );
	}
	else
	{
		EditBoxPort.EnableWindow( false );
		EditBoxServer.EnableWindow( false );
		EditBoxPass.EnableWindow( false );
		ComboBoxPin.EnableWindow( true );
	}
}

void CsimplepttDlg::OnCbnSelchangeCombo2()
{
	m_nPTTPin = ComboBoxPin.GetCurSel();
}

void CsimplepttDlg::OnBnClickedCheck1()
{
	togglePTT( PTTButton.GetCheck() );
}

void CsimplepttDlg::togglePTT( bool fState )
{
	if( m_sPort == "Remote" )
	{
		if( fState )
		{
			PTTButton.SetCheck( false );
			sendPTTON = true;
		}
		else
		{
			PTTButton.SetCheck( true );
			sendPTTOFF = true;
		}
		return;
	}

	if( fState )
	{
		PTTButton.SetCheck( false );
		switch( m_nPTTPin )
		{
			case 0:
				m_serialPort.SetDTR();
				break;
			case 1:
				m_serialPort.SetRTS();
				break;
			case 2:
				m_serialPort.SetRTS();
				m_serialPort.SetDTR();
		}

		if( ::GetLastError() == ERROR_SUCCESS )
		{
			PTTButton.SetCheck( true );
		}
	}
	else
	{
		PTTButton.SetCheck( false );
		m_serialPort.ClrDTR();
		m_serialPort.ClrRTS();
	}
}

void CsimplepttDlg::remoteStopped()
{
	m_hRemoteThread = INVALID_HANDLE_VALUE;
	toggleEnable( false );
}

void CsimplepttDlg::remoteConnected()
{
	ComboBoxPin.EnableWindow( false );
	ComboBoxPort.EnableWindow( false );
	PTTButton.EnableWindow( true );
	EditBoxServer.EnableWindow( false );
	EditBoxPort.EnableWindow( false );
	EditBoxPass.EnableWindow( false );
	ChkBoxEnable.SetCheck( true );
}

void CsimplepttDlg::remoteTogglePTT( bool fState )
{
	if( fState )
	{
		PTTButton.SetCheck( true );
	}
	else
	{
		PTTButton.SetCheck( false );
	}
}


BOOL CsimplepttDlg::PreTranslateMessage( MSG* pMsg )
{
	switch( pMsg->message ) {
		case WM_KEYUP: // PTT push with space
		case WM_KEYDOWN:
			if( ChkBoxEnable.GetCheck() )
			{
				if( pMsg->wParam == VK_SPACE )
				{
					if( pMsg->message == WM_KEYUP )
					{
						if( PTTButton.GetCheck() )
							togglePTT( 0 );
					}
					if( pMsg->message == WM_KEYDOWN )
					{
						if( !PTTButton.GetCheck() )
							togglePTT( 1 );
					}
					PostMessage( WM_PAINT, 0, 0 );
					return 1;
				}
			}
			PostMessage( WM_PAINT, 0, 0 );
	}
	return 0;
}
