#ifndef __COWDOWNLOADER_H__
#define __COWDOWNLOADER_H__

#include "../datatypes/Cow.h"
#include "RepoCow.h"
#include "context/Context.h"
#include "DownloadController.h"
#include "Outcome.h"
#include "HentSynchronizer.h"

typedef Outcome<Stock> StockDecisionOutcome;

class CowDownloader
{
public:	
	CowDownloader(Context* pContext);	
	Cow* DownloadCow(const RepoCow& cow,DownloadController* pController);	
protected:
	StockDecisionOutcome ExecuteMissingStockDecision(MissingStockDecisionObj decision,const CString& sStockCode,DownloadController* pController); 
	StockDecisionOutcome ExecuteStockInsertDecision(const CString& sStockCode);
	StockDecisionOutcome ExecuteStockReplaceDecision(DownloadController* pController);

private:
	Context* m_pContext;
	HentSynchronizer m_hentSynchronizer;
	
	MissingStockDecisionObj		m_LastMissingStockDecision;


};
	

#endif