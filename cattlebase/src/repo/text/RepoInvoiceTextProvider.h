#ifndef __REPOINVIOCETEXTPROVIDER_H__
#define __REPOINVOICETEXTPROVIDER_H__

#include "../../textproviders/BaseTextProvider.h"
#include "../RepoInvoice.h"

class RepoInvoiceTextProvider : public BaseTextProvider<RepoInvoice>
{
public:
	virtual CString GetDataText(RepoInvoice *pData,UINT memberId);

};

#endif