#include "stdafx.h"
#include "ChangeVATInvoiceSession.h"
#include "CattleSQL.h"
#include "../../exceptions/StrException.h"
#include "../../global_arrays.h"
#include "../../classes/match/DocMatcher.h"
#include "../../classes/forallactions/SelectorAction.h"
#include "../../dlgs/additem/AddItem.h"
#include "../../ctrls/DocListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CChangeVATInvoiceSession::CChangeVATInvoiceSession(VATInvoice *pInvoice,InvoiceContext* pInvoiceContext) : CowPriceInvoiceSession(pInvoiceContext)
{
		if(!pInvoice->CanEditThisInvoice())
		{
			throw new CStrException("Cant edit this VAT invoice, it is not valid for edit.");
		}
		m_Inv2Edit = pInvoice;

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
	
		GetInvoiceDocs().Append(m_Inv2Edit->invoiceDocs);

}
CString CChangeVATInvoiceSession::GetSessionTitle() const
{
	CString caption;	
	caption.Format(IDS_EDITINVCAPTION,m_Inv2Edit->GetInvoiceNo(),(LPCSTR)GetInvoiceHent()->GetName());	
	return caption;
}



void CChangeVATInvoiceSession::AddItem()
{
	if(m_Inv2Edit->GetThisInvoiceType() == CORRCT_INV)
	{
		AfxMessageBox(IDS_CANTADDDELDOCSWHENEDITINGCORRECT);
		return;
	}

	int i,j,t;
	
	CDocDataArray *docArray = NULL;
	Doc *docd = NULL;
	
	CDocDataArray filterDocs;

	switch(GetInvoiceContext()->GetBuyOrSell())
	{
	case BUY:
		docArray = &ary_indocs;
	break;

	case SELL:
		docArray = &ary_outdocs;
	break;
	}

	CDocDataMatcher docMatcher;
	docMatcher.FindHent(GetInvoiceHent());
	docMatcher.FindWithInvoice(false);
	CSelectorAction<CDocDataArray,Doc,CDocDataMatcher> selectDocs(filterDocs,&docMatcher);

	docArray->ForAllAction(selectDocs);
	
	
	filterDocs.RemoveArray(GetInvoiceDocs());
	filterDocs.RemoveArray(m_docsAdded);	
	filterDocs.Append(m_docsDeleted);


	for(j=0,t = filterDocs.GetSize();j<t;j++)
	{
		docd = filterDocs[j];
		if(!docd->CanMakeVATInvoiceOnThisDoc())
		{
			filterDocs.RemoveAt(j);
			j--;
			t--;
		}
	}

	CAddItemDlg<CDocDataArray,CDocListCtrl> addDocDlg(filterDocs);
	if(addDocDlg.DoModal() == IDOK)
	{
		i = addDocDlg.GetSelectedItemIndex();
		if(i >= 0)
		{
			docd = filterDocs[i];
			m_docsAdded.Add(docd);
			m_docsDeleted.RemovePtr(docd);
			GetInvoiceDocs().Add(docd);

			CCowArray::ArrayIterator cowIt = docd->GetCows()->iterator(); 
			while(cowIt.hasNext())
			{
				AddCow(*(cowIt++));				
			}			
		}
	}
}
void CChangeVATInvoiceSession::DelItem()
{
	if(m_Inv2Edit->GetThisInvoiceType() == CORRCT_INV)
	{
		AfxMessageBox(IDS_CANTADDDELDOCSWHENEDITINGCORRECT);
		return;
	}

	CowPriceData *pCowPrice = NULL;
	Doc *pDoc = NULL;
	int iSel = GetSelectedDocItem();
	if(iSel < 0)
	{
		AfxMessageBox(IDS_ERRNODOCTODEL,MB_OK,0);
		return;
	}

	if(GetInvoiceDocs().GetSize() == 1)
	{
		AfxMessageBox(IDS_ERRCANTDELALLDOCS);
		return;
	}

	
	pDoc = GetInvoiceDocs().GetAt(iSel);
	m_docsDeleted.Add(pDoc);
	m_docsAdded.RemovePtr(pDoc);
	
  	GetInvoiceDocs().RemovePtr(pDoc);
	
	for(int i = 0,s = GetCowCount();i < s;i++)
	{
		pCowPrice = GetCowPrice(i);
		if(GetInvoiceContext()->GetDoc(pCowPrice->GetCow()) == pDoc)
		{
			DeleteCowPrice(i);			
			i--;
			s--;
		}
	}
}

bool CChangeVATInvoiceSession::CommitInvoice()
{
	if(!CAN_DO(CANEDIT,m_Inv2Edit))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return false;		
	}

	VATInvoice* pInvoice = new VATInvoice(m_Inv2Edit);
	auto_ptr<Invoice> pInvoiceDelete(pInvoice);
	
	UpdateInvoiceValues(pInvoice);
	
	pInvoice->ChangeInvoiceNetValue(GetTotalNetValue());

	CCowInvoiceEntryArray cowInvEntries;
	CCowInvoiceEntryArray::AutoArray autoCowInvEntries(cowInvEntries);

	CCowPriceDataArray::ArrayIterator it = GetCowPriceIterator();
	while(it.hasNext())
	{
		cowInvEntries.Add(CreateNewInvoiceEntry(m_Inv2Edit,*(it++)));
	}

	if(!pInvoice->UpdateThisInvoice(false))
	{
		SQLDB.SQLMessageBox(IDS_ERRUPDATINGINVOICE);
		SQLDB.Rollback();		
		return false;
	}	

	if(!DeleteInvoiceEntries(m_Inv2Edit->invEntries))
	{
		SQLDB.Rollback();		
		return false;
	}
	if(!InsertCowInvoiceEntries(cowInvEntries))
	{
		SQLDB.Rollback();		
		return false;
	}


	if(!UpdateDocsWithInvoice(m_docsDeleted,NULL))
	{
		m_docsDeleted.RollbackChanges();
		SQLDB.SQLMessageBox(IDS_ERRWHILEDELETINGDOCSFROMINV);
		SQLDB.Rollback();		
		return false;
	
	}
	if(!UpdateDocsWithInvoice(m_docsAdded,m_Inv2Edit))
	{
		m_docsAdded.RollbackChanges();
		m_docsDeleted.RollbackChanges();
		SQLDB.SQLMessageBox(IDS_ERRWHILEADDINGDOCSTOINV);
		SQLDB.Rollback();
		return false;
		
	}

	m_Inv2Edit->CopyFrom(*pInvoice);
	m_Inv2Edit->invoiceDocs.RemoveAll();
	m_Inv2Edit->invoiceDocs.Append(GetInvoiceDocs());	

	m_Inv2Edit->invEntries.FreeAndFlush();
	m_Inv2Edit->invEntries.Append(cowInvEntries);
	autoCowInvEntries.Release();
	SQLDB.Commit();

	m_docsAdded.CommitChanges();
	m_docsDeleted.CommitChanges();

	return true;
}


bool CChangeVATInvoiceSession::UpdateDocsWithInvoice(CDocDataArray &docs,VATInvoice* pUpdateInv)
{
	Doc *pDoc;
	CDocDataArray::ArrayIterator docIt = docs.iterator();
	while(docIt.hasNext())
	{
		pDoc = *(docIt++);
		pDoc->AssignToInvoice(pUpdateInv);
		if(!pDoc->UpdateThisDoc(false))
		{			
			return false;
		}			
	}
	return true;

}