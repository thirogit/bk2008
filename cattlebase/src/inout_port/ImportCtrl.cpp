#include "stdafx.h"
#include "ImportCtrl.h"
#include "ImportExportDocs.h"
#include "ImportExportTasks.h"
#include "inout_port/StockMergeDlg.h"
#include "inout_port/HentMergeDlg.h"
#include "CattleSQL.h"
#include "inout_port/InfoTextBoxDlg.h"
#include "../global_arrays.h"
#include "../mainfrm_ctrl.h"
#include "../PassportStorage/PassImgBase.h"
#include "../classes/match/CowMatcher.h"
#include "IssueValueDlg.h"
#include "../classes/forallactions/CountAction.h"
#include "FileDeleter.h"
#include "../dispacher/NewHentDispatcher.h"
#include "../dispacher/NewInDocDispatcher.h"
#include "../dispacher/InvoiceDispatcher.h"
#include "HentDiffMergeDataDlg.h"
#include "log.h"
#include "ImportExportErrorCodeTranslator.h"
#include "../classes/TempStringLoader.h"
#include "../Utils/StringFormat.h"
#include "ImportOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CImportCtrl::CImportCtrl(CWnd* pParentWnd) : m_pParentWnd(pParentWnd)
{
}

void CImportCtrl::FindRRInvoiceForCows(CowArrayPtr cows,CInvoiceDataArray& rrInvoices)
{
	std::set<Invoice*> RRInvsSet;
	CCowArray::ArrayIterator cowIt = cows->iterator();
	while(cowIt.hasNext())
	{
		Cow* pCow = *(cowIt++);
		if(pCow->GetRRInvoice())
			RRInvsSet.insert(pCow->GetRRInvoice());
	}
	std::set<Invoice*>::iterator RRInvIt = RRInvsSet.begin();
	while(RRInvIt != RRInvsSet.end())
		rrInvoices.Add(*(RRInvIt++));

}

void  CImportCtrl::ImportInDoc(const CString &inputFile)
{
	ImportSettings importSettings;
	importSettings.Load();
	CImportOptionsDlg importOptsDlg(importSettings,m_pParentWnd);

	UINT uiDialogResult = importOptsDlg.DoModal();
	bool bDoImport = uiDialogResult == IDSAVEANDUSE || uiDialogResult == IDUSEONLYONCE;
	if(uiDialogResult == IDSAVEANDUSE)
	{
		importSettings.Save();		
	}

	if(bDoImport)
	{

		ImportExportTaskTranslator importTaskTrans;
		CTaskProgressDialog TaskDlg(CTempStringLoader(IDS_IMPORTINGDOCS),&importTaskTrans);
		TaskDlg.Create();

		if(!ImportInDoc(inputFile,importSettings,&TaskDlg))
		{
			if(!TaskDlg.CheckCancelButton())
			{
				AfxMessageBox(TaskDlg.GetLastError(),MB_OK);
			}
		}
	}
}




