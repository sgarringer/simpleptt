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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsimplepttApp

BEGIN_MESSAGE_MAP(CsimplepttApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CsimplepttApp construction

CsimplepttApp::CsimplepttApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CsimplepttApp object

CsimplepttApp theApp;
CsimplepttDlg dlg;


// CsimplepttApp initialization

BOOL CsimplepttApp::InitInstance()
{
	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.Create( IDD_SIMPLEPTT_DIALOG, 0 );
	//dlg.ShowWindow( SW_HIDE );
	dlg.ShowWindow(SW_SHOW);
	dlg.RunModalLoop( 0 );
	return FALSE;
}
