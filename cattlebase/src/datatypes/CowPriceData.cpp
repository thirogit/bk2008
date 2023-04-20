#include "stdafx.h"
#include "CowPriceData.h"
#include <math.h>
#include "invoice/CowPriceInvoiceSession.h"

CowPriceData::CowPriceData(Cow *pPricedCow,const CowPriceInvoiceSession *pCowPriceSession) : 
	priceRule(NULL),iPriceCustom(CUSTOM_DISABLED),m_Cow(pPricedCow),bError(false),
	customPriceNetto(ZEROMONEY),customPricePerKg(ZEROMONEY),
	customStock(NULL),m_pCowPriceSession(pCowPriceSession)
{
	ASSERT(pPricedCow);

}

bool CowPriceData::IsError() const
{
	const InvoiceContext* pInvoiceContext = m_pCowPriceSession->GetInvoiceContext();
	Money tooLowPrice = pInvoiceContext->GetTooLowPrice();
	bool isWarnTooLowPrice = pInvoiceContext->IsWarnTooLowPrice();
	Money price = GetCowPriceNetto();
	Weight weight = GetWeight();
	if(price < MINIMALPRICE ||  (isWarnTooLowPrice && price < tooLowPrice) || weight < 0.001)
	{
		return true;
	}
	return false;
}

Cow* CowPriceData::GetCow() const
{
	return m_Cow;
}

Rule *CowPriceData::GetPriceRule() const 
{
	Rule* rule4all = m_pCowPriceSession->GetRule4All();
	if(rule4all) return rule4all;
	else if(priceRule) return priceRule;
	else if(GetStock()->GetRule())
		return GetStock()->GetRule();

	return NULL;
}
Money CowPriceData::GetCowPriceNetto() const
{
	Money ret;
	switch(iPriceCustom)
	{
		case CUSTOM_DISABLED:

			if(m_pCowPriceSession->IsAvgPriceEnabled()) 
			{
				ret = m_pCowPriceSession->GetAvgPrice();
				break;
			}
			ret = (GetWeight()*GetPricePerKg());
		break;

		case CUSTOM_PRICEPERKG:
			ret = (GetWeight()*GetPricePerKg());
		break;
		case CUSTOM_BRUTTO:
		case CUSTOM_NETTO:
			ret =  customPriceNetto;
		break;
			//ret = customPriceBrutto/(m_vat+1);
		default:
			ASSERT(0);
	}

	return ret;

}

void CowPriceData::SetCustomWeight(const Weight& weight)
{
	SetCustomWeight(NullDouble(weight.GetDouble()));
}

void CowPriceData::SetCustomWeight(const NullDouble& weight)
{
	NullDouble ddWeight = weight;
	if(ddWeight < 0)
		ddWeight.MakeNull();

	if(ddWeight != GetCow()->GetWeight().GetDouble())
		customWeight = ddWeight;
	else
		customWeight.MakeNull();
}
void CowPriceData::SetCustomStock(Stock *pStock)
{
	if(pStock == NULL)
		customStock = NULL;
	else
	{
		if(GetCow()->GetStock() == pStock)
			customStock = NULL;
		else
			customStock = pStock;

	}
}
NullDouble CowPriceData::GetCustomWeight() const
{
	return customWeight;
}
Stock *CowPriceData::GetCustomStock() const
{
	return customStock;
}

bool CowPriceData::IsCustomStock() const
{
	return customStock != NULL;
}
bool CowPriceData::IsCustomWeight() const
{
	return !customWeight.IsNull();
}

Weight CowPriceData::GetWeight() const
{
	if(!customWeight.IsNull())		
		return Weight(customWeight.GetDouble());

	if(m_pCowPriceSession->IsAvgWeightEnabled())
		return Weight(m_pCowPriceSession->GetAvgWeight().GetDouble());

	return GetCow()->GetWeight();
}
Stock* CowPriceData::GetStock() const
{
	if(!customStock)
		return GetCow()->GetStock();

	return customStock;
}

