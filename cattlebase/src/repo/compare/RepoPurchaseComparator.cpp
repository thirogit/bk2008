#include "stdafx.h"
#include "RepoPurchaseComparator.h"


COMPARE_RESULT RepoPurchaseComparator::CompareBy(UINT dataMember,RepoPurchase* DataLeft,RepoPurchase* DataRight)
{
	RepoPurchase *left = DataLeft,*right = DataRight;
	
	switch(dataMember)
	{
		default:
			ASSERT(0);
		case REPOPURCHASE_MEMBER_AGENTCODE:
			return PolishStringCompare(left->GetAgentCode(),right->GetAgentCode());
		case REPOPURCHASE_MEMBER_PLATENO:
			return PolishStringCompare(left->GetPlateNo(),right->GetPlateNo());
		case REPOPURCHASE_MEMBER_STARTDATE:
			return CompareDateTime(left->GetStartDate(),right->GetStartDate());
		case REPOPURCHASE_MEMBER_ENDDATE:
			return CompareDateTime(left->GetEndDate(),right->GetEndDate());
		case REPOPURCHASE_MEMBER_COWCOUNT:
			return CompareInt(left->GetCowCount(),right->GetCowCount());
		case REPOPURCHASE_MEMBER_WASDOWNLOADED:
			return CompareBool(left->GetWasDownloaded(),right->GetWasDownloaded());
	}
	ASSERT(0);
	return COMPARE_LESS;
}