bool CImportCtrl::ImportInDoc(const CString &inputFile,const ImportSettings& importSettings,ITaskProgres *progress)
{
	CImportExportDocs iwd;
	
	Doc *pDoc;
	Cow *pCow;
	Stock *pStock;
	Hent* pHent;
	
	StockToStockMap stockMapping;
	HentToHentMap hentMapping;
	CString mergeText;

	ImportDocContext importDocContext;
	CStockDataArray stockPool;
	CDocDataArray docPool;
	CHentArray hentPool;
	CCowArray cowPool;
	CInvoiceDataArray rrinvPool;

	importDocContext.docFilePath = inputFile;

	int i;

	
	if(!iwd.ReadInDoc(importDocContext,progress))
	{
		return false;		
	}

	stockPool.Copy(importDocContext.stockPool);
	docPool.Copy(importDocContext.docPool);
	hentPool.Copy(importDocContext.hentPool);
	rrinvPool.Copy(importDocContext.rrInvPool);

	Hent* pCompany = new Hent(NULL_HENTID);
	pCompany->CopyFrom(importDocContext.company);

	CStockDataArray::AutoArray autoStockPool(stockPool);
	CDocDataArray::AutoArray autoDocPool(docPool);
	CCowArray::AutoArray autoCowPool(cowPool);
	CHentArray::AutoArray autoHentPool(hentPool);
	CInvoiceDataArray::AutoArray autoRRInvPool(rrinvPool);
	std::auto_ptr<Hent> pCompanyDeleter(pCompany);


	CCowArray rejectedCows;
	CCowArray::AutoArray autoRejectedCows(rejectedCows);
	CDocDataArray rejectedDocs;
	CDocDataArray::AutoArray autoRejectedDocs(rejectedDocs);
	CInvoiceDataArray rejectedRRInvs;
	CInvoiceDataArray::AutoArray autoRejectedRRInvs(rejectedRRInvs);
	
	progress->SetTask(FIND_DUPLICATES,docPool.GetSize());
	CDocDataArray::ArrayIterator docIt = docPool.iterator();
	while(docIt.hasNext())
	{
		pDoc = *(docIt++);
		if(!FindCowDuplicates(pDoc->GetCows()))
		{
			FindRRInvoiceForCows(pDoc->GetCows(),rejectedRRInvs);
			rejectedDocs.Add(pDoc);
			rejectedCows.AppendPtr(pDoc->GetCows());
		}
		else
			cowPool.AppendPtr(pDoc->GetCows());

		if(!progress->OnStep(docIt.index()))
			return false;
	}

	

	docPool.RemoveArray(rejectedDocs);
	rrinvPool.RemoveArray(rejectedRRInvs);

	if(rejectedDocs.GetSize() > 0)
	{
		CString msgText;
		msgText.LoadString(IDS_IMPORTREJECTEDDOCS);
		docIt = rejectedDocs.iterator();

		while(docIt.hasNext())
		{
			pDoc = *(docIt++);

			msgText.AppendFormat(IDS_IMPORTREJECTEDDOCLINE,
					(LPCSTR)pDoc->GetDocDate().GetDateInFmt(CDateTime::YYYYdashMMdashDD),
					(LPCSTR)pDoc->GetHent()->GetName());

			CCowArray::ArrayIterator cowIt = pDoc->GetCows()->iterator();
			while(cowIt.hasNext())
			{
				pCow = *(cowIt++);
				msgText.AppendFormat(IDS_IMPORTREJECTEDCOWLINE,
					(LPCSTR)pCow->GetEAN(),
					(LPCSTR)pCow->GetPassNo(),
					(LPCSTR)pCow->GetSex(),
					(LPCSTR)pCow->GetWeight().ToString());
			}
		}
		CInfoTextBoxDlg::ShowTextInfoBox(msgText,m_pParentWnd);

		msgText.Format(IDS_REJDOCSQUESTION,rejectedDocs.GetSize()+docPool.GetSize(),rejectedDocs.GetSize());
		if(AfxMessageBox(msgText,MB_YESNO) != IDYES)
		{		
			return true;
		}
	}

	if(docPool.GetSize() == 0)
	{			
		progress->OnTaskError(CTempStringLoader(IDS_ALLDOCSREJECTED));
		return false;
	}

	CMap<Cow*,Cow*,CString,CString&> cowDataToPassPath;
	CFileDeleter tmpFileDeleter;
	
	bool bFileCorrupted = false;
	POSITION pos;

	if(importDocContext.cowPassMap.GetSize() > 0)
	{
		pos = importDocContext.cowPassMap.GetStartPosition();
		CString tmpPassPath;
		int cowid;
		while(pos)
		{
			importDocContext.cowPassMap.GetNextAssoc(pos,cowid,tmpPassPath);
			pCow = cowPool.GetCowByCowId(cowid);
			if(!pCow)
			{
				LOG("CANT FIND COW WITH ID %d",cowid);
				bFileCorrupted = true;
			}
			else
			{
				cowDataToPassPath.SetAt(pCow,tmpPassPath);
			}

			tmpFileDeleter.AddFile(tmpPassPath);
		}
	}
	if(bFileCorrupted)
	{
		progress->OnTaskError(CTempStringLoader(IDS_INPUTFILECORRUPTED));
		return false;
	}
		
	if(!MergeStocks(stockPool,stockMapping,importSettings.bImportInsertMissingStocks,progress))
	{		
		SQLDB.Rollback();
		return false;
	}

	if(!importSettings.bImportSaveOriginalHent)
	{
		Hent* pCompanyHent = hentPool.GetHentByHentNo(pCompany->GetFarmNo()); 
		if(pCompanyHent)
		{
			delete pCompany;
			pCompanyDeleter.release();
			pCompany = pCompanyHent;
		}
		else
		{
			hentPool.Add(pCompany);
			pCompanyDeleter.release();
		}
	}
	

	if(!MergeHents(hentPool,hentMapping,importSettings.bImportInsertMissingHents,progress))
	{
		SQLDB.Rollback();
		return false;
	}

	if(!MergeCows(cowPool,stockMapping,hentMapping,progress))
	{
		SQLDB.Rollback();
		return false;
	}

	if(!MergeDocs(docPool,hentMapping,pCompany,importSettings.bImportSaveDocDates,progress))
	{		
		SQLDB.Rollback();
		return false;
	}

	if(!MergeInvoices(rrinvPool,stockMapping,progress))
	{
		SQLDB.Rollback();
		return false;
	}

	bool bSaveOrginalDocDates = importSettings.bImportSaveDocDates;

	if(AfxMessageBox(IDS_ASKINSERTINTODB,MB_YESNO) == IDYES)
	{
		CDocDataArray::ArrayIterator docIt = docPool.iterator();
		progress->SetTask(INSERTING_DOCS,docPool.GetSize());
		
		while(docIt.hasNext())
		{
			pDoc = *(docIt++);

			pDoc->ResetId();

			if(!bSaveOrginalDocDates)
			{
				pDoc->SetLoadDate(CDateTime::now());
				pDoc->SetDocDate(CDateTime::now());
			}
			
			if(!pDoc->InsertThisDoc(false))
			{
				progress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGDOC) + '\n' + SQLDB.GetLastError());
				SQLDB.Rollback();
				return false;
			}

			CCowArray::ArrayIterator cowIt = pDoc->GetCows()->iterator();
			while(cowIt.hasNext())
			{
				if(!(*(cowIt++))->InsertThisCow(false))
				{
					progress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGCOW) + '\n' + SQLDB.GetLastError());
					SQLDB.Rollback();
					return false;
				}

			}	

			if(!progress->OnStep(docIt.index()))
			{
				SQLDB.Rollback();
				return false;
			}
			
		}
		progress->SetTask(INSERTING_RRINVS,rrinvPool.GetSize());
		CInvoiceDataArray::ArrayIterator rrinvIt = rrinvPool.iterator();
		while(rrinvIt.hasNext())
		{			
			RRInvoice *rrinv = (RRInvoice*)(*(rrinvIt++));
			rrinv->ResetId();
			if(!rrinv->InsertThisInvoice(false))
			{
				progress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGINVOICE) + '\n' + SQLDB.GetLastError());
				SQLDB.Rollback();
				return false;
			}

			CCowInvoiceEntryArray::ArrayIterator entryIt = rrinv->invEntries.iterator();
			while(entryIt.hasNext())
			{
				if(!(*(entryIt++))->InsertThis(false))
				{					
					progress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGINVOICE) + '\n' + SQLDB.GetLastError());
					SQLDB.Rollback();
					return false;
				}
			}

			entryIt = rrinv->invEntries.iterator();
			while(entryIt.hasNext())
			{
				pCow = (*(entryIt++))->GetCow(); 
				pCow->SetRRInvoice(NULL);
				pCow->AssignToRRInvoice(rrinv);
				if(!pCow->UpdateThisCow(false))
				{
					progress->OnTaskError(CTempStringLoader(IDS_ERRUPDATEINGCOW) + '\n' + SQLDB.GetLastError());				
					SQLDB.Rollback();
					return false;
				}
				pCow->CommitChanges();
			}		
			if(!progress->OnStep(rrinvIt.index()))
			{
				SQLDB.Rollback();
				return false;
			}

		}
		

		CStockDataArray::ArrayIterator stockIt = stockPool.iterator();
		while(stockIt.hasNext())
		{
			pStock = *stockIt;
			if(!stockMapping.PLookup(pStock)) //if stock ptr dont have a mapping it means it was added
			{
				ary_stocks.Add(pStock);
				stockIt.remove();
			}
			else
				stockIt++;
		}

		CHentArray::ArrayIterator hentIt = hentPool.iterator();
		while(hentIt.hasNext())
		{
			pHent = *hentIt;
			if(!hentMapping.PLookup(pHent))
			{
				NewHentDispatcher::Dispatch(pHent);					
				hentIt.remove();
			}
			else
			{
				Hent *pHent1 = NULL,*pHent2 = pHent;
				hentMapping.Lookup(pHent2,pHent1);				
				
				CHentDiffMergeDataDlg hentDiffDlg(pHent1,pHent2);
				if(hentDiffDlg.AreDiffrent())
				{
					CString askMsg;
					askMsg.Format(IDS_HENTDIFFASK,(LPCSTR)pHent1->GetFarmNo(),(LPCSTR)pHent1->GetName());
					if(AfxMessageBox(askMsg,MB_YESNO) == IDYES)
					{	
						if(hentDiffDlg.DoModal() == IDOK)
						{
							if(!pHent1->UpdateThisHent(false))
							{
								pHent1->RollbackChanges();
								progress->OnTaskError(CTempStringLoader(IDS_ERRUPDATINGHENT) + '\n' + SQLDB.GetLastError());
								SQLDB.Rollback();
								return false;
							}
							else
							{
								pHent1->CommitChanges();
							}
						}
					}
				}

				hentIt++;
			}
		}

		autoDocPool.Release();
		autoCowPool.Release();
		autoRRInvPool.Release();

		
		docIt = docPool.iterator();
		while(docIt.hasNext())
			NewInDocDispatcher::Dispatch((InDoc*)*(docIt++));

		rrinvIt = rrinvPool.iterator();
		while(rrinvIt.hasNext())
			InvoiceDispatcher::Dispatch(*(rrinvIt++));
	

		if(importSettings.bImportPassImages)
		{
			CString passImgPath;
			progress->SetTask(INSERTING_PASSIMGS,cowDataToPassPath.GetSize());
			pos = cowDataToPassPath.GetStartPosition();
			i = 0;
			while (pos)
			{
				cowDataToPassPath.GetNextAssoc(pos,pCow,passImgPath);
				if(!CPassImgBase::GetInstance()->StorePassImage(pCow,(LPCSTR)passImgPath,false))
				{
					if(AfxMessageBox(CStringFormat(CTempStringLoader(IDS_FAILDTOSTOREPASSIMGASK),(LPCSTR)pCow->GetEAN()),MB_YESNO) != IDYES)						
					{
						progress->OnTaskError(CTempStringLoader(IDS_IMPORTINPERUPTEDBYUSER));
						SQLDB.Rollback();
						return false;
					}
					
				}
				
				pCow->CommitChanges();
				progress->OnStep(i++);
			}
		}
		SQLDB.Commit();
	}			
	else
	{
		SQLDB.Rollback();		
	}

	return true;

}

