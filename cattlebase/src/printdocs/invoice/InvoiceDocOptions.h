#ifndef __INVOICEDOCOPTIONS_H__
#define __INVOICEDOCOPTIONS_H__

#include "../PrintDocsCfg.h"
#include "../../classes/format/FloatFormat.h"

class InvoiceOptions
{
public:
	enum INVOICE_OPTIONS_INDEX
	{
		PKWIU = CPrintCfg::CFG_PRINT_OPTION_MAX,
		NUMERALNAME1,
		NUMERALNAME2_4,
		NUMERALNAMEOTHER,
		TITLE,
		EXTRASTEMPLATE,
		ENTRYFORMAT,
		PRICEPERKGDECIMALPT,
		PRICEPERKGPRECISION
	};


	void Save(UINT uInvoiceCfgItem);
	void Load(UINT uInvoiceCfgItem);

	const CString& GetPKWIU() const;
	const CString& GetNumeralName1() const;
	const CString& GetNumeralName2_4() const;
	const CString& GetNumeralNameOther() const;
	const CString& GetExtrasTemplate() const;
	const CString& GetEntryFormat() const;
	const CString& GetTitle() const;
	const FloatFormat& GetPricePerKgFormat() const;

	void SetPKWIU(const CString& sPKWIU);
	void SetNumeralName1(const CString& sNumeralName1);
	void SetNumeralName2_4(const CString&sNumeralName2_4);
	void SetNumeralNameOther(const CString& sNumeralNameOther);
	void SetExtrasTemplate(const CString& sExtrasTemplate);
	void SetEntryFormat(const CString& sEntryFormat);
	void SetTitle(const CString& sTitle);
	void SetPricePerKgFormat(const FloatFormat& fmt);


private:
	CString m_sPKWIU;
	CString m_sNumeralName1;
	CString m_sNumeralName2_4;
	CString m_sNumeralNameOther;
	CString m_sExtrasTemplate;
	CString m_sEntryFormat;
	CString m_sTitle;
	FloatFormat m_PricePerKgFormat;
};

#endif
