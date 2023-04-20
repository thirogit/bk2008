#ifndef __NEWINDOC_H__
#define __NEWINDOC_H__

#include "../BaseDocDlg.h"
#include "../../Ctrls/CowListCtrl.h"
#include "Ctrls/ScanPassportsBtn.h"
#include "../../PassportRecognizer\ui\OCRPassDlg.h"
#include "../../dlgs/PassSearchDlg.h"

typedef CMap<Cow*,Cow*,Cow*,Cow*> CCowToCowMap;

class CNewInDocDlg : public CBaseDocDlg 
{
public:
	CNewInDocDlg();
	virtual ~CNewInDocDlg();	
protected:

	CInCowListCtrl	m_InCowList;
	CImageButton m_Add;
	CScanPassportsBtn m_ScanBtn;	
	Hent* m_prevOwner;		
	CCowDataPassImgMap m_passImgs;
	CowInAccessor m_InAccessor;	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual BOOL OnInitDialog() ;

	virtual void AddNewCow(Cow *pCow);
	virtual void ChangeGrp4Cow(Cow *pCow,int grp);
	virtual void AddCow();
	virtual void DeleteCow(int iElement);
	virtual void EditCow(int pElement);
	virtual void CancelDoc();
	virtual void GetCows();
	virtual bool CommitDoc();
	virtual CString GetDocDlgCaption();
	virtual void SetupBtns() ;
	virtual void ShowPass(int nItem);
	virtual void FindPass();	

	bool CheckCowDuplicates(Cow *pCow,Cow *pSkipInList,Cow *pSkipInAryCows);	

	afx_msg void OnBtnMenuScanUpdate();
	afx_msg void OnBtnMenuScanConfigure();

	DECLARE_MESSAGE_MAP()	
};

#endif