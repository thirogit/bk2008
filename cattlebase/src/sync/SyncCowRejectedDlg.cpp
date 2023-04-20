#include "stdafx.h"
#include "SyncCowRejectedDlg.h"
// CSyncCowRejectedDlg dialog

#define ID_DROP_CHNGS 2002


IMPLEMENT_DYNAMIC(CSyncCowRejectedDlg, CSyncRejectedDlg)
CSyncCowRejectedDlg::CSyncCowRejectedDlg(CSyncCowDataArray &rejectedCowsArray,UINT nCaption,int nListCfgId)
	: CSyncRejectedDlg(nCaption),
	m_rejectedCows(&rejectedCowsArray,nListCfgId),
	m_rejectedCowsArray(rejectedCowsArray)

{
}
CSyncCowRejectedDlg::~CSyncCowRejectedDlg()
{
}

void CSyncCowRejectedDlg::DoDataExchange(CDataExchange* pDX)
{
	CSyncRejectedDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_REJECTEDLIST,m_rejectedCows);
}


BEGIN_MESSAGE_MAP(CSyncCowRejectedDlg, CSyncRejectedDlg)
	ON_NOTIFY(LVN_ENDLABELEDIT,IDC_REJECTEDLIST, OnRejCorrect)
	ON_NOTIFY(NM_RCLICK,IDC_REJECTEDLIST,OnRClick)
	ON_COMMAND(ID_DROP_CHNGS,OnDropChanges)
	
END_MESSAGE_MAP()


void CSyncCowRejectedDlg::OnRejCorrect (NMHDR* pNMHDR, LRESULT* pResult) 
{
	 LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
    LV_ITEM	*plvItem = &plvDispInfo->item;
	int nItem = plvItem->iItem;

	if(nItem >= 0)
	{
		CSyncCowData *pCow = m_rejectedCowsArray[nItem];
    	
		switch(plvItem->iSubItem)
		{
		case CRejectedCowsListCtrl::REJCOW_STOCK:

			
			pCow->stock = (Stock*)plvItem->lParam;
			if(pCow->stock)
			{
				pCow->rejectReason = pCow->rejectReason | CSyncCowData::STOCK_NOT_FOUND_REPAIRED;
				
			}
			else
			{
				pCow->rejectReason &= ~CSyncCowData::STOCK_NOT_FOUND_REPAIRED;
			}
			
		break;
		case CRejectedCowsListCtrl::REJCOW_TERMBUYSTOCK:


			pCow->termbuystock = (Stock*)plvItem->lParam;
			if(pCow->termbuystock)
			{
				pCow->rejectReason = pCow->rejectReason | CSyncCowData::TERMBUYSTOCK_NOT_FOUND_REPAIRED;

			}
			else
			{
				pCow->rejectReason &= ~CSyncCowData::TERMBUYSTOCK_NOT_FOUND_REPAIRED;
			}

		break;
		case CRejectedCowsListCtrl::REJCOW_TERMSELLSTOCK:


			pCow->termsellstock = (Stock*)plvItem->lParam;
			if(pCow->termsellstock)
			{
				pCow->rejectReason = pCow->rejectReason | CSyncCowData::TERMSELLSTOCK_NOT_FOUND_REPAIRED;

			}
			else
			{
				pCow->rejectReason &= ~CSyncCowData::TERMSELLSTOCK_NOT_FOUND_REPAIRED;
			}

		break;
		}		
		m_rejectedCows.Invalidate(FALSE);
	}
	*pResult = 0;
}

BOOL CSyncCowRejectedDlg::OnInitDialog()
{
	CSyncRejectedDlg::OnInitDialog();
	UpdateCount();
	return TRUE;
}


