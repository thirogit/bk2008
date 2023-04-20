#ifndef __INVOICEDATA_H__
#define __INVOICEDATA_H__

#include "RawData.h"
#include "DocDataArray.h"
#include "InvoiceTypes.h"

class InvoiceData : public RawData
{
public:

	enum PAYWAYS 
	{
		PAY_CASH = '$',
		PAY_TRANS = '@'
	};
	

	InvoiceData();

	virtual void CopyFrom(const InvoiceData& copyFrom);
		
	int invoiceno;
	int invoiceid;
	InvoiceTypes invtype;
	CString extras;
	CDateTime invoicedate;
	CDateTime paid;
	Money invoicevalue;	//netto
	CDocDataArray invDocs;
	double VATrate;
	PAYWAYS payway;
	InvoiceData *correct; //pointer to correcting invoice 
	InvoiceData *correctfor; //this is a correct for some other invoice ,valid only if invoicetype == CORRCT_INV;
	CString customnumber;

	enum SQLUPNAMES
		{
		 update_extras,
		 update_invoicedate,
         update_paid,
		 update_invoicevalue,
		 update_VATrate,
		 update_payway,
		 update_customnumber,
		 sql_update_size
		};

	typedef UpdateData<sql_update_size,SQLUPNAMES> UpdateInvoiceData;
	
	UpdateInvoiceData SQLUpdate;

	virtual bool UpdateThisInvoice(bool bCommit = true);
	virtual bool InsertThisInvoice(bool bCommit = true);
	virtual bool DeleteThisInvoice(bool bCommit = true);
	Money& GetInvoiceValue(double *outVATrate = NULL);
	Money GetBruttoValueAfterCorrect();
	virtual int GetCowQty();

	virtual CString GetInvoiceNoStr();
	
	//void GetLinkNoStr(CString &outLink,bool blinkdoc = false);
	
	virtual BuyOrSell GetBuyOrSell();
	virtual HentData* GetInvoiceHent();

	InvoiceTypes GetInvoiceType();
	bool CanEditThisInvoice();
	bool IsPaid();
	bool CanDeleteThisInv();
	
	
};

#endif