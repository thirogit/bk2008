#ifndef __RULEPART_H__
#define __RULEPART_H__

#include "DataMember.h"
#include "DataMembersDefs.h"

class RulePart
{
public:
	RulePart();
	
	virtual void CopyFrom(const RulePart& copy);

	int		GetLimWeight() const;
	Money	GetPriceOverWeightXY() const;
	Money	GetPriceUnderWeightXY() const;
	Money	GetPriceOverWeightXX() const;
	Money	GetPriceUnderWeightXX() const;
	
	void	SetLimWeight(int iLimWeight);
	void	SetPriceOverWeightXY(const Money& pricePerKg);
	void	SetPriceUnderWeightXY(const Money& pricePerKg);
	void	SetPriceOverWeightXX(const Money& pricePerKg);
	void	SetPriceUnderWeightXX(const Money& pricePerKg);
	
	void	ChangeLimWeight(int iLimWeight);
	void	ChangePriceOverWeightXY(const Money& pricePerKg);
	void	ChangePriceUnderWeightXY(const Money& pricePerKg);
	void	ChangePriceOverWeightXX(const Money& pricePerKg);
	void	ChangePriceUnderWeightXX(const Money& pricePerKg);


private:
	IntegerDataMember			limweight;
	MoneyDataMember				priceoverweightxy;
	MoneyDataMember				priceunderweightxy;
	MoneyDataMember				priceoverweightxx;
	MoneyDataMember				priceunderweightxx;	

	friend class Rule;
};




#endif