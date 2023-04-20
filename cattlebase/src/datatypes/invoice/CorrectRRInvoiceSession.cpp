#include "stdafx.h"
#include "CorrectRRInvoiceSession.h"
#include "../../exceptions/StrException.h"
#include "../../CattleSQL.h"
#include "../../dispacher/InvoiceDispatcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCorrectRRInvoiceSession::CCorrectRRInvoiceSession(RRInvoice *pInvoice,InvoiceContext* pInvoiceContext) : 	CowPriceInvoiceSession(pInvoiceContext)
{
	if(!pInvoice->CanEditThisInvoice())
	{
		m_Inv2Edit = pInvoice->GetLastGoodRRInvoice();		
	}
	else
		throw new CStrException("Cant create correcting invoice for this invoice, it is not valid for edit.");
	

	SetVATRate(m_Inv2Edit->GetVATRate());
	SetPayDueDays(m_Inv2Edit->GetPayDueDays());	
	SetExtras(m_Inv2Edit->GetExtras());
	SetCustomNumber(m_Inv2Edit->GetCustomNumber());
	SetInvoiceDate(m_Inv2Edit->GetInvoiceDate());
	SetPayway(m_Inv2Edit->GetPayWay());
	SetInvoiceHent(m_Inv2Edit->GetInvoiceHent());
	SetTransactionDate(m_Inv2Edit->GetTransactionDate());
	SetTransactionPlace(m_Inv2Edit->GetTransactionPlace());

	CCowInvoiceEntryArray::ArrayIterator it = m_Inv2Edit->invEntries.iterator();

	while(it.hasNext())
	{	
		CowInvoiceEntry *pEntry = *(it++);
		CowPriceData *cowPrice = AddCow(pEntry->GetCow());
		cowPrice->SetCustomPriceNetto(pEntry->GetPrice());
		cowPrice->SetCustomStock(pEntry->GetStock());
		cowPrice->SetCustomWeight(pEntry->GetWeight());		
	}

	GetInvoiceDocs().Add(m_Inv2Edit->GetInDoc());
	
}

CString CCorrectRRInvoiceSession::GetSessionTitle() const
{
	CString caption;
	caption.Format(IDS_INVCORRECTCAPTION,m_Inv2Edit->GetInvoiceNo());
	return caption;
}

void CCorrectRRInvoiceSession::AddItem()
{
	AfxMessageBox(IDS_CANTADDCOWSWHENCREATINGCORRECTINVOICE);
}
void CCorrectRRInvoiceSession::DelItem()
{
	AfxMessageBox(IDS_CANTDELCOWSWHENCREATINGCORRECTINVOICE);
}


bool CCorrectRRInvoiceSession::CommitInvoice()
{
	RRInvoice *pInvoice = NULL;
	Cow *cowd = NULL;
	
	auto_ptr<RRInvoice> pInvoiceDelete;

	
	NullInvoice *pNullInvoice = CreateNullInvoice();
	pNullInvoice->SetInvoiceType(CORRCT_INV);
	pNullInvoice->SetCorrectFor(m_Inv2Edit);
	pInvoice = new RRInvoice(pNullInvoice);
	delete pNullInvoice;

	pInvoiceDelete.reset(pInvoice);
	pInvoice->SetInvoiceNetValue(GetTotalNetValue());	
					
	if(pInvoice->InsertThisInvoice(false))
	{			
		CCowInvoiceEntryArray cowInvEntries;
		CCowInvoiceEntryArray::AutoArray autoCowInvEntries(cowInvEntries);

		CCowPriceDataArray::ArrayIterator it = GetCowPriceIterator();
		while(it.hasNext())
		{
			cowInvEntries.Add(CreateNewInvoiceEntry(pInvoice,*(it++)));
		}

		if(!InsertCowInvoiceEntries(cowInvEntries))
		{
			SQLDB.Rollback();		
			return false;
		}
		pInvoice->invEntries.Append(cowInvEntries);
		m_Inv2Edit->SetCorrect(pInvoice);
		SQLDB.Commit();			
		autoCowInvEntries.Release();
		pInvoiceDelete.release();
		InvoiceDispatcher::Dispatch(pInvoice);			
	}
	else
	{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGINVOICE);			
			SQLDB.Rollback();
			return false;
	}

	
	return true;
}

