// DDE_StopperDll.h : DDE_StopperDll.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error ���̃t�@�C���� PCH �Ɋ܂ޑO�ɁA'stdafx.h' ���܂߂Ă��������B
#endif

#include "resource.h"		// ���C�� �V���{��


// CDDE_StopperDllApp
// ���̃N���X�̎����Ɋւ��Ă� DDE_StopperDll.cpp ���Q�Ƃ��Ă��������B
//

class CDDE_StopperDllApp : public CWinApp
{
public:
	CDDE_StopperDllApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
