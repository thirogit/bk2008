#include "stdafx.h"

#include "InvMatcher.h"
#include "MatchString.h"

#include "MatchPtr.h"

#include "MatchClass.h"
#include "MatchType.h"
#include "MatchDouble.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CInvDataMatcher::FindInvNo(int InvNoFrom,int InvNoTo)
{
	AddMatch(new MatchIntRange<Invoice,CInvDataMatcher>(this,InvNoFrom,InvNoTo,&CInvDataMatcher::GetInvNo));
}
int CInvDataMatcher::GetInvNo(Invoice *pInv)
{
	return pInv->GetInvoiceNo();
}

void CInvDataMatcher::FindExtras(const CString& extras)
{
	AddMatch(new MatchString<Invoice,CInvDataMatcher>(this,extras,&CInvDataMatcher::GetExtras));
}
CString CInvDataMatcher::GetExtras(Invoice* pInv)
{
	return pInv->GetExtras();
}

void CInvDataMatcher::FindInvDate(const CDateTime& invDt1,const CDateTime& invDt2,TIMESPAN spanType)
{
	AddMatch(new MatchDate<Invoice,CInvDataMatcher>(this,invDt1,invDt2,spanType,&CInvDataMatcher::GetInvDate));
}
CDateTime CInvDataMatcher::GetInvDate(Invoice* pInv)
{
	return pInv->GetInvoiceDate();
}

void CInvDataMatcher::FindVATRate(double vat,CMP_ACTION_TYPE cmpAction)
{
	AddMatch(new MatchDouble<Invoice,CInvDataMatcher>(this,vat,cmpAction,&CInvDataMatcher::GetVATRate));
}
double CInvDataMatcher::GetVATRate(Invoice* pInv)
{
	return pInv->GetVATRate();
}

void CInvDataMatcher::FindTotal(double totalValue,CMP_ACTION_TYPE cmpAction)
{
	AddMatch(new MatchDouble<Invoice,CInvDataMatcher>(this,totalValue,cmpAction,&CInvDataMatcher::GetTotalValue));
}
double CInvDataMatcher::GetTotalValue(Invoice* pInv)
{
	return pInv->GetInvoiceNetValue().GetDouble();
}

void CInvDataMatcher::FindPaidBool(bool bPaid)
{
	AddMatch(new MatchType<Invoice,CInvDataMatcher,bool>(this,bPaid,&CInvDataMatcher::GetPaidBool));
}
bool CInvDataMatcher::GetPaidBool(Invoice* pInv)
{
	return pInv->IsPaid();
}

void CInvDataMatcher::FindPaidDate(const CDateTime& paidDt1,const CDateTime& paidDt2,TIMESPAN spanType)
{
	AddMatch(new MatchDate<Invoice,CInvDataMatcher>(this,paidDt1,paidDt2,spanType,&CInvDataMatcher::GetPaidDate));
}
CDateTime CInvDataMatcher::GetPaidDate(Invoice* pInv)
{
	return pInv->GetInvoiceDate();
}

void CInvDataMatcher::FindPayway(Invoice::PAYWAYS payway)
{
	AddMatch(new MatchType<Invoice,CInvDataMatcher,Invoice::PAYWAYS>(this,payway,&CInvDataMatcher::GetPayway));
}
Invoice::PAYWAYS CInvDataMatcher::GetPayway(Invoice* pInv)
{
	return pInv->GetPayWay();
}

void CInvDataMatcher::FindOnlyCorrects()
{
	AddMatch(new MatchType<Invoice,CInvDataMatcher,InvoiceTypes>(this,CORRCT_INV,&CInvDataMatcher::GetInvoiceType));
}
InvoiceTypes CInvDataMatcher::GetInvoiceType(Invoice* pInv)
{
	return pInv->GetThisInvoiceType();
}

