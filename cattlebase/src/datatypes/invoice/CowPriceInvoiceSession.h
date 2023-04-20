#ifndef __COWPRICEINVOICESESSION_H__
#define __COWPRICEINVOICESESSION_H__

#include "../../classes/types/VATRate.h"
#include "InvoiceContext.h"
#include "../CowPriceDataArray.h"
#include "../NullInvoice.h"

class CowPriceInvoiceSession
{
public:	
	CowPriceInvoiceSession(const InvoiceContext *pInvoiceContext);
	~CowPriceInvoiceSession();

	const VATRate& GetVATRate() const;
	void SetVATRate(const VATRate& vatRate);
	const InvoiceContext* GetInvoiceContext() const;
	Rule* GetRule4All() const;
	void SetRule4All(Rule* pRule4All);

	void SetUseStockMyPerKgPrice(bool bUse);
	bool IsUseStockMyPerKgPrice() const;

	void SetCustomPricePreKgForAll(const Money& customPricePerKgForAll);
	const Money& GetCustomPricePerKgForAll() const;
	void SetUseCustomPricePerKgForAll(bool bUse);
	bool IsUseCustomPricePerKgForAll() const;

	void EnableAvgPrice(bool bEnable);
	bool IsAvgPriceEnabled() const;
	void SetAvgPrice(const Money& avgPrice);
	const Money& GetAvgPrice() const;

	void EnableAvgWeight(bool bEnable);
	bool IsAvgWeightEnabled() const;
	void SetAvgWeight(const NullDouble& avgWeight);
	const NullDouble& GetAvgWeight() const;
	
	CowPriceData* AddCow(Cow* pCow);
	int GetCowCount() const;
	CowPriceData* GetCowPrice(int iIndex) const;
	void DeleteCowPrice(int iIndex);
	CCowPriceDataArray::ArrayIterator GetCowPriceIterator();
	void SortPrices(CCowPriceDataArray::SortArrayComparator& comparator);

	void GetSessionCows(CCowArray& cows) const;

	CDocDataArray& GetInvoiceDocs();

	Money GetTotalNetValue() const;
	Money GetTotalVATValue() const;
	Money GetTotalGrossValue() const;
	Weight GetTotalWeight() const;

	int GetSelectedDocItem() const;
	int GetSelectedCowPriceItem() const;

	void SetSelectedDocItem(int iItem);
	void SetSelectedCowPriceItem(int iItem);

	unsigned short GetPayDueDays() const;	
	CString GetExtras() const;
	CString GetCustomNumber() const;
	CDateTime GetInvoiceDate() const;
	Invoice::PAYWAYS GetPayway() const;

	void SetPayDueDays(unsigned short paydueDays);	
	void SetExtras(const CString& sExtras);
	void SetCustomNumber(const CString& sCustomNumber);
	void SetInvoiceDate(const CDateTime& invoiceDt);
	void SetPayway(Invoice::PAYWAYS payway);
	Hent* GetInvoiceHent() const;

	void SetTransactionDate(const CDateTime& transactionDt);
	void SetTransactionPlace(const CString& sTransactionPlace);

	const CDateTime& GetTransactionDate() const;
	const CString& GetTransactionPlace() const;

	virtual void AddItem() = 0;
	virtual void DelItem() = 0;
	virtual CString GetSessionTitle() const = 0;
	virtual bool CommitInvoice() = 0;
protected:
	void SetInvoiceHent(Hent* pHent);
	CowInvoiceEntry* CreateNewInvoiceEntry(Invoice* pInvoice,CowPriceData *pCowPrice);
	NullInvoice* CreateNullInvoice();

	void UpdateInvoiceValues(Invoice* pInvoice);
	bool InsertCowInvoiceEntries(CCowInvoiceEntryArray &cowInvEntries);	
	bool DeleteInvoiceEntries(CCowInvoiceEntryArray& invoiceEntries);
private:
	Rule *m_pRule4All;
	bool m_bAvgPriceEnabled;
	bool m_bAvgWeightEnabled;
	Money m_AvgPrice;
	NullDouble m_AvgWeight;
	VATRate m_VATRate;
	const InvoiceContext *m_pInvoiceContext;
	bool m_bUseStockMyPerKgPrice;

	Money m_CustomPricePerKgForAll;
	bool m_bCustomPricePerKgForAll;

	CCowPriceDataArray m_cowprices;
	CDocDataArray m_invoiceDocs;

	int m_selectedDocItem;
	int m_selectedCowPriceItem;

	unsigned short m_paydueDays;
	CString m_sExtras;
	CString m_sCustomNumber;
	CDateTime m_dtInvoiceDate;
	Invoice::PAYWAYS m_payway;

	Hent* m_pInvoiceHent;

	CDateTime m_TransactionDt;
	CString m_sTransactionPlace;
};

#endif