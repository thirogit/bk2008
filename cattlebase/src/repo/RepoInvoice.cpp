#include "stdafx.h"
#include "RepoInvoice.h"

RepoInvoice::RepoInvoice() : m_PayWay(Invoice::PAY_CASH),m_pInvoiceHent(NULL),m_PayDueDays(0),m_InvoiceType(Invoice_Regular)
{
}

RepoInvoice::~RepoInvoice()
{
	m_Cows.FreeAndFlush();
}

RepoInvoice::RepoInvoice(const RepoInvoice& src)
{
	CopyFrom(src);
}

void RepoInvoice::CopyFrom(const RepoInvoice& src)
{
	m_Cows.FreeAndFlush();
	m_InvoiceDate = src.m_InvoiceDate;
	m_TransactionDate = src.m_TransactionDate;	
	m_TransactionPlace = src.m_TransactionPlace;	
	m_VatRate = src.m_VatRate;
	m_PayWay = src.m_PayWay;
	m_CustomNumber = src.m_CustomNumber;	
	m_PayDueDays = src.m_PayDueDays;	
	m_pInvoiceHent = src.m_pInvoiceHent;
	m_InvoiceType = src.m_InvoiceType;

	NewPtrFlushArray<RepoCow>::const_iterator cowIt = src.m_Cows.begin();

	while(cowIt != src.m_Cows.end())
	{
		m_Cows.Add(new RepoCow(*(*(cowIt++))));
	}
}

const CDateTime& RepoInvoice::GetInvoiceDate() const
{
	return m_InvoiceDate;
}

const CDateTime& RepoInvoice::GetTransactionDate() const
{
	return m_TransactionDate;
}
	
const CString& RepoInvoice::GetTransactionPlace() const
{
	return m_TransactionPlace;
}
	
const VATRate& RepoInvoice::GetVatRate() const
{
	return m_VatRate;
}

Invoice::PAYWAYS RepoInvoice::GetPayWay() const
{
	return m_PayWay;
}

const CString& RepoInvoice::GetCustomNumber() const
{
	return m_CustomNumber;
}
	
int RepoInvoice::GetPayDueDays() const
{
	return m_PayDueDays;
}
	
RepoHent* RepoInvoice::GetInvoiceHent() const
{
	return m_pInvoiceHent;
}

void RepoInvoice::SetInvoiceType(RepoInvoiceType invoiceType)
{
	m_InvoiceType = invoiceType;
}

void RepoInvoice::SetInvoiceDate(const CDateTime& invoiceDt)
{
	m_InvoiceDate = invoiceDt;
}

void RepoInvoice::SetTransactionDate(const CDateTime& transactionDt)
{
	m_TransactionDate = transactionDt;
}
	
void RepoInvoice::SetTransactionPlace(const CString& sTransactionPlace)
{
	m_TransactionPlace = sTransactionPlace;
}
	
void RepoInvoice::SetVatRate(const VATRate& vatRate)
{
	m_VatRate = vatRate;
}

void RepoInvoice::SetPayWay(Invoice::PAYWAYS payWay)
{
	m_PayWay = payWay;
}

void RepoInvoice::SetCustomNumber(const CString& sCustomNumber)
{
	m_CustomNumber = sCustomNumber;
}
	
void RepoInvoice::SetPayDueDays(int payDueDays)
{
	m_PayDueDays = payDueDays;
}
	
void RepoInvoice::SetInvoiceHent(RepoHent* pInvoiceHent)
{
	m_pInvoiceHent = pInvoiceHent;
}

int RepoInvoice::GetCowCount() const
{
	return m_Cows.GetSize();
}

RepoCow* RepoInvoice::GetCow(int iCow)
{
	return m_Cows.GetAt(iCow);
}

const RepoCow* RepoInvoice::GetCow(int iCow) const
{
	return const_cast<RepoInvoice*>(this)->GetCow(iCow);
}

void RepoInvoice::AddCow(RepoCow* pCow)
{
	m_Cows.Add(pCow);
}

Money RepoInvoice::GetTotalNet() const
{
	Money totalNet(0.0);
	NewPtrFlushArray<RepoCow>::const_iterator cowIt = m_Cows.begin();
	while(cowIt != m_Cows.end())
	{
		totalNet += (*cowIt)->GetPrice();
		cowIt++;
	}
	return totalNet;
	
}

Money RepoInvoice::GetTotalGross() const
{
	Money totalNet = GetTotalNet();
	return totalNet + totalNet*GetVatRate();
}

RepoInvoiceType RepoInvoice::GetInvoiceType() const
{
	return m_InvoiceType;
}

Weight RepoInvoice::GetTotalWeight() const
{
	Weight totalWeight(0.0);
	NewPtrFlushArray<RepoCow>::const_iterator cowIt = m_Cows.begin();
	while(cowIt != m_Cows.end())
	{
		totalWeight += (*cowIt)->GetWeight();
		cowIt++;
	}
	return totalWeight;
}