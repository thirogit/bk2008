#include "stdafx.h"
#include "PayAdvance.h"


PayAdvance::PayAdvance(UINT Id) : advanceId(Id)
{
}

PayAdvance::~PayAdvance()
{
}

void PayAdvance::CopyFrom(const PayAdvance& copy)
{
	advanceId = copy.advanceId;
	invoice.CopyFrom(copy.invoice);
	advanceamount.CopyFrom(copy.advanceamount);
	advancedate.CopyFrom(copy.advancedate);
	nextadvancedate.CopyFrom(copy.nextadvancedate);
}

UINT PayAdvance::GetId() const
{
	return advanceId;
}

Money PayAdvance::GetAdvanceAmount() const
{
	return advanceamount.GetValue();
}

Invoice* PayAdvance::GetInvoice() const
{
	return invoice.GetValue();
}

CDateTime PayAdvance::GetAdvanceDate() const
{
	return advancedate.GetValue();
}

CDateTime PayAdvance::GetNextAdvanceDate() const
{
	return nextadvancedate.GetValue();
}

void PayAdvance::SetAdvanceAmount(const Money&  advanceAmount)
{
	advanceamount.SetTo(advanceAmount);
}

void PayAdvance::SetInvoice(Invoice* pInvoice)
{
	invoice.SetTo(pInvoice);
}

void PayAdvance::SetAdvanceDate(const CDateTime& advanceDt)
{
	advancedate.SetTo(advanceDt);
}

void PayAdvance::SetNextAdvanceDate(const CDateTime& nextAdvanceDt)
{
	nextadvancedate.SetTo(nextAdvanceDt);
}

void PayAdvance::ChangeAdvanceAmount(const Money&  advanceAmount)
{
	advanceamount.ChangeTo(advanceAmount);
}

void PayAdvance::ChangeInvoice(Invoice* pInvoice)
{
	invoice.ChangeTo(pInvoice);
}

void PayAdvance::ChangeAdvanceDate(const CDateTime& advanceDt)
{
	advancedate.ChangeTo(advanceDt);
}

void PayAdvance::ChangeNextAdvanceDate(const CDateTime& nextAdvanceDt)
{
	nextadvancedate.ChangeTo(nextAdvanceDt);
}

