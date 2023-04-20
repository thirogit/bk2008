#include "stdafx.h"
#include "CowDownloader.h"
#include <memory>
#include "../global_arrays.h"

CowDownloader::CowDownloader(Context* pContext) : m_pContext(pContext),
												  m_LastMissingStockDecision(MissingStockDecision_None,false),
												  m_hentSynchronizer(pContext)
												  
{
}

Cow* CowDownloader::DownloadCow(const RepoCow& cow,DownloadController* pController)
{

	std::auto_ptr<Cow> ptrLocalCow = std::auto_ptr<Cow>(new Cow(NULL_COWID));


	ptrLocalCow->SetEAN(cow.GetCowNo());			
	ptrLocalCow->SetSex(cow.GetSex());				
	ptrLocalCow->SetLatitude(cow.GetLatitude());
	ptrLocalCow->SetLongitude(cow.GetLongitude());
	ptrLocalCow->SetPassNo(cow.GetPassportNo());
	ptrLocalCow->SetPassDate(cow.GetPassportIssueDt());
    ptrLocalCow->SetHealthCertNo(cow.GetHealthCertNo());
	ptrLocalCow->SetMotherEAN(cow.GetMotherNo());
	ptrLocalCow->SetBirthPlace(cow.GetBirthPlace());
	ptrLocalCow->SetBirthDate(cow.GetBirthDt());

	CString sStockCode = cow.GetStockCode();
	Stock* pStock = m_pContext->GetStock(sStockCode);
	if(!pStock)
	{
		if(!m_LastMissingStockDecision.IsDoRemember())
			m_LastMissingStockDecision = pController->DecideOnMissingStock(sStockCode);

		if(m_LastMissingStockDecision.GetDecision()  == MissingStockDecision_None)
		{		
			return NULL;
		}

		StockDecisionOutcome outcome = ExecuteMissingStockDecision(m_LastMissingStockDecision,sStockCode,pController);
		if(outcome.IsDoFallback())
		{
			return NULL;
		}
		pStock = outcome.GetOutcome();		
	}
	
	ptrLocalCow->SetStock(pStock);

	ptrLocalCow->SetWeight(cow.GetWeight());

	RepoHent* pRepoFirstHent = cow.GetFirstOwner();
	if(pRepoFirstHent)
	{
		HentSyncOutcome outcome = m_hentSynchronizer.SynchronizeHent(pRepoFirstHent,pController);
		if(outcome.IsDoFallback())
			return NULL;

		ptrLocalCow->SetFirstOwner(outcome.GetOutcome());
	}
	
	return ptrLocalCow.release();
	
};




StockDecisionOutcome CowDownloader::ExecuteMissingStockDecision(MissingStockDecisionObj decision,const CString& sStockCode,DownloadController* pController)
{
	switch(decision.GetDecision())
	{
	case StockDiscrepancyDecision_Insert:
		return ExecuteStockInsertDecision(sStockCode);		
	case StockDiscrepancyDecision_Replace:
		return ExecuteStockReplaceDecision(pController);
	default:
		return StockDecisionOutcome(NULL,true);		
	}

}

StockDecisionOutcome CowDownloader::ExecuteStockInsertDecision(const CString& sStockCode)
{
	Stock stockTemplate(NULL_STOCKID);
	stockTemplate.SetStockCode(sStockCode);

	Stock* pNewStock = m_pContext->InsertStock(&stockTemplate);
	if(!pNewStock)
	{
		return StockDecisionOutcome(NULL,true);		
	}
	return StockDecisionOutcome(pNewStock,false);
}

StockDecisionOutcome CowDownloader::ExecuteStockReplaceDecision(DownloadController* pController)
{
	//pController->ChooseStock();
	return StockDecisionOutcome(NULL,true);
}




