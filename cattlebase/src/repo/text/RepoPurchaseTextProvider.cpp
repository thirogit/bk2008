#include "stdafx.h"
#include "../text/RepoPurchaseTextProvider.h"
#include "../compare/RepoPurchaseComparator.h"
#include "../../classes/IntString.h"
#include "../../consts.h"

CString RepoPurchaseTextProvider::GetDataText(RepoPurchase *pData,UINT memberId)
{
	CString sText;
   	switch(memberId)
	{
		case REPOPURCHASE_MEMBER_AGENTCODE:
			sText =  pData->GetAgentCode();
			break;
		case REPOPURCHASE_MEMBER_PLATENO:
			sText =  pData->GetPlateNo();
			break;
		case REPOPURCHASE_MEMBER_STARTDATE:
			sText =  pData->GetStartDate().GetDateInFmt(CDateTime::YYYYdashMMdashDD_HHcolonMI);
			break;
		case REPOPURCHASE_MEMBER_ENDDATE:
			sText =  pData->GetEndDate().GetDateInFmt(CDateTime::YYYYdashMMdashDD_HHcolonMI);
			break;	
		case REPOPURCHASE_MEMBER_COWCOUNT:
			sText = (CString)CIntString(pData->GetCowCount());
			break;
		case REPOPURCHASE_MEMBER_WASDOWNLOADED:
			sText =  g_szYesNo[pData->GetWasDownloaded() ? 1 : 0];
			break;
		default:
			ASSERT(0);
			break;
	}	
	return sText;
}

	