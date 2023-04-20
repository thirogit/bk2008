#include "stdafx.h"
#include "RejectedDocsListCtrl.h"
#include "../combo/HentComboCell.h"
#include "../combo/ReasonComboCell.h"
#include "../consts.h"


#define IDC_HENTCOMBOCELL 1001
#define IDC_REASONCOMBOCELL 1002

#define MIN_COL_WIDTH_HENTS 100
#define MIN_COL_WIDTH_REASONS 50
#define DROPDOWNHEIGHT 300

BEGIN_MESSAGE_MAP(CRejectedDocsListCtrl, CBaseListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK , OnDblClick)
	
END_MESSAGE_MAP()


CRejectedDocsListCtrl::CRejectedDocsListCtrl(CSyncDocDataArray *rejectedDocArray,const GRIDHEADERS *ppHeaders,int iConfigId/* = -1*/) : 
CBaseListCtrl(ppHeaders,iConfigId) ,m_rejDocArray(rejectedDocArray)
{
	ASSERT(rejectedDocArray);
}

/*void CRejectedDocsListCtrl::SortByCol(int Column,bool bAsc)
{
   //m_docArray->SortBy((DOCS_SORT_BY)GetColID(Column),bAsc);
}*/


void CRejectedDocsListCtrl::UpdateItemCount()
{
	SetItemCount(m_rejDocArray->GetSize());
}
	
void CRejectedDocsListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{
	CSyncDocData *pDoc = (*m_rejDocArray)[nItem];
	CString temp;
	const char *szText = NULL;
	const char szRejectedText[] = "---";

	switch(GetColID(nSubItem))
	{
	case REJDOC_DOCNO:
		if(pDoc->rejectReason & CSyncDocData::DOCNO_NOT_FOUND)
			szText = szRejectedText;
		else
		{
			temp.Format("%04d",pDoc->docno);
			szText =  (LPCSTR)temp;
		}
		break;
	case REJDOC_DOCDATE:
		szText = pDoc->docdate.GetDayDate();
		break;
	case REJDOC_HENT:
		if(pDoc->rejectReason & CSyncDocData::HENT_NOT_FOUND &&
			!(pDoc->rejectReason & CSyncDocData::HENT_NOT_FOUND_REPAIRED))
			szText = szRejectedText;
		else
			szText = (LPCSTR)pDoc->hent->GetAlias();
		break;
	case REJDOC_LOADDATE:
		szText = pDoc->loaddate.GetDayDate();
		break;
	case REJDOC_PLATENO:
		szText = (LPCSTR)pDoc->plateno;
		break;	
	case REJDOC_REASON:
		if(pDoc->rejectReason & CSyncDocData::REASON_NOT_FOUND &&
			!(pDoc->rejectReason & CSyncDocData::REASON_NOT_FOUND_REPAIRED))
			szText = szRejectedText;
		else
			szText = (LPCSTR)pDoc->reason->GetReasonCode();
		break;
	case REJDOC_NOCOWS:
		temp.Format("%d",pDoc->cowArray.GetSize());
		szText = (LPCSTR)temp;
		break;
	default:
		ASSERT(0);
		break;
	}
	lstrcpy(pszItemText,szText);
}

bool CRejectedDocsListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	CSyncDocData *pDoc = (*m_rejDocArray)[nItem];
	
	switch(GetColID(nSubItem))
	{
	case REJDOC_DOCNO:
		if(pDoc->rejectReason & CSyncDocData::DOCNO_NOT_FOUND)
		{
			*cellBkg = REJ_CLR_INREPAIRABLE;
			break;
		}
		return false;	
	case REJDOC_HENT:
		if(pDoc->rejectReason & CSyncDocData::HENT_NOT_FOUND_REPAIRED)
		{
			*cellBkg = REJ_CLR_REPAIRED;
		}
		else if(pDoc->rejectReason & CSyncDocData::HENT_NOT_FOUND)
			
		{
			*cellBkg = REJ_CLR_REPAIRABLE;
			break;
		}
		return false;		
	case REJDOC_REASON:
		/*if(pDoc->rejectReason & CSyncDocData::REASON_NOT_FOUND_REPAIRED)			
		{
			*cellBkg = REJ_CLR_REPAIRED;
			break;
		}
		else if(pDoc->rejectReason & CSyncDocData::REASON_NOT_FOUND)			
		{
			*cellBkg = REJ_CLR_REPAIRABLE;
			break;
		}*/
		*cellBkg = REJ_CLR_INREPAIRABLE;
		return false;


	case REJDOC_NOCOWS:
		if(pDoc->rejectReason & CSyncDocData::NO_COWS)
		{
			*cellBkg = REJ_CLR_INREPAIRABLE;
			break;
		}
		return false;
	default:
		return false;
	}	
	return true;
}

void CRejectedDocsListCtrl::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
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
		CSyncDocData *pDoc = (*m_rejDocArray)[nItem];
	
		switch(GetColID(nSubItem))
		{
			case REJDOC_HENT:
			{
				if(pDoc->rejectReason & CSyncDocData::HENT_NOT_FOUND)
				{
					CRect Rect;
					if (!GetCellRect(nItem,nSubItem,Rect,true,MIN_COL_WIDTH_HENTS)) 
    					break;
					CComboBox *pCombo = new gxHentComboCell(this, nItem, nSubItem, pDoc->hent);
					Rect.bottom += DROPDOWNHEIGHT;
					pCombo->Create(WS_VSCROLL | WS_VISIBLE | CBS_DROPDOWNLIST |  
							WS_CHILD | WS_BORDER |  ES_AUTOHSCROLL | ES_AUTOVSCROLL |  CBS_DISABLENOSCROLL, 
						Rect,(CWnd*)this, IDC_HENTCOMBOCELL);
				}

			break;
			}
			/*case REJDOC_REASON:
				if(pDoc->rejectReason & CSyncDocData::REASON_NOT_FOUND)
				{
					CRect Rect;
					Reason::REASON_INOUT_TYPE inout = 
						(pDoc->srcDoc->in_out == Doc::IN_DOC ? Reason::REASON_IN : Reason::REASON_OUT);

					if (!GetCellRect(nItem,nSubItem,Rect,true,MIN_COL_WIDTH_REASONS)) 
    					break;
					CComboBox *pCombo = new gxReasonComboCell(inout,this, nItem, nSubItem, pDoc->reason);
					Rect.bottom += DROPDOWNHEIGHT;
					pCombo->Create(WS_VSCROLL | WS_VISIBLE | CBS_DROPDOWNLIST |  
							WS_CHILD | WS_BORDER |  ES_AUTOHSCROLL | ES_AUTOVSCROLL |  CBS_DISABLENOSCROLL, 
						Rect,(CWnd*)this, IDC_REASONCOMBOCELL);
				}

				break;*/
		}
	}

	*pResult = 0;
}






