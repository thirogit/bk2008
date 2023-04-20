#ifndef __INVOICEDOCOPTIONSDLG_H__
#define __INVOICEDOCOPTIONSDLG_H__

#include "../CustomPrintDocOptDlg.h"
#include "../../FilterEdit/FilterEdit.h"
#include "../../Ctrls/floatformat/FloatFormatCtrl.h"

class CInvoiceDocOptionsDlg : public  CCustomPrintDocOptDlg 
{
	DECLARE_DYNCREATE(CInvoiceDocOptionsDlg)
public:
	CInvoiceDocOptionsDlg (CWnd *pParent = NULL);
	enum { IDD = IDD_ALLINVOPT };
	virtual UINT GetTemplateID() { return IDD; };
	virtual bool Validate();
	virtual void Save();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	
	CString m_nazwaLiczeb1;
	CString m_nazwaLiczeb2;
	CString m_nazwaLiczebPozost;
	CString m_sExtrasTemplate;
	CFilterEdit m_PKWiU;
	CString m_sEntryFormat;
	CFloatFormatCtrl m_PricePerKgFormatCtrl;
	CString m_sTitle;
	
	DECLARE_MESSAGE_MAP()
};


#endif