bool CImportCtrl::MergeCows(CCowArray &cowPool,
							StockToStockMap &stockMapping,HentToHentMap &hentMapping,
							ITaskProgres *taskProgress)
{
	Cow *pCow;
	Stock *mappedStock;
	Hent *mappedHent;
	taskProgress->SetTask(MERGING_COWS,cowPool.GetSize());

	CCowArray::ArrayIterator cowIt = cowPool.iterator();
	while(cowIt.hasNext())
	{
		pCow = *(cowIt++);
		if(stockMapping.Lookup(pCow->GetStock(),mappedStock))
			pCow->SetStock(mappedStock);

		if(pCow->HasFirstOwner())
		{
			if(hentMapping.Lookup(pCow->GetFirstOwner(),mappedHent))
				pCow->SetFirstOwner(mappedHent);
		}	
		
		pCow->ResetId(); //reset id, so it wont be rejected when inserted to DB

		if(!taskProgress->OnStep(cowIt.index())) return false;	
	}
	return true;
}

bool CImportCtrl::MergeInvoices(CInvoiceDataArray& invoices,StockToStockMap &stockMapping,ITaskProgres *taskProgress)
{
	Stock *mappedStock;
	taskProgress->SetTask(MERGING_INVOICES,invoices.GetSize());

	CInvoiceDataArray::ArrayIterator invIt = invoices.iterator();
	while(invIt.hasNext())
	{
		Invoice* pInvoice = *(invIt++);
		CCowInvoiceEntryArray::ArrayIterator entryIt = pInvoice->invEntries.iterator();
		while(entryIt.hasNext())
		{
			CowInvoiceEntry* pEntry = *(entryIt++);
			if(stockMapping.Lookup(pEntry->GetStock(),mappedStock))
				pEntry->SetStock(mappedStock);
		
		}
		if(!taskProgress->OnStep(invIt.index())) return false;	
	}
	return true;
}

