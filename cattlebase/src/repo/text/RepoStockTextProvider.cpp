#include "stdafx.h"
#include "../text/RepoStockTextProvider.h"
#include "../compare/RepoStockComparator.h"

CString RepoStockTextProvider::GetDataText(RepoStock *pData,UINT memberId)
{
	CString sText;
   	switch(memberId)
	{
		case REPOSTOCK_MEMBER_NAME:
			sText =  pData->GetStockName();
			break;
		case REPOSTOCK_MEMBER_CODE:
			sText =  pData->GetStockCode();
			break;
		case REPOSTOCK_MEMBER_MYPRICE:
			sText =  pData->GetMyPerKgPrice().ToString();
			break;
		case REPOSTOCK_MEMBER_PREDEFSEX:
			sText =  (LPCSTR)pData->GetPredefSex();
			break;	
		default:
			ASSERT(0);
			break;
	}	
	return sText;
}
