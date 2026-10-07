// **************************************************************************
//  @file       SPLITBAR.INC
//  @brief      スプリッタバー
//
//  @author     Iwao (https://mish.work/)
//  @date       2026-04-03
//
//  @modify
//  2026-03-10  新規作成
//  2026-03-11  CSplitterBase 関係をファイルとして分離
//  2026-03-24  ModeFixed 関係の実装
//  2026-04-03  SetInitFixed などの追加
//
//  @disclaimer
//  本コードの使用により生じたいかなる損害についても著作者は責任を負いません
//  引用時は上記 URL を明記してください
//
//  (C) 2026 Iwao. All Rights Reserved.
// **************************************************************************

#pragma once

#include <afxwin.h>

////
// **************************************************************************
//  CSplitterBar クラス
// **************************************************************************
class CSplitterBar : public CStatic
{
public:
	enum	ResizeMode {
		ModeFixedLT = 0, // 左端（または上端）からの距離を維持
		ModeFixedRB,     // 右端（または下端）からの距離を維持
		ModeRelative,    // 比率を維持
		ModeAutoExpand   // 大きい方を可変（現状は比率維持と同等として実装）
		};

    CSplitterBar() 
		: m_bDragging   (FALSE)
		, m_nDragOffset (0)
        , m_nOldGhostPos(0)
		, m_pLeftWnd    (NULL)
		, m_pRightWnd   (NULL)
		, m_nMinWidth   (30)
		, m_nBarSize    (20)
		, m_dRatio      (0.5)
		, m_nFixedDist  (-1)
		, m_bIsVertical (TRUE) 
		, m_nMode       (ModeAutoExpand)


    {

    }

protected:
	CWnd*			m_pLeftWnd;
	CWnd*			m_pRightWnd;
	BOOL			m_bIsVertical;

	int 			m_nMinWidth;
	int 			m_nBarSize;

	ResizeMode		m_nMode;
	double			m_dRatio;
	int 			m_nFixedDist;

	BOOL			m_bDragging;
	int 			m_nDragOffset;
    int 			m_nOldGhostPos;

protected:
    // 派生クラスや各関数からメンバ変数を直接書き換えず、ここを通す
    void SetLayoutState(ResizeMode mode, double dRatio, int nFixedDist) {
        m_nMode       = mode;
        m_dRatio      = dRatio;
        m_nFixedDist  = nFixedDist;
    }

public:
    void	SetInitFixed(int nFixedDist) {        	m_nFixedDist  = nFixedDist;    	}
    int		GetFixedDist(void)			 {	return	m_nFixedDist ;    				}

public:
	// 初期化 (bVertical = TRUE: 左右分割, FALSE: 上下分割)
	void Init(CWnd* pLeft, CWnd* pRight
			, BOOL bVertical  = TRUE
			, int nBarSize    = 5
			, int nMinWidth   = 30
			, ResizeMode mode = ModeAutoExpand
			)
	{
		m_pLeftWnd    = pLeft;
		m_pRightWnd   = pRight;
		m_nMode       = mode;
		m_bIsVertical = bVertical;
	//	m_nBarSize    = nBarSize;
		m_nBarSize    = GetScaledSize(nBarSize);
		m_nMinWidth   = nMinWidth;
		m_nMinWidth   = GetScaledSize(nMinWidth);

		ModifyStyle  (0, SS_NOTIFY,           0);
		ModifyStyleEx(0, WS_EX_DLGMODALFRAME, SWP_FRAMECHANGED);

		CRect rc;
		::GetWindowRect(m_hWnd, &rc);
		CWnd* pParent = GetParent();
		if (pParent) {
			pParent->ScreenToClient(&rc);
			}

		if (m_bIsVertical) {	::MoveWindow(m_hWnd, rc.left, rc.top, m_nBarSize,  rc.Height(), TRUE);        } 
		else               {	::MoveWindow(m_hWnd, rc.left, rc.top, rc.Width(),  m_nBarSize,  TRUE);        }

		UpdateRatio();
		Relayout();
		}

protected:
    int GetScaledSize(int nSize) {
        HDC hdc  = ::GetDC(m_hWnd);
        int nLog = ::GetDeviceCaps(hdc, m_bIsVertical ? LOGPIXELSX : LOGPIXELSY);
        ::ReleaseDC(m_hWnd, hdc);
        return ::MulDiv(nSize, nLog, 96);
    }

public:
    void PostRelayout()
    {
        CWnd* pParent = GetParent();
        if (pParent && ::IsWindow(pParent->m_hWnd)) {
            CRect rcP;
            pParent->GetClientRect(&rcP);
            // 親ウィンドウ（ダイアログ）の OnSize をメッセージ経由で叩く
            pParent->PostMessage(WM_SIZE, SIZE_RESTORED, MAKELONG(rcP.Width(), rcP.Height()));
        }
    }

public:
    // 基底クラスのシグネチャに合わせ、RelayoutAll からの呼び出しに対応させる
    virtual void RelayoutAll(LPCRECT lpRect = NULL)
    {
        Relayout(lpRect);
    }

public:

