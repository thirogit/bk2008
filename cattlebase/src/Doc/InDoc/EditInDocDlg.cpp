#include "stdafx.h"
#include "EditInDocDlg.h"
#include "../../Cow/EditCowDlg.h"
#include "../../PassportStorage/PassImgBase.h"
#include "../../mainfrm_ctrl.h"
#include "../../CattleSQL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CEditInDocDlg::CEditInDocDlg(InDoc *pDoc) : m_pEditDoc(pDoc) 
{	
	ASSERT(pDoc);
	ASSERT(pDoc->GetId());
}


BOOL  CEditInDocDlg::OnInitDialog()
{
	CBaseDocDlg::OnInitDialog(); //skip OnInitDialog in CNewInDocDlg

	m_DocDate.SetDate(m_pEditDoc->GetDocDate());
	m_TransportDate.SetDate(m_pEditDoc->GetLoadDate());
	SetDlgItemText(IDC_PLATENO,m_pEditDoc->GetPlateNo());
	m_extras.SetWindowText(m_pEditDoc->GetExtras());
	m_reasonCombo.ReloadCombo(m_pEditDoc->GetReason()->GetId());
	m_hentCombo.ReloadCombo(m_pEditDoc->GetHent());
	m_cows.AppendPtr(m_pEditDoc->GetCows());	
	UpdateCowCount();
	UpdateSummary();

	m_loadStartTm.SetTime(m_pEditDoc->GetLoadStartTime());
	m_loadEndTm.SetTime(m_pEditDoc->GetLoadEndTime());
	m_AgentCb.ReloadCombo(m_pEditDoc->GetAgent());

#ifdef SPECIAL_TRIAL_RELEASE
	m_ScanBtn.ShowWindow(SW_HIDE);
#endif

	return TRUE ;
}

void CEditInDocDlg::EditCow(int nElement)
{
	Cow *pCow = m_cows[nElement];
	if(pCow->IsNew())
	{
		CNewInDocDlg::EditCow(nElement);
		return;
	}

	CEditCowDlg editCowDlg(pCow,this);
	editCowDlg.LockEANEdit();
	editCowDlg.DoModal();

}

void CEditInDocDlg::DeleteCow(int iElement)
{
	if(m_pEditDoc->HasInvoice())
	{
		AfxMessageBox(IDS_CANTADDDELCOWSBECOUSEINVOICE);
		return;
	}

	Cow *pCow = m_cows[iElement];
	if(pCow->IsNew())
	{	
		m_newCows.RemovePtr(pCow);
		delete pCow;
	}
	else
	{
		if(!pCow->CanDeleteThisCow())
		{
			AfxMessageBox(IDS_ERRCANTDELCOW);
			return;
		}
		m_delCows.Add(pCow);
	}

	m_cows.RemoveAt(iElement);
	CString passImgPath;
	if(m_passImgs.Lookup(pCow,passImgPath))
	{
		if(!passImgPath.IsEmpty())
			DeleteFile((LPCSTR)passImgPath);
		m_passImgs.RemoveKey(pCow);				
	}
		
}

void CEditInDocDlg::SetInDocForCows(Doc* pInDoc,CCowArray& cows)
{
	CCowArray::ArrayIterator it = cows.iterator();

	while(it.hasNext()) 
		(*(it++))->SetInDoc(pInDoc);
}


