#include "stdafx.h"
#include "InvoiceDispatcher.h"



void InvoiceDispatcher::Dispatch(Invoice *pInvoice)
{

	switch(pInvoice->GetBuyOrSell())
	{
	case BUY:
		switch(pInvoice->GetRealInvoiceType())
		{
		case IN_INV_RR:
			ary_rr_invs.Add(pInvoice);
			UpdateGrid(RRINVSGRID);
			break;
		case IN_INV_VAT:
			ary_buy_invs.Add(pInvoice);
			UpdateGrid(BUYINVSGRID);		
			break;
		}
	break;

	case SELL:
		ary_sell_invs.Add(pInvoice);
		UpdateGrid(SELLINVSGRID);			
	break;
	}	


}
