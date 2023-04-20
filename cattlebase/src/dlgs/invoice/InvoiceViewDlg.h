#ifndef __INVOICEVIEWDLG_H__
#define __INVOICEVIEWDLG_H__

#include "../../datatypes/Invoice.h"
#include "../../ctrls/CowPriceViewList/CowPriceViewList.h"

class CInvoiceViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CInvoiceViewDlg)

public:
	CInvoiceViewDlg(Invoice *invoice,CWnd* pParent = NULL);   
	virtual ~CInvoiceViewDlg();
	enum { IDD = IDD_INVOICEVIEW };

private:
	Invoice *m_invoice;
	CCowPriceViewList m_viewprices;
	void ComputeTotalValues();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedFindPassports();
	DECLARE_MESSAGE_MAP()


};


#endif