// **************************************************************************
//  @file       SPLITBDG.H
//  @brief      CSplitBarDlg クラスの宣言
//
//  @author     Iwao (https://mish.work/)
//  @date       2026-04-10
//
//  @modify
//  2026-04-10  新規作成
//
//  @disclaimer
//  本コードの使用により生じたいかなる損害についても著作者は責任を負いません
//  引用時は上記 URL を明記してください
//
//  (C) 2026 Iwao. All Rights Reserved.
// **************************************************************************

#include "SplitBar.inc"

#if !defined(AFX_SPLITBDG_H__B01E45C4_21C8_473A_B4DF_3D42CBF638D2__INCLUDED_)
#define AFX_SPLITBDG_H__B01E45C4_21C8_473A_B4DF_3D42CBF638D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSplitBarDlg ダイアログ

class CSplitBarDlg : public CDialog
{
// 構築
public:
	CSplitBarDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSplitBarDlg)
	enum { IDD = IDD_SPLITBAR_DIALOG };
	CStatic	m_RightPane;
	CStatic	m_Left_Pane;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSplitBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSplitBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CSplitterBar	m_Splitter ;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SPLITBDG_H__B01E45C4_21C8_473A_B4DF_3D42CBF638D2__INCLUDED_)
