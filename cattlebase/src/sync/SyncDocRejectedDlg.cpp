#include "stdafx.h"
#include "SyncDocRejectedDlg.h"


const GRIDHEADERS CSyncRejectedUpdDocsDlg::RejectedUpdDocHeader[] =
{
	{IDS_HEADERDOCNO,CRejectedDocsListCtrl::REJDOC_DOCNO},
	{IDS_HEADERDOCDATE,CRejectedDocsListCtrl::REJDOC_DOCDATE},
	{IDS_HEADERINOUTDATE,CRejectedDocsListCtrl::REJDOC_LOADDATE},
	{IDS_HEADERHENT,CRejectedDocsListCtrl::REJDOC_HENT},
	{IDS_HEADERPLATENO,CRejectedDocsListCtrl::REJDOC_PLATENO},
	{IDS_HEADERREASON,CRejectedDocsListCtrl::REJDOC_REASON},
	{0,0}
};


const GRIDHEADERS CSyncRejectedNewDocsDlg::RejectedNewDocHeader[] =
{
	{IDS_HEADERDOCDATE,CRejectedDocsListCtrl::REJDOC_DOCDATE},
	{IDS_HEADERINOUTDATE,CRejectedDocsListCtrl::REJDOC_LOADDATE},
	{IDS_HEADERHENT,CRejectedDocsListCtrl::REJDOC_HENT},
	{IDS_HEADERPLATENO,CRejectedDocsListCtrl::REJDOC_PLATENO},
	{IDS_HEADERREASON,CRejectedDocsListCtrl::REJDOC_REASON},
	{IDS_HEADERAMOUNT,CRejectedDocsListCtrl::REJDOC_NOCOWS},
	{0,0}
};


IMPLEMENT_DYNAMIC(CSyncRejectedDocsDlg, CSyncRejectedDlg)
CSyncRejectedDocsDlg::CSyncRejectedDocsDlg(CSyncDocDataArray &rejectedDocsArray,UINT nCaption,
										   const GRIDHEADERS *ppHeaders,int nListCfgId,CWnd* pParent/* = NULL*/)
	: CSyncRejectedDlg(nCaption),
	m_rejectedDocs(&rejectedDocsArray,ppHeaders,nListCfgId),
	m_rejectedDocsArray(rejectedDocsArray)
{
}
CSyncRejectedDocsDlg::~CSyncRejectedDocsDlg()
{
}

void CSyncRejectedDocsDlg::DoDataExchange(CDataExchange* pDX)
{
	CSyncRejectedDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_REJECTEDLIST,m_rejectedDocs);
	
}


BEGIN_MESSAGE_MAP(CSyncRejectedDocsDlg, CSyncRejectedDlg)
	ON_NOTIFY(LVN_ENDLABELEDIT,IDC_REJECTEDLIST, OnRejCorrect)
	
	
END_MESSAGE_MAP()


void CSyncRejectedDocsDlg::OnRejCorrect (NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
    LV_ITEM	*plvItem = &plvDispInfo->item;
	int nItem = plvItem->iItem;

	if(nItem >= 0)
	{
		CSyncDocData *pDoc = m_rejectedDocsArray[nItem];
    	
		switch(m_rejectedDocs.GetColID(plvItem->iSubItem))
		{
		case CRejectedDocsListCtrl::REJDOC_HENT:

			
			pDoc->hent = (Hent*)plvItem->lParam;
			if(pDoc->hent)
			{
				pDoc->rejectReason = pDoc->rejectReason | CSyncDocData::HENT_NOT_FOUND_REPAIRED;
				
			}
			else
			{
				pDoc->rejectReason &= ~CSyncDocData::HENT_NOT_FOUND_REPAIRED;
			}
			
		break;
		case CRejectedDocsListCtrl::REJDOC_REASON:
			pDoc->reason = (Reason*)plvItem->lParam;
			ASSERT(pDoc->reason);
			pDoc->rejectReason |= CSyncDocData::REASON_NOT_FOUND_REPAIRED;
		break;

		}		
		m_rejectedDocs.Invalidate(FALSE);
	}
	*pResult = 0;

}

void CSyncRejectedDocsDlg::DoRefresh()
{

	CSyncDocData *pDoc = NULL;
	int i = 0,s = m_rejectedDocsArray.GetSize();
	for(;i < s;i++)
	{
		pDoc = m_rejectedDocsArray[i];
		if(pDoc->rejectReason & CSyncDocData::HENT_NOT_FOUND_REPAIRED)
		{
			pDoc->rejectReason = pDoc->rejectReason & 
				(~(CSyncDocData::HENT_NOT_FOUND | CSyncDocData::HENT_NOT_FOUND_REPAIRED));

			if(pDoc->srcDoc->IsNew()) 
				pDoc->srcDoc->ChangeHent(pDoc->hent);
			else
				pDoc->srcDoc->SetHent(pDoc->hent);
		}

		if(pDoc->rejectReason & CSyncDocData::REASON_NOT_FOUND_REPAIRED)
		{
			pDoc->rejectReason = pDoc->rejectReason & 
				(~(CSyncDocData::REASON_NOT_FOUND | CSyncDocData::REASON_NOT_FOUND_REPAIRED));

			if(pDoc->srcDoc->IsNew()) 
				pDoc->srcDoc->ChangeReason(pDoc->reason);
			else
				pDoc->srcDoc->SetReason(pDoc->reason);
		}
		
	}
	UpdateCount();

}


BOOL CSyncRejectedDocsDlg::OnInitDialog()
{

	CSyncRejectedDlg::OnInitDialog();
	UpdateCount();
	
	return TRUE;

}

UINT CSyncRejectedDocsDlg::RejectedCount()
{
	return m_rejectedDocsArray.GetSize();
}



CString CSyncRejectedDocsDlg::FormatRejectedReasonsText(int nItemClicked)
{
	if(nItemClicked < m_rejectedDocsArray.GetSize())
	{
		CSyncDocData *pDoc = m_rejectedDocsArray[nItemClicked];
		CString m_retStr,reasonStr;

		int reason2stringid[][2] =
		{
			{CSyncDocData::DOCNO_NOT_FOUND,IDS_REJECTREASON_DOCNO_NOT_FOUND},
			{CSyncDocData::HENT_NOT_FOUND,IDS_REJECTREASON_HENTNOTFOUND},
			{CSyncDocData::NO_COWS,IDS_REJECTREASON_NOCOWS},
			{CSyncDocData::REASON_NOT_FOUND,IDS_REJECTREASON_REASONNOTFOUND}			
		};

		for(int i = 0; i < (sizeof(reason2stringid)/(sizeof(int)*2));i ++)
		{
			if(pDoc->rejectReason & reason2stringid[i][0])
			{
				reasonStr.LoadString(reason2stringid[i][1]);
				m_retStr += reasonStr + '\n';
			}
		}
		return m_retStr;
	}
	
	return CString();
}

void CSyncRejectedDocsDlg::UpdateCount()
{
	if(m_hWnd != NULL && IsWindow(m_hWnd))
		m_rejectedDocs.UpdateItemCount();
}

