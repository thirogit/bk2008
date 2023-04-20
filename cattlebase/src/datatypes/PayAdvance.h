#ifndef __PAYADVANCE_H__
#define __PAYADVANCE_H__

#include "DataMembersDefs.h"
#include "Invoice.h"

#define NULL_PAYADVANCEID (0)

typedef DataMember<Invoice*,Invoice*> InvoiceDataMember;

class PayAdvance 
{
public:
	PayAdvance(UINT Id);
	~PayAdvance();
	virtual void CopyFrom(const PayAdvance& copy);

	UINT GetId() const;
	Money GetAdvanceAmount() const;
	Invoice* GetInvoice() const;
	CDateTime GetAdvanceDate() const;
	CDateTime GetNextAdvanceDate() const;

	void SetAdvanceAmount(const Money&  advanceAmount);	
	void SetInvoice(Invoice* pInvoice);
	void SetAdvanceDate(const CDateTime& advanceDt);
	void SetNextAdvanceDate(const CDateTime& nextAdvanceDt);

	void ChangeAdvanceAmount(const Money&  advanceAmount);	
	void ChangeInvoice(Invoice* pInvoice);
	void ChangeAdvanceDate(const CDateTime& advanceDt);
	void ChangeNextAdvanceDate(const CDateTime& nextAdvanceDt);

private:

	UINT advanceId;
	InvoiceDataMember invoice;
	MoneyDataMember advanceamount;
	DateTimeDataMember advancedate;
	DateTimeDataMember nextadvancedate;
};




#endif