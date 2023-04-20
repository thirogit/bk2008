#ifndef __NEWOUTDOC_H__
#define __NEWOUTDOC_H__

#include "../Ctrls/button/OneItemBtnMenu.h"
#include "ctrls\CowListCtrl.h"
#include "../Doc/BaseDocDlg.h"

class CNewOutDocDlg : public CBaseDocDlg
{
public:
	CNewOutDocDlg();
	CNewOutDocDlg(CCowArray &cows);
	virtual ~CNewOutDocDlg();
	
protected:
	COutCowListCtrl	m_OutCowList;	
	CowOutAccessor m_OutAccessor;
	COneItemBtnMenu  m_Select;	
		
	virtual void AddCow();
	virtual void DeleteCow(int iElement);
	virtual void EditCow(int iElement);
	virtual void CancelDoc();
	virtual void GetCows();
	virtual bool CommitDoc();
	virtual CString GetDocDlgCaption();
	virtual void SetupBtns();
	virtual void ShowPass(int nItem);
	virtual void FindPass();	
	virtual void AddSelectedCows(bool bFromAllCows = false);
	void ChangeGrp4Cow(Cow *pCow,int grp);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog() ;
protected:
	
	afx_msg void OnGetSelInAllCows();

	

	DECLARE_MESSAGE_MAP()
	
};

#endif
