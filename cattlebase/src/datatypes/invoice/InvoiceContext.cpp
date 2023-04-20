#include "stdafx.h"
#include "InvoiceContext.h"
#include "MainCfg.h"

InvoiceContext::InvoiceContext(BuyOrSell buyOrSell,InvoiceTypes invoiceType) : 
		m_buyOrSell(buyOrSell),m_invoiceType(invoiceType)
{
	m_pAccessor = CowAccessorFactory::createAccessor(GetBuyOrSell());
	m_bWarnTooLowPrice = AppCfg().GetBool(CFG_WARNTOSMALLPRICE) == TRUE;
	m_TooLowPrice = AppCfg().GetFloat(CFG_WARNMINPRICE);
}

InvoiceContext::~InvoiceContext()
{
	delete m_pAccessor;
}

BuyOrSell InvoiceContext::GetBuyOrSell() const
{
	return m_buyOrSell;
}

Doc* InvoiceContext::GetDoc(Cow* pCow) const
{
	return m_pAccessor->GetDoc(pCow);
}

RulePart* InvoiceContext::GetRulePart(Rule* pRule) const
{
	return pRule->GetRulePart(GetBuyOrSell());
}

Doc* InvoiceContext::GetCowOtherDoc(Cow* pCow) const
{
	if(GetBuyOrSell() == SELL)
	{		
		return pCow->GetInDoc();
	}
	else
	{
		return pCow->GetOutDoc();
	}
}

Money InvoiceContext::GetCowOtherPrice(Cow* pCow) const
{
	if(GetBuyOrSell() == SELL)
	{	
		if(pCow->HasAnyBuyInvoice())
			return pCow->GetBuyPrice();
		
	}
	else
	{
		if(pCow->HasVATSellInvoice())
			return pCow->GetSellPrice();
	}
	return Money();
}

bool InvoiceContext::HasAnyTermValue(Cow* pCow) const
{
	return !GetTermPrice(pCow).IsNull() || GetTermStock(pCow) != NULL ||  GetTermWeight(pCow) > 0.0;		
}

Stock* InvoiceContext::GetTermStock(Cow* pCow) const
{
	return m_pAccessor->GetTermStock(pCow);
}
Money InvoiceContext::GetTermPrice(Cow* pCow) const
{
	return m_pAccessor->GetTermPrice(pCow);
}
NullDouble InvoiceContext::GetTermWeight(Cow* pCow) const
{
	return m_pAccessor->GetTermWeight(pCow);
}

InvoiceTypes InvoiceContext::GetInvoiceType() const
{
	return m_invoiceType;
}

bool InvoiceContext::IsWarnTooLowPrice() const
{
	return m_bWarnTooLowPrice;
}

Money InvoiceContext::GetTooLowPrice() const
{
	return m_TooLowPrice;
}
