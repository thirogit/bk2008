#ifndef __INVOICE_H__
#define __INVOICE_H__

#include "RawData.h"
#include "InvoiceTypes.h"
#include "CowInvoiceEntryArray.h"
#include "Cow.h"
#include "CowDataArray.h"

#define NULL_INVOICEID (0)
#define MIN_PAYDUEDAYS (1)
#define MAX_PAYDUEDAYS 365

class Invoice : public RawData
{
public:

	enum PAYWAYS 
	{
		PAY_CASH = '$',
		PAY_TRANS = '@'
	};

	typedef DataMember<PAYWAYS,PAYWAYS> PayWayDataMember;
	
	Invoice(UINT Id = NULL_INVOICEID);	
	virtual ~Invoice();
	void CopyFrom(const Invoice& src);

	UINT GetId() const;
	void ResetId();
	UINT GetInvoiceNo() const;
	void SetInvoiceNo(UINT uiInvoiceNo);
	void SetCorrect(Invoice* pCorrect);
	Invoice* GetCorrect() const;
	bool HasCorrect() const;

	void SetCorrectFor(Invoice* pCorrectFor);
	Invoice* GetCorrectFor() const;
	void SetInvoiceType(InvoiceTypes invoiceType);
	InvoiceTypes GetThisInvoiceType() const;
	InvoiceTypes GetRealInvoiceType() const;

	CString GetExtras() const;
	void SetExtras(const CString& sExtras);
	void ChangeExtras(const CString& sExtras);

	CDateTime GetInvoiceDate() const;
	void SetInvoiceDate(const CDateTime& invoiceDt);
	void ChangeInvoiceDate(const CDateTime& invoiceDt);

	CDateTime GetPaidDate() const;
	void SetPaidDate(const CDateTime& paidDt);
	void ChangePaidDate(const CDateTime& paidDt);

	Money GetInvoiceGrossValue() const;
	Money GetInvoiceNetValue() const;
	void SetInvoiceNetValue(const Money& netValue);
	void ChangeInvoiceNetValue(const Money& netValue);

	double GetVATRate() const;
	void SetVATRate(double dVatRate);
	void ChangeVATRate(double dVatRate);

	PAYWAYS GetPayWay() const;
	void SetPayWay(PAYWAYS aPayWay);
	void ChangePayWay(PAYWAYS aPayWay);

	CString GetCustomNumber() const;
	void SetCustomNumber(const CString& sCustomNumber);
	void ChangeCustomNumber(const CString& sCustomNumber);

	CString GetTransactionPlace() const;
	void SetTransactionPlace(const CString& sTransactionPlace);
	void ChangeTransactionPlace(const CString& sTransactionPlace);

	CDateTime GetTransactionDate() const;
	void SetTransactionDate(const CDateTime& transactionDt);
	void ChangeTransactionDate(const CDateTime& transactionDt);

	int GetPayDueDays() const;
	void SetPayDueDays(int iPayDueDays);
	void ChangePayDueDays(int iPayDueDays);	

	virtual bool UpdateThisInvoice(bool bCommit = true);
	virtual bool InsertThisInvoice(bool bCommit = true);
	virtual bool DeleteThisInvoice(bool bCommit = true);
	bool CanEditThisInvoice() const;
	bool CanDeleteThisInvoice() const;

	
	Money GetBruttoValueAfterCorrect() const;
	virtual int GetCowQty() const;
	virtual CString GetInvoiceNoStr() const = 0;	
	virtual BuyOrSell GetBuyOrSell() const = 0;
	virtual Hent* GetInvoiceHent() const = 0;
	virtual void GetInvoiceCows(CCowArray &cows) const;
	CowInvoiceEntry* GetEntryForCow(const Cow* pCow);
	Invoice* GetFirstInvoice();	
	bool IsPaid() const;
	Invoice* GetLastGoodInvoice();
	CDateTime GetPayDueDate() const;

	void RollbackChanges();
	void CommitChanges();
	bool IsUncommited();

protected:
	UINT invoiceno;
	UINT invoiceid;
	Invoice *correct;		//pointer to correcting invoice 
	Invoice *correctfor;	//this is a correct for some other invoice ,valid only if invoicetype == CORRCT_INV;
	InvoiceTypes invtype;
	
	StringDataMember extras;
	DateTimeDataMember invoicedate;
	DateTimeDataMember paiddate;
	MoneyDataMember invoicevalue;	//net value	
	FloatDataMember VATrate;
	PayWayDataMember payway;	
	StringDataMember customnumber;	
	IntegerDataMember payduedays;
	StringDataMember transactionplace;
	DateTimeDataMember transactiondate;


	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;
public:
	CCowInvoiceEntryArray invEntries;
};

#endif