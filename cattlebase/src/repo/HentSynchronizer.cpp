#include "stdafx.h"
#include "HentSynchronizer.h"
#include "../inout_port/HentDiffMergeDataDlg.h"
#include "RepoException.h"
#include "../CattleSQL.h"
#include "HentConverter.h"
#include "HentEquivalence.h"
#include <memory>

HentSynchronizer::HentSynchronizer(Context* pContext) : m_pContext(pContext),
														m_LastMissingHentDecision(MissingHentDecision_None,false),
														m_LastHentDiscrepancyDecicion(HentDiscrepancyDecision_None,false)
{
}

HentSyncOutcome HentSynchronizer::SynchronizeHent(const RepoHent* pRepoHent,DownloadController* pController)
{
	Hent* pLocalHent = m_pContext->GetHent(pRepoHent->GetFarmNo());
	
	Hent remoteHent(NULL_HENTID);
	HentConverter::ConvertHent(*pRepoHent,remoteHent);
	Hent* pRemoteHent = &remoteHent;
	
	if(pLocalHent == NULL)
	{
		if(!m_LastMissingHentDecision.IsDoRemember())
			m_LastMissingHentDecision = pController->DecideOnMissingHent(&remoteHent,false);

		if(m_LastMissingHentDecision.GetDecision() != MissingHentDecision_None)
		{
			HentDecisionOutcome outcome = ExecuteMissingHentDecision(m_LastMissingHentDecision,&remoteHent);
			if(!outcome.IsDoFallback())
			{
				return HentSyncOutcome(outcome.GetOutcome(),false);
			}
		}
	}
	else
	{
		RepoHent* pLocalRepoHent = new RepoHent(*pLocalHent);
		RepoHent* pRemoteRepoHent = new RepoHent(*pRepoHent);

		HentEquivalence hentEquivalence(pLocalRepoHent,pRemoteRepoHent);

		if(!hentEquivalence.IsEqual())
		{
			if(!m_LastHentDiscrepancyDecicion.IsDoRemember())
			{
				m_LastHentDiscrepancyDecicion = pController->DecideOnHentDiscrepancy(pLocalHent,pRemoteHent);
			}

			if(m_LastHentDiscrepancyDecicion.GetDecision() != HentDiscrepancyDecision_None)
			{							
				HentDecisionOutcome outcome(ExecuteHentDiscrepancyDecision(m_LastHentDiscrepancyDecicion,pRemoteHent));
				if(!outcome.IsDoFallback())
				{
					return HentSyncOutcome(outcome.GetOutcome(),false);
				}
			}
		}
		else
		{
			return HentSyncOutcome(pLocalHent,false);
		}
	}
	return HentSyncOutcome(NULL,true);
}



HentDecisionOutcome HentSynchronizer::ExecuteMissingHentDecision(MissingHentDecisionObj decision,const Hent* pRemoteHent)
{
	switch(decision.GetDecision())
	{
		case MissingHentDecision_Skip:
			return HentDecisionOutcome(NULL,false);

		case MissingHentDecision_Insert:
			return ExcuteInsertHentDecision(pRemoteHent);

		case MissingHentDecision_Replace:
			return HentDecisionOutcome(NULL,false);
	}

	return HentDecisionOutcome(NULL,true);
	
}

HentDecisionOutcome HentSynchronizer::ExecuteHentDiscrepancyDecision(HentDiscrepancyDecisionObj decision,const Hent* pRemoteHent)
{
	switch(decision.GetDecision())
	{
		case HentDiscrepancyDecision_Merge:
			return ExecuteHentMergeDecision(pRemoteHent);
		case HentDiscrepancyDecision_UseLocal:
			break;
	}
	return HentDecisionOutcome(NULL,false);
}

HentDecisionOutcome HentSynchronizer::ExecuteHentMergeDecision(const Hent* pRemoteHent)
{
	Hent* pLocalHent = m_pContext->GetHent(pRemoteHent->GetFarmNo());

	Hent* pNonConstRemoteHent = const_cast<Hent*>(pRemoteHent);

	if(pLocalHent == NULL)
		throw new RepoException("Unexpected! No local hent for remote hent.");

	CHentDiffMergeDataDlg hentDiffDlg(pLocalHent,pNonConstRemoteHent);
	if(hentDiffDlg.AreDiffrent())
	{			
		if(hentDiffDlg.DoModal() == IDOK)
		{
			if(!pLocalHent->UpdateThisHent(false))
			{
				pLocalHent->RollbackChanges();
				SQLDB.SQLMessageBox(IDS_ERRUPDATINGHENT);				
				return HentDecisionOutcome(NULL,false);
			}
			else
			{
				SQLDB.Commit();
				pLocalHent->CommitChanges();		
				
			}
		}
	}
	return HentDecisionOutcome(pLocalHent,false);
}

HentDecisionOutcome HentSynchronizer::ExcuteInsertHentDecision(const Hent* pRemoteHent)
{
	Hent* pNewHent = m_pContext->InsertHent(pRemoteHent);
	if(!pNewHent)
	{
		return HentDecisionOutcome(NULL,true);		
	}
	SQLDB.Commit();
	return HentDecisionOutcome(pNewHent,false);
}


	

