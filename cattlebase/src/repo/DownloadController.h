#ifndef __DOWNLOADCONTROLLER_H__
#define __DOWNLOADCONTROLLER_H__

#include "DiscrepancyDecision.h"

#include "../datatypes/Hent.h"
#include "../datatypes/Stock.h"
#include "../datatypes/Agent.h"


enum MissingHentDecision
{	
	MissingHentDecision_None,
	MissingHentDecision_Skip,
	MissingHentDecision_Insert,
	MissingHentDecision_Replace
};

enum HentDiscrepancyDecision
{
	HentDiscrepancyDecision_None,
	HentDiscrepancyDecision_Merge,
	HentDiscrepancyDecision_UseLocal,	
};

enum MissingStockDecision
{
	MissingStockDecision_None,
	StockDiscrepancyDecision_Insert,
	StockDiscrepancyDecision_Replace	
};

enum MissingAgentDecision
{
	MissingAgentDecision_None,
	MissingAgentDecision_Skip,
	MissingAgentDecision_Insert,
	MissingAgentDecision_Replace	
};

typedef DiscrepancyDecision<MissingHentDecision, MissingHentDecision_None> MissingHentDecisionObj; 
typedef DiscrepancyDecision<HentDiscrepancyDecision, HentDiscrepancyDecision_None> HentDiscrepancyDecisionObj;
typedef DiscrepancyDecision<MissingStockDecision, MissingStockDecision_None> MissingStockDecisionObj; 
typedef DiscrepancyDecision<MissingAgentDecision, MissingAgentDecision_None> MissingAgentDecisionObj;

class DownloadController
{
public:	
	virtual bool AskCommit() = 0;
	virtual MissingHentDecisionObj		DecideOnMissingHent(const Hent* pRemoteHent,bool bCanSkip) = 0;
	virtual HentDiscrepancyDecisionObj  DecideOnHentDiscrepancy(const Hent* pLocalHent,const Hent* pRemoteHent) = 0;
	virtual MissingStockDecisionObj		DecideOnMissingStock(const CString& sStockCode) = 0;
	virtual MissingAgentDecisionObj		DecideOnMissingAgent(const CString& sAgentCode) = 0;
	
	virtual Stock* ChooseStock() = 0;
	virtual Hent* ChooseHent() = 0;
	virtual Agent* ChooseAgent() = 0;
	virtual bool MergeHent(Hent* pLocal,const Hent* pPurchaseHent) = 0;
	
};

#endif