#include "stdafx.h"
#include "HentArray.h"
#include "HentComparator.h"


Hent *CHentArray::GetHentByHentNo(CString &hentNo)
{
	Hent *temp;
	for(int i = 0,s = GetSize();i< s;i++)
	{
		temp = (Hent*)GetAt(i); 
        if(temp->GetFarmNo() == hentNo) return temp;
	}
	return NULL;

}

Hent *CHentArray::GetHentByHentId(int Id)
{
	Hent *temp;
	for(int i = 0,s = GetSize();i< s;i++)
	{
		temp = (Hent*)GetAt(i); 
		if(temp->GetId() == Id) return temp;
	}
	return NULL;
		
}


