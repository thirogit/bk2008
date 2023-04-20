#ifndef __EDITOUTDOC_H__
#define __EDITOUTDOC_H__

#include "NewOutDocDlg.h"
#include "../datatypes/OutDoc.h"

class CEditOutDocDlg : public CNewOutDocDlg
{
public:
	CEditOutDocDlg(OutDoc *pDoc2Edit);	
	virtual ~CEditOutDocDlg();
	
protected:
	OutDoc *m_pEditDoc;
	CCowArray m_delCows;
	virtual void DeleteCow(int iElement);		
	virtual void CancelDoc();	
	virtual bool CommitDoc();
	virtual CString GetDocDlgCaption();	
	virtual void SetupBtns();
	virtual void AddSelectedCows(bool bFromAllCows = false);
protected:	
	virtual BOOL OnInitDialog();
	
};

#endif
