#ifndef __IMPORTCTRL_H__
#define __IMPORTCTRL_H__

#include "../DataTypes.h"
#include "../taskprogress/ProgressIFace.h"
#include "ImportSettings.h"

typedef CMap<Stock*,Stock*,Stock*,Stock*> StockToStockMap;
typedef CMap<Hent*,Hent*,Hent*,Hent*> HentToHentMap;



class CImportCtrl
{
public:
	CImportCtrl(CWnd* pParentWnd);
	void ImportInDoc(const CString &inputFile);
	
private:

	bool ImportInDoc(const CString &inputFile,const ImportSettings& importSettings,ITaskProgres *progress);

	bool ValidateDuplicateAliasOrWetNo(Hent* pHent);
	
	bool MergeStocks(		CStockDataArray &stockPool,
							StockToStockMap &stockMapping,
							bool bInsertMissingStocks,
							ITaskProgres *taskProgress);

	bool MergeHents(CHentArray &hentPool,
					HentToHentMap &hentMapping,
					bool bInsertMissingHents,
					ITaskProgres *taskProgress);

	bool FindCowDuplicates(CowArrayPtr cows);
	void FindRRInvoiceForCows(CowArrayPtr cows,CInvoiceDataArray& rrInvoices);

	bool MergeCows(	CCowArray &cowPool,
					StockToStockMap &stockMapping,
					HentToHentMap &hentMapping,
					ITaskProgres *taskProgress);


	bool MergeDocs(	CDocDataArray &docPool,
					HentToHentMap &hentMapping,
					Hent *pCompany,
					bool bSaveOriginalHent,
					ITaskProgres *taskProgress);

	bool MergeInvoices( CInvoiceDataArray& invoices,
						StockToStockMap &stockMapping,
						ITaskProgres *taskProgress);

private:
	CWnd* m_pParentWnd;
};

#endif