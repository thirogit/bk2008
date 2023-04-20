#ifndef __COWACCESSOR_H__
#define __COWACCESSOR_H__

#include "../datatypes/Cow.h"

class CowAccessor
{
public:
	virtual Stock* GetStock(Cow* pCow) = 0;
	virtual Money GetPrice(Cow* pCow) = 0;
	virtual NullInt GetWeight(Cow* pCow) = 0;

	virtual Stock* GetTermStock(Cow* pCow) = 0;
	virtual Money GetTermPrice(Cow* pCow) = 0;
	virtual NullInt GetTermWeight(Cow* pCow) = 0;	

	virtual Doc* GetDoc(Cow* pCow) = 0;
	virtual NullInt GetGroup(Cow* pCow) = 0;
};


class CowAccessorFactory
{
public:
	static CowAccessor* createAccessor(BuyOrSell buyOrSell);
};

class CowInAccessor : public CowAccessor
{
	virtual Stock* GetStock(Cow* pCow);
	virtual Money GetPrice(Cow* pCow);
	virtual NullInt GetWeight(Cow* pCow);

	virtual Stock* GetTermStock(Cow* pCow);
	virtual Money GetTermPrice(Cow* pCow);
	virtual NullInt GetTermWeight(Cow* pCow);

	virtual Doc* GetDoc(Cow* pCow);
	virtual NullInt GetGroup(Cow* pCow);

};

class CowOutAccessor : public CowAccessor
{
	virtual Stock* GetStock(Cow* pCow);
	virtual Money GetPrice(Cow* pCow);
	virtual NullInt GetWeight(Cow* pCow);

	virtual Stock* GetTermStock(Cow* pCow);
	virtual Money GetTermPrice(Cow* pCow);
	virtual NullInt GetTermWeight(Cow* pCow);

	virtual Doc* GetDoc(Cow* pCow);
	virtual NullInt GetGroup(Cow* pCow);

};



#endif