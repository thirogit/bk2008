#include "stdafx.h"
#include "EditOutDocDlg.h"
#include "../classes/match/CowMatcher.h"
#include "../classes/forallactions/CountAction.h"
#include "../global_arrays.h"
#include "../mainfrm_ctrl.h"
#include "../CattleSQL.h"

CEditOutDocDlg::CEditOutDocDlg(OutDoc *pDoc2Edit) : m_pEditDoc(pDoc2Edit)				
{
	ASSERT(pDoc2Edit);	
}


CEditOutDocDlg::~CEditOutDocDlg()
{

}

CString CEditOutDocDlg::GetDocDlgCaption()
{
	CString wndCaption;
	wndCaption.Format(IDS_OUTDOCEDIT,m_pEditDoc->GetId());
	return wndCaption;
}

void CEditOutDocDlg::SetupBtns() 
{
	CString btnCaption;
	btnCaption.LoadString(IDS_BTNEDIT);
	m_OK.SetWindowText(btnCaption);	
	btnCaption.LoadString(IDS_SELECT);
	m_Select.SetButtonImage( IDB_MAGICSELECT ,RGB(255,0,255));
	m_Select.SetWindowText(btnCaption);
	GetDlgItem(IDC_GETCOWS)->EnableWindow(FALSE);
}




BOOL  CEditOutDocDlg::OnInitDialog()
{
	CBaseDocDlg::OnInitDialog();
	m_DocDate.SetDate(m_pEditDoc->GetDocDate());
	m_TransportDate.SetDate(m_pEditDoc->GetLoadDate());
	SetDlgItemText(IDC_PLATENO,m_pEditDoc->GetPlateNo());
	m_extras.SetWindowText(m_pEditDoc->GetExtras());
	m_hentCombo.ReloadCombo(m_pEditDoc->GetHent());
	m_cows.AppendPtr(m_pEditDoc->GetCows());
	m_reasonCombo.ReloadCombo(m_pEditDoc->GetReason()->GetId());
	m_loadStartTm.SetTime(m_pEditDoc->GetLoadStartTime());
	m_loadEndTm.SetTime(m_pEditDoc->GetLoadEndTime());
	m_AgentCb.ReloadCombo(m_pEditDoc->GetAgent());
		
	UpdateSummary();
	UpdateCowCount();
	return TRUE;
}

bool CEditOutDocDlg::CommitDoc()
{	
	if(!m_pEditDoc->CanEditThisDoc())
	{
		AfxMessageBox(IDS_ERRCANTEDITDOC,MB_OK,0);
		return false;
	}
	
	Cow *pCow;
	CCowArray newCows,oldCows;

	CCowArray::ArrayIterator it = m_cows.iterator();
	while(it.hasNext())
	{			
		pCow = *(it++);
		if(!pCow->IsOut())
			newCows.Add(pCow);				
		else
			oldCows.Add(pCow);
	}

	if(m_pEditDoc->HasInvoice() && (m_delCows.GetSize() > 0 || newCows.GetSize() > 0))
	{
		AfxMessageBox(IDS_CANTADDDELCOWSBECOUSEINVOICE);
		return false;
	}

	

	it = m_delCows.iterator();
	while(it.hasNext())
	{
		pCow = *(it++);
		pCow->DeleteOutDoc();
		if(!pCow->UpdateThisCow(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRDELETINGCOWFROMOUTDOC);
			SQLDB.Rollback();
			while(!it.isFirst()) (*(--it))->AssignOutDoc(m_pEditDoc);
			return false;
		}
	}	

	it = oldCows.iterator();
	while(it.hasNext())
	{
		pCow = *(it++);
		if(!pCow->UpdateThisCow(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRADDINGCOWTOOUTDOC);
			SQLDB.Rollback();	
			return false;
		}
	}

	it = newCows.iterator();
	while(it.hasNext())
	{
		pCow = *(it++);
		if(!pCow->IsOut())
		{
			pCow->AssignOutDoc(m_pEditDoc);		
			if(!pCow->UpdateThisCow(false))
			{
				SQLDB.SQLMessageBox(IDS_ERRADDINGCOWTOOUTDOC);
				SQLDB.Rollback();	
				while(!it.isFirst()) (*(--it))->DeleteOutDoc();
				return false;
			}
		}
	}

	UpdateDoc(m_pEditDoc);

	if(m_pEditDoc->HasInvoice() && m_pEditDoc->GetInvoice()->GetInvoiceDate() < m_pEditDoc->GetDocDate())
	{
		m_pEditDoc->RollbackChanges();
		AfxMessageBox(IDS_INVOICEDATEISBEFOREDOCDATE);
		return false;
	}

	if(!m_pEditDoc->UpdateThisDoc(false)) 
	{
		m_pEditDoc->RollbackChanges();
		SQLDB.SQLMessageBox(IDS_ERRUPDATEINGDOC);
		SQLDB.Rollback();
		return false;
	}
	
	m_pEditDoc->GetCows()->Copy(m_cows);	


	m_pEditDoc->CommitChanges();
	SQLDB.Commit();
	CommitChangesForCows(m_cows);
	CommitChangesForCows(m_delCows);

	LockGrid(INCOWSGRID);	
	LockGrid(ALLCOWSGRID);
				
	ary_incows.Append(m_delCows);
	ary_incows.RemoveArray(newCows);				
	
	UpdateGrid(INCOWSGRID);
	UpdateGrid(ALLCOWSGRID);
	UpdateGrid(OUTDOCSGRID);
	
	return true;
}


void CEditOutDocDlg::DeleteCow(int iElement)
{
	if(m_pEditDoc->HasInvoice())
	{
		AfxMessageBox(IDS_CANTADDDELCOWSBECOUSEINVOICE);
		return;
	}

	Cow *pCow = m_cows[iElement];
	if(!pCow->IsOut())
	{
		pCow->RollbackChanges();
		m_cows.RemoveAt(iElement);
	}
	else
	{
		CCowDataMatcher cowMatcher;
		cowMatcher.FindEAN(pCow->GetEAN());
		CCountAction<CCowArray::ArrayAction,CCowDataMatcher,Cow> duplicateEAN(&cowMatcher);
		ary_incows.ForAllAction(duplicateEAN);
		if(duplicateEAN.GetCount() > 0)
		{
			AfxMessageBox(IDS_DELOUTDOCCOWAGAINNOTSOLD);
			return;
		}
		else
		{
			pCow->RollbackChanges();
			pCow->ChangeOutGrp(NULL_INOUT_GRP);
			m_delCows.Add(pCow);
			m_cows.RemoveAt(iElement);
		}
	}	
	UpdateSummary();

}

void CEditOutDocDlg::CancelDoc()
{	
	RollbackChangesForCows(m_cows);
	RollbackChangesForCows(m_delCows);
}
void CEditOutDocDlg::AddSelectedCows(bool bFromAllCows /*= false*/)
{
	if(m_pEditDoc->HasInvoice())
	{
		AfxMessageBox(IDS_CANTADDDELCOWSBECOUSEINVOICE);
		return;
	}
	CNewOutDocDlg::AddSelectedCows(bFromAllCows);
}