#include "stdafx.h"
#include "HentCombo.h"
#include "global_arrays.h"
//#include "../consts.h"
#include "../classes/TempIconLoader.h"
#include "../classes/match/HentMatcher.h"
#include "../classes/forallactions/SelectorAction.h"
#include "../classes/forallactions/IndexOfAction.h"
#include "../datatypes/HentComparator.h"

/////////////////////////////////////////////////////////////////////////////////////////
int CHentCombo::HentComboItemDrawer::Draw(CDC* pDC, const CString& strText, LPRECT lpRect, UINT nFormat/* = 0*/)
{
	return 18;
}
/////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHentCombo, CVComboBox)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

PtrFlushArray<CHentCombo> CHentCombo::m_hentCombos;

CHentCombo::CHentCombo(bool bIclMissingItem/* = false*/) : m_bIclMissingItem(bIclMissingItem),
		m_bOnlyCompanies(false),m_bOnlyIndividuals(false)
{
	m_hentCombos.Add(this);
	m_hentComboMenu.LoadMenu(IDR_HENTCOMBOMENU);
}

CHentCombo::~CHentCombo()
{
	VERIFY(m_hentCombos.RemovePtr(this));
}

CString CHentCombo::GetItemText(UINT nItem)
{
	return m_hents[nItem]->GetComboItemText();
}


int CHentCombo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CVComboBox::OnCreate(lpCreateStruct) == -1) return -1;

	CreateImageList();
	
	return 0;
	
}

void CHentCombo::CreateImageList()
{
	SetDroppedVisibleItemCount(20);
	m_imlHentIcons.DeleteImageList();
	m_imlHentIcons.Create(16,16,ILC_COLOR8|ILC_MASK,0,1);
	m_imlHentIcons.SetBkColor(CLR_NONE);
	m_imlHentIcons.Add(CTempIconLoader(IDI_HENTGREEN));
	m_imlHentIcons.Add(CTempIconLoader(IDI_HENTBLUE));
	m_imlHentIcons.Add(CTempIconLoader(IDI_HENTRED));	
	//SetImageList(&m_imlHentIcons);

}

void CHentCombo::PreSubclassWindow() 
{
	CVComboBox::PreSubclassWindow();
	
	CreateImageList();
}


void CHentCombo::ReloadCombo(Hent *curSel/* = NULL*/)
{

	Hent *pHent = NULL;
	int iItem = -1,selItem = -1;
	//ResetContent();
	m_hents.RemoveAll();

	HentType *pHentType = NULL;

	//CHentArray hentItems;
	CHentMatcher hentMatcher;
	CSelectorAction<CHentArray,Hent,CHentMatcher> hentSelect(m_hents,&hentMatcher);

	if(m_bOnlyIndividuals)
	{
		hentMatcher.FindHentType(ary_htypes.GetHentTypeByHentTypeId(HTYPE_INDIVIDUAL));
		ary_hents.ForAllAction(hentSelect);		
	}		
	else
	{
		if(m_bOnlyCompanies)
		{
			hentMatcher.FindHentType(ary_htypes.GetHentTypeByHentTypeId(HTYPE_COMPANY));
			ary_hents.ForAllAction(hentSelect);				
		}
		else
		{
			for(int i = 0;i < 15;i++)
				m_hents.Add(ary_hents.GetAt(i));
			//m_hents.Append(ary_hents);
		}
	}

	m_hents.SortBy(HENTS_SORT_BY_COMBOITEM,true);

	if(curSel != NULL)
	{
		CIndexOfAction<Hent> indexOfAction(curSel);
		m_hents.ForAllAction(indexOfAction);
		int iCurSel = indexOfAction.GetIndex();
		if(iCurSel >= 0)
			SetCurSel(iCurSel);
	}

	
	/*for(int i = 0;i < hentItems.GetSize();i++)
	{
		pHent = hentItems[i];
		switch(pHent->GetHentType()->henttypeid)
		{
			case HTYPE_COMPANY:
				if(m_bOnlyIndividuals) continue;
				iItem = AddItem((LPCSTR)pHent->GetComboItemText(),2);
			break;
			case HTYPE_INDIVIDUAL:
				if(m_bOnlyCompanies) continue;
				iItem = AddItem((LPCSTR)pHent->GetComboItemText(),0);
				break;
			default:
				ASSERT(0);
		}
		SetItemData(iItem,(DWORD_PTR)pHent);
		if(curSel != NULL && pHent == curSel)
			selItem = iItem;

		if(iItem < selItem)
			selItem++;

	}
	if(m_bIclMissingItem)
	{
		iItem = AddItem(szMissing,1);
		SetItemData(iItem,(DWORD_PTR)NULL);	
		if(selItem >= 0)
		{
			if(iItem < selItem)	selItem++;
		}
		else
			selItem = iItem;
	}
	SetCurSel(selItem);*/

	SetItemCount(m_hents.GetCount());
//	UpdateHorzExtent(16);
}

