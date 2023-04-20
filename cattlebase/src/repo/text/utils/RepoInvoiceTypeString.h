#ifndef __REPOINVOICETYPESTRING_H__
#define __REPOINVOICETYPESTRING_H__


#include "../../RepoInvoiceType.h"

class RepoInvoiceTypeString 
{
public:
	RepoInvoiceTypeString(RepoInvoiceType invoiceType);
	operator CString() const;
	operator LPCSTR() const;
	const CString& ToString();
private:

	CString m_sRepoInvoiceType;
	

};

#endif