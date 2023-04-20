#include "stdafx.h"
#include "PurchaseDownloader.h"

#include "../datatypes/VATInvoice.h"
#include "../datatypes/NullInvoice.h"
#include "../datatypes/RRInvoice.h"
#include "NewPtrFlushArray.h"
#include "../global_arrays.h"
#include <memory>
#include "../cattleSQL.h"
#include "dispacher/NewInDocDispatcher.h"
#include "dispacher/InvoiceDispatcher.h"
#include "RepoException.h"
#include "HentConverter.h"

#define DEFAULT_REASON_ID 2

PurchaseDownloader::PurchaseDownloader(Context* pContext) : m_pContext(pContext),
															m_CowDownloader(pContext),
															m_hentSynchronizer(pContext)
{
}

void PurchaseDownloader::DownloadPurchase(const RepoPurchase& purchase,DownloadController* pControler)
{
	NewPtrFlushArray<const RepoInvoice> lumplInvoices;
		
	for(int iInvoice = 0,iInvoiceCount = purchase.GetInvoiceCount(); iInvoice < iInvoiceCount;iInvoice++)
	{	
		const RepoInvoice* pRepoInvoice = purchase.GetInvoice(iInvoice);
		RepoInvoiceType repoInvoiceType = pRepoInvoice->GetInvoiceType();
		if(repoInvoiceType == Invoice_Regular)
		{
			DownloadRegularInvoice(pRepoInvoice,purchase.GetAgentCode(),
									purchase.GetPlateNo(),pControler);	
		}
		else if(repoInvoiceType == Invoice_Lump)
		{
			lumplInvoices.Add(pRepoInvoice);
		}
	}

	if(lumplInvoices.GetSize() > 0)
	{
		DownloadLumpInvoices(lumplInvoices,purchase.GetAgentCode(),
									purchase.GetPlateNo(),pControler);	
	}
}

