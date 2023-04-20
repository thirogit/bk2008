#ifndef __COWDATACOMPARATOR_H__
#define __COWDATACOMPARATOR_H__

#include "RawData.h"
#include "Cow.h"
#include "RawDataComparator.h"


enum COWDATA_MEMBERS
{
	 COWS_SORT_BY_STATE = RAWDATA_MEMBER_BASE+1,
	 COWS_SORT_BY_EAN,
	 COWS_SORT_BY_PASSNO,
	 COWS_SORT_BY_DELIVER,
	 COWS_SORT_BY_FSTOWNER,
	 COWS_SORT_BY_BIRTHDATE,
	 COWS_SORT_BY_WEIGHT,
	 COWS_SORT_BY_SEX,
	 COWS_SORT_BY_STOCK,
	 COWS_SORT_BY_INDOCNO,
	 COWS_SORT_BY_OUTDOCNO,
	 COWS_SORT_BY_MYPRICE,
	 COWS_SORT_BY_BUYPRICE,
	 COWS_SORT_BY_SELLPRICE,
	 COWS_SORT_BY_RRINV,
	 COWS_SORT_BY_EXTRAS,
	 COWS_SORT_BY_INDATE,
	 COWS_SORT_BY_OUT_DATE,
	 COWS_SORT_BY_BUYER,	 
	 COWS_SORT_BY_OUTGRP,
	 COWS_SORT_BY_INGRP,
	 COWS_SORT_BY_BUYSTOCK,
	 COWS_SORT_BY_SELLSTOCK,
	 COWS_SORT_BY_BUYWEIGHT,
	 COWS_SORT_BY_SELLWEIGHT,
	
	 COWS_SORT_BY_TERMBUYSTOCK,
	 COWS_SORT_BY_TERMSELLSTOCK,
	 COWS_SORT_BY_TERMBUYWEIGHT,
	 COWS_SORT_BY_TERMSELLWEIGHT,
	 COWS_SORT_BY_TERMBUYPRICE,
	 COWS_SORT_BY_TERMSELLPRICE,
	 COWS_SORT_BY_INAGENT,

	 COWDATA_MEMBER_MAX
};




class CowDataComparator : public RawDataComparator<Cow>
{
public:
	virtual COMPARE_RESULT CompareDataBy(UINT dataMember,Cow* DataLeft,Cow* DataRight);
protected:
	template<typename U>
		COMPARE_RESULT CompareBuy(Cow *leftCow,Cow *rightCow,
		U leftValue,U rightValue, COMPARE_RESULT (CowDataComparator::*fComapreValues)(Cow*,U,Cow*,U));

	template<typename U>
		COMPARE_RESULT CompareSell(Cow *leftCow,Cow *rightCow,
		U leftValue,U rightValue, COMPARE_RESULT (CowDataComparator::*fComapreValues)(Cow*,U,Cow*,U));

	COMPARE_RESULT CompareCowWeight(Cow* leftCow,const NullDouble& leftValue,Cow* rightCow,const NullDouble& rightValue);
	COMPARE_RESULT CompareTermCowWeight(Cow* leftCow,const NullDouble& leftValue,Cow* rightCow,const NullDouble& rightValue);
	COMPARE_RESULT CompareCowPrice(Cow* leftCow,const Money& leftValue,Cow* rightCow,const Money& rightValue);
	COMPARE_RESULT CompareCowStock(Cow* leftCow,const Stock* leftValue,Cow* rightCow,const Stock* rightValue);
	COMPARE_RESULT CompareTermCowStock(Cow* leftCow,const Stock* leftValue,Cow* rightCow,const Stock* rightValue);
	COMPARE_RESULT CompareCowSex(const CowSex& leftCowSex,const CowSex& rightCowSex);

};


#endif