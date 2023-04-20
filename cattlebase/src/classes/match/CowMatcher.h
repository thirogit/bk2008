#ifndef _COWMATCHER_H_
#define _COWMATCHER_H_

#include "RawDataMatcher.h"
#include "MatchNullInt.h"
#include "MatchDate.h"

class CCowDataMatcher : public CRawDataMatcher<Cow>
{
public:
	
	void FindEAN(CString& EAN);
	CString GetEAN(Cow *pCow);
	
	void FindMotherEAN(const CString& motherEAN);
	CString GetMotherEAN(Cow *pCow);

	void FindPassNo(const CString& passNo);
	CString GetPassNo(Cow *pCow);

	void FindBirthPlace(const CString& birthPlace);
	CString GetBirthPlace(Cow *pCow);
	
	void FindExtras(const CString& extras);
	CString GetExtras(Cow *pCow);

	
	void FindPassDate(const CDateTime& passDt1,const CDateTime& passDt2,TIMESPAN spanType);
	CDateTime GetPassDate(Cow *pCow);

	void FindBirthDate(const CDateTime& birthDt1,const CDateTime& birthDt2,TIMESPAN spanType);
	CDateTime GetBirthDate(Cow *pCow);
	
	void FindFirstOwner(const Hent* pHent);
	Hent* GetFirstOwner(Cow* pCow);

	void FindProvider(const Hent* pHent);
	Hent* GetProvider(Cow* pCow);

	void FindBuyer(const Hent* pHent);
	Hent* GetBuyer(Cow* pCow);

	void FindStock(const Stock* pStock);
	Stock* GetStock(Cow* pCow);

	void FindBuyStock(const Stock* pStock);
	Stock* GetBuyStock(Cow* pCow);

	void FindSellStock(const Stock* pStock);	
	Stock* GetSellStock(Cow* pCow);
	
	void FindWeight(double weight,CMP_ACTION_TYPE cmpAction);
	double GetWeight(Cow* pCow);

	void FindBuyWeight(double weight,CMP_ACTION_TYPE cmpAction);
	NullDouble GetBuyWeight(Cow* pCow);

	void FindSellWeight(double weight,CMP_ACTION_TYPE cmpAction);
	NullDouble GetSellWeight(Cow* pCow);

	void FindBuyPrice(double price,CMP_ACTION_TYPE cmpAction);
	double GetBuyPrice(Cow *pCow);

	void FindSellPrice(double price,CMP_ACTION_TYPE cmpAction);
	double GetSellPrice(Cow *pCow);

	void FindSex(const CowSex& cowsex);
	CowSex GetCowSex(Cow *pCow);

	void FindNotSold();
	bool GetSold(Cow *pCow);

	void FindWithVATBuyInvoice(bool bWithVATBuyInvoice);
	bool HasVATBuyInvoice(Cow *pCow);

	void FindWithRRInvoice(bool bWithRRInvoice);
	bool HasRRInvoice(Cow *pCow);

};




#endif


	
	





