#ifndef __NEWRULEDLG_H__
#define __NEWRULEDLG_H__

#include "BaseRuleDlg.h"

class CNewRuleDlg : public CBaseRuleDlg
{
public:
	CNewRuleDlg(Rule* pNewRule,CWnd* pParent = NULL);
protected:
	virtual CString GetWindowCaption();
	virtual CString GetBtnCaption();
	virtual bool CommitRule();
	virtual void SetupControls();
	virtual bool Validate();

	Rule* m_pNewRule;

};

#endif