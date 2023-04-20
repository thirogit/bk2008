#include "stdafx.h"
#include "HentCombo.h"
#include "global_arrays.h"
#include "../consts.h"
#include "../classes/TempIconLoader.h"
#include "../classes/match/HentMatcher.h"
#include "../classes/forallactions/SelectorAction.h"
#include "../datatypes/HentComparator.h"

BEGIN_MESSAGE_MAP(CHentCombo, COwnerDrawComboBox)
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


int CHentCombo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(COwnerDrawComboBox::OnCreate(lpCreateStruct) == -1) return -1;

	CreateImageList();
	
	return 0;
	
}

void CHentCombo::CreateImageList()
{
	m_imlHentIcons.DeleteImageList();
	m_imlHentIcons.Create(16,16,ILC_COLOR8|ILC_MASK,0,1);
	m_imlHentIcons.SetBkColor(CLR_NONE);
	m_imlHentIcons.Add(CTempIconLoader(IDI_HENTGREEN));
	m_imlHentIcons.Add(CTempIconLoader(IDI_HENTBLUE));
	m_imlHentIcons.Add(CTempIconLoader(IDI_HENTRED));	
//	SetImageList(&m_imlHentIcons);

}

void CHentCombo::PreSubclassWindow() 
{
	COwnerDrawComboBox::PreSubclassWindow();
	
	CreateImageList();
}


void CHentCombo::ReloadCombo(Hent *curSel/* = NULL*/)
{

	Hent *pHent = NULL;
	int iItem = -1,selItem = -1;
	ResetContent();

	HentType *pHentType = NULL;

	CHentArray hentItems;
	CHentMatcher hentMatcher;
	CSelectorAction<CHentArray,Hent,CHentMatcher> hentSelect(hentItems,&hentMatcher);



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
			hentItems.Append(ary_hents);
		}
	}

	hentItems.SortBy(HENTS_SORT_BY_COMBOITEM,true);
	
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

//	UpdateHorzExtent(16);
}

Hent* CHentCombo::GetSelectedHent()
{
	int iItemSel = GetCurSel();
	if(iItemSel >= 0)
		return (Hent*)GetItemData(iItemSel);

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

    if(RetCmd)
	{
		switch(RetCmd)
		{
			case IDM_HENTCOMBO_POP_ALLHENTS:
			{
				m_bOnlyIndividuals = false;
				m_bOnlyCompanies = false;
				ReloadCombo(GetSelectedHent());
				break;
			}
			case IDM_HENTCOMBO_POP_ADDHENT:
			{
				OnAddHent();
				break;
			}
			case IDM_HENTCOMBO_POP_FINDHENT:
			{
				OnFindHent();
				break;
			}
			case IDM_HENTCOMBO_POP_ONLYCOMPANIES:
			{
				if(m_bOnlyIndividuals)
					m_bOnlyIndividuals = false;
				m_bOnlyCompanies = true;
				ReloadCombo(GetSelectedHent());
				break;
			}
			case IDM_HENTCOMBO_POP_ONLYINDIVIDUALS:
			{
				if(m_bOnlyCompanies)
					m_bOnlyCompanies = false;
				m_bOnlyIndividuals = true;
				ReloadCombo(GetSelectedHent());
				break;
			}		
	
		}
	}
}

void CHentCombo::OnAddHent()
{
	CNewHentDlg newhent;
	if(newhent.DoModal() == IDOK)
	{
		Hent *pNewHent = newhent.GetNewHent();
		if(pNewHent)
		{
			m_bOnlyCompanies = false;
			m_bOnlyIndividuals = false;
			NotifyReload();
			ReloadCombo(pNewHent);			
		}
	}
}

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

