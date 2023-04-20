#include "stdafx.h"
#include "InvoiceDetailsOptionsDlg.h"
#include "../PrintDocsCfg.h"
#include "InvoiceDetails.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CInvoiceDetailsOptionsDlg, CCustomPrintDocOptDlg)


BEGIN_MESSAGE_MAP(CInvoiceDetailsOptionsDlg, CCustomPrintDocOptDlg)
	

END_MESSAGE_MAP()



CInvoiceDetailsOptionsDlg::CInvoiceDetailsOptionsDlg(CWnd *pParent/* = NULL*/) : 
	CCustomPrintDocOptDlg(IDD,pParent)	
{
}

void CInvoiceDetailsOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomPrintDocOptDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PRICEPERKGFORMAT, m_PricePerKgFormatCtrl);
	
}
BOOL CInvoiceDetailsOptionsDlg::OnInitDialog()
{
	CCustomPrintDocOptDlg::OnInitDialog();

	UINT uInvoiceDetailsCfgItem = GetCfgItemID();
	SetPricePerKgFormat(FloatFormat(PrintCfg().GetNum(uInvoiceDetailsCfgItem,CInvoiceDetailsDoc::PRICEPERKGPRECISION),
			                        (char)PrintCfg().GetNum(uInvoiceDetailsCfgItem,CInvoiceDetailsDoc::PRICEPERKGDECIMALPT)));
	
	return TRUE;
}

void CInvoiceDetailsOptionsDlg::Save()
{
	UpdateData();

	UINT uInvoiceDetailsCfgItem = GetCfgItemID();
	
	PrintCfg().SetNum(uInvoiceDetailsCfgItem,CInvoiceDetailsDoc::PRICEPERKGDECIMALPT,GetPricePerKgFormat().GetDecimalPoint());
	PrintCfg().SetNum(uInvoiceDetailsCfgItem,CInvoiceDetailsDoc::PRICEPERKGPRECISION,GetPricePerKgFormat().GetPrecision());
}

bool  CInvoiceDetailsOptionsDlg::Validate()
{
	return true;
}

void  CInvoiceDetailsOptionsDlg::SetPricePerKgFormat(const FloatFormat& fmt)
{
	m_PricePerKgFormatCtrl.SetFloatFormat(fmt);
}

FloatFormat CInvoiceDetailsOptionsDlg::GetPricePerKgFormat() const
{
	return m_PricePerKgFormatCtrl.GetFloatFormat();
}



