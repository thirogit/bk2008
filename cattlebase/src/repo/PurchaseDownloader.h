#ifndef __PURCHASEDOWNLOADER_H__
#define __PURCHASEDOWNLOADER_H__

#include "RepoPurchase.h"
#include "DownloadController.h"
#include "context/Context.h"
#include "CowDownloader.h"
#include "../datatypes/NullInvoice.h"
#include "Outcome.h"
#include "HentSynchronizer.h"

typedef Outcome<Agent> AgentDecisionOutcome;
typedef Outcome<Agent> AgentSyncOutcome;


class PurchaseDownloader
{
public:
	PurchaseDownloader(Context* pContext);
	void DownloadPurchase(const RepoPurchase& purchase,DownloadController* pControler);
private:
	Money CalculateInvoiceTotalNet(const RepoInvoice& invoice);
	InvoiceTypes GetInvoiceTypeForRepoInvoiceType(RepoInvoiceType invoiceType);
	NullInvoice* CreateInvoiceTemplate(const RepoInvoice& invoice);
	void DownloadRegularInvoice(const RepoInvoice* pRepoInvoice,const CString& sAgentCode,
								const CString& sPlateNo,DownloadController* pControler);

	void DownloadLumpInvoices(NewPtrFlushArray<const RepoInvoice>& lumpInvoices,
							  const CString& sAgentCode,
							  const CString& sPlateNo,
							  DownloadController* pController);

	bool InsertCowInvoiceEntries(CCowInvoiceEntryArray &cowInvEntries);
	bool InsertCows(CCowArray& cows);
	AgentSyncOutcome SynchronizeAgent(const CString& sAgentCode,DownloadController* pController);
	AgentDecisionOutcome ExecuteAgentDecision(const MissingAgentDecisionObj& decision,const CString& sAgentCode);
	
	
private:
	Context* m_pContext;
	CowDownloader m_CowDownloader;
	HentSynchronizer m_hentSynchronizer;

	MissingAgentDecisionObj m_LastMissingAgentDecision;
	
};


#endif