    // **************************************************************************
    //  UpdateRatio :  【物理位置】 -> 【論理データ】 への同期
    //              :   現在のウィンドウ位置を読み取り、m_dRatio や m_nFixedDist を更新保存する
    //  作成日      :   2026-03-10
    //  変更日      :   2026-03-24  論理データ保存の厳格化
    // **************************************************************************
	void UpdateRatio()
	{
	    if (m_hWnd == NULL || !::IsWindow(m_hWnd))    {
	        return;
	    }

	    CRect rcP, rcB;
	    ::GetClientRect(::GetParent(m_hWnd), &rcP);
	    ::GetWindowRect(m_hWnd,              &rcB);
	    ::ScreenToClient(::GetParent(m_hWnd), (LPPOINT)&rcB);

	    int nParentSize = m_bIsVertical ? rcP.Width() : rcP.Height();
	    int nPos        = m_bIsVertical ? rcB.left    : rcB.top;
	    int nBarSize    = m_nBarSize;

	    // 親のサイズが 0 (最小化中) なら、比率を更新せずに古い値を守る
	    if (nParentSize <= 0)    {
	        return;
	    }

	    // 現在の物理位置から、各モード用の論理値を算出
	    double dRatio = (double)nPos / nParentSize;
	    int nFixedLT  = nPos;
	    int nFixedRB  = nParentSize - (nPos + nBarSize);

    // モードに応じて適切な値を論理状態として保存
	    if (m_nMode == ModeFixedLT)    {
	        SetLayoutState(m_nMode, dRatio, nFixedLT);
	    }
	    else if (m_nMode == ModeFixedRB)    {
	        SetLayoutState(m_nMode, dRatio, nFixedRB);
	    }
	    else    {
	        // ModeRelative または ModeAutoExpand (比率を優先して保持)
	        SetLayoutState(m_nMode, dRatio, nFixedRB);
	    }
	}

    // **************************************************************************
    //  Relayout    :  【論理データ】 -> 【物理座標】 への逆算
    //              :   保存されている m_dRatio 等から、現在の親サイズに適した nPos を算出する
    //  作成日      :   2026-03-10
    //  変更日      :   2026-03-24  モード別計算の実装
    // **************************************************************************
    void Relayout(LPCRECT lpRect = NULL)
    {
        // 1. ガード処理
        if (m_hWnd == NULL || !::IsWindow(m_hWnd) || !m_pLeftWnd) {
            return;
        }
        CWnd* pParent = GetParent();
        if (!pParent || pParent->IsIconic()) {
            return;
        }

        // 2. 基準となる親領域(rcP)の決定
        CRect rcP;
        if (lpRect) {
            // 子スプリッタとして呼ばれた場合は、渡された領域を使用
            rcP = *lpRect;
        } 
       else {
            // ルートとして呼ばれた場合は、親のクライアント領域を取得
            pParent->GetClientRect(&rcP);
        }

        // 3. 現在のスプリッタ位置(rcB)の取得
        CRect rcB;
        ::GetWindowRect(m_hWnd, &rcB);
        ::ScreenToClient(pParent->m_hWnd, (LPPOINT)&rcB);

        // 4. 計算用の変数準備
        int nParentSize = m_bIsVertical ? rcP.Width() : rcP.Height();
        int nBarSize    = m_nBarSize;
        int nPos        = 0;

        // 5. モードに応じた位置計算
        if (m_nMode == ModeFixedLT) {
            // 左端（または上端）からの距離を維持
            nPos = m_nFixedDist;
        }
        else if (m_nMode == ModeFixedRB) {
            // 右端（または下端）からの距離を維持
            nPos = nParentSize - nBarSize - m_nFixedDist;
        }
        else if (m_nMode == ModeRelative) {
            // 比率を維持
            nPos = (int)(nParentSize * m_dRatio);
        }
        else {
            // ModeAutoExpand (暫定的に比率維持)
            nPos = (int)(nParentSize * m_dRatio);
        }

        // 6. 実際の配置処理（nPos は rcP 内の相対座標として渡す）
        DoRelayout(nPos, &rcP); 
    }

