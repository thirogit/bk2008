#include "stdafx.h"
#include "NewVATInvoiceSession.h"
#include "../../classes/match/DocMatcher.h"
#include "../../classes/forallactions/SelectorAction.h"
#include "../../dlgs/additem/AddItem.h"
#include "../../global_arrays.h"
#include "../../dispacher/InvoiceDispatcher.h"
#include "../../CattleSQL.h"
#include <memory>
#include "../../datatypes/VATInvoice.h"
#include "MainCfg.h"
#include "../../ctrls/DocListCtrl.h"
#include "../../dlgs/additem/AddItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


NewVATInvoiceSession::NewVATInvoiceSession(InvoiceContext* pInvoiceContext,CDocDataArray &docArray) : CowPriceInvoiceSession(pInvoiceContext)
{
	ASSERT(docArray.GetSize() > 0);	


	SetVATRate(VATRate(AppCfg().GetFloat(CFG_PRESELVATRATEVATINV)));
	SetPayDueDays((unsigned short)AppCfg().GetNum(CFG_PRESELPAYDUEDAYSVAT));		
	SetInvoiceDate(CDateTime::now());
	SetPayway((Invoice::PAYWAYS)AppCfg().GetNum(CFG_PRESELPAYWAYVATINV));
	SetInvoiceHent(docArray[0]->GetHent());	

	GetInvoiceDocs().Append(docArray);

	CCowArray::ArrayIterator cowIt = CCowArray::ArrayIterator::nullIterator();
	CDocDataArray::ArrayIterator docIt = docArray.iterator(); 
	while(docIt.hasNext())
	{
		cowIt = (*(docIt++))->GetCows()->iterator();
		while(cowIt.hasNext())
		{
			AddCow(*(cowIt++));
			
		}
	}
}

CString NewVATInvoiceSession::GetSessionTitle() const
{
	CString caption;
	caption.Format(IDS_NEWINVOICECAPTION,(LPCSTR)GetInvoiceHent()->GetName());
	return caption;
}

void NewVATInvoiceSession::AddItem()
{

	int i,j,t;
	CDocDataArray *docArray = NULL;
	Doc *docd = NULL;
	CowPriceData *cowPrice = NULL;

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
			GetInvoiceDocs().Add(docd);

			CCowArray::ArrayIterator cowIt = docd->GetCows()->iterator();
			while(cowIt.hasNext())
			{
				AddCow(*(cowIt++));				
			}			
		}
	}
}
void NewVATInvoiceSession::DelItem()
{
	
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


bool NewVATInvoiceSession::CommitInvoice()
{
	Doc *pDoc = NULL;
	VATInvoice *pInvoice = NULL;
	Cow *cowd = NULL;
	auto_ptr<Invoice> pInvoiceDelete;
	
	NullInvoice *pNullInvoice = CreateNullInvoice();	
	pInvoice = new VATInvoice(pNullInvoice);
	delete pNullInvoice;
	pInvoiceDelete.reset(pInvoice);

	pInvoice->SetInvoiceNetValue(GetTotalNetValue());
	pInvoice->invoiceDocs.Append(GetInvoiceDocs());	
				
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
	
		CDocDataArray::ArrayIterator docIt = pInvoice->invoiceDocs.iterator();
		Doc *pDoc;
		while(docIt.hasNext())
		{
			pDoc = *(docIt++);
			pDoc->AssignToInvoice(pInvoice);
			if(!pDoc->UpdateThisDoc(false))
			{
				pInvoice->invoiceDocs.RollbackChanges();
				SQLDB.Rollback();
				SQLDB.SQLMessageBox(IDS_ERRUPDATINGDOCSFORINV);
				return false;			
			}			
		}
		
		pInvoice->invoiceDocs.CommitChanges();
		pInvoice->invEntries.Append(cowInvEntries);
		SQLDB.Commit();
		pInvoiceDelete.release();
		autoCowInvEntries.Release();
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




