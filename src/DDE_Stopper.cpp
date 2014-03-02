#include "stdafx.h"
#include "DDE_Stopper.h"
#include "DDE_StopperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CDDE_StopperApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CDDE_StopperApp::CDDE_StopperApp()
{
}

CDDE_StopperApp theApp;

BOOL CDDE_StopperApp::InitInstance()
{
	InitCommonControls();
	CWinApp::InitInstance();
//	AfxEnableControlContainer();

	CDDE_StopperDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	return FALSE;
}
