// DDE_Stopper.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error ���̃t�@�C���� PCH �Ɋ܂߂�O�ɁA'stdafx.h' ���܂߂Ă��������B
#endif

#include "resource.h"		// ���C�� �V���{��


// CDDE_StopperApp:
// ���̃N���X�̎����ɂ��ẮADDE_Stopper.cpp ���Q�Ƃ��Ă��������B
//

class CDDE_StopperApp : public CWinApp
{
public:
	CDDE_StopperApp();

// �I�[�o�[���C�h
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CDDE_StopperApp theApp;
