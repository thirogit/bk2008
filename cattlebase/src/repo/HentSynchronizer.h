#ifndef __HENTSYNCHRONIZER_H__
#define __HENTSYNCHRONIZER_H__

#include "../datatypes/Hent.h"
#include "RepoHent.h"
#include "context/Context.h"
#include "DownloadController.h"
#include "Outcome.h"

typedef Outcome<Hent> HentDecisionOutcome;
typedef Outcome<Hent> HentSyncOutcome;

class HentSynchronizer
{
public:
	HentSynchronizer(Context* pContext);
	HentSyncOutcome SynchronizeHent(const RepoHent* pRepoHent,DownloadController* pController);
protected:
	HentDecisionOutcome  ExcuteInsertHentDecision(const Hent* pRemoteHent);
	HentDecisionOutcome  ExecuteHentMergeDecision(const Hent* pRemoteHent);
	HentDecisionOutcome  ExecuteMissingHentDecision(MissingHentDecisionObj decision,const Hent* pRemoteHent);
	HentDecisionOutcome  ExecuteHentDiscrepancyDecision(HentDiscrepancyDecisionObj decision,const Hent* pRemoteHent);	
	
private:
	
	HentDiscrepancyDecisionObj	m_LastHentDiscrepancyDecicion;
	MissingHentDecisionObj		m_LastMissingHentDecision;
	Context*					m_pContext;
	
	
};

#endif