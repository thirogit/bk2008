#ifndef __INVOICEDRIVER_H__
#define __INVOICEDRIVER_H__

#include "../../datatypes/InvoiceData.h"
#include "../../datatypes/invoice/InvoiceContext.h"
#include "../../datatypes/CowPriceData.h"

class CBaseInvoiceDlg;

class InvoiceDriver
{
public:
	InvoiceDriver(InvoiceContext* pInvoiceContext,CWnd* pParent = NULL);   
	void Do();
	~InvoiceDriver();
protected:
	virtual void AddItem() = 0;
	virtual void DelItem() = 0;
	virtual int GetPayDueDays() const = 0;
	virtual VATRate GetVATRate() const = 0;
	virtual CString GetExtras() const = 0;
	virtual CString GetCustomNumber() const = 0;
	virtual CDateTime GetInvoiceDate() const = 0;
	virtual InvoiceData::PAYWAYS GetPayway() const = 0 ;
	virtual bool CommitInvoice() = 0;
	virtual CString GetInvDlgCaption() = 0;
	virtual void TabChanged(int newTabIndex) = 0;
	virtual Hent* GetInvoiceHent() const = 0;


	bool InsertCowInvoiceEntries(CCowInvoiceEntryArray &cowInvEntries);
	void UpdateInvoiceValues(InvoiceData* pInvoice);
	bool DeleteInvoiceEntries(CCowInvoiceEntryArray& invoiceEntries);
	CowInvoiceEntry* CreateNewInvoiceEntry(InvoiceData* pInvoice,CowPriceData *pCowPrice);

protected:
	InvoiceContext* m_pInvoiceContext;
	CWnd* m_pParent;
	CBaseInvoiceDlg* pInvoiceDialog;

	friend class CBaseInvoiceDlg;
};

#endif