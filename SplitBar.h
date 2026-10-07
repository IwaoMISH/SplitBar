// SplitBar.h : SPLITBAR アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_SPLITBAR_H__11BE643E_0B0A_45B3_9AFF_C38B63E336E1__INCLUDED_)
#define AFX_SPLITBAR_H__11BE643E_0B0A_45B3_9AFF_C38B63E336E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSplitBarApp:
// このクラスの動作の定義に関しては SplitBar.cpp ファイルを参照してください。
//

class CSplitBarApp : public CWinApp
{
public:
	CSplitBarApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSplitBarApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CSplitBarApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SPLITBAR_H__11BE643E_0B0A_45B3_9AFF_C38B63E336E1__INCLUDED_)
