#ifndef __EDITCOWDLG_H__
#define __EDITCOWDLG_H__


#include "BaseCowDlg.h"

class CEditCowDlg : public CBaseCowDlg
{
public:
	CEditCowDlg(Cow *pCow,CWnd *pParent = NULL);
	void LockEANEdit();
protected:
	virtual BOOL OnInitDialog();
	virtual bool CommitCow();
	virtual CString GetWindowCaption();
	virtual void SetupControls();
	void UpdateCow(Cow *pCow);
	
	
protected:
	bool	m_bLockEANEdit;

};
	

#endif