void CSyncCowRejectedDlg::DoRefresh()
{

	CSyncCowData *pCow = NULL;
	int i = 0,s = m_rejectedCowsArray.GetSize();
	for(;i < s;i++)
	{
		pCow = m_rejectedCowsArray[i];
		if(pCow->rejectReason & CSyncCowData::STOCK_NOT_FOUND_REPAIRED)
		{
			pCow->rejectReason = pCow->rejectReason & 
				(~(CSyncCowData::STOCK_NOT_FOUND | CSyncCowData::STOCK_NOT_FOUND_REPAIRED));

			if(!pCow->srcCow->IsNew())
				pCow->srcCow->ChangeStock(pCow->stock);
			else
				pCow->srcCow->SetStock(pCow->stock);
		}

		if(pCow->rejectReason & CSyncCowData::TERMBUYSTOCK_NOT_FOUND_REPAIRED)
		{
			pCow->rejectReason = pCow->rejectReason & 
				(~(CSyncCowData::TERMBUYSTOCK_NOT_FOUND | CSyncCowData::TERMBUYSTOCK_NOT_FOUND_REPAIRED));

			if(!pCow->srcCow->IsNew())
				pCow->srcCow->ChangeTermBuyStock(pCow->termbuystock);
			else
				pCow->srcCow->SetTermBuyStock(pCow->termbuystock);
		}

		if(pCow->rejectReason & CSyncCowData::TERMSELLSTOCK_NOT_FOUND_REPAIRED)
		{
			pCow->rejectReason = pCow->rejectReason & 
				(~(CSyncCowData::TERMSELLSTOCK_NOT_FOUND | CSyncCowData::TERMSELLSTOCK_NOT_FOUND_REPAIRED));

			if(!pCow->srcCow->IsNew())
				pCow->srcCow->ChangeTermSellStock(pCow->termsellstock);
			else
				pCow->srcCow->SetTermSellStock(pCow->termsellstock);
		}

		if(pCow->rejectReason & CSyncCowData::CHNGS_DROPPED)
			pCow->rejectReason &= (~(CSyncCowData::CHNGS_VIOLATE_INVOICE | CSyncCowData::CHNGS_DROPPED));


		if(pCow->rejectReason == CSyncCowData::NOT_REJECTED)
		{
			if(pCow->sync_docout)
				pCow->sync_docout->cowArray.Add(pCow);
		}

		
	}
	UpdateCount();
}


UINT CSyncCowRejectedDlg::RejectedCount()
{
	return m_rejectedCowsArray.GetSize();
}

CString CSyncCowRejectedDlg::FormatRejectedReasonsText(int nItemClicked)
{

	if(nItemClicked < m_rejectedCowsArray.GetSize())
	{
		CSyncCowData *pCow = m_rejectedCowsArray[nItemClicked];
		CString m_retStr,reasonStr;

		int reason2stringid[][2] =
		{
			{CSyncCowData::DOCIN_NOT_FOUND,IDS_REJECTREASON_DOCINNOTFOUND},
			{CSyncCowData::DOCOUT_NOT_FOUND,IDS_REJECTREASON_DOCOUTNOTFOUND},
			{CSyncCowData::DUPLICATE_NOTSOLD_FOUND,IDS_REJECTREASON_DUPLICATE},
			{CSyncCowData::EAN_NOT_FOUND,IDS_REJECTREASON_EANNOTFOUND},
			{CSyncCowData::STOCK_NOT_FOUND,IDS_REJECTREASON_STOCKNOTFOUND},
			{CSyncCowData::TERMBUYSTOCK_NOT_FOUND,IDS_REJECTREASON_TERMBUYSTOCKNOTFOUND},
			{CSyncCowData::TERMSELLSTOCK_NOT_FOUND,IDS_REJECTREASON_TERMSELLSTOCKNOTFOUND},
			{CSyncCowData::CHNGS_VIOLATE_INVOICE,IDS_REJECTREASON_CHNGSVIOLATEINV}
		};

		for(int i = 0; i < (sizeof(reason2stringid)/(sizeof(int)*2));i ++)
		{
			if(pCow->rejectReason & reason2stringid[i][0])
			{
				reasonStr.LoadString(reason2stringid[i][1]);
				m_retStr += reasonStr + '\n';
			}
		}
		return m_retStr;
	}
	
	return CString();
}

void CSyncCowRejectedDlg::UpdateCount()
{
	if(m_hWnd != NULL && IsWindow(m_hWnd))
		m_rejectedCows.UpdateItemCount();
}

void CSyncCowRejectedDlg::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;		
		if(nItem >= 0)
		{
			CSyncCowData *pCow = m_rejectedCowsArray[nItem];
			if((pCow->rejectReason & CSyncCowData::CHNGS_VIOLATE_INVOICE) &&
				!(pCow->rejectReason & CSyncCowData::CHNGS_DROPPED))
			{			
				CPoint ptCursor;			
				CMenu DropChngsMenu;
				CString strMenuText;
				GetCursorPos(&ptCursor);
				DropChngsMenu.CreatePopupMenu();			
				strMenuText.LoadString(IDS_MENUTEXTDROPCHNGS);
				DropChngsMenu.InsertMenu(0, MF_BYPOSITION ,ID_DROP_CHNGS,strMenuText);
				DropChngsMenu.TrackPopupMenu(TPM_LEFTALIGN,ptCursor.x,ptCursor.y,this,NULL);

			}

		}		
	}
	*pResult = 0;
}
void CSyncCowRejectedDlg::OnDropChanges()
{
	int iSel = m_rejectedCows.GetCurSel();
	if(iSel >= 0)
	{
		CSyncCowData *pCow = m_rejectedCowsArray[iSel];
		pCow->rejectReason |= CSyncCowData::CHNGS_DROPPED;			
	}
	
}