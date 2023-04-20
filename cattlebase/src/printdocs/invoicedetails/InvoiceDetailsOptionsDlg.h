#ifndef __INVOICEDETAILSOPTIONSDLG_H__
#define __INVOICEDETAILSOPTIONSDLG_H__

#include "../CustomPrintDocOptDlg.h"
#include "../../Ctrls/floatformat/FloatFormatCtrl.h"

class CInvoiceDetailsOptionsDlg : public  CCustomPrintDocOptDlg 
{
	DECLARE_DYNCREATE(CInvoiceDetailsOptionsDlg)
public:
	CInvoiceDetailsOptionsDlg (CWnd *pParent = NULL);
	enum { IDD = IDD_INVOICEDETAILSOPTS };
	virtual UINT GetTemplateID() { return IDD; };
	virtual bool Validate();
	virtual void Save();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	void SetPricePerKgFormat(const FloatFormat& fmt);
	FloatFormat GetPricePerKgFormat() const;
	
	CFloatFormatCtrl m_PricePerKgFormatCtrl;
	
	DECLARE_MESSAGE_MAP()
};


#endif
