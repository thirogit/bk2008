#if !defined(AFX_MAINFRM_H__F9B594FC_EE43_486B_88B2_D8269C9BAA68__INCLUDED_)
#define AFX_MAINFRM_H__F9B594FC_EE43_486B_88B2_D8269C9BAA68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VisualFX\VisualFx.h"
#include "CattleSQL.h"
#include "dlgs\finddlg.h"
#include "MainCfg.h"
#include "cattlebase.h"
#include "grids\BaseDataGrid.h"
#include "dlgs\CaptionButtonDlg.h"
#include "ctrls\enteredit\FastSelEdit.h"
#include "msg_defs.h"
#include "hkey\HKeyExpiryProgressCtrl.h"
#include "hkey\HKeyNotPresentProgressCtrl.h"

#include < Prof-UIS.h >

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL SetupStatusBar();
	HICON m_hIconGood;
	HICON m_hIconBad;
	HICON m_hIconNoBarcode;
	HICON m_hIconYesBarcode;
	bool SetupFastSelectToolBar();
	bool SetupGridCtrlToolBar();

private:


	BaseDataGrid * GetCurrGrid();
	UINT GetCurrGridID();
	BaseDataGrid *GetGridView(int nId);

	CExtStatusControlBar				m_wndStatusBar;
	CExtToolControlBar					m_wndToolBar;
	CExtMenuControlBar					m_wndMenuBar;
	CFastSelEdit						m_fastSelect;
	CExtToolControlBar					m_wndToolBarFastSelect;
	CExtToolControlBar					m_wndToolBarGridCtrl;
	CHardwareKeyNotPresentProgressCtrl	m_KeyNotPresentProgressCtrl;
	bool								m_bShowingKeyState;

    TVisualFramework m_Framework;
	
	bool TryTrashCow(Cow *cowd);
	bool TrashHent(Hent *pHent);
	bool TrashInDoc(int iAryIdx);
	bool TrashOutDoc(int iAryIdx);
	void TrashCow(CCowArray &mainArray,CCowArray &subArray,int nElement);
	bool TrashHent(CHentArray &mainArray,CHentArray &subArray,int nElement);
	void TrashExpense(CExpenseDataArray &expenses,int nElement);
	bool TrashInvoice(CInvoiceDataArray &invArray,int nItem);
	bool TrashCorrectInvoice(Invoice* pInv);
	bool TrashVATInvoice(VATInvoice* pInv);
	bool TrashRRInvoice(RRInvoice* pInv);
	bool DeleteCowInvoiceEntries(CCowInvoiceEntryArray &invEntries,bool bCommit);


	bool LoadFirmInfo();
	void FlushArrays();
	void UpdateAllGrids();
	void DoCowEdit(Cow *cowd);	
	void EnableUserTabs();

	
protected:
	

	void LoadDB(LPCSTR pszLoadDir);
protected:

	BOOL m_FastSelMarkRow;
	BOOL m_FastSelSelRow;
	BOOL m_FastSelFindFirst;
	BOOL m_FastSelPosOnRow;
		
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//afx_msg void OnKillFocus(CWnd* pNewWnd);

	afx_msg	void OnNewInDoc ();
	afx_msg	void OnNewHent ();
	afx_msg void OnNewOutDoc();
	afx_msg void OnPrint();
	afx_msg void OnUnmarkAll();
	afx_msg void OnFind();
	afx_msg void OnTermSync();
	afx_msg void OnOptions();
	afx_msg void OnExpense();
	afx_msg void OnHentSync();


	afx_msg void OnExit();
	afx_msg void OnStockEdit();
	afx_msg void OnStats();

	afx_msg void OnMarkRow();
	afx_msg void OnUnMarkRow();
	afx_msg void OnRuleEdit();
	afx_msg void OnEditExpenseCategory();
	afx_msg void OnEditAgents();
	

	afx_msg void OnClose();
	afx_msg void OnDestroy();
	
	
	afx_msg void OnLogout();
	afx_msg void OnPrintDocs(UINT nID);
	afx_msg void OnHoldDlg(UINT nID);	
	afx_msg void OnDelRow();	
	

	afx_msg void OnGridFontZoomOut();
	afx_msg void OnGridFontZoomIn();
	afx_msg void OnGridGoToBottom();
	afx_msg void OnGridGoToTop();
	afx_msg void OnUpdateGridFontZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGridFontZoomIn(CCmdUI* pCmdUI);

	
	afx_msg void OnFastSelect();	
	afx_msg void OnFastSelectOptCommand(UINT nID);
	afx_msg BOOL OnInsertNew(UINT nID);

	afx_msg void OnGridMenuCommand(UINT id);
	afx_msg void OnGridMenuUpdateCommand(CCmdUI *pCmdUI);

	afx_msg void OnAnyCommand();
	afx_msg BOOL OnLoadArchiveBackupDB(UINT nID);
	afx_msg void OnLoadPrimaryDB();
	afx_msg void OnLoadCustomDirDB();
	
	afx_msg LRESULT OnNewData(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnGetSelRows(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDeleteRows(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUpdateGrid(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnRestoreBackup(WPARAM wParam,LPARAM lParam);
	afx_msg void OnFastSelReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnSetActivePane(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDoArchive(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnLockGrid(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDrawPopupLeftArea(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateAnyCommand(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStats(CCmdUI * pCmdUI);
	afx_msg void OnUpdatePassImgDBStat(CCmdUI * pCmdUI);
	afx_msg void OnUpdatePrintDoc(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFastSelEdit( CCmdUI* pCmdUI );
	afx_msg void OnFastSelectOptUpdateCommand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBarcodeScannerStatus(CCmdUI * pCmdUI);
	afx_msg void OnUpdateFrameTitle(BOOL bAddToTitle);	


	DECLARE_MESSAGE_MAP()
};


#endif 