Hent* CHentCombo::GetSelectedHent()
{
	int iItemSel = GetCurSel();
	if(iItemSel >= 0)
		return m_hents[iItemSel];

	return NULL;
}


void CHentCombo::NotifyReload()
{
	CHentCombo *pHentCombo = NULL;
	int iItem = -1;

	for(int i = 0; i < m_hentCombos.GetSize();i++)
	{
		pHentCombo = m_hentCombos[i];
		iItem = pHentCombo->GetCurSel();
		if(iItem >= 0)
			pHentCombo->ReloadCombo((Hent*)pHentCombo->GetItemData(iItem));

	}
}


void CHentCombo::OnContextMenu(CWnd* pWnd, CPoint point)
{

	SetFocus();
    
	
	m_hentComboMenu.CheckMenuItem(IDM_HENTCOMBO_POP_ONLYCOMPANIES,MF_BYCOMMAND | (m_bOnlyCompanies ? MF_CHECKED : MF_UNCHECKED));
	m_hentComboMenu.CheckMenuItem(IDM_HENTCOMBO_POP_ONLYINDIVIDUALS,MF_BYCOMMAND | (m_bOnlyIndividuals ? MF_CHECKED : MF_UNCHECKED));
	m_hentComboMenu.CheckMenuItem(IDM_HENTCOMBO_POP_ALLHENTS,MF_BYCOMMAND | (!(m_bOnlyCompanies || m_bOnlyIndividuals)) ? MF_CHECKED :  MF_UNCHECKED);

	CMenu* pSubMenu = m_hentComboMenu.GetSubMenu(0);
	
    int RetCmd = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON |  TPM_RETURNCMD, 
		point.x, point.y, this);

 //   if(RetCmd)
	//{
	//	switch(RetCmd)
	//	{
	//		case IDM_HENTCOMBO_POP_ALLHENTS:
	//		{
	//			m_bOnlyIndividuals = false;
	//			m_bOnlyCompanies = false;
	//			ReloadCombo(GetSelItemData());
	//			break;
	//		}
	//		case IDM_HENTCOMBO_POP_ADDHENT:
	//		{
	//			OnAddHent();
	//			break;
	//		}
	//		case IDM_HENTCOMBO_POP_FINDHENT:
	//		{
	//			OnFindHent();
	//			break;
	//		}
	//		case IDM_HENTCOMBO_POP_ONLYCOMPANIES:
	//		{
	//			if(m_bOnlyIndividuals)
	//				m_bOnlyIndividuals = false;
	//			m_bOnlyCompanies = true;
	//			ReloadCombo(GetSelItemData());
	//			break;
	//		}
	//		case IDM_HENTCOMBO_POP_ONLYINDIVIDUALS:
	//		{
	//			if(m_bOnlyCompanies)
	//				m_bOnlyCompanies = false;
	//			m_bOnlyIndividuals = true;
	//			ReloadCombo(GetSelItemData());
	//			break;
	//		}		
	//
	//	}
	//}
}

