#if !defined(__LASTINVOICESTIP_H__)
#define __LASTINVOICESTIP_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "../TipWnd.h"
#include "../../datatypes/InvoiceTypes.h"
#include "../../datatypes/InvoiceDataArray.h"

#define LAST_INV_COUNT 5

class CLastInvoicesTip : public CTipWnd
{
public:
	CLastInvoicesTip();
	BOOL Show(InvoiceTypes invoiceType,CPoint &pt);
private:
	CInvoiceDataArray m_lastInvoices;

};



#endif 