bool CEditInDocDlg::CommitDoc()
{
	CPassImgBase *imgBase = CPassImgBase::GetInstance();
	if(!m_pEditDoc->CanEditThisDoc())
	{
		AfxMessageBox(IDS_ERRCANTEDITDOC);
		return false;
	}


	if(m_pEditDoc->HasInvoice() && (m_delCows.GetSize() > 0 || m_newCows.GetSize() > 0))
	{
		AfxMessageBox(IDS_CANTADDDELCOWSBECOUSEINVOICE);
		return false;
	}
		
	CCowArray::ArrayIterator it = m_delCows.iterator();
	while(it.hasNext())
	{
		if(!(*(it++))->DeleteThisCow(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRDELETINGCOWFROMINDOC);
			SQLDB.Rollback();
			return false;
		}
	}

	UpdateDoc(m_pEditDoc);

	if(m_pEditDoc->HasInvoice() && m_pEditDoc->GetInvoice()->GetInvoiceDate() < m_pEditDoc->GetDocDate())
	{
		m_pEditDoc->RollbackChanges();
		AfxMessageBox(IDS_INVOICEDATEISBEFOREDOCDATE);
		return false;
	}
	
	SetInDocForCows(m_pEditDoc,m_newCows);
	it = m_newCows.iterator();
	while(it.hasNext())
	{		
		if(!((*(it++))->InsertThisCow(false)))
		{
			SetInDocForCows(NULL,m_newCows);
			m_pEditDoc->RollbackChanges();
			SQLDB.SQLMessageBox(IDS_ERRADDINGCOWTOINDOC);
			SQLDB.Rollback();			
			return false;	
		}
	}

	Cow* pCow;
	it = m_cows.iterator();
	while(it.hasNext())
	{
		pCow = *(it++);
		if(!pCow->IsNew() && pCow->IsUncommited()) 
		{
			if(!pCow->UpdateThisCow(false))
			{
				SetInDocForCows(NULL,m_newCows);	
				m_pEditDoc->RollbackChanges();
				SQLDB.SQLMessageBox(IDS_ERRUPDATEINGCOWININDOC);
				SQLDB.Rollback();				
				return false;	
			}
		}
	}
	
	if(!m_pEditDoc->UpdateThisDoc(false)) 
	{
		SetInDocForCows(NULL,m_newCows);	
		m_pEditDoc->RollbackChanges();
		SQLDB.SQLMessageBox(IDS_ERRUPDATEINGDOC);
		SQLDB.Rollback();		
		return false;
	}

	CString passImgPath;
	it = m_cows.iterator();
	while(it.hasNext())
	{
		pCow = *(it++);
		if(m_passImgs.Lookup(pCow,passImgPath) && !passImgPath.IsEmpty())
		{
			imgBase->StorePassImage(pCow,passImgPath,true);
			pCow->UpdateThisCow(false);
		}
	}
	
	it = m_delCows.iterator();
	while(it.hasNext())
		imgBase->DeletePassImg(*(it++));
		

	LockGrid(INCOWSGRID);		
	LockGrid(ALLCOWSGRID);

	ary_cows.RemoveArray(m_delCows);
	ary_incows.RemoveArray(m_delCows);
	m_pEditDoc->GetCows()->RemoveArray(m_delCows);
	
	m_delCows.FreeAndFlush();		
	
	ary_cows.Append(m_newCows);	    
	ary_incows.Append(m_newCows);
	m_pEditDoc->GetCows()->Append(m_newCows);
	
	SQLDB.Commit();		
	CommitChangesForCows(m_cows);
	m_pEditDoc->CommitChanges();

	UpdateGrid(INCOWSGRID);	
	UpdateGrid(ALLCOWSGRID);
	UpdateGrid(INDOCSGRID);
	return true;

}

void CEditInDocDlg::CancelDoc()
{
	RollbackChangesForCows(m_cows);

	m_newCows.FreeAndFlush();
}

CString CEditInDocDlg::GetDocDlgCaption()
{
	CString wndCaption;
	wndCaption.Format(IDS_INDOCEDIT,m_pEditDoc->GetId());
	return wndCaption;
}

void CEditInDocDlg::SetupBtns() 
{
	CString btnCaption;
	btnCaption.LoadString(IDS_BTNEDIT);
	m_OK.SetWindowText(btnCaption);
	
	btnCaption.LoadString(IDS_SCANBTNCAPTION);
	m_ScanBtn.SetButtonImage( IDB_SCANNER ,RGB(255,0,255));
	m_ScanBtn.SetWindowText(btnCaption);

	btnCaption.LoadString(IDS_ADDCOWBTNCAPTION);
	m_Add.SetButtonImage( IDB_ADDCOW );
	m_Add.SetWindowText(btnCaption);
}

CEditInDocDlg::~CEditInDocDlg()
{	

}
void CEditInDocDlg::AddCow()
{
	if(m_pEditDoc->HasInvoice())
	{
		AfxMessageBox(IDS_CANTADDDELCOWSBECOUSEINVOICE);
		return;
	}
	CNewInDocDlg::AddCow();
}

void CEditInDocDlg::AddNewCow(Cow *pCow)
{
	
	CNewInDocDlg::AddNewCow(pCow);
	m_newCows.Add(pCow);
}

void CEditInDocDlg::ChangeGrp4Cow(Cow *pCow,int grp)
{
	pCow->ChangeInGrp(grp);
}
