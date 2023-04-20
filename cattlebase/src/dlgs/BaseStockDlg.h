#ifndef __BASESTOCKDLG_H__
#define __BASESTOCKDLG_H__

#include "../combo/RuleCombo.h"
#include "../FilterEdit/FilterEdit.h"
#include "../combo/SexCombo.h"

class CBaseStockDlg : public  CDialog 
{
public:
	CBaseStockDlg (CWnd* pParent = NULL);
	enum { IDD = IDD_STOCK };
protected:
	CFilterEdit	  m_StockCode;
	CString		  m_StockName;
	CRuleCombo    m_RuleListCombo;
	CFilterEdit   m_StockPricePerKg;	
	CSexCombo	  m_predefSexCombo;
	CString	      m_OfficialCode;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual bool CommitStock() = 0;
	virtual void CancelStock() = 0;
	virtual CString GetDlgCaption() = 0;
	bool Validate();
protected:	
	afx_msg void OnAddRule();
	DECLARE_MESSAGE_MAP()
};
#endif