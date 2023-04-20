#ifndef __BASERULEDLG_H__
#define __BASERULEDLG_H__

#include "../datatypes/Rule.h"
#include "RulePartPage.h"

class CBaseRuleDlg : public CDialog
{
	DECLARE_DYNAMIC(CBaseRuleDlg)
public:
	CBaseRuleDlg(CWnd* pParent = NULL);   
	virtual ~CBaseRuleDlg();
	enum { IDD = IDD_RULES };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void LoadNoSexValueCB(COWSEX cowSex);
	virtual CString GetWindowCaption() = 0;
	virtual CString GetBtnCaption() = 0;
	virtual bool CommitRule() = 0;
	virtual void SetupControls() = 0;
	virtual bool Validate() = 0;

	
	CString      m_RuleName;
	CComboBox    m_noSexValueCB;

	CPropertySheet m_dlgPartSheet;
	CRulePartPage  m_buyrule;
	CRulePartPage m_sellrule;	
		

	DECLARE_MESSAGE_MAP()
};

#endif