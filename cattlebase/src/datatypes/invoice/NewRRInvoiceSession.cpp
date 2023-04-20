#include "stdafx.h"
#include "NewRRInvoiceSession.h"

#include "../../datatypes/RRInvoice.h"
#include "../../classes/match/CowMatcher.h"
#include "../../classes/forallactions/SelectorAction.h"
#include "../../dlgs/additem/AddItem.h"
#include "../../global_arrays.h"
#include "../../ctrls/CowListCtrl.h"
#include "../../dispacher/InvoiceDispatcher.h"
#include "../../CattleSQL.h"
#include "MainCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


NewRRInvoiceSession::NewRRInvoiceSession(CCowArray &cowArray,InvoiceContext* pInvoiceContext) :  CowPriceInvoiceSession(pInvoiceContext)
{
	
	SetVATRate(VATRate(AppCfg().GetFloat(CFG_PRESELVATRATERRINV)));
	SetPayDueDays((unsigned short)AppCfg().GetNum(CFG_PRESELPAYDUEDAYSRR));	
	
	SetInvoiceDate(CDateTime::now());
	SetPayway((Invoice::PAYWAYS)AppCfg().GetNum(CFG_PRESELPAYWAYRRINV));
	SetInvoiceHent(cowArray[0]->GetFirstOwner());
	

	m_RRInDoc = RRInvoice::GetInDocAllCowsBelongTo(cowArray);
	for(int i = 0;i < cowArray.GetSize();i++)
	{
		AddCow(cowArray[i]);
	}

	GetInvoiceDocs().Add(m_RRInDoc);
}

CString NewRRInvoiceSession::GetSessionTitle() const
{
	CString caption;
	caption.Format(IDS_NEWRRINVOICECAPTION,(LPCSTR)GetInvoiceHent()->GetName());
	return caption;
}

void NewRRInvoiceSession::AddItem()
{

	CCowDataMatcher cowMatcher;

	cowMatcher.FindFirstOwner(GetInvoiceHent());
	cowMatcher.FindWithVATBuyInvoice(false);
	cowMatcher.FindWithRRInvoice(false);
	
	CCowArray cows2Add,invoiceCows;

	CSelectorAction<CCowArray,Cow,CCowDataMatcher> selectCows(cows2Add,&cowMatcher);

	m_RRInDoc->GetCows()->ForAllAction(selectCows);	
	GetSessionCows(invoiceCows);

	cows2Add.RemoveArray(invoiceCows);

	CAddItemDlg<CCowArray,CInCowListCtrl> addCowDlg(cows2Add);
	if(addCowDlg.DoModal() == IDOK)
	{
		int i = addCowDlg.GetSelectedItemIndex();
		if(i >= 0)
		{
			Cow *pCow = cows2Add[i];		
			AddCow(pCow);
		}			
	}
}
void NewRRInvoiceSession::DelItem()
{
	CowPriceData *pCowPrice = NULL;
	Cow *pCow2Del = NULL;
    int iSel = GetSelectedCowPriceItem();
	if(iSel < 0)
	{
		AfxMessageBox(IDS_NOCOWSELECTED,MB_OK,0);
		return;
	}

	if(GetCowCount() == 1)
	{
		AfxMessageBox(IDS_CANTDELALLCOWS,MB_OK,0);
		return;
	}
	
	pCowPrice = GetCowPrice(iSel);
	pCow2Del = pCowPrice->GetCow();	
  	DeleteCowPrice(iSel);
}

bool NewRRInvoiceSession::CommitInvoice()
{
	RRInvoice *pInvoice = NULL;
	Cow *cowd = NULL;
	auto_ptr<RRInvoice> pInvoiceDelete;
 
	NullInvoice *pNullInvoice = CreateNullInvoice();
	pInvoice = new RRInvoice(pNullInvoice);
	delete pNullInvoice;

	pInvoiceDelete.reset(pInvoice);
	pInvoice->SetInvoiceNetValue(GetTotalNetValue());	
					
	if(pInvoice->InsertThisInvoice(false))
	{			
		CCowInvoiceEntryArray cowInvEntries;
		CCowInvoiceEntryArray::AutoArray autoCowInvEntries(cowInvEntries);

		CCowPriceDataArray::ArrayIterator it = GetCowPriceIterator();
		while(it.hasNext())
		{
			cowInvEntries.Add(CreateNewInvoiceEntry(pInvoice,*(it++)));
		}

		if(!InsertCowInvoiceEntries(cowInvEntries))
		{
			SQLDB.Rollback();		
			return false;
		}
		if(!UpdateCowsWithInvoice(cowInvEntries))
		{
			cowInvEntries.RollbackCowChanges();
			SQLDB.Rollback();		
			return false;
		}
		pInvoice->invEntries.Append(cowInvEntries);
		cowInvEntries.CommitCowChanges();
		SQLDB.Commit();			
		autoCowInvEntries.Release();
		pInvoiceDelete.release();
		InvoiceDispatcher::Dispatch(pInvoice);			
	}
	else
	{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGINVOICE);			
			SQLDB.Rollback();
			return false;
	}

	
	return true;
	
}
bool NewRRInvoiceSession::UpdateCowsWithInvoice(CCowInvoiceEntryArray &cowInvEntries)
{
	CCowInvoiceEntryArray::ArrayIterator it = cowInvEntries.iterator();
	while(it.hasNext())
	{
		CowInvoiceEntry *pEntry = *(it++);
		Cow *pCow = pEntry->GetCow();
		pCow->AssignToRRInvoice((RRInvoice*)pEntry->GetInvoice());

		if(!pCow->UpdateThisCow(false))		
		{		
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGRRINVFORCOW);						
			return false;
		}		
	}
	return true;
}

