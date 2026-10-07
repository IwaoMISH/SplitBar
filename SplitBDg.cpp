// **************************************************************************
//  @file       SPLITBDG.CPP
//  @brief      CSplitBarDlg クラスのインプリメンテーション
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

#include "stdafx.h"
#include "SplitBar.h"
#include "SplitBDg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitBarDlg ダイアログ

CSplitBarDlg::CSplitBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSplitBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSplitBarDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSplitBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplitBarDlg)
	DDX_Control(pDX, IDC_RIGHT_PANE, m_RightPane);
	DDX_Control(pDX, IDC_LEFT_PANE, m_Left_Pane);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSplitBarDlg, CDialog)
	//{{AFX_MSG_MAP(CSplitBarDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitBarDlg メッセージ ハンドラ

BOOL CSplitBarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	{
		// 1. スプリッタバーを動的に生成
		m_Splitter.Create(_T(""), WS_CHILD | WS_VISIBLE | SS_NOTIFY, CRect(0,0,0,0), this, 1300);
		// 2. スプリッタの初期化
		// 左右分割(TRUE), バーサイズ3, 最小幅10, 右端固定モード
		m_Splitter.Init(&m_Left_Pane, &m_RightPane, TRUE, 3, 10, CSplitterBar::ModeFixedRB);
		// 3. 右ペインの初期幅を 200px に設定
		m_Splitter.SetInitFixed(200);
		// 4. レイアウトの確定
		m_Splitter.PostRelayout();
	}
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CSplitBarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CSplitBarDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();

		// 動作確認用：ペインの色分けとテキスト描画
		{
			CClientDC dcL(&m_Left_Pane);
			CClientDC dcR(&m_RightPane);
			
			CRect rcL, rcR;
			m_Left_Pane.GetClientRect(&rcL);
			m_RightPane.GetClientRect(&rcR);

			dcL.FillSolidRect(&rcL, RGB(192, 255, 255)); // 水色 
			dcR.FillSolidRect(&rcR, RGB(255, 192, 255)); // ピンク 

			for (int i = 0; i < 10; i++) {
				dcL.TextOut(10, i * 20 + 10, _T("Left Pane"));
				dcR.TextOut(10, i * 20 + 10, _T("Right Pane"));
			}
		}

	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CSplitBarDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSplitBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	m_Splitter.RelayoutAll();
	/*	コントロールによってはゴミが残るので
	if (m_Splitter.m_hWnd != NULL) {
		m_Splitter. ShowWindow(SW_HIDE) ;
		m_Splitter. ShowWindow(SW_SHOW) ;
		m_Left_Pane.ShowWindow(SW_HIDE) ;
		m_Left_Pane.ShowWindow(SW_SHOW) ;
		m_RightPane.ShowWindow(SW_HIDE) ;
		m_RightPane.ShowWindow(SW_SHOW) ;
		}
	*/
}
