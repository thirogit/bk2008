#include "stdafx.h"
#include "StockPricePerKgList.h"
#include "../stocklistctrl/stocklistctrl.h" // need some definitions
#include "resource.h"
#include "cattlebase.h"

#define MIN_EDIT_WIDTH 60
#define IDC_EDITCELL 1001


const GRIDHEADERS CStockPricePerKgList::szStockPricePerKgListCols[] =
{
	{IDS_HEADERSTOCK,STOCKPERKGPRICE_COL1},
	{IDS_HEADERMYPERKGPRICE,STOCKPERKGPRICE_COL2},
	{0,0}
};

BEGIN_MESSAGE_MAP(CStockPricePerKgList,CBaseListCtrl)
	ON_WM_HSCROLL()
    ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK , OnDblClick)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

CStockPricePerKgList::CStockPricePerKgList(const CowPriceInvoiceSession* pInvoiceSession) : 
	CBaseListCtrl(szStockPricePerKgListCols,CFGID_STOCKPERKGLIST),
	m_pInvoiceSession(pInvoiceSession)
{
}
void CStockPricePerKgList::UpdateItemCount()
{
	SetItemCount(m_stockSet.GetSize());
}

void CStockPricePerKgList::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	
	Stock *stock = m_stockSet[nItem];
	CString sText;

	
	switch(GetColID(nSubItem))
	{
		case STOCKPERKGPRICE_COL1:
			sText = stock->GetStockCode();
		break;
		case STOCKPERKGPRICE_COL2:
			sText = stock->GetMyPerKgPrice().ToString();
		break;
		default:
			ASSERT(0);

	}

	strcpy(pszItemText,sText);

}


void CStockPricePerKgList::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (GetFocus() != this) 
		SetFocus();
    
    CBaseListCtrl::OnHScroll (nSBCode, nPos, pScrollBar);
}

void CStockPricePerKgList::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (GetFocus() != this) 
		SetFocus();
    CBaseListCtrl::OnVScroll (nSBCode, nPos, pScrollBar);
}


CFloatEditCell*  CStockPricePerKgList::FloatEditSubItem (int Item, int Column,double initValue,int prec/* = 4*/)
{
		CRect Rect;
		if (!GetCellRect(Item,Column,Rect,true,MIN_EDIT_WIDTH)) 
    		return NULL;

		CFloatEditCell *pEdit = new CFloatEditCell (this, Item, Column,initValue,prec);
		pEdit->Create (Rect,(CWnd*)this, IDC_EDITCELL);
	
		return pEdit;

}


BOOL CStockPricePerKgList::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}
	
	
	if(nItem >= 0)
	{
		
		switch(GetColID(nSubItem))
		{
			case STOCKPERKGPRICE_COL2:
				FloatEditSubItem(nItem,nSubItem,m_stockSet[nItem]->GetMyPerKgPrice().GetDouble());
			break;
		
			
		}
	}

	*pResult = 0;
	return TRUE;
}

void CStockPricePerKgList::SortByCol(int Column,bool bAsc)
{
	m_stockSet.SortBy(GetColID(Column),bAsc);
	Invalidate();
}

void CStockPricePerKgList::OnShowWindow(BOOL bShow,UINT nStatus)
{

	if(bShow)
	{
		m_stockSet.RemoveAll();
		for(int i=0;i<m_pInvoiceSession->GetCowCount();i++)
			m_stockSet.AddDistinct(m_pInvoiceSession->GetCowPrice(i)->GetStock());
		
		UpdateItemCount();
	}
}

Stock * CStockPricePerKgList::GetStockItem(int iItem)
{
	if(iItem >= 0 && iItem < m_stockSet.GetSize())
	{
		return m_stockSet[iItem];
	}
	return NULL;
}
