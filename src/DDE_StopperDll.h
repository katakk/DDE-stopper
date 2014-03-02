// DDE_StopperDll.h : DDE_StopperDll.DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含む前に、'stdafx.h' を含めてください。
#endif

#include "resource.h"		// メイン シンボル


// CDDE_StopperDllApp
// このクラスの実装に関しては DDE_StopperDll.cpp を参照してください。
//

class CDDE_StopperDllApp : public CWinApp
{
public:
	CDDE_StopperDllApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
