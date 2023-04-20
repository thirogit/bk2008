// RejectedBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RejectedBaseDlg.h"
#include ".\rejectedbasedlg.h"
#include "MainCfg.h"



// CRejectedBaseDlg dialog

IMPLEMENT_DYNAMIC(CRejectedBaseDlg, CDialog)
CRejectedBaseDlg::CRejectedBaseDlg(CSyncDocDataArray &rejUpdInDocs,
	CSyncDocDataArray &rejNewInDocs,
	CSyncDocDataArray &rejUpdOutDocs,
	CSyncDocDataArray &rejNewOutDocs,
	CSyncDocDataArray &newInDocs,
	CSyncDocDataArray &newOutDocs,
	CSyncDocDataArray &updInDocs,
	CSyncCowDataArray &newCowArray,
	CSyncCowDataArray &updCowArray,
	CSyncCowDataArray &rejNewCowArray,
	CSyncCowDataArray &rejUpdCowArray,
	CWnd* pParent /*=NULL*/)
	: CDialog(CRejectedBaseDlg::IDD, pParent),
	m_rejUpdInDocs(rejUpdInDocs),
	m_rejNewInDocs(rejNewInDocs),
	m_rejUpdOutDocs(rejUpdOutDocs),
	m_rejNewOutDocs(rejNewOutDocs),
	m_newInDocs(newInDocs),
	m_newOutDocs(newOutDocs),
	m_updInDocs(updInDocs),
	m_newCowArray(newCowArray),
	m_updCowArray(updCowArray),
	m_rejNewCowArray(rejNewCowArray),
	m_rejUpdCowArray(rejUpdCowArray),
	m_rejuInDocsDlg(m_rejUpdInDocs, IDS_REJUPDINDOCSDLGCAPTION,CFGID_REJUPDINDOCSLIST),
	m_rejnInDocsDlg(m_rejNewInDocs, IDS_REJNEWINDOCSDLGCAPTION,CFGID_REJNEWINDOCSLIST),
	m_rejnOutDocsDlg(m_rejNewOutDocs, IDS_REJNEWOUTDOCSDLGCAPTION,CFGID_REJNEWOUTDOCSLIST),
	m_rejnCowsDlg(m_rejNewCowArray,IDS_REJNEWCOWSDLGCAPTION,CFGID_REJNEWCOWSLIST),
	m_rejuCowsDlg(m_rejUpdCowArray,IDS_REJUPDCOWSDLGCAPTION,CFGID_REJUPDCOWSLIST)
{

	
}

CRejectedBaseDlg::~CRejectedBaseDlg()
{
}

void CRejectedBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CRejectedBaseDlg, CDialog)
	ON_BN_CLICKED(IDC_REJREFRESH, OnBnClickedRefresh)
	ON_MESSAGE(WM_SETREJECTTEXT,OnSetRejectText)
END_MESSAGE_MAP()


// CRejectedBaseDlg message handlers

