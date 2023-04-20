#include "stdafx.h"
#include "PassSearchDlg.h"
#include "../MainCfg.h"

GroupComparator::GroupComparator(CowAccessor* pGroupAccessor,bool bAsc) : CCowArray::SortArrayComparator(bAsc),m_pGroupAccessor(pGroupAccessor)
{

}

COMPARE_RESULT GroupComparator::Compare(Cow *DataLeft,Cow *DataRight)
{
	return CompareNullable<NullInt>(m_pGroupAccessor->GetGroup(DataLeft),m_pGroupAccessor->GetGroup(DataRight));
}

COMPARE_RESULT GroupComparator::CompareBy(UINT dataMember,Cow* DataLeft,Cow* DataRight)
{
	return COMPARE_EQUAL;
}



#define COWDATA_MEMBER_GROUP (COWDATA_MEMBER_MAX+1001)

const GRIDHEADERS CSearchPassListCtrl::PassSearchCowHeader[]  =
{	
	{IDS_HEADERCOWNO,COWS_SORT_BY_EAN},
	{IDS_HEADERPASSNO,COWS_SORT_BY_PASSNO},
	{IDS_HEADEROUTGRP,COWDATA_MEMBER_GROUP},
	{0,0}
};



CSearchPassListCtrl::CSearchPassListCtrl(CCowArray &cowArray,CFoundPassMarkMap &markMap,CowAccessor* pGroupAccessor) :
CCowListCtrl(&cowArray,PassSearchCowHeader,CFGID_COWPASSSEARCHLIST),m_markMap(markMap),m_pGroupAccessor(pGroupAccessor)
{

}


void CSearchPassListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText) 
{
	if(GetColID(nSubItem) == COWDATA_MEMBER_GROUP)
	{
		CString sTemp = m_pGroupAccessor->GetGroup((*m_cowArray)[nItem]).ToString();
		strcpy(pszItemText,(LPCSTR)sTemp);
	}
	else
	{
		CCowListCtrl::GetItemDispText(nItem,nSubItem,pszItemText);
	}
	
}


bool CSearchPassListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	bool bMark = false;
	Cow *cowdata = (*m_cowArray)[nItem];
    m_markMap.Lookup(cowdata,bMark);
	if(bMark)
	{
		*cellBkg = RGB(0,128,255);
		return true;
	}
	return false;
}

void CSearchPassListCtrl::SortByCol(int Column,bool bAsc)
{
	if(GetColID(Column) == COWDATA_MEMBER_GROUP)
	{
		m_cowArray->Sort(GroupComparator(m_pGroupAccessor,bAsc));
	}
	else
	{
		CCowListCtrl::SortByCol(Column,bAsc);
	}
}


IMPLEMENT_DYNAMIC(CPassSearchDlg, CDialog)
CPassSearchDlg::CPassSearchDlg(CCowArray &searchArray,CowAccessor* pGroupAccessor,CWnd* pParent /*=NULL*/)
	: CDialog(CPassSearchDlg::IDD, pParent),m_searchArray(searchArray),
			m_cowlist(m_searchArray,m_markMap,pGroupAccessor),m_pGroupAccessor(pGroupAccessor)
{
	int i;
	for(i = 0,m_cowsLeft = searchArray.GetSize();i<m_cowsLeft;i++)
		m_markMap.SetAt(searchArray[i],false);

	m_bActiveGroupCheck = FALSE;
	
}

CPassSearchDlg::~CPassSearchDlg()
{
}

void CPassSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_COWLIST,m_cowlist);
	DDX_Control(pDX,IDC_LASTSEARCHSTATUS,m_lastSearchStatus);
	DDX_Control(pDX,IDC_BARCODE,m_barcode);
	DDX_Control(pDX,IDC_GRPCOMBO,m_grpCombo);
	DDX_Check(pDX,IDC_ACTIVEGRPCHECK,m_bActiveGroupCheck);
}


BEGIN_MESSAGE_MAP(CPassSearchDlg, CDialog)
	ON_BN_CLICKED(IDC_ACTIVEGRPCHECK,OnActiveGrpCheck)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

NullInt CPassSearchDlg::GetCowGroup(Cow* pCow)
{
	return m_pGroupAccessor->GetGroup(pCow);
}

void CPassSearchDlg::FindPassport(const CString& sBarcode)
{
	PASSSEARCH_RESULT lastResult = NOTFOUND;
	bool bMarked;
	
	for(int i = 0,s = m_searchArray.GetSize();i<s;i++)
	{
		Cow *pCow = m_searchArray[i];
		if(pCow->GetEAN().CompareNoCase(sBarcode) == 0)
		{

			if(m_bActiveGroupCheck && GetCowGroup(pCow).ToInt(NULL_INOUT_GRP) != m_grpCombo.GetSelOutGroup())
			{
				lastResult = NOTINGRP;
				MessageBeep(MB_ICONASTERISK);
				break;
			}

			if(!m_markMap.Lookup(pCow,bMarked)) bMarked = false;

			if(m_cowsLeft > 0 && !bMarked)
				m_cowsLeft --;
			lastResult = FOUND;
			m_markMap.SetAt(pCow,true);		
			m_cowlist.Invalidate();
			MessageBeep((MB_ICONEXCLAMATION));
		}
	}

	UpdateLastResult(lastResult);
	UpdateCowsLeft();
		
}

BOOL CPassSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	m_lastSearchStatus.SetFont((LPCSTR)NULL,-25);
	m_barcode.SetFont((LPCSTR)NULL,-25);
	
	m_grpCombo.InitCombo();
	m_grpCombo.EnableWindow(m_bActiveGroupCheck);
	
	UpdateCowsLeft();
	UpdateLastResult(WAITING);

	CBarcodeScannerDriver::AddReceiver(this);

	return TRUE;
}

void CPassSearchDlg::UpdateCowsLeft()
{
	CString cowsLeftStr;
	cowsLeftStr.Format(IDS_COWSLEFT,m_cowsLeft,m_cowsLeft);
	GetDlgItem(IDC_COWSLEFT)->SetWindowText(cowsLeftStr);
}


void CPassSearchDlg::UpdateLastResult(PASSSEARCH_RESULT lastResult)
{

	CString statusStr;
	UINT nID;
	COLORREF clr;
	

	switch(lastResult)
	{
	case FOUND:
		nID = IDS_PASSSEARCHSTATFOUND;
		clr = RGB(0,255,0);
	break;
	case NOTFOUND:
		nID = IDS_PASSSEARCHSTATNOTFOUND;
		clr = RGB(255,0,0);
	break;
	case WAITING:
		nID = IDS_PASSSEARCHSTATWAIT;
		clr = RGB(255,255,255);
	break;
	case NOTINGRP:
		nID = IDS_PASSSEARCHSTATNOTINGRP;
		clr = RGB(0,0,255);
	break;
	}

	statusStr.LoadString(nID);
	m_lastSearchStatus.SetWindowText(statusStr);
	m_lastSearchStatus.SetBackgroundColor(clr,TRUE);

}
void CPassSearchDlg::OnActiveGrpCheck()
{
	UpdateData();
	m_grpCombo.EnableWindow(m_bActiveGroupCheck);

}

void CPassSearchDlg::OnBarcode(const CString& sBarcode)
{
	m_barcode.SetWindowText(sBarcode);	
	FindPassport(sBarcode);
}

void CPassSearchDlg::OnDestroy()
{
	CBarcodeScannerDriver::RemoveReceiver(this);
}