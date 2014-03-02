// DDE_StopperDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"


// CDDE_StopperDlg ダイアログ
class CDDE_StopperDlg : public CDialog
{
// コンストラクション
public:
	CDDE_StopperDlg(CWnd* pParent = NULL);	// 標準コンストラクタ
	virtual ~CDDE_StopperDlg();
// ダイアログ データ
	enum { IDD = IDD_DDE_Stopper_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HHOOK m_hHook;
	HMODULE m_hModule;

	HHOOK (CALLBACK  *m_SetHook)(HWND hWnd);
	void  (CALLBACK  *m_UnHook)();
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