//void CHentCombo::OnAddHent()
//{
//	CNewHentDlg newhent;
//	if(newhent.DoModal() == IDOK)
//	{
//		Hent *pNewHent = newhent.GetNewHent();
//		if(pNewHent)
//		{
//			m_bOnlyCompanies = false;
//			m_bOnlyIndividuals = false;
//			NotifyReload();
//			ReloadCombo(pNewHent);			
//		}
//	}
//}
//
bool CHentCombo::GetShowOnlyCompanies()
{
	return m_bOnlyCompanies;
}
bool CHentCombo::GetShowOnlyIndividuals()
{
	return m_bOnlyIndividuals;
}

void CHentCombo::SetShowOnlyCompanies(bool bOnlyCompanies)
{
	m_bOnlyCompanies = bOnlyCompanies;
}
void CHentCombo::SetShowOnlyIndividuals(bool bOnlyIndividuals)
{
	m_bOnlyIndividuals = bOnlyIndividuals;
}

bool CHentCombo::SelectHent(const CString& farmNo)
{
	int	iFoundIndex = FindHentInCombo(farmNo);

	if(iFoundIndex < 0)
	{
		if(m_bOnlyCompanies || m_bOnlyIndividuals)
		{
			iFoundIndex = FindHentInArray(farmNo,ary_hents);
			if(iFoundIndex >= 0)
			{
				m_bOnlyCompanies = false;
				m_bOnlyIndividuals = false;
				ReloadCombo(ary_hents[iFoundIndex]);
				return true;
				
			}
		}
		return false;
	}
	SetCurSel(iFoundIndex);
	return true;
}
//
//void CHentCombo::OnFindHent()
//{
//	int iFoundIndex = -1;
//	CString hentNo;
//	
//	CFastFindHent fastFindHent(this);
//	if(fastFindHent.GetHentNo(hentNo))
//	{	
//		iFoundIndex = FindHentInCombo(hentNo);
//
//		if(iFoundIndex < 0)
//		{
//			if(m_bOnlyCompanies || m_bOnlyIndividuals)
//			{
//				iFoundIndex = FindHentInArray(hentNo,ary_hents);
//				if(iFoundIndex >= 0)
//				{
//					if(AfxMessageBox(IDS_HENTFOUNDNOTVISIBLE,MB_YESNO) == IDYES)
//					{
//						m_bOnlyCompanies = false;
//						m_bOnlyIndividuals = false;
//						ReloadCombo(ary_hents[iFoundIndex]);
//						return;
//					}
//				}
//			}
//			if(AfxMessageBox(IDS_HENTNOTFOUNDINCOMBO,MB_YESNO) == IDYES)
//			{
//				Hent newHent(NULL_HENTID);
//				newHent.SetFarmNo(hentNo);
//				CNewHentDlg newHentDlg(&newHent);
//				if(newHentDlg.DoModal() == IDOK)
//				{
//					Hent* pNewlyAddedHent = newHentDlg.GetNewHent();
//					if((m_bOnlyCompanies && pNewlyAddedHent->GetHentType()->IsIndividual()) ||
//						(m_bOnlyIndividuals && pNewlyAddedHent->GetHentType()->IsCompany()))
//					{
//						m_bOnlyCompanies = pNewlyAddedHent->GetHentType()->IsCompany();
//						m_bOnlyIndividuals = pNewlyAddedHent->GetHentType()->IsIndividual();						
//					}
//					ReloadCombo(pNewlyAddedHent);
//
//				}
//			}
//				
//			
//		}	
//		else
//		{
//			SetCurSel(iFoundIndex);
//		}
//
//	}
//}
//
int CHentCombo::FindHentInArray(const CString& farmNo,CHentArray &hentArray)
{
	int index = 0;
	CHentArray::ArrayIterator hentIt = hentArray.iterator();
	while(hentIt.hasNext())
	{
		Hent *pHent = *(hentIt++);
		if(!farmNo.Compare(pHent->GetFarmNo()))
		{
			return index;
		}
		index++;					
	}
	return -1;
}
int CHentCombo::FindHentInCombo(const CString& farmNo)
{
	return FindHentInArray(farmNo,m_hents);
}