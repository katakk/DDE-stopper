#include "stdafx.h"
#include "DDE_StopperDll.h"
#include "dde.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(linker, "/section:.HookData,rws")
#pragma data_seg(".HookData")
HWND      ghWnd = NULL;
HINSTANCE gInstance = NULL;
HHOOK     hHook = NULL;
#pragma data_seg()

LPCTSTR sDDE[WM_DDE_LAST - WM_DDE_FIRST + 1] = {
    "INITIATE", "TERMINATE", "ADVISE",
    "UNADVISE", "ACK", "DATA",
    "REQUEST", "POKE", "EXECUTE"
};

BEGIN_MESSAGE_MAP(CDDE_StopperDllApp, CWinApp)
END_MESSAGE_MAP()

CDDE_StopperDllApp::CDDE_StopperDllApp()
{

}

// 唯一の CDDE_StopperDllApp オブジェクトです。
CDDE_StopperDllApp theApp;

// CDDE_StopperDllApp 初期化
BOOL CDDE_StopperDllApp::InitInstance()
{
    CWinApp::InitInstance();
    gInstance = m_hInstance;
    return TRUE;
}

void AddLog(LPCTSTR str)
{
    SendMessage(ghWnd,
        EM_SETSEL,
        SendMessage(ghWnd, WM_GETTEXTLENGTH, 0, 0),
        -1);
    SendMessage(ghWnd, EM_REPLACESEL, (WPARAM)0, (LPARAM)(LPCTSTR)str);
}

HWND GetParentOwner(HWND hWnd)
{
	// check for permanent-owned window first
	CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
	if (pWnd != NULL)
		return pWnd->GetOwner()->GetSafeHwnd();

	// otherwise, return parent in the Windows sense
	return (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD) ?
		::GetParent(hWnd) : ::GetWindow(hWnd, GW_OWNER);
}

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    CString str;
    LPCWPSTRUCT pMsg = (LPCWPSTRUCT) lParam;
    TCHAR szDDEReceiver[MAX_PATH];
    TCHAR szDDESender[MAX_PATH];
    TCHAR szApp[MAX_PATH];
    TCHAR szTopic[MAX_PATH];

    HWND hwndDDEReceiver;
    HWND hwndDDESender;
    HWND htopDDEReceiver;
    HWND htopDDESender;
    static DWORD _pid;
    DWORD pid;
	int logging = 1;

    /* NO DDE ? throw */
    if(!( pMsg->message >= WM_DDE_FIRST &&
        pMsg->message <= WM_DDE_LAST ) )
    {
        return CallNextHookEx(hHook, nCode, wParam, lParam);
    }

    hwndDDEReceiver = (HWND)pMsg->hwnd;
    hwndDDESender = (HWND)pMsg->wParam;

	htopDDEReceiver = GetParentOwner(hwndDDEReceiver);
	htopDDESender = GetParentOwner(hwndDDESender);

    GetWindowThreadProcessId( hwndDDESender, &pid);

    if( pMsg->message == WM_DDE_INITIATE )
    {

        GlobalGetAtomName(LOWORD(pMsg->lParam), szApp,
            sizeof(szApp) / sizeof(TCHAR));
        GlobalGetAtomName(HIWORD(pMsg->lParam), szTopic,
            sizeof(szTopic) / sizeof(TCHAR));

        if(GetWindowText(htopDDEReceiver, szDDEReceiver,
            sizeof(szDDEReceiver) / sizeof(TCHAR)) == 0)
        {
			logging = 0;
            _stprintf(szDDEReceiver, _T("[%08p]"), hwndDDEReceiver);
        }
		szDDEReceiver[31] = _T('\0');

		if(GetWindowText(htopDDESender, szDDESender,
            sizeof(szDDESender) / sizeof(TCHAR)) == 0)
        {
//			logging = 0;
            _stprintf(szDDESender, _T("[%08p]"), hwndDDESender);
        }

		if( logging )
		{
        str.Format(_T("PID:%4d %-8s (%-32s %-32s [%s][%s])\n"),
            pid,
			sDDE[pMsg->message - WM_DDE_FIRST],
            szDDEReceiver,
			szDDESender,
			szApp,
			szTopic
			);
        AddLog(str);
		}

        // DDEInitiate("IExplore", "WWW_OpenURL")
        if (
        /*  _tcscmp(szApp, _T("IExplore") ) == 0 && */
            _tcscmp(szTopic, _T("WWW_OpenURL") ) == 0)
        {
            /* send dummy ACK (not post) */
            SendMessage(hwndDDESender, WM_DDE_ACK, (WPARAM)hwndDDEReceiver, 0);
        }

    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}


void CALLBACK UnHook( )
{
    UnhookWindowsHookEx(hHook);
    hHook = NULL;
}

HHOOK CALLBACK SetHook(HWND hWnd)
{
    ghWnd = hWnd;
    if ( hHook != NULL )
        UnHook();

    hHook = SetWindowsHookEx(
        WH_CALLWNDPROC,
        HookProc, gInstance, 0 );

    if ( hHook == NULL )
    {
        TRACE("EORROR hook install\n");
        return 0;
    }
    return hHook;
}