bool CImportCtrl::MergeDocs(CDocDataArray &docPool,
							HentToHentMap &hentMapping, 
							Hent *pCompany,
							bool bSaveOriginalHent,
							ITaskProgres *taskProgress)
{

	Doc *pDoc;
	Hent *mappedHent;
	
	taskProgress->SetTask(MERGING_DOCS,docPool.GetSize());
	CDocDataArray::ArrayIterator docIt = docPool.iterator();
	while(docIt.hasNext())
	{
		pDoc = *(docIt++);

		if(bSaveOriginalHent)
		{
			if(hentMapping.Lookup(pDoc->GetHent(),mappedHent))
					pDoc->SetHent(mappedHent);
		}
		else
		{
			if(hentMapping.Lookup(pCompany,mappedHent))
					pDoc->SetHent(mappedHent);
			else
				pDoc->SetHent(pCompany);
		}		
		if(!taskProgress->OnStep(docIt.index())) return false;				
	}

	return true;

}

bool CImportCtrl::FindCowDuplicates(CowArrayPtr cows)
{
	Cow *pCow;		
	CCowArray::ArrayIterator it = cows->iterator();
	while(it.hasNext())
	{
		pCow = *(it++);
		if(ary_incows.GetCowByEAN(pCow->GetEAN()))
			return false;	
	}	
	return true;
}

