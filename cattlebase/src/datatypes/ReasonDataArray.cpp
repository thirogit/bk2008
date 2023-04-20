#include "stdafx.h"
#include "ReasonDataArray.h"

Reason *CReasonDataArray::GetReasonDataByReasonId(int reasonid)
{
	Reason *temp;
	for(int i = 0;i < GetSize();i++)
	{
		temp = (Reason*)GetAt(i); 
        if(temp->GetId() == reasonid) return temp;
	}
	return NULL;
}		


