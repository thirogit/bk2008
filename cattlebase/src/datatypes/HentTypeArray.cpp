#include "stdafx.h"
#include "HentTypeArray.h"

HentType* CHentTypeArray::GetHentTypeByHentTypeId(int htid)
{
	for(int i=0;i < GetSize();i++)
	{
		HentType *temp = (HentType*)GetAt(i);
		if(temp->henttypeid == htid)	return temp;
		
	}
	return NULL;
}
