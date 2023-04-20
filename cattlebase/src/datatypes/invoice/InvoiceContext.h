#ifndef __INVOICECONTEXT_H__
#define __INVOICECONTEXT_H__

#include "../Rule.h"
#include "../Doc.h"
#include "../InvoiceTypes.h"
#include "../../accessor/CowAccessor.h"
#include "../../classes/types/VATRate.h"

class InvoiceContext
{
protected:
	InvoiceContext(BuyOrSell buyOrSell,InvoiceTypes invoiceType);
	~InvoiceContext();
public:
	BuyOrSell GetBuyOrSell() const;
	Doc* GetDoc(Cow* pCow) const;
	RulePart* GetRulePart(Rule* pRule) const;
	Doc* GetCowOtherDoc(Cow* pCow) const;
	Money GetCowOtherPrice(Cow* pCow) const;
	bool HasAnyTermValue(Cow* pCow) const;
	Stock* GetTermStock(Cow* pCow) const;
	Money GetTermPrice(Cow* pCow) const;
	NullDouble GetTermWeight(Cow* pCow) const;
	InvoiceTypes GetInvoiceType() const;
	const VATRate& GetVATRate() const;
	void SetVATRate(const VATRate& vatRate);

	bool IsWarnTooLowPrice() const;
	Money GetTooLowPrice() const;

private:
	BuyOrSell m_buyOrSell;
	InvoiceTypes m_invoiceType;
	CowAccessor *m_pAccessor;
	VATRate m_VATRate;
	bool m_bWarnTooLowPrice;
	Money m_TooLowPrice;


};

#endif