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

#pragma once
#include "afxwin.h"


// CsimplepttDlg dialog
class CsimplepttDlg : public CDialog
{
// Construction
public:
	CsimplepttDlg(CWnd* pParent = NULL);	// standard constructor
	int ShowError( LONG lError, LPCTSTR lptszMessage );
	void remoteStopped();
	void remoteConnected();
	void remoteTogglePTT( bool fState );

// Dialog Data
	enum { IDD = IDD_SIMPLEPTT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	NOTIFYICONDATA m_hSysTrayIcon;
	CMenu m_trayMenu;
	char m_nPTTPin;
	CString m_sPort;
	CIniFile m_iniFile;
	CSerialWnd m_serialPort;
	HANDLE m_hRemoteThread;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnSize( UINT nType, int cx, int cy );
	void onSysTrayMenuClose();
	void onSysTrayMenuShow();
	LRESULT onTrayNotify( WPARAM wParam, LPARAM lParam );
	void toggleEnable( bool fState );
	void togglePTT( bool fState );
	BOOL PreTranslateMessage( MSG* pMsg );

	DECLARE_MESSAGE_MAP()
public:
	CButton PTTButton;
	CEdit EditBoxServer;
	CEdit EditBoxPort;
	CComboBox ComboBoxPort;
	CComboBox ComboBoxPin;
	CButton ChkBoxEnable;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedCheck1();
	CEdit EditBoxPass;
};
