#ifndef __RULEPARTDLG_H__
#define __RULEPARTDLG_H__

#include "../classes/types/Money.h"
#include "../datatypes/RulePart.h"
#include "../FilterEdit/FilterEdit.h"
// CRulePart dialog

class CRulePartPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CRulePartPage)

public:
	CRulePartPage(UINT nIDCaption);
	virtual ~CRulePartPage();
	enum { IDD = IDD_RULEPART };	
	void InitWith(const RulePart* pUseOnInit);
	bool ValidateFields(bool bVerbose);
	void UpdateRulePart(RulePart* pRulePart);
protected:	
	UINT		 m_LimWeight;	
	CFilterEdit	 m_RateBelowWeightXY;
	CFilterEdit  m_RateAboveWeightXY;	
	CFilterEdit	 m_RateBelowWeightXX;
	CFilterEdit  m_RateAboveWeightXX;
	CRegEx       m_REsigned;
	CRegEx       m_REunsigned;
	RulePart*	 m_pUseOnInit;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	bool ValidatePriceEdit(CEdit& priceEdit,bool bVerbose);
	Money GetPrice(CEdit& priceEdit);
	void SetControls(const RulePart* pRulePart);

	afx_msg void OnSameAsXY();

	DECLARE_MESSAGE_MAP()
};


#endif