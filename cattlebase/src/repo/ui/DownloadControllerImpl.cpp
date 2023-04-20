#include "stdafx.h"
#include "DownloadControllerImpl.h"

bool DownloadControllerImpl::AskCommit()
{
	return true;
}

MissingHentDecisionObj	DownloadControllerImpl::DecideOnMissingHent(const Hent* pRemoteHent,bool bCanSkip)
{
	return MissingHentDecisionObj(MissingHentDecision_Insert,true);
}

HentDiscrepancyDecisionObj  DownloadControllerImpl::DecideOnHentDiscrepancy(const Hent* pLocalHent,const Hent* pRemoteHent)
{
	return HentDiscrepancyDecisionObj(HentDiscrepancyDecision_Merge,true);
}

MissingStockDecisionObj	DownloadControllerImpl::DecideOnMissingStock(const CString& sStockCode)
{
	return MissingStockDecisionObj(StockDiscrepancyDecision_Insert,true);
}

MissingAgentDecisionObj		DownloadControllerImpl::DecideOnMissingAgent(const CString& sAgentCode)
{
	return MissingAgentDecisionObj(MissingAgentDecision_Insert,true);
}
	
Stock* DownloadControllerImpl::ChooseStock()
{
	return NULL;
}

Hent* DownloadControllerImpl::ChooseHent()
{
	return NULL;
}

Agent* DownloadControllerImpl::ChooseAgent()
{
	return NULL;
}

bool DownloadControllerImpl::MergeHent(Hent* pLocal,const Hent* pPurchaseHent)
{
	return false;
}
