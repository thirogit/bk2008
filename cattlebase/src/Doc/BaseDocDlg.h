#ifndef __BASEDOCDLG_H__
#define __BASEDOCDLG_H__

#include "imgbutton\ImageButton.h"
#include "combo\ReasonCombo.h"
#include "combo\HentCombo.h"
#include "ctrls\curdateedit\curdateedit.h"
#include "ctrls\BaseListCtrl.h"
#include "../combo/OutGrpCombo.h"
#include "../ctrls/GrpSummaryListCtrl.h"
#include "../datatypes/NullDoc.h"
#include "../ctrls/HourMinuteEdit.h"
#include "combo/AgentCombo.h"

class CBaseDocDlg : public CDialog 
{
public:
	CBaseDocDlg(CBaseListCtrl *pListCtrl,Reason::REASON_INOUT_TYPE reasonType,CowAccessor* pGrpAccessor);
	virtual ~CBaseDocDlg();
	
	enum { IDD = IDD_DOCDLG };
protected:
	CBaseListCtrl	*m_cowlist;

	CImageButton m_OK;
	CImageButton m_Delete;
	CImageButton m_Cancel;	
	CImageButton m_FindPass;
	
	CReasonCombo m_reasonCombo;
	CCurDateEdit m_DocDate;
	CCurDateEdit m_TransportDate;
	CHourMinuteEdit m_loadStartTm;
	CHourMinuteEdit m_loadEndTm;
	CHentCombo m_hentCombo;
	CString		  m_plateNo;
	CExtrasMemo   m_extras;
	CMenu		m_listMenu;
	COutGrpCombo m_activeGrpCombo;
	CGrpSummaryListCtrl m_GrpSummaryList;
	CowAccessor* m_pGrpAccessor;
	CAgentCombo m_AgentCb;
		
	CCowArray m_cows;

	CMenu      m_PlateNoMenu;
	
protected:

	virtual void AddCow() = 0;
	virtual void DeleteCow(int iElement) = 0;
	virtual void EditCow(int pElement) = 0;
	virtual void CancelDoc() = 0;
	virtual void GetCows() = 0;
	virtual bool CommitDoc() = 0;
	virtual CString GetDocDlgCaption() = 0;
	virtual void SetupBtns()  = 0;
	virtual void ModifyListMenu(CMenu &listMenu);
	virtual void ShowPass(int nItem) = 0;
	virtual void FindPass() = 0;

	NullDoc* CreateNewDoc();
	virtual void UpdateDoc(Doc *pDoc);
	
	virtual void RightClick(CPoint &ptCursor);
	virtual void DoDataExchange(CDataExchange* pDX);    	
	virtual BOOL OnInitDialog() ;	
	void OnOK();
	void UpdateCowCount();
	void UpdateSummary();
	int GetActiveGroup();
	virtual void ChangeGrp4Cow(Cow *pCow,int grp) = 0;
	void CommitChangesForCows(CCowArray& cows);
	void RollbackChangesForCows(CCowArray& cows);
	void SetPlateNo(const CString& sPlateNo);


protected:

	afx_msg void OnAddCows();
	afx_msg void OnDeleteCow();
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnGetCows();
	afx_msg void OnCancel();	
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnMenuEditCow();
	afx_msg void OnMenuShowPass();
	afx_msg void OnClose();
	afx_msg void OnFindPass();
	afx_msg void OnGrpChange(UINT grpItemId);

	afx_msg void OnSetLoadTimeStart();
	afx_msg void OnSetLoadTimeEnd();
	afx_msg void OnPlateNoMenuBtn();

	afx_msg void OnSetPlateNoFromAgent();
	afx_msg void OnSetPlateNoFromHent();
	afx_msg void OnClearAgent();
	
	
	DECLARE_MESSAGE_MAP()
};




#endif