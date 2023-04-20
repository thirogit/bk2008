#ifndef __INVOICEDATATEXTPROVIDER_H__
#define __INVOICEDATATEXTPROVIDER_H__

#include "BaseTextProvider.h"
#include "../datatypes/InvoiceDataArray.h"

class InvoiceDataTextProvider : public BaseTextProvider<Invoice>
{
public:
	virtual CString GetDataText(Invoice *pData,UINT memberID);

};

#endif