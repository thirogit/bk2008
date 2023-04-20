#include "stdafx.h"

#include "CowMatcher.h"
#include "MatchString.h"
#include "MatchPtr.h"
#include "MatchClass.h"
#include "MatchType.h"
#include "MatchDouble.h"
#include "MatchNullDouble.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void CCowDataMatcher::FindEAN(CString& EAN)
{
	AddMatch(new MatchString<Cow,CCowDataMatcher>(this,EAN,&CCowDataMatcher::GetEAN));
}
CString CCowDataMatcher::GetEAN(Cow *pCow)
{
	return pCow->GetEAN();
}

void CCowDataMatcher::FindMotherEAN(const CString& motherEAN)
{
	AddMatch(new MatchString<Cow,CCowDataMatcher>(this,motherEAN,&CCowDataMatcher::GetMotherEAN));
}
CString CCowDataMatcher::GetMotherEAN(Cow *pCow)
{
	return pCow->GetMotherEAN();
}

void CCowDataMatcher::FindPassNo(const CString& passNo)
{
	AddMatch(new MatchString<Cow,CCowDataMatcher>(this,passNo,&CCowDataMatcher::GetPassNo));
}
CString CCowDataMatcher::GetPassNo(Cow *pCow)
{
	return pCow->GetPassNo();
}
void CCowDataMatcher::FindBirthPlace(const CString& birthPlace)
{
	AddMatch(new MatchString<Cow,CCowDataMatcher>(this,birthPlace,&CCowDataMatcher::GetBirthPlace));
}
CString CCowDataMatcher::GetBirthPlace(Cow *pCow)
{
	return pCow->GetBirthplace();
}
void CCowDataMatcher::FindExtras(const CString& extras)
{
	AddMatch(new MatchString<Cow,CCowDataMatcher>(this,extras,&CCowDataMatcher::GetExtras));
}
CString CCowDataMatcher::GetExtras(Cow *pCow)
{
	return pCow->GetExtras();
}

void CCowDataMatcher::FindPassDate(const CDateTime& passDt1,const CDateTime& passDt2,TIMESPAN spanType)
{
	AddMatch(new MatchDate<Cow,CCowDataMatcher>(this,passDt1,passDt2,spanType,&CCowDataMatcher::GetPassDate));

}
CDateTime CCowDataMatcher::GetPassDate(Cow *pCow)
{
	return pCow->GetPassDate();
}

void CCowDataMatcher::FindBirthDate(const CDateTime& birthDt1,const CDateTime& birthDt2,TIMESPAN spanType)
{
	AddMatch(new MatchDate<Cow,CCowDataMatcher>(this,birthDt1,birthDt2,spanType,&CCowDataMatcher::GetBirthDate));
}
CDateTime CCowDataMatcher::GetBirthDate(Cow *pCow)
{
	return pCow->GetBirthDate();
}


void CCowDataMatcher::FindFirstOwner(const Hent* pHent)
{
	AddMatch(new MatchPtr<Cow,CCowDataMatcher,Hent>(this,pHent,true,&CCowDataMatcher::GetFirstOwner));
}

Hent* CCowDataMatcher::GetFirstOwner(Cow* pCow)
{
	return pCow->GetFirstOwner();
}

void CCowDataMatcher::FindProvider(const Hent* pHent)
{
	AddMatch(new MatchPtr<Cow,CCowDataMatcher,Hent>(this,pHent,false,&CCowDataMatcher::GetProvider));
}

Hent* CCowDataMatcher::GetProvider(Cow* pCow)
{
	return pCow->GetDeliver();
}
void CCowDataMatcher::FindBuyer(const Hent* pHent)
{
	AddMatch(new MatchPtr<Cow,CCowDataMatcher,Hent>(this,pHent,false,&CCowDataMatcher::GetBuyer));
}
Hent* CCowDataMatcher::GetBuyer(Cow* pCow)
{
	return pCow->GetBuyer();
}

void CCowDataMatcher::FindStock(const Stock* pStock)
{
	AddMatch(new MatchPtr<Cow,CCowDataMatcher,Stock>(this,pStock,false,&CCowDataMatcher::GetStock));
}

Stock* CCowDataMatcher::GetStock(Cow* pCow)
{
	return pCow->GetStock();
}

