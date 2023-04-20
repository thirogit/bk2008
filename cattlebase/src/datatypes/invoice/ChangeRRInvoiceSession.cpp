#include "stdafx.h"
#include "ChangeRRInvoiceSession.h"

#include "CattleSQL.h"
#include "../../exceptions/StrException.h"
#include "../../classes/match/CowMatcher.h"
#include "../../classes/forallactions/SelectorAction.h"
#include "../../global_arrays.h"
#include "../../dlgs/additem/AddItem.h"
#include "../../ctrls/CowListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CChangeRRInvoiceSession::CChangeRRInvoiceSession(RRInvoice *pInvoice,InvoiceContext* pInvoiceContext) : CowPriceInvoiceSession(pInvoiceContext)
{
	if(!pInvoice->CanEditThisInvoice())
	{
		throw new CStrException("Cant edit this RR invoice, it is not valid for edit.");
	}
	m_Inv2Edit = pInvoice;
	
	CCowArray cowsInInvoice;
	pInvoice->GetInvoiceCows(cowsInInvoice);
	m_RRInDoc = RRInvoice::GetInDocAllCowsBelongTo(cowsInInvoice);

	GetInvoiceDocs().Add(m_RRInDoc);

	SetVATRate(m_Inv2Edit->GetVATRate());
	SetPayDueDays(m_Inv2Edit->GetPayDueDays());	
	SetExtras(m_Inv2Edit->GetExtras());
	SetCustomNumber(m_Inv2Edit->GetCustomNumber());
	SetInvoiceDate(m_Inv2Edit->GetInvoiceDate());
	SetPayway(m_Inv2Edit->GetPayWay());
	SetInvoiceHent(m_Inv2Edit->GetInvoiceHent());
	SetTransactionDate(m_Inv2Edit->GetTransactionDate());
	SetTransactionPlace(m_Inv2Edit->GetTransactionPlace());

	CCowInvoiceEntryArray::ArrayIterator it = m_Inv2Edit->invEntries.iterator();

	while(it.hasNext())
	{	
		CowInvoiceEntry *pEntry = *(it++);
		CowPriceData *cowPrice = AddCow(pEntry->GetCow());
		cowPrice->SetCustomPriceNetto(pEntry->GetPrice());
		cowPrice->SetCustomStock(pEntry->GetStock());
		cowPrice->SetCustomWeight(pEntry->GetWeight());		
	}
}

CString CChangeRRInvoiceSession::GetSessionTitle() const
{
	CString caption;	
	caption.Format(IDS_EDITINVCAPTION,m_Inv2Edit->GetInvoiceNo(),(LPCSTR)GetInvoiceHent()->GetName());	
	return caption;
}

bool CChangeRRInvoiceSession::CommitInvoice()
{

	if(!CAN_DO(CANEDIT,m_Inv2Edit))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return false;		
	}

	RRInvoice* pInvoice = new RRInvoice(m_Inv2Edit);		
	UpdateInvoiceValues(pInvoice);
	
	auto_ptr<RRInvoice> pInvoiceDelete(pInvoice);
	pInvoice->ChangeInvoiceNetValue(GetTotalNetValue());
	
	if(pInvoice->UpdateThisInvoice(false))
	{	
		if(!DeleteInvoiceEntries(m_Inv2Edit->invEntries))
		{
			SQLDB.Rollback();		
			return false;
		}
		if(!DeleteRRInvoiceForCows(m_cowsDeleted))
		{
			m_cowsDeleted.RollbackChanges();
			SQLDB.Rollback();		
			return false;
		}
		if(!UpdateCowsWithRRInvoice(m_cowsAdded,m_Inv2Edit))
		{
			m_cowsDeleted.RollbackChanges();
			m_cowsAdded.RollbackChanges();
			SQLDB.Rollback();		
			return false;
		}

		CCowInvoiceEntryArray cowInvEntries;
		CCowInvoiceEntryArray::AutoArray autoCowInvEntries(cowInvEntries);

		CCowPriceDataArray::ArrayIterator it = GetCowPriceIterator();
		while(it.hasNext())
		{
			cowInvEntries.Add(CreateNewInvoiceEntry(m_Inv2Edit,*(it++)));
		}

		if(!InsertCowInvoiceEntries(cowInvEntries))
		{
			m_cowsDeleted.RollbackChanges();
			m_cowsAdded.RollbackChanges();
			SQLDB.Rollback();		
			return false;
		}

		m_Inv2Edit->CopyFrom(*pInvoice);
		m_Inv2Edit->invEntries.FreeAndFlush();
		m_Inv2Edit->invEntries.Append(cowInvEntries);		
		autoCowInvEntries.Release();
		m_cowsDeleted.CommitChanges();
		m_cowsAdded.CommitChanges();

		SQLDB.Commit();		
	}
	else
	{
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGINVOICE);			
			SQLDB.Rollback();
			return false;
	}

	return true;
}


bool CChangeRRInvoiceSession::DeleteRRInvoiceForCows(CCowArray& cows)
{
	CCowArray::ArrayIterator it = cows.iterator();
	while(it.hasNext())
	{
		Cow* pCow = *(it++);
		pCow->DeleteRRInvoice();
		if(!pCow->UpdateThisCow(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRADDCOWSTORRINV);
			return false;
		}
	}
	return true;
}

bool CChangeRRInvoiceSession::UpdateCowsWithRRInvoice(CCowArray& cows,RRInvoice* pInvoice)
{
	CCowArray::ArrayIterator it = cows.iterator();
	while(it.hasNext())
	{
		Cow* pCow = *(it++);
		pCow->AssignToRRInvoice(pInvoice);
		if(!pCow->UpdateThisCow(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRDELETINGCOWFROMRRINV);
			return false;
		}
	}
	return true;
}

void CChangeRRInvoiceSession::AddItem()
{
	if(m_Inv2Edit->GetThisInvoiceType() == CORRCT_INV)
	{
		AfxMessageBox(IDS_CANTADDDELCOWSWHENEDITINGCORRECT);
		return;
	}

	CCowDataMatcher cowMatcher;

	cowMatcher.FindFirstOwner(GetInvoiceHent());
	cowMatcher.FindWithVATBuyInvoice(false);
	cowMatcher.FindWithRRInvoice(false);
		
	CCowArray cows2Add;

	CSelectorAction<CCowArray,Cow,CCowDataMatcher> selectCows(cows2Add,&cowMatcher);

	m_RRInDoc->GetCows()->ForAllAction(selectCows);	
	
	cows2Add.RemoveArray(m_cowsAdded);	
	cows2Add.Append(m_cowsDeleted);

	CAddItemDlg<CCowArray,CInCowListCtrl> addCowDlg(cows2Add);
	if(addCowDlg.DoModal() == IDOK)
	{
		int i = addCowDlg.GetSelectedItemIndex();
		if(i >= 0)
		{
			Cow *pCow = cows2Add[i];
			m_cowsAdded.Add(pCow);
			m_cowsDeleted.RemovePtr(pCow);
			AddCow(pCow);
		}			
	}
}
void CChangeRRInvoiceSession::DelItem()
{
	if(m_Inv2Edit->GetThisInvoiceType() == CORRCT_INV)
	{
		AfxMessageBox(IDS_CANTADDDELCOWSWHENEDITINGCORRECT);
		return;
	}

	CowPriceData *pCowPrice = NULL;
	Cow *pCow2Del = NULL;
	


	int iSel = GetSelectedDocItem();
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

	m_cowsDeleted.Add(pCow2Del);
	m_cowsAdded.RemovePtr(pCow2Del);
  	DeleteCowPrice(iSel);
	
}