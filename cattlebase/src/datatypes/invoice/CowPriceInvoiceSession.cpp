#include "stdafx.h"
#include "CowPriceInvoiceSession.h"
#include "CattleSQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CowPriceInvoiceSession::CowPriceInvoiceSession(const InvoiceContext *pInvoiceContext) : 
		m_pInvoiceContext(pInvoiceContext),
		m_pRule4All(NULL),
		m_bAvgPriceEnabled(false),
		m_bUseStockMyPerKgPrice(false),
		m_bCustomPricePerKgForAll(false),
		m_selectedCowPriceItem(-1),
		m_selectedDocItem(-1),
		m_bAvgWeightEnabled(false),
		m_AvgWeight(NULL_WEIGHT)
{
	m_AvgWeight.MakeNull();	
}

const VATRate& CowPriceInvoiceSession::GetVATRate() const
{
	return m_VATRate;
}

void CowPriceInvoiceSession::SetVATRate(const VATRate& vatRate)
{
	m_VATRate = vatRate;
}

const InvoiceContext* CowPriceInvoiceSession::GetInvoiceContext() const
{
	return m_pInvoiceContext;
}

Rule* CowPriceInvoiceSession::GetRule4All() const
{
	return m_pRule4All;
}

void CowPriceInvoiceSession::SetRule4All(Rule* pRule4All)
{
	m_pRule4All = pRule4All;
}

void CowPriceInvoiceSession::SetUseStockMyPerKgPrice(bool bUse)
{
	m_bUseStockMyPerKgPrice = bUse;
}

bool CowPriceInvoiceSession::IsUseStockMyPerKgPrice() const
{
	return m_bUseStockMyPerKgPrice;
}

void CowPriceInvoiceSession::SetCustomPricePreKgForAll(const Money& customPricePerKgForAll)
{
	m_CustomPricePerKgForAll = customPricePerKgForAll;
}

const Money& CowPriceInvoiceSession::GetCustomPricePerKgForAll() const
{
	return m_CustomPricePerKgForAll;
}

void CowPriceInvoiceSession::SetUseCustomPricePerKgForAll(bool bUse)
{
	m_bCustomPricePerKgForAll = bUse;
}

bool CowPriceInvoiceSession::IsUseCustomPricePerKgForAll() const
{
	return m_bCustomPricePerKgForAll;
}

void CowPriceInvoiceSession::EnableAvgPrice(bool bEnable)
{
	m_bAvgPriceEnabled = bEnable;
}

bool CowPriceInvoiceSession::IsAvgPriceEnabled() const
{
	return m_bAvgPriceEnabled;
}

void CowPriceInvoiceSession::SetAvgPrice(const Money& avgPrice)
{
	m_AvgPrice = avgPrice;
}

const Money& CowPriceInvoiceSession::GetAvgPrice() const
{
	return m_AvgPrice;
}

CowPriceData* CowPriceInvoiceSession::AddCow(Cow* pCow)
{
	CowPriceData* pPrice = new CowPriceData(pCow,this);
	m_cowprices.Add(pPrice);
	return pPrice;
}

CowPriceInvoiceSession::~CowPriceInvoiceSession()
{
	m_cowprices.FreeAndFlush();
}

CowPriceData* CowPriceInvoiceSession::GetCowPrice(int iIndex) const
{
	return m_cowprices[iIndex];
}

int CowPriceInvoiceSession::GetCowCount() const
{
	return m_cowprices.GetCount();
}

CCowPriceDataArray::ArrayIterator CowPriceInvoiceSession::GetCowPriceIterator()
{
	return m_cowprices.iterator();
}

void CowPriceInvoiceSession::DeleteCowPrice(int iIndex)
{
	CowPriceData* pPrice = m_cowprices.GetAt(iIndex);
	m_cowprices.RemoveAt(iIndex);
	delete pPrice;
}

void CowPriceInvoiceSession::SortPrices(CCowPriceDataArray::SortArrayComparator& comparator)
{
	m_cowprices.Sort(comparator);
}	

Money CowPriceInvoiceSession::GetTotalNetValue() const
{
	CCowPriceDataArray::ArrayIterator it = const_cast<CowPriceInvoiceSession*>(this)->GetCowPriceIterator();
	Money total(0.0);
		
	while(it.hasNext())
	{		
		total += (*(it++))->GetCowPriceNetto();
	}
	return total;
}

Weight CowPriceInvoiceSession::GetTotalWeight() const
{
	CCowPriceDataArray::ArrayIterator it = const_cast<CowPriceInvoiceSession*>(this)->GetCowPriceIterator();
	Weight totalWeight;
		
	while(it.hasNext())
	{		
		totalWeight += (*(it++))->GetWeight();
	}
	return totalWeight;
}

Money CowPriceInvoiceSession::GetTotalVATValue() const
{
	return GetTotalNetValue()* GetVATRate();
}

Money CowPriceInvoiceSession::GetTotalGrossValue() const
{
	Money netValue = GetTotalNetValue();
	return netValue*GetVATRate() + netValue;
}

void CowPriceInvoiceSession::GetSessionCows(CCowArray& cows) const
{
	CCowPriceDataArray::ArrayIterator it = const_cast<CowPriceInvoiceSession*>(this)->GetCowPriceIterator();
	
		
	while(it.hasNext())
	{		
		cows.Add((*(it++))->GetCow());
	}
	
}

CDocDataArray&  CowPriceInvoiceSession::GetInvoiceDocs()
{
	return m_invoiceDocs;
}

int CowPriceInvoiceSession::GetSelectedDocItem() const
{
	return m_selectedDocItem;
}

int CowPriceInvoiceSession::GetSelectedCowPriceItem() const
{
	return m_selectedCowPriceItem;
}

