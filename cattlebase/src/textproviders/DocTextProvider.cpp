#include "stdafx.h"
#include "DocTextProvider.h"
#include "../consts.h"
#include "../datatypes/DocDataComparator.h"
#include "../classes/IntString.h"
#include "../datatypes/VATInvoice.h"

CString DocTextProvider::GetDataText(Doc *pData,UINT memberID)
{
	CString retStr;
   	switch(memberID)
	{
		case DOCS_SORT_BY_NUMBER:
			retStr = pData->GetDocNoStr();
			break;
		case DOCS_SORT_BY_DOCDATE:
			retStr = pData->GetDocDate().GetDayDate();
			break;
		case DOCS_SORT_BY_HENT:
			retStr = pData->GetHent()->GetComboItemText();
			break;
		case DOCS_SORT_BY_LOADDATE:
			retStr = pData->GetLoadDate().GetDayDate();
			break;
		case DOCS_SORT_BY_PLATENO:
			retStr = pData->GetPlateNo();
			break;
		case DOCS_SORT_BY_COWCOUNT:
			retStr = (LPCSTR)CIntString(pData->GetCowCount());
			break;
		case DOCS_SORT_BY_INVOICE:
			if(pData->HasInvoice())
				retStr = pData->GetInvoice()->GetInvoiceNoStr();			
			else 
				retStr = szMissing;
			break;
		case DOCS_SORT_BY_EXTRAS:
				retStr = pData->GetExtras();
				retStr.Replace('\n',';');
				retStr.Remove('\r');
			break;
		case DOCS_MEMBER_AGENT:
			{
				Agent* pAgent = pData->GetAgent();
				if(pAgent)
					retStr = pAgent->GetAgentCode();
				else
					retStr = '-';
			}
			break;
		default:
			ASSERT(0);
			break;
	}	
	return retStr;
}
