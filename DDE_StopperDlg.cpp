#include "stdafx.h"
#include "DDE_Stopper.h"
#include "DDE_StopperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CDDE_StopperDlg::CDDE_StopperDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDDE_StopperDlg::IDD, pParent)
{
	m_hHook = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

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
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDDE_StopperDlg メッセージ ハンドラ

BOOL CDDE_StopperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	
	m_hHook = m_SetHook( GetDlgItem(IDC_EDITLOG)->GetSafeHwnd() );

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
