// DDE_StopperDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"


// CDDE_StopperDlg �_�C�A���O
class CDDE_StopperDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CDDE_StopperDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^
	virtual ~CDDE_StopperDlg();
// �_�C�A���O �f�[�^
	enum { IDD = IDD_DDE_Stopper_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HHOOK m_hHook;
	HMODULE m_hModule;

	HHOOK (CALLBACK  *m_SetHook)(HWND hWnd);
	void  (CALLBACK  *m_UnHook)();
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
