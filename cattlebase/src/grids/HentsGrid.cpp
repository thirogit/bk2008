#include "stdafx.h"
#include "HentsGrid.h"
#include "MainCfg.h"
#include "consts.h"
#include "global_arrays.h"
#include "../dlgs/EditHentDlg.h"
#include "../datatypes/HentComparator.h"
#include "../textproviders/HentTextProvider.h"
#include "CattleSQL.h"

GRIDHEADERS CHentsGrid::HentHeader[] =
{
	{IDS_HEADERNAME,HENTS_SORT_BY_NAME},
	{IDS_HEADERARIMRNO,HENTS_SORT_BY_ARIMRNO},
	{IDS_HEADERALIAS,HENTS_SORT_BY_ALIAS},
	{IDS_HEADERSTREET,HENTS_SORT_BY_STREET},
	{IDS_HEADERPOBOX,HENTS_SORT_BY_POBOX},
	{IDS_HEADERCITY,HENTS_SORT_BY_CITY},
	{IDS_HEADERZIP,HENTS_SORT_BY_ZIP},
	{IDS_HEADERPLATENO,HENTS_SORT_BY_PLATE},
	{IDS_HEADERHENTTYPE,HENTS_SORT_BY_HENTTYPE},
	{IDS_HEADERSYNCIT, HENTS_SORT_BY_SYNCIT},
	{IDS_HEADEREXTRAS, HENTDATA_MEMBER_EXTRAS},
	{0,0}
};

MY_IMPLEMENT_DYNCREATE(CHentsGrid,CGrid)

CHentsGrid::CHentsGrid() 
					: DataGrid<CHentArray,Hent>(ary_hents,HentHeader,CFGID_HGRID), 
					  m_textProvider(ary_hents)
{   
}

CHentsGrid::CHentsGrid(int pConfigItemId,CHentArray & HentAry) 
					: DataGrid<CHentArray,Hent>(HentAry,HentHeader,pConfigItemId),
					  m_textProvider(HentAry)
{
}

void CHentsGrid::GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData)
{
	m_textProvider.GetText(iItem,GetColID(nCol),pszText);	
}

void CHentsGrid::AddPopMenuItem(CPopMenuNodeList &popmenulist)
{
	popmenulist.Add(new CPopMenuNode(0,IDS_HEADERSYNCIT,IDR_HENTSYNCIT_YN_MENU));
}

bool CHentsGrid::HandleUpdateMessage(CCmdUI *pCmdUI)
{
	switch(pCmdUI->m_nID)
	{
		case IDM_GRID_HENTS_HENTSYNCIT_Y:
		{
			Hent *pHent = GetRowData(GetCurSel());
			pCmdUI->SetCheck(pHent->GetSyncIt());
			return true;
		}
		case IDM_GRID_HENTS_HENTSYNCIT_N:
		{
			Hent *pHent = GetRowData(GetCurSel());
			pCmdUI->SetCheck(!pHent->GetSyncIt());
			return true;
		}
		default:
			return false;
	}

	return false;
	
}

bool CHentsGrid::HandleMessage(UINT message)
{
	if(DataGrid<CHentArray,Hent>::HandleMessage(message))
		return true;

	switch(message)
	{
		
		case IDM_GRID_EDITROW:
		{
			Hent *pHent = GetRowData(GetCurSel());
			
			CEditHentDlg hentedit(pHent);
			hentedit.DoModal();			

			break;
		}

		case IDM_GRID_HENTS_HENTSYNCIT_Y:
			OnSyncItHentYes();
			break;
		case IDM_GRID_HENTS_HENTSYNCIT_N:
			OnSyncItHentNo();
			break;
		
		default:
			return false;
	}
	return true;
}

void CHentsGrid::OnSyncItHentYes()
{
	HentSyncIt(true);
}

void CHentsGrid::OnSyncItHentNo()
{
	HentSyncIt(false);
}

void CHentsGrid::HentSyncIt(bool bSyncIt)
{
	
	CListCtrl &pList = GetListCtrl();
	POSITION pos = pList.GetFirstSelectedItemPosition();
	int nSelItem;
	Hent *pHent = NULL;	
	while(pos)
	{
		nSelItem = pList.GetNextSelectedItem(pos);
		pHent = (Hent*)GetItemPtr(nSelItem);

		pHent->ChangeSyncIt(bSyncIt);
		if(!pHent->UpdateThisHent())
		{
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGHENT);
			pHent->RollbackChanges();
		}
		else
		{
			pHent->CommitChanges();
		}
		
	}	
	Invalidate(FALSE);

}



MY_IMPLEMENT_DYNCREATE(CCompanyHentsGrid,CHentsGrid)

CCompanyHentsGrid::CCompanyHentsGrid() : CHentsGrid(CFGID_COMPANYHGRID,ary_companies) 
{   
}

MY_IMPLEMENT_DYNCREATE(CFarmerHentsGrid,CHentsGrid)

CFarmerHentsGrid::CFarmerHentsGrid() : CHentsGrid(CFGID_FARMERHGRID,ary_farmers) 
{   
}

