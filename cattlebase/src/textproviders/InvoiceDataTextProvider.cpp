#include "stdafx.h"
#include "InvoiceDataTextProvider.h"
#include "../consts.h"
#include "../datatypes/InvoiceDataComparator.h"


CString InvoiceDataTextProvider::GetDataText(Invoice *pData,UINT memberID)
{
	Invoice *pInv = pData;

	CString retStr;

	Money iVal;

	switch(memberID)
	{
		case INVS_SORT_BY_NUMBER:
			retStr = pInv->GetInvoiceNoStr();			
		break;
		case INVS_SORT_BY_INVDATE:
			retStr = pInv->GetInvoiceDate().GetDayDate();			
		break;
		case INVS_SORT_BY_CORRECT:					
			if(pInv->HasCorrect())
				retStr = pInv->GetCorrect()->GetInvoiceNoStr();
			else
				retStr = "-";			
		break;		
		case INVS_SORT_BY_HENT:
			retStr = pInv->GetInvoiceHent()->GetComboItemText();
		break;
		case INVS_SORT_BY_QTY:
			retStr.Format("%d",pInv->GetCowQty());
		break;
		case INVS_SORT_BY_VAT:
			retStr.Format("%.2f%%",pInv->GetVATRate()*100.0);
		break;
		case INVS_SORT_BY_PAYWAY:
			switch(pInv->GetPayWay())
			{
			case Invoice::PAY_CASH:
				retStr = PayWayNames[1];
			break;
			case Invoice::PAY_TRANS:
				retStr = PayWayNames[0];
			break;
			}
		break;
		case INVS_SORT_BY_CUSTOMNO:
			retStr = pInv->GetCustomNumber();
		break;
		
		case INVS_SORT_BY_GROSSVALUE:
			retStr =  pInv->GetInvoiceGrossValue().ToString();
		break;
		
		case INVS_SORT_BY_GROSSVALUEAFTERCORRECT:
			if(pInv->HasCorrect())
			{
				retStr = pInv->GetBruttoValueAfterCorrect().ToString();
			}
			else retStr =  "-";
		break;
		case INVS_SORT_BY_PAID:
			if(!pInv->IsPaid())	retStr =  g_szYesNo[FALSE];
			else retStr = pInv->GetPaidDate().GetDayDate();
		break;
		case INVS_SORT_BY_EXTRAS:
			retStr = pInv->GetExtras();
			retStr.Replace('\n',';');
			retStr.Remove('\r');
			break;
		case INVS_SORT_BY_PAYDUEDAYSDATE:
			retStr = pInv->GetPayDueDate().GetDayDate();
			break;
	
	default:
		ASSERT(0);
		break;
	}
	return retStr;
}