Money CowPriceData::GetCowPriceBrutto() const
{
	Money netto;
	netto = GetCowPriceNetto();		
	return (netto + netto*m_pCowPriceSession->GetVATRate() );
}

Money CowPriceData::GetVATValue() const
{
	return GetCowPriceNetto()*m_pCowPriceSession->GetVATRate();
}


void CowPriceData::SetCustomPricePerKg(Money &pricePerKg)
{
	Weight weight = GetWeight();
	iPriceCustom = CUSTOM_PRICEPERKG;
	customPricePerKg = pricePerKg;
	customPriceNetto = customPricePerKg*weight;
	
}

void CowPriceData::SetCustomPriceBrutto(Money& bruttoPrice)
{
	iPriceCustom = CUSTOM_BRUTTO;
	customPriceNetto = bruttoPrice /(1+m_pCowPriceSession->GetVATRate().GetDouble());
}

void CowPriceData::SetCustomPriceNetto(Money& nettoPrice)
{
	iPriceCustom = CUSTOM_NETTO;
	customPriceNetto = nettoPrice;	
}
int CowPriceData::GetPriceCustomInd() const
{
	return iPriceCustom;
}
int CowPriceData::DisableCustomPrice()
{
	int old = iPriceCustom;
	iPriceCustom = CUSTOM_DISABLED;
	return old;
}

Money CowPriceData::GetCowPricePerKg(Rule* pRule,const Weight& weight,const CowSex& sex) const
{
	RulePart* rulePart = m_pCowPriceSession->GetInvoiceContext()->GetRulePart(pRule);
	CowSex validSex = sex.IsUNK() ? pRule->GetNoSexValue() : sex;
	Money weightPricePerKg;

	if(weight <= (double)rulePart->GetLimWeight())
		weightPricePerKg = (validSex.IsXY()) ? rulePart->GetPriceUnderWeightXY() : rulePart->GetPriceUnderWeightXX();
		
	else
		weightPricePerKg =  (validSex.IsXY()) ? rulePart->GetPriceOverWeightXY() : rulePart->GetPriceOverWeightXX();

	return weightPricePerKg;
}


Money CowPriceData::GetPricePerKg() const
{
	Rule *rule; 
	Weight weight = GetWeight();
	Money weightPricePerKg;	
	
	Cow* pCow = GetCow();

	switch(iPriceCustom)
	{
	case CUSTOM_PRICEPERKG:
		weightPricePerKg = customPricePerKg;
		break;
	case CUSTOM_BRUTTO:
	case CUSTOM_NETTO:
		if(weight > 0)
			weightPricePerKg = customPriceNetto/weight;
		else
			weightPricePerKg = ZEROMONEY;
		break;
	default:
	
		if(m_pCowPriceSession->IsAvgPriceEnabled())
		{
			weightPricePerKg = m_pCowPriceSession->GetAvgPrice()/GetWeight();
		} 
		else if(m_pCowPriceSession->IsUseCustomPricePerKgForAll())
		{	
			weightPricePerKg = m_pCowPriceSession->GetCustomPricePerKgForAll();			
		}
		else if(m_pCowPriceSession->IsUseStockMyPerKgPrice())
		{
			weightPricePerKg = GetStock()->GetMyPerKgPrice();
		}
		else if((rule = GetPriceRule()))
		{
			weightPricePerKg = GetCowPricePerKg(rule,weight,pCow->GetSex());	
		}
		else 
		{
			weightPricePerKg = ZEROMONEY;
		}
		break;

	}	
	return weightPricePerKg;

}

Money CowPriceData::GetProfit() const
{

	const InvoiceContext* pInvoiceContext = m_pCowPriceSession->GetInvoiceContext();
	Money priceDiffrence = GetCowPriceNetto() - pInvoiceContext->GetCowOtherPrice(GetCow());

	return priceDiffrence * (pInvoiceContext->GetBuyOrSell() == SELL ? 1.0 : -1.0);
}

Rule* CowPriceData::GetRule() const
{
	return priceRule;
}

void CowPriceData::SetRule(Rule* pRule)
{
	priceRule = pRule;
}