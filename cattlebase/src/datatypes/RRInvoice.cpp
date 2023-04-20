#include "stdafx.h"
#include "RRInvoice.h"

#include "log.h"
#include "CattleSQL.h"
#include "consts.h"

bool RRInvoice::DoesAllCowsHasOwners(CCowArray& cows)
{	
	for(int i = 0;i < cows.GetSize();i++)
		if(cows[i]->GetFirstOwner() == NULL) return false;

	return true;
}

bool RRInvoice::HaveSameOwner(CCowArray &cows)
{
	if(!DoesAllCowsHasOwners(cows)) return false;
	int size = cows.GetSize();
	if(size > 0)
	{
		Cow *pFirstCow = cows[0];
		Hent *pFirstCowFirstOwner = pFirstCow->GetFirstOwner();

		for(int i = 1;i < size;i++)
			if(cows[i]->GetFirstOwner() != pFirstCowFirstOwner) return false;
		return true;
	}
	return false;	
}


Doc* RRInvoice::GetInDocAllCowsBelongTo(CCowArray &cows)
{
	if(cows.GetSize() > 0)
	{
		CCowArray::ArrayIterator it = cows.iterator();
		Doc *pFirstInDoc = (*(it++))->GetInDoc();
		while(it.hasNext())
			if((*(it++))->GetInDoc() != pFirstInDoc) return NULL;
		return pFirstInDoc;
	}
	return NULL;
}

bool RRInvoice::DoesAllCowsBelongToSameInDoc(CCowArray &cows)
{
	return GetInDocAllCowsBelongTo(cows) != NULL;
}

RRInvoice::RRInvoice(const Invoice *pInvoice/* = NULL*/) 
{
	if(pInvoice)
		CopyFrom(*pInvoice);
}


bool RRInvoice::CanMakeInvoice(CCowArray& cows)
{
	if(!(HaveSameOwner(cows) && DoesAllCowsBelongToSameInDoc(cows))) return false;

	
	for(CCowArray::ArrayIterator cowIt = cows.iterator();cowIt.hasNext();cowIt++)
	{
		Cow* pCow = *cowIt;
		if(pCow->HasVATBuyInvoice() || pCow->HasRRBuyInvoice()) return false;	
	}
	return true;

}

CString RRInvoice::GetInvoiceNoStr() const 
{
	CString outNo;
	outNo.Format("RR%04d",invoiceno);
	return outNo;
}

BuyOrSell RRInvoice::GetBuyOrSell() const
{
	return BUY;
}

Hent* RRInvoice::GetInvoiceHent() const
{
	return invEntries[0]->GetCow()->GetFirstOwner();
}


RRInvoice* RRInvoice::GetLastGoodRRInvoice() const
{
	RRInvoice* pNonConstThis = const_cast<RRInvoice*>(this);
	return (RRInvoice*)pNonConstThis->GetLastGoodInvoice();
}

InDoc* RRInvoice::GetInDoc() const
{
	return (InDoc*)invEntries[0]->GetCow()->GetInDoc();
}