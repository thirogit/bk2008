#include "stdafx.h"
#include "RejectedCowsListCtrl.h"
#include "../combo/StockComboCell.h"
#include "../consts.h"
#include "../classes/IntString.h"

BEGIN_MESSAGE_MAP(CRejectedCowsListCtrl, CBaseListCtrl)

	ON_NOTIFY_REFLECT(NM_DBLCLK , OnDblClick)
END_MESSAGE_MAP()

#define MIN_COL_WIDTH 50
#define DROPDOWNHEIGHT 100
#define IDC_STOCKCOMBOCELL 1001

const GRIDHEADERS CRejectedCowsListCtrl::RejectedCowHeader[] =
{
	{IDS_HEADERCOWNO,REJCOW_EAN},
	{IDS_HEADERSTOCK,REJCOW_STOCK},
	{IDS_HEADERWEIGHT,REJCOW_WEIGHT},
	{IDS_HEADERMYPRICE,REJCOW_MYPRICE},
	{IDS_HEADERSEX,REJCOW_SEX},
	{IDS_HEADERINDOCNO,REJCOW_DOCIN},
	{IDS_HEADEROUTDOCNO,REJCOW_DOCOUT},
	{IDS_HEADERTERMBUYSTOCK,REJCOW_TERMBUYSTOCK},
	{IDS_HEADERTERMBUYWEIGHT,REJCOW_TERMBUYWEIGHT},
	{IDS_HEADERTERMBUYPRICE,REJCOW_TERMBUYPRICE},
	{IDS_HEADERTERMSELLSTOCK,REJCOW_TERMSELLSTOCK},
	{IDS_HEADERTERMSELLWEIGHT,REJCOW_TERMSELLWEIGHT},
	{IDS_HEADERTERMSELLPRICE,REJCOW_TERMSELLPRICE},
	{0,0}
};


CRejectedCowsListCtrl::CRejectedCowsListCtrl(CSyncCowDataArray *rejCowArray,int iConfigId/* = -1*/) 
: CBaseListCtrl(RejectedCowHeader,iConfigId) ,m_rejCowArray(rejCowArray)
{	
}
/*
void CRejectedCowsListCtrl::SortByCol(int Column,bool bAsc)
{
   m_docArray->SortBy((DOCS_SORT_BY)GetColID(Column),bAsc);
}
*/

void CRejectedCowsListCtrl::UpdateItemCount()
{
	SetItemCount(m_rejCowArray->GetSize());
}
	
void CRejectedCowsListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{
	CSyncCowData *pCow = (*m_rejCowArray)[nItem];
	CString sText;
	const char emptyCell[] = "---",newInd[] = "+++";
		
	switch(GetColID(nSubItem))
	{
	case REJCOW_EAN:
		sText = pCow->ean;
		break;
	case REJCOW_STOCK:
		if(!(pCow->rejectReason & CSyncCowData::STOCK_NOT_FOUND && 
			!(pCow->rejectReason & CSyncCowData::STOCK_NOT_FOUND_REPAIRED)))
				sText = (LPCSTR)pCow->stock->GetStockCode();
		break;
	case REJCOW_WEIGHT:
		sText = pCow->weight.ToString();
		break;
	case REJCOW_MYPRICE:
		sText = pCow->myprice.ToString();
		break;
	case REJCOW_DOCIN:
		if(!(pCow->rejectReason & CSyncCowData::DOCIN_NOT_FOUND))
		{
			if(!pCow->sync_docin)
			{
				if(pCow->docin)
				{					
					sText = (LPCSTR)CIntString(pCow->docin->GetId());
					break;
				}
			}
			else
				sText = newInd;
		}
		break;
	case REJCOW_DOCOUT:
		sText = newInd;
		break;

	case REJCOW_TERMBUYSTOCK:
		if(!(pCow->rejectReason & CSyncCowData::TERMBUYSTOCK_NOT_FOUND && 
			!(pCow->rejectReason & CSyncCowData::TERMBUYSTOCK_NOT_FOUND_REPAIRED)))
		{
			if(pCow->termbuystock)
				sText = pCow->termbuystock->GetStockCode();
			else
			{
				if(pCow->srcCow->GetTermBuyStock())
					sText = pCow->srcCow->GetTermBuyStock()->GetStockCode();
			}
		}
		break;

	case REJCOW_TERMBUYWEIGHT:
		sText = pCow->termbuyweight.ToString();
		break;
	case REJCOW_TERMBUYPRICE:
		sText = pCow->termbuyprice.ToString();
		break;
	case REJCOW_TERMSELLSTOCK:
		if(!(pCow->rejectReason & CSyncCowData::TERMSELLSTOCK_NOT_FOUND && 
			!(pCow->rejectReason & CSyncCowData::TERMSELLSTOCK_NOT_FOUND_REPAIRED)))
		{
			if(pCow->termsellstock)
				sText = pCow->termsellstock->GetStockCode();
			else
			{
				if(pCow->srcCow->GetTermSellStock())
					sText = (LPCSTR)pCow->srcCow->GetTermSellStock()->GetStockCode();
			}
		}
		break;
	case REJCOW_TERMSELLWEIGHT:
		sText = pCow->termsellweight.ToString();		
		break;
	case REJCOW_TERMSELLPRICE:
		sText = pCow->termsellprice.ToString();
		break;
	case REJCOW_SEX:
		sText = (LPCSTR)pCow->sex;
	break;
	default:
		ASSERT(0);
		break;
	}
	lstrcpy(pszItemText,sText);
}