void CRejectedBaseDlg::OnBnClickedRefresh()
{


	m_rejuCowsDlg.DoRefresh();
	m_rejnCowsDlg.DoRefresh();
	m_rejuInDocsDlg.DoRefresh();
	m_rejnInDocsDlg.DoRefresh();
	m_rejnOutDocsDlg.DoRefresh();
	
	CSyncDocData *pDoc = NULL;
	CSyncCowData *pCow = NULL;

	int i,j,s,t;
	for(i = 0,s = m_rejNewInDocs.GetSize();i < s;i++)
	{
		pDoc = m_rejNewInDocs[i];
		if(pDoc->rejectReason == CSyncDocData::NO_COWS)
		{
			for(j = 0,t = m_rejNewCowArray.GetSize();j < t;j++)
			{
				pCow = m_rejNewCowArray[j];
				if(pCow->rejectReason == CSyncCowData::DOCIN_NOT_FOUND && pCow->docinno == pDoc->docno)
				{
					pCow->sync_docin = pDoc;
					pCow->rejectReason &= (~CSyncCowData::DOCIN_NOT_FOUND);
					pDoc->cowArray.Add(pCow);
					pDoc->rejectReason &= (~CSyncDocData::NO_COWS);
				}
			}
		}
	}

	for(i = 0,s = m_rejNewOutDocs.GetSize();i < s;i++)
	{
		pDoc = m_rejNewOutDocs[i];
		if(pDoc->rejectReason == CSyncDocData::NO_COWS)
		{
			for(j = 0,t = m_rejNewCowArray.GetSize();j < t;j++)
			{
				pCow = m_rejNewCowArray[j];
				if(pCow->rejectReason == CSyncCowData::DOCOUT_NOT_FOUND && pCow->docoutno == pDoc->docno)
				{
					pCow->sync_docout = pDoc;
					pCow->rejectReason &= (~CSyncCowData::DOCOUT_NOT_FOUND);
					pDoc->cowArray.Add(pCow);
					pDoc->rejectReason &= (~CSyncDocData::NO_COWS);
				}
			}

			for(j = 0,t = m_rejUpdCowArray.GetSize();j < t;j++)
			{
				pCow = m_rejUpdCowArray[j];
				if(pCow->rejectReason == CSyncCowData::DOCOUT_NOT_FOUND && pCow->docoutno == pDoc->docno)
				{
					pCow->sync_docout = pDoc;
					pCow->rejectReason &= (~CSyncCowData::DOCOUT_NOT_FOUND);
					pDoc->cowArray.Add(pCow);
					pDoc->rejectReason &= (~CSyncDocData::NO_COWS);
				}
			}
		}

	}
	
	for(i = 0,s = m_rejNewInDocs.GetSize();i < s;i++)
	{
		pDoc = m_rejNewInDocs[i]; 
		if(pDoc->rejectReason == CSyncDocData::NOT_REJECTED)
		{
			m_rejNewInDocs.RemoveAt(i);
			i--;s--;
			m_newInDocs.Add(pDoc);
		}
	}
	for(i = 0,s = m_rejNewOutDocs.GetSize();i < s;i++)
	{
		pDoc = m_rejNewOutDocs[i]; 
		if(pDoc->rejectReason == CSyncDocData::NOT_REJECTED)
		{
			m_rejNewOutDocs.RemoveAt(i);
			i--;s--;
			m_newOutDocs.Add(pDoc);
		}
	}
	
	for(i = 0,s = m_rejUpdInDocs.GetSize();i < s;i++)
	{
		pDoc = m_rejUpdInDocs[i]; 
		if(pDoc->rejectReason == CSyncDocData::NOT_REJECTED)
		{
			m_rejUpdInDocs.RemoveAt(i);
			i--;s--;
			m_updInDocs.Add(pDoc);
		}
	}

	for(j = 0,t = m_rejUpdCowArray.GetSize();j < t;j++)
	{
		pCow = m_rejUpdCowArray[j]; 
		if(pCow->rejectReason == CSyncCowData::NOT_REJECTED)
		{
			m_rejUpdCowArray.RemoveAt(j);
			j--;t--;
			m_updCowArray.Add(pCow);
		}
		else
		{
			for(i = 0,s = m_newOutDocs.GetSize();i < s;i++)
			{
				pDoc = m_newOutDocs[i];
				if(pDoc->docno == pCow->docoutno)
				{
					pCow->sync_docout = pDoc;
					pCow->rejectReason &= (~CSyncCowData::DOCOUT_NOT_FOUND); 
				}
			}
		}
	}
	for(j = 0,t = m_rejNewCowArray.GetSize();j < t;j++)
	{
		pCow = m_rejNewCowArray[j]; 
		if(pCow->rejectReason == CSyncCowData::NOT_REJECTED)
		{
			m_rejNewCowArray.RemoveAt(j);
			j--;t--;
			m_newCowArray.Add(pCow);
		}

	}



	m_rejuCowsDlg.UpdateCount();
	m_rejnCowsDlg.UpdateCount();
	m_rejuInDocsDlg.UpdateCount();
	m_rejnInDocsDlg.UpdateCount();
	m_rejnOutDocsDlg.UpdateCount();
	UpdatePropertyPageTitles();	
}




void CRejectedBaseDlg::UpdatePropertyPageTitles()
{
	CSyncRejectedDlg *pPropPage = NULL;
	CTabCtrl* pTab = m_rejSheet.GetTabControl();
    ASSERT (pTab);

	CString tabTitle;


	TC_ITEM ti;
    ti.mask = TCIF_TEXT;

	for(int i = 0,s = m_rejSheet.GetPageCount();i<s;i++)
	{
		pPropPage = (CSyncRejectedDlg*)m_rejSheet.GetPage(i);
		tabTitle.Format(pPropPage->GetCaptionId(),pPropPage->RejectedCount());
		//pPropPage->m

		ti.pszText = tabTitle.GetBuffer();
		pTab->SetItem(i, &ti);
		tabTitle.ReleaseBuffer();
	}

}



BOOL CRejectedBaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_rejSheet.AddPage(&m_rejuInDocsDlg);
	m_rejSheet.AddPage(&m_rejnInDocsDlg);
	m_rejSheet.AddPage(&m_rejnOutDocsDlg);
	m_rejSheet.AddPage(&m_rejnCowsDlg);
	m_rejSheet.AddPage(&m_rejuCowsDlg);

	m_rejSheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	
	m_rejSheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_rejSheet.ModifyStyle( 0, WS_TABSTOP );

	CRect rcSheet,Rect;

	GetClientRect( &rcSheet );
	GetDlgItem(IDC_REJECTREASONGRPBOX)->GetWindowRect(&Rect);	
	ScreenToClient(&Rect);
	
	m_rejSheet.SetWindowPos( NULL,0,0, rcSheet.Width(),Rect.top-5,
				SWP_NOZORDER |SWP_NOACTIVATE );

	UpdatePropertyPageTitles();
	SetDlgItemText(IDC_REJECTREASONTEXT,"");	

	return TRUE;
}


LRESULT CRejectedBaseDlg::OnSetRejectText(WPARAM wParam,LPARAM lParam)
{
	SetDlgItemText(IDC_REJECTREASONTEXT,(LPCSTR)wParam);	
	return (LRESULT)0;
}

void CRejectedBaseDlg::OnCancel()
{
	if(AfxMessageBox(IDS_SYNCCANCEL,MB_YESNO,0) == IDYES)
	{
		RollbackChangesToRejectedUpdatedCows();
		EndDialog(IDCANCEL);
	}

}

void CRejectedBaseDlg::OnOK()
{
	RollbackChangesToRejectedUpdatedCows();
	EndDialog(IDOK);
}

void CRejectedBaseDlg::RollbackChangesToRejectedUpdatedCows()
{
	CSyncCowDataArray::ArrayIterator it = m_rejUpdCowArray.iterator();
	while(it.hasNext())
	{
		CSyncCowData* pSyncCow = *(it++);
		pSyncCow->srcCow->RollbackChanges();
	}
}