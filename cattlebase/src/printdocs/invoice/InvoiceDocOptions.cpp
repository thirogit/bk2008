#include "stdafx.h"
#include "InvoiceDocOptions.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void InvoiceOptions::Save(UINT uInvoiceCfgItem)
{
	PrintCfg().SetString(uInvoiceCfgItem,PKWIU,GetPKWIU());
	PrintCfg().SetString(uInvoiceCfgItem,NUMERALNAME1,GetNumeralName1());
	PrintCfg().SetString(uInvoiceCfgItem,NUMERALNAME2_4,GetNumeralName2_4());
	PrintCfg().SetString(uInvoiceCfgItem,NUMERALNAMEOTHER,GetNumeralNameOther());
	PrintCfg().SetString(uInvoiceCfgItem,TITLE,GetTitle());
	PrintCfg().SetString(uInvoiceCfgItem,EXTRASTEMPLATE,GetExtrasTemplate());
	PrintCfg().SetString(uInvoiceCfgItem,ENTRYFORMAT,GetEntryFormat());

	
		
	PrintCfg().SetNum(uInvoiceCfgItem,PRICEPERKGDECIMALPT,GetPricePerKgFormat().GetDecimalPoint());
	PrintCfg().SetNum(uInvoiceCfgItem,PRICEPERKGPRECISION,GetPricePerKgFormat().GetPrecision());

}

void InvoiceOptions::Load(UINT uInvoiceCfgItem)
{
	SetPKWIU(PrintCfg().GetString(uInvoiceCfgItem,PKWIU));
	SetNumeralName1(PrintCfg().GetString(uInvoiceCfgItem,NUMERALNAME1));
	SetNumeralName2_4(PrintCfg().GetString(uInvoiceCfgItem,NUMERALNAME2_4));
	SetNumeralNameOther(PrintCfg().GetString(uInvoiceCfgItem,NUMERALNAMEOTHER));
	SetTitle(PrintCfg().GetString(uInvoiceCfgItem,TITLE));
	SetExtrasTemplate(PrintCfg().GetString(uInvoiceCfgItem,EXTRASTEMPLATE));
	SetEntryFormat(PrintCfg().GetString(uInvoiceCfgItem,ENTRYFORMAT));
	
	SetPricePerKgFormat(FloatFormat(PrintCfg().GetNum(uInvoiceCfgItem,PRICEPERKGPRECISION),
			                        (char)PrintCfg().GetNum(uInvoiceCfgItem,PRICEPERKGDECIMALPT)));
}

const CString& InvoiceOptions::GetPKWIU() const
{
	return m_sPKWIU;
}

const CString& InvoiceOptions::GetNumeralName1() const
{
	return m_sNumeralName1;
}

const CString& InvoiceOptions::GetNumeralName2_4() const
{
	return m_sNumeralName2_4;
}

const CString& InvoiceOptions::GetNumeralNameOther() const
{
	return m_sNumeralNameOther;
}

const CString& InvoiceOptions::GetExtrasTemplate() const
{
	return m_sExtrasTemplate;
}

const CString& InvoiceOptions::GetEntryFormat() const
{
	return m_sEntryFormat;
}

const CString& InvoiceOptions::GetTitle() const
{
	return m_sTitle;
}

void InvoiceOptions::SetPKWIU(const CString& sPKWIU)
{
	m_sPKWIU = sPKWIU;
}

void InvoiceOptions::SetNumeralName1(const CString& sNumeralName1)
{
	m_sNumeralName1 = sNumeralName1;
}

void InvoiceOptions::SetNumeralName2_4(const CString& sNumeralName2_4)
{
	m_sNumeralName2_4 = sNumeralName2_4;
}

void InvoiceOptions::SetNumeralNameOther(const CString& sNumeralNameOther)
{
	m_sNumeralNameOther = sNumeralNameOther;
}

void InvoiceOptions::SetExtrasTemplate(const CString& sExtrasTemplate)
{
	m_sExtrasTemplate = sExtrasTemplate;
}

void InvoiceOptions::SetEntryFormat(const CString& sEntryFormat)
{
	m_sEntryFormat = sEntryFormat;
}

void InvoiceOptions::SetTitle(const CString& sTitle)
{
	m_sTitle = sTitle;
}


const FloatFormat& InvoiceOptions::GetPricePerKgFormat() const
{
	return m_PricePerKgFormat;
}

void InvoiceOptions::SetPricePerKgFormat(const FloatFormat& fmt)
{
	m_PricePerKgFormat = fmt;
}