// mainfrm.h : interface of the CMainFrame class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __MAINFRM_H___
#define __MAINFRM_H___

#define GetMainFrame() (DYNAMIC_DOWNCAST(CMainFrame,AfxGetMainWnd())) 
#include "DRAWVW.H"


#define MAXZOOM 150

class CMainFrame
	: public CMDIFrameWnd
#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	, public CExtCustomizeSite
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)
{
	DECLARE_DYNAMIC(CMainFrame)
		
public:
#if (defined __EXT_MFC_NO_CUSTOMIZE)
	CExtBarColorButton * m_pBtnColorFill, * m_pBtnColorOutline;
#endif // (defined __EXT_MFC_NO_CUSTOMIZE)

	CMainFrame();

#if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

#ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	

	class CMyMdiWindowsListDlg : public CExtMdiWindowsListDlg
	{
	public:
		CMyMdiWindowsListDlg(
				CMDIFrameWnd * pMDIFrameWnd,
				CWnd * pParent = NULL
				)
			: CExtMdiWindowsListDlg(
				pMDIFrameWnd,
				pParent
				)
		{
		}
		CMyMdiWindowsListDlg(
				HWND hMDIFrameWnd,
				CWnd * pParent = NULL
				)
			: CExtMdiWindowsListDlg(
				hMDIFrameWnd,
				pParent
				)
		{
		}
	protected:
		virtual void OnUpdateButtons()
		{
			CExtMdiWindowsListDlg::OnUpdateButtons();
			
			CMainFrame * pFrame = 
				static_cast < CMainFrame * > (
					CWnd::FromHandlePermanent( m_hMDIFrameWnd )
					);
			if ( !pFrame->m_wndMenuBar.IsDisplayMdiDocumentButtons() )
			{
				m_btnCascade.ShowWindow(SW_HIDE);
				m_btnTileVert.ShowWindow(SW_HIDE);
				m_btnTileHorz.ShowWindow(SW_HIDE);
				m_btnMinimize.ShowWindow(SW_HIDE);
			}
			else
			{
				m_btnCascade.ShowWindow(SW_SHOW);
				m_btnTileVert.ShowWindow(SW_SHOW);
				m_btnTileHorz.ShowWindow(SW_SHOW);
				m_btnMinimize.ShowWindow(SW_SHOW);
			}
		}
	}; // class CMyMdiWindowsListDlg : public CExtMdiWindowsListDlg

#endif // #ifndef __EXT_MFC_NO_MDI_WINDOWS_LIST_DIALOG	

#endif // #if (defined _AFXDLL && !defined __STATPROFUIS_WITH_DLLMFC__)

// Attributes
public:

	// window placement persistence
	WINDOWPLACEMENT m_dataFrameWP;

// Operations
public:

// Implementation
public:
	virtual ~CMainFrame();
	virtual void RecalcLayout(BOOL bNotify = TRUE);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	CExtBarButton *  OnCreateToolbarButton(
		CExtToolControlBar * pBar,
		CExtCustomizeCmdTreeNode * pNodeI,
		CExtCustomizeCmdTreeNode * pNodeC
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

public:
	// control bar embedded members
	CExtMenuControlBar		m_wndMenuBar;
	CExtStatusControlBar	m_wndStatusBar;
	CExtToolControlBar		m_wndToolBarStandard;
	CExtToolControlBar		m_wndToolBarCustom;
	CExtToolControlBar		m_wndPalette;
   CExtToolControlBar    m_wndToolBarZoom;


   class CZoomBarSliderButton: public CExtBarSliderButton
	{
		
	public:
		CZoomBarSliderButton(
			CExtToolControlBar * pBar = NULL,
			UINT nCmdID = ID_SEPARATOR,
			UINT nStyle = 0,
			ULONG nScrollTotalRange = 0L,
			ULONG nScrollPos = 0L,
			ULONG nScrollPageSize = 0L,
			INT nScrollButtonExtentH = 0,
			INT nScrollButtonExtentV = 0,
			INT nScrollControlExtentH = 50,
			INT nScrollControlExtentV = 50
			)
			: CExtBarSliderButton(
			pBar,
			nCmdID,
			nStyle,
			nScrollTotalRange,
			nScrollPos,
			nScrollPageSize,
			nScrollButtonExtentH,
			nScrollButtonExtentV,
			nScrollControlExtentH,
			nScrollControlExtentV
			)
		{
		
		};


		~CZoomBarSliderButton()
		{
	
		};


		void SetZoomPos(int zoom)
		{

			int pos = 0;
			CMainFrame *pFrame = GetMainFrame();

			switch(zoom)
			{
			case 100:
				pos = 0;
				pFrame->m_wndStatusBar.SetPaneText(1,    _T("100 %"));
			
				break;
			case 125:
				pos = 1;
				pFrame->m_wndStatusBar.SetPaneText(1,    _T("125 %"));
				break;
			case 150:
				pos = 2;
				pFrame->m_wndStatusBar.SetPaneText(1,    _T("150 %"));
				break;
			}

			CExtBarSliderButton::ScrollPosSet( pos );
			RedrawButton();

		}
			
		ULONG ScrollPosSet( ULONG nScrollPos )
		{
			ULONG ulRet = CExtBarSliderButton::ScrollPosSet( nScrollPos );
			RedrawButton();


			CMainFrame *pFrame = GetMainFrame();
			CMDIChildWnd *pChild =	pFrame->MDIGetActive();
			


			if(pChild != NULL)
			{
				int nZoomPercent = 100;

				switch(nScrollPos) {
				case 0:
					nZoomPercent = 100;
							
					pFrame->m_wndStatusBar.SetPaneText(1,    _T("100 %"));
					break;
				case 1:
					nZoomPercent = 125;
					pFrame->m_wndStatusBar.SetPaneText(1,    _T("125 %"));
					break;
				case 2:
					nZoomPercent = MAXZOOM;
					char strZoom[17];
					sprintf_s(strZoom,sizeof(strZoom),"%d%%",nZoomPercent);
					pFrame->m_wndStatusBar.SetPaneText(1,    _T(strZoom));
					break;
				}


				CDrawView *pView = (CDrawView*)pChild->GetActiveView();
				pView->SetZoom(nZoomPercent);
				

				
			}		
			return ulRet;
		}
	}; // class CZoomBarSliderButton




#if (!defined __EXT_MFC_NO_TABMDI_CTRL)
	CExtTabMdiOneNoteWnd	m_wndMdiTabs;
#endif // (!defined __EXT_MFC_NO_TABMDI_CTRL)

#if (!defined __EXT_MFC_NO_CUSTOMIZE)
	virtual HICON OnColorItemGenerateIcon(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF clr
		);
	virtual bool OnColorItemValueGet(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF & clr,
		bool bSelected
		);
	virtual bool OnColorItemValueSet(
		CExtBarColorButton * pTBB,
		CExtCustomizeCmdTreeNode * pNode,
		COLORREF & clr,
		bool bSelected
		);
#endif // (!defined __EXT_MFC_NO_CUSTOMIZE)

public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL
	
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMditabsTop();
	afx_msg void OnUpdateMditabsTop(CCmdUI* pCmdUI);
	afx_msg void OnMditabsBottom();
	afx_msg void OnUpdateMditabsBottom(CCmdUI* pCmdUI);
	afx_msg void OnMditabsLeft();
	afx_msg void OnUpdateMditabsLeft(CCmdUI* pCmdUI);
	afx_msg void OnMditabsRight();
	afx_msg void OnUpdateMditabsRight(CCmdUI* pCmdUI);
	afx_msg void OnMditabsShowClose();
	afx_msg void OnUpdateMditabsShowClose(CCmdUI* pCmdUI);
	afx_msg void OnMditabsShowCtxHelp();
	afx_msg void OnUpdateMditabsShowCtxHelp(CCmdUI* pCmdUI);
	afx_msg void OnUistyle2000();
	afx_msg void OnUpdateUistyle2000(CCmdUI* pCmdUI);
	afx_msg void OnUistyleXp();
	afx_msg void OnUpdateUistyleXp(CCmdUI* pCmdUI);
	afx_msg void OnViewLikeOffice2003();
	afx_msg void OnUpdateViewLikeOffice2003(CCmdUI* pCmdUI);
	afx_msg void OnViewLikeStudio2005();
	afx_msg void OnUpdateViewLikeStudio2005(CCmdUI* pCmdUI);
	afx_msg void OnContextHelp();
	afx_msg void OnUpdateContextHelp(CCmdUI* pCmdUI);
	afx_msg void OnCustomize();
	afx_msg void OnUpdateCustomize(CCmdUI* pCmdUI);
	//afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd );
	//}}AFX_MSG
	afx_msg LRESULT OnExtMenuPrepare(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDrawPopupMenuItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorSelectCustom(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif //__MAINFRM_H___
/////////////////////////////////////////////////////////////////////////////