void CHentCombo::OnFindHent()
{
	int iFoundIndex = -1;
	CString hentNo;
	
	CFastFindHent fastFindHent(this);
	if(fastFindHent.GetHentNo(hentNo))
	{	
		iFoundIndex = FindHentInCombo(hentNo);

		if(iFoundIndex < 0)
		{
			if(m_bOnlyCompanies || m_bOnlyIndividuals)
			{
				iFoundIndex = FindHentInArray(hentNo,ary_hents);
				if(iFoundIndex >= 0)
				{
					if(AfxMessageBox(IDS_HENTFOUNDNOTVISIBLE,MB_YESNO) == IDYES)
					{
						m_bOnlyCompanies = false;
						m_bOnlyIndividuals = false;
						ReloadCombo(ary_hents[iFoundIndex]);
						return;
					}
				}
			}
			if(AfxMessageBox(IDS_HENTNOTFOUNDINCOMBO,MB_YESNO) == IDYES)
			{
				Hent newHent(NULL_HENTID);
				newHent.SetFarmNo(hentNo);
				CNewHentDlg newHentDlg(&newHent);
				if(newHentDlg.DoModal() == IDOK)
				{
					Hent* pNewlyAddedHent = newHentDlg.GetNewHent();
					if((m_bOnlyCompanies && pNewlyAddedHent->GetHentType()->IsIndividual()) ||
						(m_bOnlyIndividuals && pNewlyAddedHent->GetHentType()->IsCompany()))
					{
						m_bOnlyCompanies = pNewlyAddedHent->GetHentType()->IsCompany();
						m_bOnlyIndividuals = pNewlyAddedHent->GetHentType()->IsIndividual();						
					}
					ReloadCombo(pNewlyAddedHent);

				}
			}
				
			
		}	
		else
		{
			SetCurSel(iFoundIndex);
		}

	}
}

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
	int hentCount = GetCount();
		
	for(int i  = 0;i < hentCount;i++)
	{
		Hent *pHent = (Hent*)GetItemData(i);
		if(pHent) // skip 'missing' item
		{				
			if(!farmNo.Compare(pHent->GetFarmNo()))
			{
				return i;
			}
		}		
	}
	return -1;
}

	//if (GetCount() == 0 || lpDrawItemStruct->itemID > GetCount()) return;

	//ImageComboItem* pItem = m_items.GetAt(lpDrawItemStruct->itemID);

	//
	//CDC dc;
	//BOOL bSelected = FALSE;
	//dc.Attach(lpDrawItemStruct->hDC);

	//// Save these value to restore them when done drawing.
	//COLORREF crOldTextColor = dc.GetTextColor();
	//COLORREF crOldBkColor = dc.GetBkColor();

	//// If this item is selected, set the background color and the text color to appropriate 
	//// values. Erase the rect by filling it with the background color.
	//if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
	//	(lpDrawItemStruct->itemState & ODS_SELECTED))
	//{
	//	dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	//	dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
	//	dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	//	bSelected = TRUE;
	//}
	//else
	//{
	//	dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
	//}

	//CRect rect(lpDrawItemStruct->rcItem);
	//rect.DeflateRect(1,0);

	//// If we use images, and there is data, and the index is valid:
	//if (m_pImageList && pItem->nImageIndex != -1)
	//{
	//	DrawIconEx(dc.GetSafeHdc(),rect.left,rect.top,
	//		m_pImageList->ExtractIcon(pItem->nImageIndex),0, 0, 0, NULL, DI_NORMAL);
	//}

	//// If we use images - move text to the right:
	//if (m_pImageList)
	//{
	//	IMAGEINFO sImageInfo;
	//	m_pImageList->GetImageInfo(0, &sImageInfo);
	//	rect.left += sImageInfo.rcImage.right;
	//}

	//CFont* pOldFont = NULL;
	//CFont boldFont;
	//if (pItem->bBold)
	//{
	//	CFont* curFont = dc.GetCurrentFont();
	//	LOGFONT lf;
	//	curFont->GetLogFont(&lf);
	//	lf.lfWeight = FW_BOLD;
	//	boldFont.CreateFontIndirect(&lf);
	//	pOldFont = dc.SelectObject(&boldFont);
	//}

	//// If the item has its own color, replace text color (exception - color is black, and
	//// the item is selected - then we leave the highlight text color)
	//if (!bSelected || (bSelected && pItem->crTextColor != RGB(0,0,0)))
	//{
	//	dc.SetTextColor(pItem->crTextColor);
	//}

	//
	//dc.DrawText(pItem->itemText, -1, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	//

	//if (pOldFont)
	//{
	//	dc.SelectObject(pOldFont);
	//	boldFont.DeleteObject();
	//}

	//// Reset the background color and the text color back to their original values.
	//dc.SetTextColor(crOldTextColor);
	//dc.SetBkColor(crOldBkColor);

	//dc.Detach();
