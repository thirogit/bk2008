#ifndef __COWPRICEOBJECT_H__
#define __COWPRICEOBJECT_H__

#include "DataTypes.h"

#define CUSTOM_NETTO (-1)
#define CUSTOM_BRUTTO (1)
#define CUSTOM_DISABLED (0)
#define CUSTOM_PRICEPERKG (2)

#define MINIMALPRICE (0.01)

class CowPriceInvoiceSession;

class CowPriceData 
{
public:
	CowPriceData(Cow *pPricedCow,const CowPriceInvoiceSession *pCowPriceSession);
	Cow* GetCow() const;
	
	bool IsError() const;
	Rule* GetRule() const;
	void SetRule(Rule* pRule);

	Money GetCowPriceNetto() const;
	Money GetCowPriceBrutto() const;
	Money GetVATValue() const;
	Rule *GetPriceRule() const;

	void SetCustomWeight(const Weight& weight);
	void SetCustomWeight(const NullDouble& weight);

	void SetCustomStock(Stock *pStock);

	NullDouble GetCustomWeight() const;
	Stock *GetCustomStock() const;

	bool IsCustomStock() const;
	bool IsCustomWeight() const;

	Weight GetWeight() const;
	Stock *GetStock() const;

	Money GetPricePerKg() const;
	Money GetProfit() const;

	void SetCustomPricePerKg(Money &pricePerKg);
	void SetCustomPriceBrutto(Money& bruttoPrice);
	void SetCustomPriceNetto(Money& nettoPrice);
	int GetPriceCustomInd() const;
	int DisableCustomPrice();

	Money GetCowPricePerKg(Rule* pRule,const Weight& weight,const CowSex& sex) const;

private:
	
	Rule* priceRule;	
	bool bError;
	const CowPriceInvoiceSession *m_pCowPriceSession;
	Cow *m_Cow;
	int iPriceCustom;  
	Money customPriceNetto;
	Money customPricePerKg;
	NullDouble customWeight;
	Stock *customStock;
	
};

 








#endif