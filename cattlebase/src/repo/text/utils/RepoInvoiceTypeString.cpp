#include "stdafx.h"
#include "RepoInvoiceTypeString.h"

RepoInvoiceTypeString::RepoInvoiceTypeString(RepoInvoiceType invoiceType)
{
	switch(invoiceType)
	{
		case Invoice_Lump:
			m_sRepoInvoiceType.LoadString(IDS_INVOICETYPELUMP);
			break;
		case Invoice_Regular:
			m_sRepoInvoiceType.LoadString(IDS_INVOICETYPEREGULAR);
			break;
		
	}

}
	
RepoInvoiceTypeString::operator CString()  const
{
	return m_sRepoInvoiceType;
}
RepoInvoiceTypeString::operator LPCSTR() const
{
	return (LPCSTR)m_sRepoInvoiceType;
}

const CString& RepoInvoiceTypeString::ToString()
{
	return m_sRepoInvoiceType;
}