#include "stdafx.h"
#include "DDE_Stopper.h"
#include "DDE_StopperDlg.h"
#include ".\dde_stopperdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


UINT CDDE_StopperDlg::WM_TRAYRESTARTNOTIFY = (WM_USER + 1002);
UINT CDDE_StopperDlg::WM_TRAYICONNOTIFY    = (WM_USER + 1003);
UINT CDDE_StopperDlg::WM_ADDLOG    = (WM_USER + 1004);


CDDE_StopperDlg::CDDE_StopperDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDDE_StopperDlg::IDD, pParent)
{
	m_hHook = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hIconAct = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconIna = AfxGetApp()->LoadIcon(IDR_MAINFRAME1);

	WM_TRAYRESTARTNOTIFY = RegisterWindowMessage(TEXT("TaskbarCreated"));
	WM_TRAYICONNOTIFY = RegisterWindowMessage(TEXT("CloseWndTrayNotify"));
	WM_ADDLOG = RegisterWindowMessage(TEXT("DDEStopperAddLog"));

    m_hModule = ::LoadLibrary(_T("DDE_StopperDll.dll"));
	m_SetHook = (HHOOK (CALLBACK  *)(HWND))::GetProcAddress(m_hModule, "SetHook");
	m_UnHook = (void  (CALLBACK  *)())::GetProcAddress(m_hModule, "UnHook");
}
CDDE_StopperDlg::~CDDE_StopperDlg()
{
    m_UnHook();
	m_hHook = NULL;
	FreeLibrary(m_hModule);
}

void CDDE_StopperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDDE_StopperDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_TRAYRESTARTNOTIFY, OnTrayRestartNotify)
	ON_REGISTERED_MESSAGE(WM_TRAYICONNOTIFY, OnTrayIconNotify)
	ON_REGISTERED_MESSAGE(WM_ADDLOG, OnAddLog)
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDDE_StopperDlg メッセージ ハンドラ

BOOL CDDE_StopperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	m_hHook = m_SetHook( GetSafeHwnd() );

	m_stNotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_stNotifyIcon.uID = 0;
	m_stNotifyIcon.hWnd = m_hWnd;
	m_stNotifyIcon.hIcon = m_hIcon;
	m_stNotifyIcon.uCallbackMessage = WM_TRAYICONNOTIFY;
	lstrcpy( m_stNotifyIcon.szTip, AfxGetApp()->m_pszAppName );

	AddTaskbarIcons();
	SetTimer(127, 3000, NULL);
	return TRUE;
}

HCURSOR CDDE_StopperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDDE_StopperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CDDE_StopperDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (IsWindow(GetSafeHwnd()) &&
		IsWindow(GetDlgItem(IDC_EDITLOG)->GetSafeHwnd()))
	{
		GetDlgItem(IDC_EDITLOG)->MoveWindow(0,0,cx,cy);
	}
}


afx_msg LRESULT CDDE_StopperDlg::OnAddLog(WPARAM wParam, LPARAM lParam)
{
	CWnd *log = GetDlgItem(IDC_EDITLOG);
	log->SendMessage(
        EM_SETSEL,
        log->SendMessage(WM_GETTEXTLENGTH, 0, 0),
        -1);
    log->SendMessage(
		EM_REPLACESEL,
		0, 
		lParam);

	m_stNotifyIcon.uFlags = NIF_ICON;
	m_stNotifyIcon.hIcon = m_hIconAct;
	::Shell_NotifyIcon( NIM_MODIFY, &m_stNotifyIcon );

	ShowWindow(SW_SHOW);
	return 0;
}

//タスクトレイコールバック
afx_msg LRESULT CDDE_StopperDlg::OnTrayIconNotify(WPARAM wParam, LPARAM lParam)
{
	POINT point;
	::GetCursorPos(&point);

	switch(lParam)
	{
	case WM_RBUTTONDOWN:
		SetForegroundWindow();

		m_stNotifyIcon.uFlags = NIF_ICON;
		m_stNotifyIcon.hIcon = m_hIconAct;
		::Shell_NotifyIcon( NIM_MODIFY, &m_stNotifyIcon );
		m_CursorTrayIcon = TRUE;
		ShowWindow(SW_SHOW);

		PostMessage(WM_NULL);
		break;
	case WM_LBUTTONDOWN:
		SetForegroundWindow();

		m_stNotifyIcon.uFlags = NIF_ICON;
		m_stNotifyIcon.hIcon = m_hIconAct;
		::Shell_NotifyIcon( NIM_MODIFY, &m_stNotifyIcon );
		m_CursorTrayIcon = TRUE;

		PostMessage(WM_NULL);
		break;

	}
	return 0L;
}

void CDDE_StopperDlg::DelTaskbarIcons()
{
	m_stNotifyIcon.uFlags = 0;
	::Shell_NotifyIcon(NIM_DELETE, &m_stNotifyIcon);
}

void CDDE_StopperDlg::AddTaskbarIcons()
{
	m_stNotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	::Shell_NotifyIcon( NIM_ADD, &m_stNotifyIcon );
}

LRESULT CDDE_StopperDlg::OnTrayRestartNotify(WPARAM wParam, LPARAM lParam)
{
	AddTaskbarIcons();
	return 0L;
}

//デフォルト非表示
void CDDE_StopperDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 127)
	{

		CWnd *log = GetDlgItem(IDC_EDITLOG);
		log->SendMessage(
			EM_SETSEL,
			0,
			log->SendMessage(WM_GETTEXTLENGTH, 0, 0));
		log->SendMessage(
			EM_REPLACESEL,
			0, 
			(LPARAM)_T(""));


		m_stNotifyIcon.uFlags = NIF_ICON;
		m_stNotifyIcon.hIcon = m_hIconIna;
		::Shell_NotifyIcon( NIM_MODIFY, &m_stNotifyIcon );
		ShowWindow(SW_HIDE);
	}
	CDialog::OnTimer(nIDEvent);
}