bool CRejectedCowsListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	CSyncCowData *pCow = (*m_rejCowArray)[nItem];
 
	switch(GetColID(nSubItem))
	{
	case REJCOW_EAN:
		if(pCow->rejectReason & CSyncCowData::EAN_NOT_FOUND 
			|| pCow->rejectReason & CSyncCowData::DUPLICATE_NOTSOLD_FOUND)
		{
			*cellBkg = REJ_CLR_INREPAIRABLE;
			break;
		}
		return false;	
		
	case REJCOW_STOCK:
		if(pCow->rejectReason & CSyncCowData::STOCK_NOT_FOUND_REPAIRED)
		{
			*cellBkg = REJ_CLR_REPAIRED;
			break;
		}
		else if(pCow->rejectReason & CSyncCowData::STOCK_NOT_FOUND)
		{
			*cellBkg = REJ_CLR_REPAIRABLE;
			break;
		} else	if(pCow->rejectReason & CSyncCowData::CHNGS_DROPPED)
		{
			*cellBkg = REJ_CLR_REPAIRED;
			break;
		}
		else if(pCow->rejectReason & CSyncCowData::CHNGS_VIOLATE_INVOICE)
		{
			*cellBkg = REJ_CLR_REPAIRABLE;
			break;
		}

		return false;			
	case REJCOW_DOCIN:
		if(pCow->rejectReason & CSyncCowData::DOCIN_NOT_FOUND)
		{
			*cellBkg = REJ_CLR_INREPAIRABLE;
			break;
		}
		return false;
	case REJCOW_DOCOUT:
		if(pCow->rejectReason & CSyncCowData::DOCOUT_NOT_FOUND)
		{
			*cellBkg = REJ_CLR_INREPAIRABLE;
			break;
		}
		return false;

	case REJCOW_TERMBUYSTOCK:
		if(pCow->rejectReason & CSyncCowData::TERMBUYSTOCK_NOT_FOUND_REPAIRED)
		{
			*cellBkg = REJ_CLR_REPAIRED;
			break;
		}
		else if(pCow->rejectReason & CSyncCowData::TERMBUYSTOCK_NOT_FOUND) 
		{
			*cellBkg = REJ_CLR_REPAIRABLE;
			break;
		}
		return false;

	case REJCOW_TERMSELLSTOCK:

		if(pCow->rejectReason & CSyncCowData::TERMSELLSTOCK_NOT_FOUND_REPAIRED)
		{
			*cellBkg = REJ_CLR_REPAIRED;
			break;
		}
		else if(pCow->rejectReason & CSyncCowData::TERMSELLSTOCK_NOT_FOUND) 
		{
			*cellBkg = REJ_CLR_REPAIRABLE;
			break;
		}
		return false;
	case REJCOW_SEX:
	case REJCOW_WEIGHT:
		if(pCow->rejectReason & CSyncCowData::CHNGS_DROPPED)
		{
			*cellBkg = REJ_CLR_REPAIRED;
			break;
		}
		else if(pCow->rejectReason & CSyncCowData::CHNGS_VIOLATE_INVOICE)
        {
			*cellBkg = REJ_CLR_REPAIRABLE;
			break;
		}
		return false;


	default:
		return false;
	}
	return true;	
}
void CRejectedCowsListCtrl::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	UINT colID;
	Stock *pStock = NULL;
	bool bCreateCombo = false;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}
	*pResult = 0;

	
	if(nItem >= 0)
	{
		colID = GetColID(nSubItem);
		CSyncCowData *pCow = (*m_rejCowArray)[nItem];
		switch(colID)
		{
			case REJCOW_STOCK:
			{
				if(pCow->rejectReason & CSyncCowData::STOCK_NOT_FOUND)
				{
					bCreateCombo = true;
					pStock = pCow->stock;
				}
			break;
			}
		case REJCOW_TERMBUYSTOCK:
			{
				if(pCow->rejectReason & CSyncCowData::TERMBUYSTOCK_NOT_FOUND)
				{
					bCreateCombo = true;
					pStock = pCow->termbuystock;
				}
			break;
			}
		case REJCOW_TERMSELLSTOCK:
			{
				if(pCow->rejectReason & CSyncCowData::TERMSELLSTOCK_NOT_FOUND)
				{
					bCreateCombo = true;
					pStock = pCow->termsellstock;
				}
			break;
			}
		}

		if(bCreateCombo)
		{
			CRect Rect;
			if (!GetCellRect(nItem,nSubItem,Rect,true,MIN_COL_WIDTH)) 
				return;	
			CComboBox *pCombo = new gxStockComboCell(this, nItem, colID, pStock);
			Rect.bottom += DROPDOWNHEIGHT;
			pCombo->Create(WS_VSCROLL | WS_VISIBLE | CBS_DROPDOWNLIST |  
				WS_CHILD | WS_BORDER |  ES_AUTOHSCROLL | ES_AUTOVSCROLL |  CBS_DISABLENOSCROLL, 
				Rect,(CWnd*)this, IDC_STOCKCOMBOCELL);
		}
	}
	
}



