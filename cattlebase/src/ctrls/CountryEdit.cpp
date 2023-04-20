#include "stdafx.h"
#include "CountryEdit.h"
#include "../consts.h"
#include "global_arrays.h"
#include "../classes/TempStringLoader.h"


/////////////////////////////////////////////////////////////////////////////////////////

AscendingCountryComparator::AscendingCountryComparator() : CCountryCodeArray::SortArrayComparator(false)
{
}

COMPARE_RESULT AscendingCountryComparator::Compare(Country *DataLeft,Country *DataRight)
{
	return DataComparator<Country>::PolishStringCompare(DataLeft->GetCountryName(),DataRight->GetCountryName());
}

COMPARE_RESULT AscendingCountryComparator::CompareBy(UINT dataMember,Country* DataLeft,Country* DataRight)
{
	ASSERT(FALSE);
	return COMPARE_EQUAL;
}
/////////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CCountryEdit, CTipEdit)
    ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


CCountryEdit::CCountryEdit(LPCTSTR pszMask/*= ""*/) : CTipEdit(pszMask),m_checkedCountry(ISO_POL_NUMBER)
{
}

void CCountryEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	SetFocus();
    CMenu menu,countrymenu;
   // menu.CreatePopupMenu();
	countrymenu.CreatePopupMenu();
	

	
	DWORD flags;

	menu.LoadMenu(IDR_COUNTRYEDITMENU);

	CCountryCodeArray countries;
	countries.Append(ary_countries);
	countries.Sort(AscendingCountryComparator());

	CString sMenuItemText;
	Country *pCountry = NULL;
	CCountryCodeArray::ArrayIterator countryIt = countries.iterator();
	while(countryIt.hasNext())
	{
		flags = 0;
		pCountry = (*(countryIt++));
		if(pCountry->GetCountryISONumber() == m_checkedCountry) flags = MF_CHECKED;			
		sMenuItemText.Format("%s, %s",(LPCSTR)pCountry->GetCountryName(),(LPCSTR)pCountry->GetCountryCode2A());
		countrymenu.InsertMenu(0, MF_BYPOSITION | flags,pCountry->GetCountryISONumber() ,(LPCSTR)sMenuItemText);
		
	}

	CString menuItemOtherStr = CTempStringLoader(IDS_OTHERCOUNTRYMENUITEMTEXT),
		    menuItemCountryCodeStr = CTempStringLoader(IDS_COUNTRYCODEMENUITEMTEXT);


	countrymenu.InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR);
	flags = (m_checkedCountry ==  OTHER_COUNTRY_ISO_NUMBER ? MF_CHECKED : 0);
	countrymenu.InsertMenu(0, MF_BYPOSITION | flags ,OTHER_COUNTRY_ISO_NUMBER ,(LPCSTR)menuItemOtherStr);
			
	CMenu* pSubMenu = menu.GetSubMenu(0);
	pSubMenu->InsertMenu(-1,MF_BYPOSITION | MF_POPUP, (UINT)countrymenu.GetSafeHmenu(), (LPCSTR)menuItemCountryCodeStr);

	ExpandContextMenu(pSubMenu);
	
    int RetCmd = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON |  TPM_RETURNCMD, point.x, point.y, this);

    if(RetCmd)
	{
		if(!OnEditCommand(RetCmd))
		{
			if(!OnExpandedCommand(RetCmd))
			{
				m_checkedCountry = RetCmd;
				if(RetCmd == OTHER_COUNTRY_ISO_NUMBER)
					OnCountryChange((Country*)-1);
				else
					OnCountryChange(ary_countries.GetCountryByCountryNum(m_checkedCountry));
			}
		}
	}
}
/*
void CCountryEdit::OnUpdateEditCommand(CCmdUI* pCmdUI)
{
		UINT nID = pCmdUI->m_nID;
		switch (nID) 
		{			
		case ID_EDITPOPUP_CUT:
		case ID_EDITPOPUP_COPY:
//		case ID_EDITPOPUP_CLEAR:
			int beg,end;
			GetSel(beg,end);
			pCmdUI->Enable(beg<end);
		break;
		case ID_EDITPOPUP_UNDO:
			pCmdUI->Enable(CanUndo());
		break;
		case ID_EDITPOPUP_PASTE:
			pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
		break;
		case ID_EDITPOPUP_SELECTALL:
			pCmdUI->Enable(GetWindowTextLength()>0);
		break;
		}
	
	
}*/

BOOL CCountryEdit::OnEditCommand(UINT nID)
{
		switch (nID) {
		case IDM_EDITPOPUP_UNDO:
			Undo();
			return TRUE;
		case IDM_EDITPOPUP_CUT:
			Cut();
			return TRUE;
		case IDM_EDITPOPUP_COPY:
			Copy();
			return TRUE;
		case IDM_EDITPOPUP_PASTE:
			Paste();
			return TRUE;
		/*case ID_EDIT_CLEAR:
			Clear();*/
			return TRUE;
		case IDM_EDITPOPUP_SELECTALL:
			SetSel(0,-1);
			return TRUE;
		}
		return FALSE;
}


Country* CCountryEdit::GetCountry(UINT countryISONumber)
{
	return ary_countries.GetCountryByCountryNum(countryISONumber);
}

Country* CCountryEdit::GetCountry(const CString& countryCode2a)
{
	return ary_countries.GetCountryByCountryCode(countryCode2a);
}