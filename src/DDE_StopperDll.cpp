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

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    CString str;
    LPCWPSTRUCT pMsg = (LPCWPSTRUCT) lParam;
    TCHAR szDDEReceiver[MAX_PATH];
    TCHAR szApp[MAX_PATH];
    TCHAR szTopic[MAX_PATH];

    HWND hwndDDEReceiver;
    HWND hwndDDESender;
    static DWORD _pid;
    DWORD pid;

    /* NO DDE ? throw */
    if(!( pMsg->message >= WM_DDE_FIRST &&
        pMsg->message <= WM_DDE_LAST ) )
    {
        return CallNextHookEx(hHook, nCode, wParam, lParam);
    }

    hwndDDEReceiver = (HWND)pMsg->hwnd;
    hwndDDESender = (HWND)pMsg->wParam;
    GetWindowThreadProcessId( hwndDDESender, &pid);

    if( pMsg->message == WM_DDE_INITIATE )
    {

        GlobalGetAtomName(LOWORD(pMsg->lParam), szApp,
            sizeof(szApp) / sizeof(TCHAR));
        GlobalGetAtomName(HIWORD(pMsg->lParam), szTopic,
            sizeof(szTopic) / sizeof(TCHAR));

/*
        if(GetWindowText(hwndDDEReceiver, szDDEReceiver,
            sizeof(szDDEReceiver) / sizeof(TCHAR)) == 0)
        {
            _stprintf(szDDEReceiver, _T("[%08p]"), hwndDDEReceiver);
        }

        str.Format(_T("PID:%4d %-8s "),
            pid, sDDE[pMsg->message - WM_DDE_FIRST]);
        AddLog(str);
        str.Format(_T("(%-32s [%s][%s])\n"),
            szDDEReceiver, szApp, szTopic);
        AddLog(str);
*/

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
