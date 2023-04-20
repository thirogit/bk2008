#ifndef __NEWCOWDLG_H__
#define __NEWCOWDLG_H__

#include "BaseCowDlg.h"

class CNewCowDlg : public CBaseCowDlg
{
public:
	CNewCowDlg(Cow *pCow,Hent* pSelOwner = NULL,CWnd *pParent = NULL);
protected:
	virtual BOOL OnInitDialog();
	virtual bool CommitCow();
	virtual CString GetWindowCaption();
	virtual void SetupControls();
	void EnableAllCtrls(bool bEnable);


protected:
	Hent*	m_pSelOwner;

protected:
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

#endif