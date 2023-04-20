#ifndef __BIGINVOICEDLG_H__
#define __BIGINVOICEDLG_H__

#include "../../ctrls/XColorStatic.h"
#include "../../ctrls/Rulelistbox/RuleListbox.h"
#include "../../ctrls/cowpricelist/StockPricePerKgList.h"
#include "../../ctrls/cowpricelist/StockStatList.h"
#include "BaseInvoiceDlg.h"

class CBigInvoiceDlg : public CBaseInvoiceDlg
{	
public:

	CBigInvoiceDlg(CowPriceInvoiceSession* m_pInvoiceSession,CWnd* pParent = NULL);   
	virtual ~CBigInvoiceDlg();
	enum { IDD = IDD_BIGINVOICE };

protected:

	BOOL m_Rule4All;
	CRuleListBox m_RuleList;

	BOOL m_bCustomPricePerKg;	
    CEnterEdit m_CustomPricePerKg;
	BOOL m_useMyPerKgPrice;
	CXColorStatic m_useMyPerKgPriceClrLbl;
	CXColorStatic m_useCustomPricePerKgClrLbl;

	CStockPricePerKgList m_stockpricelist;
	CStockStatList m_statList;
	
	UINT m_termPriceLabel;
	UINT m_termStockLabel;
	UINT m_termWeightLabel;

	static const GRIDHEADERS szCowPriceListCols[];

	CEnterEdit m_totalweight;	
	CCheckableGroupBox m_settotalweight;
	BOOL m_bSetTotalWeight;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
	virtual void CreateNaviTab();
	
	virtual void TotalNettoChecked();
	virtual void TotalBruttoChecked();
	virtual void OnCowPriceSelected(CowPriceData* pCowPrice);
	virtual void UpdateTotal();
	void PutTotalWeight();
	void SetTotalWeight(const Weight& totalWeight);
	void SetDlgItemResText(UINT nID,UINT resID);
	void SetTermValue(UINT nValueLabelID,LPCSTR pszValueText,UINT nSetBtnID,BOOL bEnable);
	
	afx_msg void OnRule4All();
	afx_msg void OnRuleChange();
	afx_msg void OnEndLabelEditStockPrice (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditCustomPricePerKgComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditTotalWeightComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddRule();
	afx_msg void OnCustomPricePerKg();
	afx_msg void OnRuleEdit();
	afx_msg void OnUseMyPerKgPrice();
	afx_msg void OnSetTermWeight();
	afx_msg void OnSetTermStock();
	afx_msg void OnSetTermPrice();
	afx_msg void OnSetTermWeightAll();
	afx_msg void OnSetTermStockAll();
	afx_msg void OnSetTermPriceAll();
	afx_msg void OnCheckTotalWeight();
	DECLARE_MESSAGE_MAP()

	
};

#endif