void PurchaseDownloader::DownloadLumpInvoices(NewPtrFlushArray<const RepoInvoice>& lumpInvoices,
											  const CString& sAgentCode,
											  const CString& sPlateNo,
											  DownloadController* pController)
{
	
	InDoc* pInDoc = new InDoc(NULL_DOCID);

	pInDoc->SetHent(m_pContext->GetHent(0));

	const RepoInvoice* pFirstInvoice = lumpInvoices.GetAt(0);

	CDateTime firstInvoiceDt = pFirstInvoice->GetInvoiceDate();
	CDateTime firstInvoiceTransactionDt = pFirstInvoice->GetTransactionDate();

	if(firstInvoiceTransactionDt.IsNull())
		firstInvoiceTransactionDt = firstInvoiceDt;

	pInDoc->SetDocDate(firstInvoiceDt);
	pInDoc->SetLoadDate(firstInvoiceTransactionDt);
	
	pInDoc->SetReason(ary_reasons.GetReasonDataByReasonId(DEFAULT_REASON_ID));

	AgentSyncOutcome agentSyncOutcome = SynchronizeAgent(sAgentCode,pController);

	if(agentSyncOutcome.IsDoFallback())
	{				
		delete pInDoc;
		return;
	}
	else
	{
		pInDoc->SetAgent(agentSyncOutcome.GetOutcome());	
	}

	pInDoc->SetPlateNo(sPlateNo);

	const RepoInvoice* pRepoInvoice = NULL;
	NewPtrFlushArray<const RepoInvoice>::iterator invoiceIt = lumpInvoices.begin();
	NewPtrFlushArray<RRInvoice> invoiceBucket;

	while(invoiceIt != lumpInvoices.end())
	{
		pRepoInvoice = *(invoiceIt++);
		
		Cow* pCow = NULL;
		const RepoCow* pRepoCow = NULL;
		CCowInvoiceEntryArray cowInvEntries;		
		CowInvoiceEntry *pInvoiceEntry = NULL;
			
		NullInvoice *pNullInvoice = CreateInvoiceTemplate(*pRepoInvoice);	
		RRInvoice* pInvoice = new RRInvoice(pNullInvoice);
		delete pNullInvoice;

		HentSyncOutcome hentOutcome = m_hentSynchronizer.SynchronizeHent(pRepoInvoice->GetInvoiceHent(),pController);
		if(hentOutcome.IsDoFallback())
		{
			return;
		}
		

		for(int iCow = 0,iCowCount = pRepoInvoice->GetCowCount();iCow < iCowCount;iCow++)
		{
			pRepoCow = pRepoInvoice->GetCow(iCow);
			
			pCow = m_CowDownloader.DownloadCow(*pRepoCow,pController);
			if(!pCow)
			{
				pInDoc->GetCows()->FreeAndFlush();
				delete pInDoc;

				//free invoice bucket and invoice entries
				return;
			}

			pCow->SetFirstOwner(hentOutcome.GetOutcome());

			pInvoiceEntry = new CowInvoiceEntry(pCow,pInvoice);
			pInvoiceEntry->SetPrice(pRepoCow->GetPrice().GetDouble());
			pInvoiceEntry->SetWeight(pRepoCow->GetWeight());
			pInvoiceEntry->SetStock(pCow->GetStock());

			cowInvEntries.Add(pInvoiceEntry);

			pInDoc->GetCows()->Add(pCow);
			pCow->SetInDoc(pInDoc);
			pCow->AssignToRRInvoice(pInvoice);
			pCow->SetFirstOwner(hentOutcome.GetOutcome());
		}
		pInvoice->invEntries.Append(cowInvEntries);		
		invoiceBucket.Add(pInvoice);

	}

	if(pInDoc->InsertThisDoc(false))
	{
		CowArrayPtr cowsPtr = pInDoc->GetCows();
		if(!InsertCows(*cowsPtr.GetPtr()))
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGCOW);			
			SQLDB.Rollback();
			return;
		}	

				

		RRInvoice* pInvoice = NULL;
		for(NewPtrFlushArray<RRInvoice>::iterator bucketIt = invoiceBucket.begin(); bucketIt != invoiceBucket.end();bucketIt++)
		{
			pInvoice = *bucketIt;
			if(pInvoice->InsertThisInvoice(false))
			{			
				if(!InsertCowInvoiceEntries(pInvoice->invEntries))
				{
					SQLDB.Rollback();		
					return;
				}						
			}
			else
			{
				SQLDB.SQLMessageBox(IDS_ERRINSERTINGINVOICE);			
				SQLDB.Rollback();
				return;
			}
		}

		Cow* pCow = NULL;
		CCowArray::ArrayIterator cowIt = cowsPtr->iterator();
		while(cowIt.hasNext())
		{
			pCow = *(cowIt++);
			if(!pCow->UpdateThisCow(false))
			{
				SQLDB.SQLMessageBox(IDS_ERRUPDATEINGCOW);			
				SQLDB.Rollback();
				return;
			}
			else
			{
				pCow->CommitChanges();
			}
		}

		SQLDB.Commit();

		for(NewPtrFlushArray<RRInvoice>::iterator bucketIt = invoiceBucket.begin(); 
			bucketIt != invoiceBucket.end();
			bucketIt++)
		{
			InvoiceDispatcher::Dispatch(*bucketIt);
		}
		
		NewInDocDispatcher::Dispatch(pInDoc);
	}
	else
	{
		SQLDB.SQLMessageBox(IDS_ERRINSERTINGDOC);			
		SQLDB.Rollback();
		return;		
	}
}

