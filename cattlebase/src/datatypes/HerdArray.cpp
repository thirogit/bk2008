#include "stdafx.h"
#include "HerdArray.h"
//#include "CowDataComparator.h"

//Cow* CCowArray::GetCowByEAN(const CString &ean)
//{
//	int ii,ss;
//	Cow *pCow;
//
//	for(ii=0,ss = GetSize();ii<ss;ii++)
//	{
//		pCow = GetAt(ii);
//		if(pCow->GetEAN() == ean) return pCow;		
//	}
//	return NULL;
//}
//Cow* CCowArray::GetCowByCowId(int cowid)
//{
//	int ii,ss;
//	Cow *pCow;
//
//	for(ii=0,ss = GetSize();ii<ss;ii++)
//	{
//		pCow = GetAt(ii);
//		if(pCow->GetId() == cowid) return pCow;		
//	}
//	return NULL;
//}
//void CCowArray::CommitChanges()
//{
//	ArrayIterator it = iterator();
//	while(it.hasNext())
//		(*(it++))->CommitChanges();
//}
//void CCowArray::RollbackChanges()
//{
//	ArrayIterator it = iterator();
//	while(it.hasNext())
//		(*(it++))->RollbackChanges();
//}
//
//void CCowArray::AppendPtr(CowArrayPtr cowPtr)
//{
//	Append(*cowPtr);
//}
//void CCowArray::RemoveArrayPtr(CowArrayPtr cowPtr)
//{
//	RemoveArray(*cowPtr);
//}