    // **************************************************************************
    //  DoRelayout  :  【物理座標】 -> 【実行】
    //              :   算出された nPos を使って、実際に配置を確定させる
    //  作成日      :   2026-03-10
    //  ※ この内部で UpdateRatio を呼ぶのは「副作用」となるため、今後分離する
    // **************************************************************************
    void DoRelayout(int nPos, LPCRECT lpRect = NULL)
    {
        if (!m_pLeftWnd || !m_pRightWnd) {
            return;
        }

        CWnd* pParent = GetParent();

        CRect rcP;
        if (lpRect) {
            // 親スプリッタから渡された領域を使用
            rcP = *lpRect;
        } 
		else {
            // 単体動作時は従来通り親のクライアント領域を取得
            pParent->GetClientRect(&rcP);
        }

		int nBarSize    = m_nBarSize;
        int nParentSize = m_bIsVertical ? rcP.Width() : rcP.Height();

        // 最小サイズの制限
        if (nPos < m_nMinWidth) {
			nPos = m_nMinWidth;
			}
        if (nPos > nParentSize - nBarSize - m_nMinWidth) {
			nPos = nParentSize - nBarSize - m_nMinWidth;
			}

		{
	        HDWP hdwp = ::BeginDeferWindowPos(3);
	        if (m_bIsVertical) {
	            // 垂直分割：rcP.left からの相対座標で配置
	            ::DeferWindowPos(hdwp, m_pLeftWnd->m_hWnd,  NULL, rcP.left,                   rcP.top, nPos,                            rcP.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	            ::DeferWindowPos(hdwp, m_hWnd,              NULL, rcP.left + nPos,            rcP.top, nBarSize,                        rcP.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	            ::DeferWindowPos(hdwp, m_pRightWnd->m_hWnd, NULL, rcP.left + nPos + nBarSize, rcP.top, rcP.Width() - (nPos + nBarSize), rcP.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	        } 
	        else {
	            // 水平分割：rcP.top からの相対座標で配置
	            ::DeferWindowPos(hdwp, m_pLeftWnd->m_hWnd,  NULL, rcP.left, rcP.top,                   rcP.Width(), nPos,                             SWP_NOZORDER | SWP_NOACTIVATE);
	            ::DeferWindowPos(hdwp, m_hWnd,              NULL, rcP.left, rcP.top + nPos,            rcP.Width(), nBarSize,                         SWP_NOZORDER | SWP_NOACTIVATE);
	            ::DeferWindowPos(hdwp, m_pRightWnd->m_hWnd, NULL, rcP.left, rcP.top + nPos + nBarSize, rcP.Width(), rcP.Height() - (nPos + nBarSize), SWP_NOZORDER | SWP_NOACTIVATE);
	        }
	        ::EndDeferWindowPos(hdwp);
		}
    }




protected:

    // ドラッグ中の枠線を描画するヘルパー
    void DrawGhostBar(int nPos) {
        CWnd* pParent = GetParent();
        CClientDC dc(pParent);
        
        CRect rc;
        GetWindowRect(&rc);
        pParent->ScreenToClient(&rc);

        // バーの現在の幅/高さを維持して位置だけ計算
        int nW = rc.Width();
        int nH = rc.Height();

        if (m_bIsVertical) {	rc.left = nPos;		rc.right  = nPos + nW;        }
        else               {	rc.top  = nPos;		rc.bottom = nPos + nH;        }

        // 反転描画（2回描くと元に戻る）
        dc.InvertRect(&rc); 
    }

    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message) {
        case WM_SETCURSOR:
            ::SetCursor(::LoadCursor(NULL, m_bIsVertical ? IDC_SIZEWE : IDC_SIZENS));
            return TRUE;

        case WM_LBUTTONDOWN:
            {
                m_bDragging = TRUE;
                ::SetCapture(m_hWnd);

                CRect rc; 
                GetWindowRect(&rc); // 自分の現在の位置を取得
                ::MapWindowPoints(NULL, ::GetParent(m_hWnd), (LPPOINT)&rc, 2);

                CPoint pt(lParam); // クライアント座標のマウス位置
                m_nDragOffset  = m_bIsVertical ? pt.x :    pt.y;
                m_nOldGhostPos = m_bIsVertical ? rc.left : rc.top;
                
                // 最初の枠を描画
                DrawGhostBar(m_nOldGhostPos);
                return 0;
            }

        case WM_MOUSEMOVE:
            if (m_bDragging) {
                CPoint pt(lParam);
                ClientToScreen(&pt);
                ::ScreenToClient(::GetParent(m_hWnd), &pt);

                int nNewPos = (m_bIsVertical ? pt.x : pt.y) - m_nDragOffset;

                // 1. 前回の枠を消し、2. 新しい枠を描く
                if (nNewPos != m_nOldGhostPos) {
                    DrawGhostBar(m_nOldGhostPos);
                    DrawGhostBar(nNewPos);
                    m_nOldGhostPos = nNewPos;
                }
                return 0;
            }
            break;

        case WM_LBUTTONUP:
            if (m_bDragging) {
                // 最後に残っている枠を消す
                DrawGhostBar(m_nOldGhostPos);

                m_bDragging = FALSE;
                ::ReleaseCapture();
                
                // ★指を離した瞬間に初めて再配置を実行
                DoRelayout(m_nOldGhostPos);
                UpdateRatio();
                
                // 親（ダイアログ）に通知して、重い描画を1回だけ走らせる
                ::PostMessage(::GetParent(m_hWnd), WM_SIZE, 0, 0);
                return 0;
            }
            break;

        }

        return CStatic::WindowProc(message, wParam, lParam);
    }


};

