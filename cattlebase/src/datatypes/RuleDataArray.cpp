#include "stdafx.h"
#include "RuleDataArray.h"

Rule* CRuleDataArray::GetPtrByRuleId(int ruleid)
{
	for(int i=0;i < GetSize();i++)
	{
		Rule *temp = (Rule*)GetAt(i);
		if(temp->GetId() == ruleid)	return temp;
		
	}
	return NULL;
}






