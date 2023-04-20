#include <stdafx.h>

#include "NewOutDocDlg.h"
#include "../Cow/EditCowDlg.h"
#include "PassSearchDlg.h"
#include "../PassportViewing\PassImgViewDlg.h"
#include "MainCfg.h"
#include "../mainfrm_ctrl.h"
#include "../global_arrays.h"
#include "../classes/match/CowMatcher.h"
#include "../classes/forallactions/SelectorAction.h"
#include "../CattleSQL.h"
#include "../accessor/CowAccessor.h"
#include "../datatypes/NullDoc.h"
#include "../dispacher/NewOutDocDispatcher.h"

#include <string>
#include <memory>


#define ID_FROMALLCOWSGRID (WM_APP+123)

BEGIN_MESSAGE_MAP(CNewOutDocDlg, CBaseDocDlg)
	
	ON_COMMAND(ID_FROMALLCOWSGRID,OnGetSelInAllCows)
	
	
END_MESSAGE_MAP()



CNewOutDocDlg::CNewOutDocDlg(CCowArray &cows) : 
						m_OutCowList(&m_cows,CFGID_OLIST),
						CBaseDocDlg(&m_OutCowList,Reason::REASON_OUT,&m_OutAccessor),
						m_Select(IDS_FROMALLCOWSGRID,ID_FROMALLCOWSGRID)
{

	Cow* pCow;
	CCowArray::ArrayIterator cowIt = cows.iterator();
	while(cowIt.hasNext())
	{		
		pCow = *(cowIt++);
		if(!pCow->IsOut())
			m_cows.Add(pCow);
	}	

}


CNewOutDocDlg::CNewOutDocDlg() : 
						m_OutCowList(&m_cows,CFGID_OLIST),
						CBaseDocDlg(&m_OutCowList,Reason::REASON_OUT,&m_OutAccessor),
						m_Select(IDS_FROMALLCOWSGRID,ID_FROMALLCOWSGRID)			
{	
}


CNewOutDocDlg::~CNewOutDocDlg()
{

}

CString CNewOutDocDlg::GetDocDlgCaption()
{
	CString wndCaption;
	wndCaption.LoadString(IDS_OUTDOCNEW);
	return wndCaption;
}

void CNewOutDocDlg::SetupBtns() 
{
	CString btnCaption;
	btnCaption.LoadString(IDS_BTNINSERT);
	m_OK.SetWindowText(btnCaption);	
	btnCaption.LoadString(IDS_SELECT);
	m_Select.SetButtonImage( IDB_MAGICSELECT ,RGB(255,0,255));
	m_Select.SetWindowText(btnCaption);
	GetDlgItem(IDC_GETCOWS)->EnableWindow(FALSE);
}

void CNewOutDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDocDlg::DoDataExchange(pDX);	
	DDX_Control(pDX,IDC_ADDCOWS,m_Select);		
}



BOOL  CNewOutDocDlg::OnInitDialog()
{
	CBaseDocDlg::OnInitDialog();
	int reasonid = AppCfg().GetNum(CFG_PRESELOUTREASON);
	m_reasonCombo.ReloadCombo(reasonid);
	m_hentCombo.ReloadCombo();	
	UpdateCowCount();
	UpdateSummary();
	m_loadStartTm.SetTime(Time::now());
	return TRUE;
}

void CNewOutDocDlg::AddSelectedCows(bool bFromAllCows)
{
	m_cowlist->SetRedraw(FALSE);
	CCowArray addCowsArray;
	CCowArray selectedCows;
	
	if(bFromAllCows)
	{
		GetSelCows(selectedCows);
	}
	else
	{
		GetSelInCows(selectedCows);
	}

	CCowDataMatcher cowMatcher;
	cowMatcher.FindNotSold();
	CSelectorAction<CCowArray,Cow,CCowDataMatcher> notSoldCows(addCowsArray,&cowMatcher);
	selectedCows.ForAllAction(notSoldCows);
	addCowsArray.RemoveArray(m_cows);

	int activeGrp = GetActiveGroup();				

	CCowArray::ArrayIterator it = addCowsArray.iterator();
	while(it.hasNext())
	{
		ChangeGrp4Cow(*(it++),activeGrp); 		
	}
	m_cows.Append(addCowsArray);
	UpdateSummary();
	m_cowlist->SetRedraw(TRUE);
	UpdateCowCount();
}
 
void CNewOutDocDlg::AddCow()
{
	AddSelectedCows(false);
}

void CNewOutDocDlg::OnGetSelInAllCows()
{
	AddSelectedCows(true);	
}

bool CNewOutDocDlg::CommitDoc()
{
	NullDoc *pNullDoc = CreateNewDoc();
		
	OutDoc* pNewDoc = new OutDoc(NULL_DOCID);
	auto_ptr<OutDoc> deleteNewDoc(pNewDoc);
	pNewDoc->CopyFrom(*pNullDoc);
	delete pNullDoc;

	pNewDoc->GetCows()->Copy(m_cows);

	if(!pNewDoc->InsertThisDoc(false))
	{
		SQLDB.SQLMessageBox(IDS_ERRINSERTINGDOC);
		SQLDB.Rollback();		
		return false;
	}

	Cow *pCow = NULL;
	CCowArray::ArrayIterator it = pNewDoc->GetCows()->iterator();
	while(it.hasNext())
	{
		pCow = *(it++);
		pCow->AssignOutDoc(pNewDoc);
		if(!pCow->UpdateThisCow(false))
		{
			SQLDB.SQLMessageBox(IDS_ERRASSIGNINGCOWS);
			SQLDB.Rollback();
			while(!it.isFirst()) (*(--it))->DeleteOutDoc();
			return false;	
		}
			
	}
	
	LockGrid(INCOWSGRID);

	deleteNewDoc.release();


	NewOutDocDispatcher::Dispatch(pNewDoc);
	
	
	SQLDB.Commit();	
	CommitChangesForCows(m_cows);

	UpdateGrid(INCOWSGRID);
	UpdateGrid(OUTDOCSGRID);
	
	return true;
}



void CNewOutDocDlg::DeleteCow(int iElement)
{
	Cow *pCow = m_cows[iElement];
	pCow->RollbackChanges();
	m_cows.RemoveAt(iElement);
	UpdateSummary();
}

void CNewOutDocDlg::EditCow(int iElement)
{
	Cow *pCow = m_cows[iElement];
	CEditCowDlg editCowDlg(pCow,this);
	editCowDlg.LockEANEdit();
	NullInt currentOutGroup = pCow->GetOutGroup();
	
	if(editCowDlg.DoModal() == IDOK) 
	{	
		if(!pCow->CanEditThisCow())
		{
			pCow->RollbackChanges();
			pCow->ChangeOutGrp(currentOutGroup.ToInt(NULL_INOUT_GRP));
			AfxMessageBox(IDS_ERRCANTEDITCOW);	
		}
	}
}

void CNewOutDocDlg::FindPass()
{	
	CowOutAccessor outAccessor;
	CPassSearchDlg passSearch(m_cows,&outAccessor,this);
	passSearch.DoModal();
}
void CNewOutDocDlg::ShowPass(int nItem)
{	
	CPassImgViewDlg passView(m_cows,nItem);
	passView.DoModal();		
}

void CNewOutDocDlg::ChangeGrp4Cow(Cow *pCow,int grp)
{
	pCow->ChangeOutGrp(grp);			
}

void CNewOutDocDlg::CancelDoc()
{	
	RollbackChangesForCows(m_cows);
}

void CNewOutDocDlg::GetCows()
{
	ASSERT(0);
}