void CowPriceInvoiceSession::SetSelectedDocItem(int iItem)
{
	m_selectedDocItem = iItem;
}

void CowPriceInvoiceSession::SetSelectedCowPriceItem(int iItem)
{
	m_selectedCowPriceItem = iItem;
}

unsigned short CowPriceInvoiceSession::GetPayDueDays() const
{
	return m_paydueDays;
}
	
CString CowPriceInvoiceSession::GetExtras() const
{
	return m_sExtras;
}

CString CowPriceInvoiceSession::GetCustomNumber() const
{
	return m_sCustomNumber;
}

CDateTime CowPriceInvoiceSession::GetInvoiceDate() const
{
	return m_dtInvoiceDate;
}

Invoice::PAYWAYS CowPriceInvoiceSession::GetPayway() const
{
	return m_payway;
}

void CowPriceInvoiceSession::SetPayDueDays(unsigned short paydueDays)
{
	m_paydueDays = paydueDays;
}

void CowPriceInvoiceSession::SetExtras(const CString& sExtras)
{
	m_sExtras = sExtras;
}

void CowPriceInvoiceSession::SetCustomNumber(const CString& sCustomNumber)
{
	m_sCustomNumber = sCustomNumber;
}

void CowPriceInvoiceSession::SetInvoiceDate(const CDateTime& invoiceDt)
{
	m_dtInvoiceDate = invoiceDt;
}

void CowPriceInvoiceSession::SetPayway(Invoice::PAYWAYS payway)
{
	m_payway = payway;
}

CowInvoiceEntry* CowPriceInvoiceSession::CreateNewInvoiceEntry(Invoice* pInvoice,CowPriceData *pCowPrice)
{
	CowInvoiceEntry *pInvoiceEntry = new CowInvoiceEntry(pCowPrice->GetCow(),pInvoice);
	pInvoiceEntry->SetPrice(pCowPrice->GetCowPriceNetto().GetDouble());
	pInvoiceEntry->SetWeight(pCowPrice->GetWeight());
	pInvoiceEntry->SetStock(pCowPrice->GetStock());
	return pInvoiceEntry;
}


NullInvoice* CowPriceInvoiceSession::CreateNullInvoice()
{
	NullInvoice *pInvoice = new NullInvoice();

	pInvoice->SetInvoiceType(GetInvoiceContext()->GetInvoiceType());

	pInvoice->SetExtras(GetExtras());
	
	pInvoice->SetInvoiceDate(GetInvoiceDate());
	
	pInvoice->SetVATRate(GetVATRate().GetDouble());
		
	pInvoice->SetPayWay(GetPayway());
		
	pInvoice->SetCustomNumber(GetCustomNumber());

	pInvoice->SetPayDueDays(GetPayDueDays());

	pInvoice->SetTransactionDate(GetTransactionDate());
	pInvoice->SetTransactionPlace(GetTransactionPlace());

	return pInvoice;
}



bool CowPriceInvoiceSession::InsertCowInvoiceEntries(CCowInvoiceEntryArray &cowInvEntries)
{
	CCowInvoiceEntryArray::ArrayIterator it = cowInvEntries.iterator();
	while(it.hasNext())
	{
		if(!((*(it++))->InsertThis(false)))
		{
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGCOWPRICE);		
			return false;
		}
	}
	return true;
}


bool CowPriceInvoiceSession::DeleteInvoiceEntries(CCowInvoiceEntryArray& invoiceEntries)
{
	CCowInvoiceEntryArray::ArrayIterator it = invoiceEntries.iterator();
	while(it.hasNext())
	{
		if(!(*(it++))->DeleteThis(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRDELETEINGINVOICEENTRIES);
			return false;
		}
	}
	return true;
}

void CowPriceInvoiceSession::UpdateInvoiceValues(Invoice* pInvoice)
{
	pInvoice->ChangePayWay(GetPayway());
	pInvoice->ChangeExtras(GetExtras());
	pInvoice->ChangeInvoiceDate(GetInvoiceDate());
	pInvoice->ChangeVATRate(GetVATRate().GetDouble());
	pInvoice->ChangeCustomNumber(GetCustomNumber());
	pInvoice->ChangePayDueDays(GetPayDueDays());
	pInvoice->ChangeTransactionDate(GetTransactionDate());
	pInvoice->ChangeTransactionPlace(GetTransactionPlace());
}

Hent* CowPriceInvoiceSession::GetInvoiceHent() const
{
	return m_pInvoiceHent;
}
void CowPriceInvoiceSession::SetInvoiceHent(Hent* pHent)
{
	m_pInvoiceHent = pHent;
}

void CowPriceInvoiceSession::EnableAvgWeight(bool bEnable)
{
	m_bAvgWeightEnabled = bEnable;
}

bool CowPriceInvoiceSession::IsAvgWeightEnabled() const
{
	return m_bAvgWeightEnabled;
}

void CowPriceInvoiceSession::SetAvgWeight(const NullDouble& avgWeight)
{
	m_AvgWeight = avgWeight;
	if(m_AvgWeight.IsNull())
		EnableAvgWeight(false);
}

const NullDouble& CowPriceInvoiceSession::GetAvgWeight() const
{
	return m_AvgWeight;
}

void CowPriceInvoiceSession::SetTransactionDate(const CDateTime& transactionDt)
{
	m_TransactionDt = transactionDt;
}

void CowPriceInvoiceSession::SetTransactionPlace(const CString& sTransactionPlace)
{
	m_sTransactionPlace = sTransactionPlace;
}

const CDateTime& CowPriceInvoiceSession::GetTransactionDate() const
{
	return m_TransactionDt;
}

const CString& CowPriceInvoiceSession::GetTransactionPlace() const
{
	return m_sTransactionPlace;
}