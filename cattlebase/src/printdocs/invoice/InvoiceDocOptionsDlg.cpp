#include "stdafx.h"
#include "InvoiceDocOptionsDlg.h"
#include "InvoiceDocOptions.h"
#include "../../Mustache/Mustache.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CInvoiceDocOptionsDlg, CCustomPrintDocOptDlg)


BEGIN_MESSAGE_MAP(CInvoiceDocOptionsDlg, CCustomPrintDocOptDlg)
	

END_MESSAGE_MAP()



CInvoiceDocOptionsDlg::CInvoiceDocOptionsDlg(CWnd *pParent/* = NULL*/) : 
	CCustomPrintDocOptDlg(IDD,pParent),
	m_PKWiU("[0-9]{1,2}((\\.[0-9]{1,2}){1,3}(-[0-9]{2}\\.[0-9]{1,2})?)?")
{
}

void CInvoiceDocOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomPrintDocOptDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PKWIU,m_PKWiU);			
	DDX_Text(pDX,IDC_NAZWALICZEB1,m_nazwaLiczeb1);
	DDX_Text(pDX,IDC_NAZWALICZEB2_4,m_nazwaLiczeb2);
	DDX_Text(pDX,IDC_NAZWALICZEBRESZTA,m_nazwaLiczebPozost);
	DDX_Text(pDX,IDC_INVOICEEXATRASTEMPLATE,m_sExtrasTemplate);
	DDX_Control(pDX,IDC_PRICEPERKGFORMAT, m_PricePerKgFormatCtrl);
	DDX_Text(pDX,IDC_INVOICETITLE,m_sTitle);
	DDX_Text(pDX,IDC_INVOICEENTRYTEXTFORMAT,m_sEntryFormat);
}
BOOL CInvoiceDocOptionsDlg::OnInitDialog()
{
	CCustomPrintDocOptDlg::OnInitDialog();

	InvoiceOptions invoiceOptions;
	invoiceOptions.Load(GetCfgItemID());

	m_nazwaLiczeb1 = invoiceOptions.GetNumeralName1();
	m_nazwaLiczeb2 = invoiceOptions.GetNumeralName2_4();
	m_nazwaLiczebPozost = invoiceOptions.GetNumeralNameOther();
	m_sExtrasTemplate = invoiceOptions.GetExtrasTemplate();
	m_sTitle = invoiceOptions.GetTitle();
	m_PricePerKgFormatCtrl.SetFloatFormat(invoiceOptions.GetPricePerKgFormat());
	m_sEntryFormat = invoiceOptions.GetEntryFormat();
	UpdateData(FALSE);
//	m_PKWiU.SetWavyLineOnError(false);
	m_PKWiU.SetWindowText((LPCSTR)invoiceOptions.GetPKWIU());
	return TRUE;
}

void CInvoiceDocOptionsDlg::Save()
{
	UpdateData();
	CString sPKWIU;
	m_PKWiU.GetWindowText(sPKWIU);
	InvoiceOptions invoiceOptions;
	invoiceOptions.SetPKWIU(sPKWIU);
	invoiceOptions.SetNumeralName1(m_nazwaLiczeb1);
	invoiceOptions.SetNumeralName2_4(m_nazwaLiczeb2);
	invoiceOptions.SetNumeralNameOther(m_nazwaLiczebPozost);
	invoiceOptions.SetExtrasTemplate(m_sExtrasTemplate);
	invoiceOptions.SetPricePerKgFormat(m_PricePerKgFormatCtrl.GetFloatFormat());
	invoiceOptions.SetTitle(m_sTitle);
	invoiceOptions.SetEntryFormat(m_sEntryFormat);
	invoiceOptions.Save(GetCfgItemID());
}

bool  CInvoiceDocOptionsDlg::Validate()
{
	UpdateData();

	try
	{
		MustacheContext context = Mustache::Parse(m_sEntryFormat);
	}
	catch(MustacheException* e)
	{
		AfxMessageBox(e->GetError(),MB_OK);
		e->Delete();
		GetDlgItem(IDC_INVOICEENTRYTEXTFORMAT)->SetFocus();
		return false;
	}

	return true;
}