bool CImportCtrl::MergeStocks(CStockDataArray &stockPool,
							  StockToStockMap &stockMapping,
							  bool bInsertMissingStocks,
							  ITaskProgres *taskProgress )
{
	int i,s,t,j;

	Stock *pStock,*existingStock;

	s = ary_stocks.GetSize();
	t = stockPool.GetSize();
	
	taskProgress->SetTask(MERGING_STOCKS,s+t /*loops 1 and 2*/);

	for/*loop 1*/(i = 0;i < s;i++)
	{
		existingStock = ary_stocks[i];
		for(j = 0;j < t;j++)
		{
			pStock = stockPool[j];
			if(existingStock->GetStockCode() == pStock->GetStockCode())
			{				
				stockMapping.SetAt(pStock,existingStock);					
				break;
			}
		}
		if(!taskProgress->OnStep(i)) return false;
	}

	CStockMergeDlg stockMerge(m_pParentWnd);
	CString mergeText;

	for/*loop 2*/(i = 0,s = stockPool.GetSize();i < s;i++)
	{
		pStock = stockPool[i];
		if(!stockMapping.Lookup(pStock,existingStock))
		{
			if(bInsertMissingStocks)
			{
				pStock->ResetId();;
				if(!pStock->InsertThisStock(false))
				{
					taskProgress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGSTOCK) + '\n' + SQLDB.GetLastError());
					return false;
				}		
				
			}
			else
			{			
			
				mergeText.Format(IDS_ASKSTOCKMERGETEXT,(LPCSTR)pStock->GetStockCode());
				stockMerge.SetMergeText(mergeText);	
				if(stockMerge.DoModal() == IDOK)
				{
					switch(stockMerge.GetMergeAction())
					{
					case CMergeDlg::MERGE_ACTION_INSERT:
						pStock->ResetId();
						if(!pStock->InsertThisStock(false))
						{
							taskProgress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGSTOCK) + '\n' + SQLDB.GetLastError());
							return false;
						}					
						break;
					case CMergeDlg::MERGE_ACTION_REPLACE:
						stockMapping.SetAt(pStock,stockMerge.GetSelectedStock());
						break;
					case CMergeDlg::MERGE_ACTION_INSERT_4ALL:
						for(j = i;j < s;j++)
						{
							pStock = stockPool[j];
							pStock->ResetId();
							if(!pStock->InsertThisStock(false))
							{
								taskProgress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGSTOCK) + '\n' + SQLDB.GetLastError());
								return false;
							}						
						}
						i = s;
						break;
					case CMergeDlg::MERGE_ACTION_REPLACE_4ALL:
						for(j = i;j < s;j++)
						{
							pStock = stockPool[j];
							stockMapping.SetAt(pStock,stockMerge.GetSelectedStock());
						}
						i = s;
						break;
					}
				}
				else
				{
					taskProgress->OnTaskError(CTempStringLoader(IDS_IMPORTINPERUPTEDBYUSER));
					return false;
				}
			}
		}
		if(!taskProgress->OnStep(i))
		{
			return false;
		}
	}
	return true;
}

