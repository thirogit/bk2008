#ifndef __DOWNLOADCONTROLLERIMPL_H__
#define __DOWNLOADCONTROLLERIMPL_H__

#include "../DownloadController.h"

class DownloadControllerImpl : public DownloadController
{
public:	
	virtual bool AskCommit();
	virtual MissingHentDecisionObj		DecideOnMissingHent(const Hent* pRemoteHent,bool bCanSkip);
	virtual HentDiscrepancyDecisionObj  DecideOnHentDiscrepancy(const Hent* pLocalHent,const Hent* pRemoteHent);
	virtual MissingStockDecisionObj		DecideOnMissingStock(const CString& sStockCode);
	virtual MissingAgentDecisionObj		DecideOnMissingAgent(const CString& sAgentCode);
	
	virtual Stock* ChooseStock();
	virtual Hent* ChooseHent();
	virtual Agent* ChooseAgent();
	virtual bool MergeHent(Hent* pLocal,const Hent* pPurchaseHent);
	
};

#endif