void CCowDataMatcher::FindBuyStock(const Stock* pStock)
{
	AddMatch(new MatchPtr<Cow,CCowDataMatcher,Stock>(this,pStock,false,&CCowDataMatcher::GetBuyStock));
}

Stock* CCowDataMatcher::GetBuyStock(Cow* pCow)
{
	return pCow->GetBuyStock();
}

void CCowDataMatcher::FindSellStock(const Stock* pStock)
{
	AddMatch(new MatchPtr<Cow,CCowDataMatcher,Stock>(this,pStock,false,&CCowDataMatcher::GetSellStock));
}
	
Stock* CCowDataMatcher::GetSellStock(Cow* pCow)
{
	return pCow->GetSellStock();
}

void CCowDataMatcher::FindSex(const CowSex& cowsex)
{
	AddMatch(new MatchClass<Cow,CCowDataMatcher,CowSex>(this,cowsex,&CCowDataMatcher::GetCowSex));	
}

CowSex CCowDataMatcher::GetCowSex(Cow *pCow)
{
	return pCow->GetSex();
}

void CCowDataMatcher::FindWeight(double weight,CMP_ACTION_TYPE cmpAction)
{
	AddMatch(new MatchDouble<Cow,CCowDataMatcher>(this,weight,cmpAction,&CCowDataMatcher::GetWeight));
}

double CCowDataMatcher::GetWeight(Cow* pCow)
{
	return pCow->GetWeight().GetDouble();
}

void CCowDataMatcher::FindBuyWeight(double weight,CMP_ACTION_TYPE cmpAction)
{
	AddMatch(new MatchNullDouble<Cow,CCowDataMatcher>(this,weight,cmpAction,&CCowDataMatcher::GetBuyWeight));
}

NullDouble CCowDataMatcher::GetBuyWeight(Cow* pCow)
{
	return pCow->GetBuyWeight();
}

void CCowDataMatcher::FindSellWeight(double weight,CMP_ACTION_TYPE cmpAction)
{
	AddMatch(new MatchNullDouble<Cow,CCowDataMatcher>(this,weight,cmpAction,&CCowDataMatcher::GetSellWeight));
}

NullDouble CCowDataMatcher::GetSellWeight(Cow* pCow)
{
	return pCow->GetSellWeight();
}

void CCowDataMatcher::FindBuyPrice(double price,CMP_ACTION_TYPE cmpAction)
{
	AddMatch(new MatchDouble<Cow,CCowDataMatcher>(this,price,cmpAction,&CCowDataMatcher::GetBuyPrice));
}

double CCowDataMatcher::GetBuyPrice(Cow *pCow)
{
	return pCow->GetBuyPrice().GetDouble();
}

void CCowDataMatcher::FindSellPrice(double price,CMP_ACTION_TYPE cmpAction)
{
	AddMatch(new MatchDouble<Cow,CCowDataMatcher>(this,price,cmpAction,&CCowDataMatcher::GetSellPrice));
}

double CCowDataMatcher::GetSellPrice(Cow *pCow)
{
	return pCow->GetSellPrice().GetDouble();
}

void CCowDataMatcher::FindNotSold()
{
	AddMatch(new MatchType<Cow,CCowDataMatcher,bool>(this,false,&CCowDataMatcher::GetSold));
}
bool CCowDataMatcher::GetSold(Cow *pCow)
{
	return pCow->IsOut();
}
void CCowDataMatcher::FindWithVATBuyInvoice(bool bWithVATBuyInvoice)
{
	AddMatch(new MatchType<Cow,CCowDataMatcher,bool>(this,bWithVATBuyInvoice,&CCowDataMatcher::HasVATBuyInvoice));
}
bool CCowDataMatcher::HasVATBuyInvoice(Cow *pCow)
{
	return pCow->HasVATBuyInvoice();
}

void CCowDataMatcher::FindWithRRInvoice(bool bWithRRInvoice)
{
	AddMatch(new MatchType<Cow,CCowDataMatcher,bool>(this,bWithRRInvoice,&CCowDataMatcher::HasRRInvoice));
}
bool CCowDataMatcher::HasRRInvoice(Cow *pCow)
{
	return pCow->HasRRBuyInvoice();
}