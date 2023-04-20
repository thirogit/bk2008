#ifndef __REPOINVOICE_H__
#define __REPOINVOICE_H__

#include "../classes/types/VATRate.h"
#include "../datatypes/Invoice.h"
#include "RepoCow.h"
#include "NewPtrFlushArray.h"
#include "RepoInvoiceType.h"

class RepoInvoice
{
public:	
	RepoInvoice();
	~RepoInvoice();
	RepoInvoice(const RepoInvoice& src);
	void CopyFrom(const RepoInvoice& src);

	const CDateTime& GetInvoiceDate() const;
	const CDateTime& GetTransactionDate() const;	
	const CString& GetTransactionPlace() const;	
	const VATRate& GetVatRate() const;
	Invoice::PAYWAYS GetPayWay() const;
	const CString& GetCustomNumber() const;	
	int GetPayDueDays() const;	
	RepoHent* GetInvoiceHent() const;
	Money GetTotalNet() const;
	Money GetTotalGross() const;
	Weight GetTotalWeight() const;
	RepoInvoiceType GetInvoiceType() const;

	void SetInvoiceDate(const CDateTime& invoiceDt);
	void SetTransactionDate(const CDateTime& transactionDt);	
	void SetTransactionPlace(const CString& sTransactionPlace);	
	void SetVatRate(const VATRate& vatRate);
	void SetPayWay(Invoice::PAYWAYS payWay);
	void SetCustomNumber(const CString& sCustomNumber);	
	void SetPayDueDays(int payDueDays);	
	void SetInvoiceHent(RepoHent* pInvoiceHent);
	void SetInvoiceType(RepoInvoiceType invoiceType);


	int GetCowCount() const;
	RepoCow* GetCow(int iCow);
	const RepoCow* GetCow(int iCow) const;
	void AddCow(RepoCow* pCow);

private:

	CDateTime m_InvoiceDate;
	CDateTime m_TransactionDate;	
	CString m_TransactionPlace;	
	VATRate m_VatRate;
	Invoice::PAYWAYS m_PayWay;
	CString m_CustomNumber;	
	int m_PayDueDays;	
	RepoHent* m_pInvoiceHent;
	RepoInvoiceType m_InvoiceType;

	NewPtrFlushArray<RepoCow> m_Cows;
};
	

#endif