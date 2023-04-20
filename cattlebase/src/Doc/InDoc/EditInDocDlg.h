#ifndef __EDITINDOC_H__
#define __EDITINDOC_H__

#include "NewInDocDlg.h"
#include "../../datatypes/InDoc.h"

class CEditInDocDlg : public CNewInDocDlg 
{
public:
	CEditInDocDlg(InDoc *pDoc);
	virtual ~CEditInDocDlg();

	CCowArray m_newCows;
	CCowArray m_delCows;
	InDoc *m_pEditDoc;
	
protected:
	virtual BOOL OnInitDialog() ;	
	virtual void DeleteCow(int iElement);
	virtual void ChangeGrp4Cow(Cow *pCow,int grp);		
	virtual bool CommitDoc();
	virtual void AddCow();
	virtual CString GetDocDlgCaption();
	virtual void SetupBtns() ;
	virtual void AddNewCow(Cow *pCow);
	virtual void EditCow(int nElement);
	virtual void CancelDoc();
	void SetInDocForCows(Doc* pInDoc,CCowArray& cows);
	

};
#endif



