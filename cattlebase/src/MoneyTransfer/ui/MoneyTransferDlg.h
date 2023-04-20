#ifndef __MONEYTRANSFERDLG_H__
#define __MONEYTRANSFERDLG_H__

#include "../../datatypes/InvoiceDataArray.h"
#include "MoneyTransferListCtrl.h"
#include "../../ctrls/enteredit/EnterEdit.h"
#include "../../ctrls/CurrencyCombo.h"
#include "../ExchangeRate.h"
#include "../formatters/Formatter.h"
#include "../../ctrls\curdateedit\curdateedit.h"

class CMoneyTransferDlg : public CDialog
{
public:
	enum { IDD = IDD_MONEYTRANSFER };

	CMoneyTransferDlg(CInvoiceDataArray& invoices,CWnd* pParent = NULL);   
	virtual ~CMoneyTransferDlg();	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	void CheckIfOK();
	void InitMoneyTransferFormatsCombo();
	void Update();
	void SetTotal();

	bool SetPaidDate(PtrFlushArray<MoneyTransferEntry>& moneyTransfers);

	CComboBox m_moneyTransferFormatsCb;
	CCurrencyCombo m_CurrenciesCb;
	CEnterEdit m_ExchangeRateEdit;
	CMoneyTransferListCtrl m_moneyTransferList;
	PtrFlushArray<MoneyTransferEntry> m_moneyTransferEntries;
	PtrFlushArray<Formatter> m_moneyTransferFormatters;;
	CCurDateEdit m_TransferDate;

	ExchangeRate m_exchangeRate;

	afx_msg void OnCurrencyChange();	
	afx_msg void OnDeleteMoneyTransferEntry();
	afx_msg void OnExchangeRate(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#endif