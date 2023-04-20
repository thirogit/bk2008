#ifndef __BASEINVOICEDLG_H__
#define __BASEINVOICEDLG_H__

#include "../../maskededit\OXMaskedEdit.h"
#include "../../FilterEdit/FilterEdit.h"
#include "../../ctrls/ExtrasMemo.h"
#include "../../datatypes/CowPriceData.h"
#include "../../ctrls/cowpricelist/cowpricelist.h"
#include "../../ctrls/enteredit/enteredit.h"
#include "../../combo/VATCombo.h"
#include "../../ctrls/curdateedit/curdateedit.h"
#include "../../imgbutton/imagebutton.h"
#include "../../combo/WarnPayWayCombo.h"
#include "../../ctrls/checkabegroup/checkablegroupbox.h"
//#include "../ctrls/Rulelistbox/RuleListbox.h"
//#include "../ctrls/cowpricelist/StockPricePerKgList.h"
#include "../../ctrls/DocListCtrl.h"
//#include "../ctrls/cowpricelist/StockStatList.h"
#include "../../ctrls/invoice/LastInvoicesTip.h"
#include "../../accessor/CowAccessor.h"
#include "../../datatypes/CowInvoiceEntry.h"
#include "../../datatypes/invoice/CowPriceInvoiceSession.h"
#include "InvoiceUISettings.h"

class CBaseInvoiceDlg : public CDialog
{	
public:

	CBaseInvoiceDlg(UINT nIdDlgTemplate,CowPriceInvoiceSession* m_pInvoiceSession,InvoiceUISettings* pUISettings,CWnd* pParent = NULL);   
	virtual ~CBaseInvoiceDlg();
	//enum { IDD = IDD_INVOICE };

protected:

	//BOOL m_Rule4All;
	//CRuleListBox m_RuleList;
	CWarnPaywayCombo m_payway;	
	CVATCombo m_vat;	
	CCurDateEdit m_invoicedate;	
	CExtrasMemo m_extras;		
	CEnterEdit m_totalnetto;	
	CCheckableGroupBox m_settotalnetto;
	BOOL m_bSetTotalNetto;
	CEnterEdit m_totalbrutto;	
	CCheckableGroupBox m_settotalbrutto;
	BOOL m_bSetTotalBrutto;		
	//BOOL m_bCustomPricePerKg;
    //CEnterEdit m_CustomPricePerKg;
	//BOOL m_useMyPerKgPrice;
	CString m_customNumber;

	CowPriceInvoiceSession* m_pInvoiceSession;
	InvoiceUISettings*		m_pUISettings;
	
	CDocListCtrl m_docList;

	//CStockPricePerKgList m_stockpricelist;
	//CStockStatList m_statList;
	
	CTabCtrl m_naviTab;
	int m_prevSelectedTab;
	CSpinButtonCtrl m_spnPayDueDays;
	CLastInvoicesTip m_lastInvTip;
	
	CCowPriceList m_pricelist;
	
	CImageButton m_btncancel;
	CImageButton m_btnaccept;
	
	CImageButton m_btnadditem;
	CImageButton m_btndelitem;

	CMenu m_priceMenu;

	CString m_sTransactionPlace;
	CCurDateEdit m_TransactionDate;

	//UINT m_termPriceLabel;
	//UINT m_termStockLabel;
	//UINT m_termWeightLabel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void SetInvoiceControls();
	
	virtual void InitInvoiceDialog();
		
	void UpdateCowPriceList();
	virtual void UpdateTotal();
	void OnOK();

	virtual void AddItem();
	virtual void DelItem();
	
	virtual bool CommitInvoice();
	virtual CString GetInvDlgCaption();
	virtual void TabChanged(int newTabIndex);
	void SetCurTab(int tabIndex);
	CRect GetTabRect();

	int GetSelectedPriceItem();

	//Money ComputeInvoiceTotal();
	//virtual NullInvoiceData* CreateNewInvoice();

	int GetCurrentTabIndex();

	void SetTotalNetto(const Money& totalnetto);
	void SetTotalBrutto(const Money& totalbrutto);

	void PutTotalNetto();
	void PutTotalBrutto();
	void PutTotalVAT();
	

	void UpdateItemCount();
	bool Validate();
	
	virtual void CreateNaviTab();
	virtual void AddNaviTab(int insertPos,CBaseListCtrl &tab,UINT tabID,UINT tabTextResID,bool bShow);
	
	
	void DeleteCustomPrice(CowPriceData *pCowPrice);
	void DeleteCustomStock(CowPriceData *pCowPrice);
	void DeleteCustomWeight(CowPriceData *pCowPrice);
	void DeleteForOneRow(void (CBaseInvoiceDlg::*pDeleteAction)(CowPriceData*));
	void DeleteForAllRows(void (CBaseInvoiceDlg::*pDeleteAction)(CowPriceData*));
	void OnTabChanged(int oldTabIndex,int newTabIndex);

	virtual void TotalNettoChecked();
	virtual void TotalBruttoChecked();
	virtual void OnCowPriceSelected(CowPriceData* pCowPrice);
	//void SetDlgItemResText(UINT nID,UINT resID);
	//void SetTermValue(UINT nValueLabelID,LPCSTR pszValueText,UINT nSetBtnID,BOOL bEnable);
	
	afx_msg void OnCheckTotalBrutto();
	//afx_msg void OnRule4All();
	//afx_msg void OnRuleChange();
	afx_msg void OnCheckTotalNetto();
	afx_msg void OnEndLabelEditCowPrice (NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnEndLabelEditStockPrice (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditComplete(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnAddRule();
	//afx_msg void OnCustomPricePerKg();
	afx_msg void OnVATChange();
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void MenuHandler(UINT nId);
	//afx_msg void OnRuleEdit();
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnUseMyPerKgPrice();
	afx_msg void OnAddItem();
	afx_msg void OnDelItem();
	//afx_msg void OnSetTermWeight();
	//afx_msg void OnSetTermStock();
	//afx_msg void OnSetTermPrice();
	//afx_msg void OnSetTermWeightAll();
	//afx_msg void OnSetTermStockAll();
	//afx_msg void OnSetTermPriceAll();
	afx_msg void OnCowPriceListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDocListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnCustomNumberFocus();
	afx_msg void OnCustomNumberLostFocus();
	afx_msg void OnSetTransactionPlace();
	
	DECLARE_MESSAGE_MAP()

	
};

#endif