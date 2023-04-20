#ifndef __INVOICEDISPATCHER_H__
#define __INVOICEDISPATCHER_H__

#include "../datatypes/Invoice.h"
#include "../global_arrays.h"
#include "../mainfrm_ctrl.h"

class InvoiceDispatcher
{
public:
	static void Dispatch(Invoice *pInvoice);
};


#endif