#include "stdafx.h"
#include "RulePart.h"

RulePart::RulePart() : limweight(0),
	priceoverweightxy(ZEROMONEY),
	priceunderweightxy(ZEROMONEY),
	priceoverweightxx(ZEROMONEY),
	priceunderweightxx(ZEROMONEY)
{
}


void RulePart::CopyFrom(const RulePart &copy)
{
	limweight.CopyFrom(copy.limweight);
	priceoverweightxy.CopyFrom(copy.priceoverweightxy);
	priceunderweightxy.CopyFrom(copy.priceunderweightxy);
	priceoverweightxx.CopyFrom(copy.priceoverweightxx);
	priceunderweightxx.CopyFrom(copy.priceunderweightxx);
}



int		RulePart::GetLimWeight() const
{
	return limweight.GetValue();
}

Money	RulePart::GetPriceOverWeightXY() const
{
	return priceoverweightxy.GetValue();
}

Money	RulePart::GetPriceUnderWeightXY() const
{
	return priceunderweightxy.GetValue();
}

Money	RulePart::GetPriceOverWeightXX() const
{
	return priceoverweightxx.GetValue();
}

Money	RulePart::GetPriceUnderWeightXX() const
{
	return priceunderweightxx.GetValue();
}


void	RulePart::SetLimWeight(int iLimWeight)
{
	limweight.SetTo(iLimWeight);
}

void	RulePart::SetPriceOverWeightXY(const Money& pricePerKg)
{
	priceoverweightxy.SetTo(pricePerKg);
}

void	RulePart::SetPriceUnderWeightXY(const Money& pricePerKg)
{
	priceunderweightxy.SetTo(pricePerKg);
}

void	RulePart::SetPriceOverWeightXX(const Money& pricePerKg)
{
	priceoverweightxx.SetTo(pricePerKg);
}

void	RulePart::SetPriceUnderWeightXX(const Money& pricePerKg)
{
	priceunderweightxx.SetTo(pricePerKg);
}


void	RulePart::ChangeLimWeight(int iLimWeight)
{
	limweight.ChangeTo(iLimWeight);
}

void	RulePart::ChangePriceOverWeightXY(const Money& pricePerKg)
{
	priceoverweightxy.ChangeTo(pricePerKg);
}

void	RulePart::ChangePriceUnderWeightXY(const Money& pricePerKg)
{
	priceunderweightxy.ChangeTo(pricePerKg);
}

void	RulePart::ChangePriceOverWeightXX(const Money& pricePerKg)
{
	priceoverweightxx.ChangeTo(pricePerKg);
}

void	RulePart::ChangePriceUnderWeightXX(const Money& pricePerKg)
{
	priceunderweightxx.ChangeTo(pricePerKg);
}