AgentSyncOutcome PurchaseDownloader::SynchronizeAgent(const CString& sAgentCode,DownloadController* pController)
{
	Agent* pPuchaseAgent = m_pContext->GetAgent(sAgentCode);

	if(pPuchaseAgent == NULL)
	{				
		if(!m_LastMissingAgentDecision.IsDoRemember())
			m_LastMissingAgentDecision = pController->DecideOnMissingAgent(sAgentCode);

		if(m_LastMissingAgentDecision.GetDecision() != MissingAgentDecision_None)
		{
			AgentDecisionOutcome outcome = ExecuteAgentDecision(m_LastMissingAgentDecision,sAgentCode);
			if(!outcome.IsDoFallback())
			{
				return AgentSyncOutcome(outcome.GetOutcome(),false);
			}
		}
	}
	else
	{
		return AgentSyncOutcome(pPuchaseAgent,false);
	}

	return AgentSyncOutcome(NULL,true);
}


void PurchaseDownloader::DownloadRegularInvoice(const RepoInvoice* pRepoInvoice,
												const CString& sAgentCode,
												const CString& sPlateNo,
												DownloadController* pController)
{
	

	HentSyncOutcome hentOutcome = m_hentSynchronizer.SynchronizeHent(pRepoInvoice->GetInvoiceHent(),pController);
	if(hentOutcome.IsDoFallback())
		return;

	InDoc* pInDoc = new InDoc(NULL_DOCID);
	pInDoc->SetHent(hentOutcome.GetOutcome());

	CDateTime invoiceDt = pRepoInvoice->GetInvoiceDate();
	CDateTime transactionDt = pRepoInvoice->GetTransactionDate();

	if(transactionDt.IsNull())
		transactionDt = invoiceDt;

	pInDoc->SetDocDate(invoiceDt);
	pInDoc->SetLoadDate(transactionDt);
	
	pInDoc->SetReason(ary_reasons.GetReasonDataByReasonId(DEFAULT_REASON_ID));

	AgentSyncOutcome agentSyncOutcome = SynchronizeAgent(sAgentCode,pController);

	if(agentSyncOutcome.IsDoFallback())
	{				
		delete pInDoc;
		return;
	}
	else
	{
		pInDoc->SetAgent(agentSyncOutcome.GetOutcome());	
	}

	pInDoc->SetPlateNo(sPlateNo);

	Cow* pCow = NULL;
	const RepoCow* pRepoCow = NULL;
	CCowInvoiceEntryArray cowInvEntries;
	CCowInvoiceEntryArray::AutoArray autoCowInvEntries(cowInvEntries);
	CowInvoiceEntry *pInvoiceEntry = NULL;

	std::auto_ptr<Invoice> pInvoiceDelete;	
	NullInvoice *pNullInvoice = CreateInvoiceTemplate(*pRepoInvoice);	
	VATInvoice* pInvoice = new VATInvoice(pNullInvoice);
	delete pNullInvoice;
	pInvoiceDelete.reset(pInvoice);

	for(int iCow = 0,iCowCount = pRepoInvoice->GetCowCount();iCow < iCowCount;iCow++)
	{
		pRepoCow = pRepoInvoice->GetCow(iCow);
		
		pCow = m_CowDownloader.DownloadCow(*pRepoCow,pController);
		if(!pCow)
		{
			pInDoc->GetCows()->FreeAndFlush();
			delete pInDoc;
			return;
		}

		pInvoiceEntry = new CowInvoiceEntry(pCow,pInvoice);
		pInvoiceEntry->SetPrice(pRepoCow->GetPrice().GetDouble());
		pInvoiceEntry->SetWeight(pRepoCow->GetWeight());
		pInvoiceEntry->SetStock(pCow->GetStock());

		cowInvEntries.Add(pInvoiceEntry);

		pInDoc->GetCows()->Add(pCow);
		pCow->SetInDoc(pInDoc);
	}

	if(pInDoc->InsertThisDoc(false))
	{
		if(!InsertCows(*pInDoc->GetCows().GetPtr()))
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGCOW);			
			SQLDB.Rollback();
			return;
		}

		pInvoice->invoiceDocs.Add(pInDoc);
		pInvoice->invEntries.Append(cowInvEntries);

		if(pInvoice->InsertThisInvoice(false))
		{			
			if(!InsertCowInvoiceEntries(cowInvEntries))
			{
				SQLDB.Rollback();		
				return;
			}

			pInDoc->AssignToInvoice(pInvoice);
			if(!pInDoc->UpdateThisDoc(false))
			{
				SQLDB.Rollback();
				SQLDB.SQLMessageBox(IDS_ERRUPDATINGDOCSFORINV);
				return;											
			}
			
			pInDoc->CommitChanges();
			
			SQLDB.Commit();
			pInvoiceDelete.release();
			autoCowInvEntries.Release();
			InvoiceDispatcher::Dispatch(pInvoice);
			NewInDocDispatcher::Dispatch(pInDoc);
		}
		else
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGINVOICE);			
			SQLDB.Rollback();
			return;
		}
	}
	else
	{
		SQLDB.SQLMessageBox(IDS_ERRINSERTINGDOC);			
		SQLDB.Rollback();
		return;		
	}

	

}

