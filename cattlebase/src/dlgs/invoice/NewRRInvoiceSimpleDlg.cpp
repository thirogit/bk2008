#if 0
#include "stdafx.h"
#include "NewRRInvoiceDlg.h"

#include "../datatypes/RRInvoiceData.h"
#include "../classes/match/CowMatcher.h"
#include "../classes/forallactions/SelectorAction.h"
#include "additem/AddItem.h"
#include "../global_arrays.h"
#include "../ctrls/CowListCtrl.h"
#include "../dispacher/InvoiceDispatcher.h"
#include "CattleSQL.h"
#include "MainCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNewRRInvoiceDlg::CNewRRInvoiceDlg(CCowArray &cowArray,InvoiceContext* pInvoiceContext,CWnd* pParent/* = NULL*/) : CBaseNewInvoiceDlg(pInvoiceContext,pParent)
{
	m_baseCows.Append(cowArray);
	ConstructorSetup(cowArray[0]->GetFirstOwner());
	m_RRInDoc = RRInvoiceData::GetInDocAllCowsBelongTo(cowArray);
}

BOOL CNewRRInvoiceDlg::OnInitDialog()
{
	CBaseNewInvoiceDlg::OnInitDialog();

		
	for(int i = 0;i < m_baseCows.GetSize();i++)
		m_cowprices.Add(new CowPriceData(m_baseCows[i],m_pInvoiceContext));
		
	
	UpdateItemCount();
	ComputeTotal();

	m_btnadditem.EnableWindow(TRUE);
	m_btndelitem.EnableWindow(TRUE);

	m_showDocs.Add(m_RRInDoc);
	m_docList.UpdateItemCount();

	return TRUE;
}

CString CNewRRInvoiceDlg::GetInvDlgCaption()
{
	CString caption;
	caption.Format(IDS_NEWRRINVOICECAPTION,(LPCSTR)m_invHent->GetName());
	return caption;
}

void CNewRRInvoiceDlg::AddItem()
{

	CCowDataMatcher cowMatcher;

	cowMatcher.FindFirstOwner(m_invHent);
	cowMatcher.FindWithVATBuyInvoice(false);
	cowMatcher.FindWithRRInvoice(false);
	
	CCowArray cows2Add;

	CSelectorAction<CCowArray,Cow,CCowDataMatcher> selectCows(cows2Add,&cowMatcher);

	m_RRInDoc->GetCows()->ForAllAction(selectCows);	
	
	cows2Add.RemoveArray(m_baseCows);

	CAddItemDlg<CCowArray,CInCowListCtrl> addCowDlg(cows2Add,this);
	if(addCowDlg.DoModal() == IDOK)
	{
		int i = addCowDlg.GetSelectedDocIndex();
		if(i >= 0)
		{
			Cow *pCow = cows2Add[i];
			m_baseCows.Add(pCow);
			m_cowprices.Add(new CowPriceData(pCow,m_pInvoiceContext));
		}		
		UpdateItemCount();
		
		
	}
}
void CNewRRInvoiceDlg::DelItem()
{
	CowPriceData *pCowPrice = NULL;
	Cow *pCow2Del = NULL;
    int iSel = GetSelectedPriceItem();
	if(iSel < 0)
	{
		AfxMessageBox(IDS_NOCOWSELECTED,MB_OK,0);
		return;
	}
	if(m_cowprices.GetSize() == 1)
	{
		AfxMessageBox(IDS_CANTDELALLCOWS,MB_OK,0);
		return;
	}
	
	pCowPrice = m_cowprices[iSel];
	pCow2Del = pCowPrice->GetCow();

	
    m_baseCows.RemovePtr(pCow2Del);

	m_cowprices.RemoveAt(iSel);
	delete pCowPrice;
	
	UpdateItemCount();
	ComputeTotal();

}

bool CNewRRInvoiceDlg::CommitInvoice()
{
	Money invTotal(0.0);
	
	RRInvoiceData *pInvoice = NULL;
	Cow *cowd = NULL;
	
	auto_ptr<RRInvoiceData> pInvoiceDelete;

	invTotal = ComputeInvoiceTotal();
 
	NullInvoiceData *pNullInvoice = CreateNewInvoice();
	pInvoice = new RRInvoiceData(pNullInvoice);
	delete pNullInvoice;

	pInvoiceDelete.reset(pInvoice);
	pInvoice->invoicevalue = invTotal;	
					
	if(pInvoice->InsertThisInvoice(false))
	{			
		CCowInvoiceEntryArray cowInvEntries;
		CCowInvoiceEntryArray::AutoArray autoCowInvEntries(cowInvEntries);

		CCowPriceDataArray::ArrayIterator it = m_cowprices.iterator();
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
bool CNewRRInvoiceDlg::UpdateCowsWithInvoice(CCowInvoiceEntryArray &cowInvEntries)
{
	CCowInvoiceEntryArray::ArrayIterator it = cowInvEntries.iterator();
	while(it.hasNext())
	{
		CowInvoiceEntry *pEntry = *(it++);
		Cow *pCow = pEntry->GetCow();
		pCow->AssignToRRInvoice((RRInvoiceData*)pEntry->GetInvoice());

		if(!pCow->UpdateThisCow(false))		
		{		
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGRRINVFORCOW);						
			return false;
		}		
	}
	return true;
}
InvoiceTypes CNewRRInvoiceDlg::GetInvoiceType()
{
	return IN_INV_RR;
}
void CNewRRInvoiceDlg::TabChanged(int newTabIndex)
{
	BOOL bEnableTab = (newTabIndex == 0);	
	m_btnadditem.EnableWindow(bEnableTab);
	m_btndelitem.EnableWindow(bEnableTab);
}

double CNewRRInvoiceDlg::GetPreselectedVATRate()
{
	return AppCfg().GetFloat(CFG_PRESELVATRATERRINV);
}
InvoiceData::PAYWAYS CNewRRInvoiceDlg::GetPreselectedPayWay()
{
	return (InvoiceData::PAYWAYS)AppCfg().GetNum(CFG_PRESELPAYWAYRRINV);
}
int CNewRRInvoiceDlg::GetPreSelectedPayDueDays()
{
	return AppCfg().GetNum(CFG_PRESELPAYDUEDAYSRR);
}

#endif