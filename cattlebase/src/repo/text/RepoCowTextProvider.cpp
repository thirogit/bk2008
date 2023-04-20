#include "stdafx.h"
#include "RepoCowTextProvider.h"
#include "../compare/RepoCowComparator.h"

CString RepoCowTextProvider::GetDataText(RepoCow *pData,UINT memberId)
{
	CString sText;
   	switch(memberId)
	{
		case REPOCOW_MEMBER_COWNO:
			sText = pData->GetCowNo();
			break;
		case REPOCOW_MEMBER_COWSEX:
			sText = pData->GetSex();
			break;
		case REPOCOW_MEMBER_STOCK:
			sText = pData->GetStockCode();
			break;
		case REPOCOW_MEMBER_WEIGHT:
			sText = pData->GetWeight().ToString();
			break;
		case REPOCOW_MEMBER_PRICE:
			sText = pData->GetPrice().ToString();
			break;
		default:
			ASSERT(0);
			break;
	}	
	return sText;
}
