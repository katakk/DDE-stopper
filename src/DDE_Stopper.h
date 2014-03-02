// DDE_Stopper.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含める前に、'stdafx.h' を含めてください。
#endif

#include "resource.h"		// メイン シンボル


// CDDE_StopperApp:
// このクラスの実装については、DDE_Stopper.cpp を参照してください。
//

class CDDE_StopperApp : public CWinApp
{
public:
	CDDE_StopperApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CDDE_StopperApp theApp;
