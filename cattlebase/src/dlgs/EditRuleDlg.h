#ifndef __EDITRULEDLG_H__
#define __EDITRULEDLG_H__

#include "BaseRuleDlg.h"

class CEditRuleDlg : public CBaseRuleDlg
{
public:
	CEditRuleDlg(Rule* pRule,CWnd* pParent = NULL);
protected:
	virtual CString GetWindowCaption();
	virtual CString GetBtnCaption();
	virtual bool CommitRule();
	virtual void SetupControls();
	virtual bool Validate();

	Rule* m_pRule2Edit;

};

#endif