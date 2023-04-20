#include "stdafx.h"
#include "LastInvoicesTip.h"
#include "../../global_arrays.h"
#include "../../classes/match/InvMatcher.h"
#include "../../classes/forallactions/SelectorAction.h"
#include "../../datatypes/InvoiceDataComparator.h"
#include "../../datalimits.h"

CLastInvoicesTip::CLastInvoicesTip()
{
	


}

BOOL CLastInvoicesTip::Show(InvoiceTypes invoiceType,CPoint &pt)
{
	if(m_lastInvoices.GetSize() == 0)
	{
		CInvoiceDataArray destInvArray;
		CInvoiceDataArray *pSrcInvArray;

		switch(invoiceType)
		{
		case IN_INV_RR:
			pSrcInvArray = &ary_rr_invs;
			break;
		case IN_INV_VAT:
			pSrcInvArray = &ary_buy_invs;
			break;
		case OUT_INV_VAT:
			pSrcInvArray = &ary_sell_invs;
			break;
		default:
			ASSERT(FALSE);
		}
	

		destInvArray.Append(*pSrcInvArray);
		destInvArray.SortBy(INVS_SORT_BY_NUMBER,false);

		for(int i = 0;i < destInvArray.GetSize() && i < LAST_INV_COUNT;i++)
			m_lastInvoices.Add(destInvArray[i]);
	}

	if(m_lastInvoices.GetSize() == 0) return FALSE;

	CString tipText;

	tipText.Format(IDS_LASTINVOICESFMT,m_lastInvoices.GetSize());

	CInvoiceDataArray::ArrayIterator it = m_lastInvoices.iterator();
	Invoice *pInv = NULL;
	while(it.hasNext())
	{
		pInv = *(it++);
		tipText.AppendFormat("%s - %s - %s%c",(LPCSTR)pInv->GetInvoiceNoStr(),
											  (LPCSTR)pInv->GetInvoiceDate().GetDayDate(),
											  (LPCSTR)pInv->GetCustomNumber(),
											  it.hasNext() ? '\n' : '\0');
	}

	return CTipWnd::Show(pt,(LPCSTR)tipText);
}
