#include "stdafx.h"
#include "DocDataArray.h"
#include "DocDataComparator.h"

Doc *CDocDataArray::GetDocDataByDocId(int No)
{
	Doc *temp;
	for(int i = 0;i< GetSize();i++)
	{
		temp = (Doc*)GetAt(i); 
           if(temp->GetId() == No) return temp;
	}
	return NULL;
}

void CDocDataArray::CommitChanges()
{
	ArrayIterator it = iterator();
	while(it.hasNext())
		(*(it++))->CommitChanges();
}
void CDocDataArray::RollbackChanges()
{
	ArrayIterator it = iterator();
	while(it.hasNext())
		(*(it++))->RollbackChanges();
}