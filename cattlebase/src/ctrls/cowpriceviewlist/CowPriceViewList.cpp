#include "stdafx.h"
#include "CowPriceViewList.h"
#include "../../Cow/EditCowDlg.h"
#include "MainCfg.h"
#include "../../consts.h"
#include "../../CattleSQL.h"
#include "../../datatypes/CowInvoiceEntryComparator.h"

const GRIDHEADERS CCowPriceViewList::szCowPriceListViewCols[] =
{
	{IDS_HEADERDOC,COWINVENTRY_SORTBY_DOC},
	{IDS_HEADEREAN,COWINVENTRY_SORTBY_EAN},
	{IDS_HEADERSTOCK,COWINVENTRY_SORTBY_STOCK},
	{IDS_HEADERWEIGHT,COWINVENTRY_SORTBY_WEIGHT},
	{IDS_HEADERMYPRICE,COWINVENTRY_SORTBY_MYPRICE},
	{IDS_HEADERNETTOPRICE,COWINVENTRY_SORTBY_NETTOPRICE},
	{IDS_HEADERVATVALUE,COWINVENTRY_SORTBY_VATVALUE},
	{IDS_HEADERBRUTTOPRICE,COWINVENTRY_SORTBY_BRUTTOPRICE},
	{IDS_HEADERDELIVER,COWINVENTRY_SORTBY_ALIAS},
	{IDS_HEADERINVSTOCK,COWINVENTRY_SORTBY_INV_STOCK},
	{IDS_HEADERINVWEIGHT,COWINVENTRY_SORTBY_INV_WEIGHT},
	{IDS_HEADERTERMPRICE,COWINVENTRY_SORTBY_TERM_PRICE},
	{IDS_HEADERTERMSTOCK,COWINVENTRY_SORTBY_TERM_STOCK},
	{IDS_HEADERTERMWEIGHT,COWINVENTRY_SORTBY_TERM_WEIGHT},
	{0,0}
};


BEGIN_MESSAGE_MAP(CCowPriceViewList,CBaseListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK , OnDblClick)
END_MESSAGE_MAP()

CCowPriceViewList::CCowPriceViewList(Invoice *invoice) : 
	CBaseListCtrl(szCowPriceListViewCols,CFGID_INVPRICEVIEWLIST)
{
	m_cowPrices.Append(invoice->invEntries);
	m_cowAccessor = CowAccessorFactory::createAccessor(invoice->GetBuyOrSell());
}
CCowPriceViewList::~CCowPriceViewList()
{
	delete m_cowAccessor;
}

void CCowPriceViewList::UpdateItemCount()
{
	SetItemCount(m_cowPrices.GetSize());
}


void CCowPriceViewList::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	CowInvoiceEntry *pEntry = m_cowPrices[nItem];
	Cow* pCow = pEntry->GetCow();
	CString s("-");
	
	switch(GetColID(nSubItem))
	{
		case COWINVENTRY_SORTBY_EAN:
			s = pCow->GetEAN();
		break;
		case COWINVENTRY_SORTBY_STOCK:
			s = pCow->GetStock()->GetStockCode();
		break;
		case COWINVENTRY_SORTBY_WEIGHT:
			s = pCow->GetWeight().ToString();
			break;
		case COWINVENTRY_SORTBY_MYPRICE:
			s = pCow->GetMyPrice().ToString();
			break;	
		case COWINVENTRY_SORTBY_NETTOPRICE:
			s = pEntry->GetPrice().ToString();
		break;	
		case COWINVENTRY_SORTBY_VATVALUE:
			s = (pEntry->GetPrice()*pEntry->GetInvoice()->GetVATRate()).ToString();		
		break;
		case COWINVENTRY_SORTBY_BRUTTOPRICE:
			s = (pEntry->GetPrice() + (pEntry->GetPrice()*pEntry->GetInvoice()->GetVATRate())).ToString();
		break;
		case COWINVENTRY_SORTBY_DOC:
			s = m_cowAccessor->GetDoc(pCow)->GetDocNoStr();
		break;	
		case COWINVENTRY_SORTBY_ALIAS:
			s = m_cowAccessor->GetDoc(pCow)->GetHent()->GetAlias();
		break;
		case COWINVENTRY_SORTBY_INV_STOCK:
			s = pEntry->GetStock()->GetStockCode();
		break;
		case COWINVENTRY_SORTBY_INV_WEIGHT:
			s = pEntry->GetWeight().ToString();
		break;
		case COWINVENTRY_SORTBY_TERM_PRICE:
			s = m_cowAccessor->GetTermPrice(pCow).ToString();
		break;
		case COWINVENTRY_SORTBY_TERM_STOCK:
		{
			Stock* pTermStock = m_cowAccessor->GetTermStock(pCow);
			if(pTermStock)
				s = pTermStock->GetStockCode();
		}
		break;
		case COWINVENTRY_SORTBY_TERM_WEIGHT:
		{
			NullDouble termWeight = m_cowAccessor->GetTermWeight(pCow);
			if(!termWeight.IsNull())
				s = termWeight.ToString();
		}
		break;		
	}
	strcpy(pszItemText,(LPCSTR)s);

}


void CCowPriceViewList::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{

	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	if (pNMIA)
		nItem = pNMIA->iItem;
	
	if(nItem >= 0)
	{
		Cow *pCow = m_cowPrices[nItem]->GetCow();
		ASSERT(!pCow->IsUncommited());
		CEditCowDlg editCowDlg(pCow,this);
		editCowDlg.LockEANEdit();
		if(editCowDlg.DoModal() == IDOK)
		{
			if(pCow->CanEditThisCow())
			{
				if(pCow->UpdateThisCow())
				{
					pCow->CommitChanges();
				}
				else
					SQLDB.SQLMessageBox(IDS_ERRUPDATEINGCOW);
			}
			else
			{
				AfxMessageBox(IDS_ERRCANTEDITCOW);						
			}
		}
		
	}
	*pResult = 0;

}

void CCowPriceViewList::SortByCol(int Column,bool bAsc)
{
	UINT nColumnId = GetColID(Column);
	m_cowPrices.SortBy(nColumnId,bAsc,m_cowAccessor);
	Invalidate();
}