bool CImportCtrl::MergeHents(CHentArray &hentPool,
							 HentToHentMap &hentMapping,
							 bool bInsertMissingHents,
							 ITaskProgres *taskProgress)
{
	int i,j,s,t;
	Hent *existingHent,*pHent;

	s = ary_hents.GetSize();
	t = hentPool.GetSize();

	taskProgress->SetTask(MERGING_HENTS,s+t/*loop 1 and 2*/);
	for/*1*/(i = 0;i < s;i++)
	{
		existingHent = ary_hents[i];
		for(j = 0;j < t;j++)
		{
			pHent = hentPool[j];			
			if(existingHent->GetFarmNo() ==  pHent->GetFarmNo())
			{
				hentMapping.SetAt(pHent,existingHent);
				break;
			}			
		}
		if(!taskProgress->OnStep(i))
		{
			return false;
		}
	}

	CString mergeText;
	CHentMergeDlg hentMerge(m_pParentWnd);
	
	for/*2*/(i = 0;i < t;i++)
	{
		pHent = hentPool[i];
		if(!hentMapping.Lookup(pHent,existingHent))
		{
			if(bInsertMissingHents)
			{
				if(ValidateDuplicateAliasOrWetNo(pHent))
				{
					pHent->ResetId();
					if(!pHent->InsertThisHent(false))
					{
						taskProgress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGHENT) + '\n' + SQLDB.GetLastError());
						return false;
					}
				}
				else
				{
					taskProgress->OnTaskError(CTempStringLoader(IDS_IMPORTINPERUPTEDBYUSER));
					return false;
				}
			}
			else
			{
				mergeText.Format(IDS_ASKHENTMERGE,(LPCSTR)pHent->GetName());
				hentMerge.SetMergeText(mergeText);	

				if(hentMerge.DoModal() == IDOK)
				{
					switch(hentMerge.GetMergeAction())
					{
					case CMergeDlg::MERGE_ACTION_INSERT:

						if(ValidateDuplicateAliasOrWetNo(pHent))
						{
							pHent->ResetId();
							if(!pHent->InsertThisHent(false))
							{
								taskProgress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGHENT) + '\n' + SQLDB.GetLastError());
								return false;
							}
						}
						else
						{
							taskProgress->OnTaskError(CTempStringLoader(IDS_IMPORTINPERUPTEDBYUSER));
							return false;
						}
						break;
					case CMergeDlg::MERGE_ACTION_REPLACE:
						hentMapping.SetAt(pHent,hentMerge.GetSelectedHent());
						break;
					case CMergeDlg::MERGE_ACTION_INSERT_4ALL:
						for(j = i;j < t;j++)
						{
							pHent = hentPool[j];
							if(ValidateDuplicateAliasOrWetNo(pHent))
							{
								pHent->ResetId();
								if(!pHent->InsertThisHent(false))
								{
									taskProgress->OnTaskError(CTempStringLoader(IDS_ERRINSERTINGHENT) + '\n' + SQLDB.GetLastError());
									return false;
								}
							}
							else
							{
								taskProgress->OnTaskError(CTempStringLoader(IDS_IMPORTINPERUPTEDBYUSER));
								return false;
							}
						}
						i = t; //finish the loop
						break;
					case CMergeDlg::MERGE_ACTION_REPLACE_4ALL:
						for(j = i;j < t;j++)
						{
							pHent = hentPool[j];
							hentMapping.SetAt(pHent,hentMerge.GetSelectedHent());
						}
						i = t;
						break;
					}
				}
				else
				{
					taskProgress->OnTaskError(CTempStringLoader(IDS_IMPORTINPERUPTEDBYUSER));
					return false;
				}
			}
		}

		if(!taskProgress->OnStep(i))
		{
			return false;
		}
	}

	return true;

}