bool PurchaseDownloader::InsertCows(CCowArray& cows)
{
	CCowArray::ArrayIterator cowIt = cows.iterator();
	while(cowIt.hasNext())
	{
		if(!(*(cowIt++))->InsertThisCow(false))
		{
			return false;
		}
	}
	return true;
}

Money PurchaseDownloader::CalculateInvoiceTotalNet(const RepoInvoice& repoInvoice)
{
	Money totalNet(0.0);
	const RepoCow* pRepoCow = NULL;
	for(int iCow = 0,iCowCount = repoInvoice.GetCowCount();iCow < iCowCount;iCow++)
	{
		pRepoCow = repoInvoice.GetCow(iCow);
		totalNet += pRepoCow->GetPrice();
	}
	return totalNet;			
}

NullInvoice* PurchaseDownloader::CreateInvoiceTemplate(const RepoInvoice& repoInvoice)
{
	NullInvoice* pInvoice = new NullInvoice();
	pInvoice->SetCustomNumber(repoInvoice.GetCustomNumber());
	pInvoice->SetInvoiceDate(repoInvoice.GetInvoiceDate());

	pInvoice->SetInvoiceNetValue(CalculateInvoiceTotalNet(repoInvoice));

	pInvoice->SetInvoiceType(GetInvoiceTypeForRepoInvoiceType(repoInvoice.GetInvoiceType()));
	
	pInvoice->SetPayDueDays(repoInvoice.GetPayDueDays());

	pInvoice->SetPayWay(repoInvoice.GetPayWay());
	pInvoice->SetTransactionDate(repoInvoice.GetTransactionDate());
	pInvoice->SetTransactionPlace(repoInvoice.GetTransactionPlace());
	pInvoice->SetVATRate(repoInvoice.GetVatRate().GetDouble());

	return pInvoice;
}

InvoiceTypes PurchaseDownloader::GetInvoiceTypeForRepoInvoiceType(RepoInvoiceType invoiceType)
{
	switch(invoiceType)
	{
		case Invoice_Lump:
			return IN_INV_RR;
		case Invoice_Regular:
			return IN_INV_VAT;
	}

	throw new RepoException("Unsuported repo invoice type");
}

bool PurchaseDownloader::InsertCowInvoiceEntries(CCowInvoiceEntryArray &cowInvEntries)
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

AgentDecisionOutcome PurchaseDownloader::ExecuteAgentDecision(const MissingAgentDecisionObj& decision,const CString& sAgentCode)
{
	Agent agentTemplate(NULL_AGENTID);
	agentTemplate.SetAgentCode(sAgentCode);
	
	Agent* pNewAgent = m_pContext->InsertAgent(&agentTemplate);

	return AgentDecisionOutcome(pNewAgent,false);
}