bool CImportCtrl::ValidateDuplicateAliasOrWetNo(Hent* pHent)
{
	Hent *existingHent,*pMergeHent;
	bool bRepeatEntry;
	CString issueText;
	CIssueValueDlg issueDlg(m_pParentWnd);
	int k,l;

	
	issueDlg.SetCaption(IDS_HENTMERGECONFLICT);
	for(int i = 0,s = ary_hents.GetSize();i < s;i++)
	{
		existingHent = ary_hents[i];
	
		if(existingHent->GetAlias() == pHent->GetAlias())
		{
			issueDlg.SetValue(pHent->GetAlias());
			issueDlg.SetIssueValueName(IDS_NEWALIAS);					
			issueDlg.SetIssueText(issueText);

			
			while(TRUE)
			{
				bRepeatEntry = false;
				pMergeHent = existingHent; 
				issueText.Format(IDS_HENTALIASMERGEISSUE,(LPCSTR)pHent->GetName(),(LPCSTR)pMergeHent->GetName());
				issueDlg.SetIssueText(issueText);
				if(issueDlg.DoModal() == IDOK)
				{
					pHent->SetAlias(issueDlg.GetValue());
					for(k = 0,l = ary_hents.GetSize();k < l;k++)
					{
						pMergeHent = ary_hents[k];
						if(pHent->GetAlias() == pMergeHent->GetAlias())
						{
							bRepeatEntry = true;
							break;
						}																	
					}
					if(!bRepeatEntry) break;
				}
				else
				{
					return false;
				}
			}
				
		}

		if(pHent->HasWetNo() && existingHent->GetWetNo() == pHent->GetWetNo())
		{		
			issueDlg.SetValue(pHent->GetWetNo());
			issueDlg.SetIssueValueName(IDS_WETONO);
			issueDlg.SetIssueText(IDS_WETNOMERGEISSUE);
			issueDlg.SetRegEx("[0-9]{8}");


			while(TRUE)
			{
				bRepeatEntry = false;
				pMergeHent = existingHent; 
				issueText.Format(IDS_WETNOMERGEISSUE,(LPCSTR)pHent->GetAlias(),(LPCSTR)pMergeHent->GetAlias());
				issueDlg.SetIssueText(issueText);
				if(issueDlg.DoModal() == IDOK)
				{
					CString wetno = issueDlg.GetValue();	
					if(wetno.IsEmpty()) break;
					pHent->SetWetNo(wetno);
					for(k = 0,l = ary_hents.GetSize();k < l;k++)
					{
						pMergeHent = ary_hents[k];
						if(pHent->GetWetNo() == pMergeHent->GetWetNo())
						{
							bRepeatEntry = true;
							break;
						}																	
					}
					if(!bRepeatEntry) break;
				}
				else
				{
					return false;
				}
			}			
		}
	}
	return true;	
}
