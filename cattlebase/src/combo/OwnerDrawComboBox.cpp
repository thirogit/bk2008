#include "stdafx.h"
#include "OwnerDrawComboBox.h"
#include "../exceptions/ArrayBoundsException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_SEARCH_BUFFER 20

COwnerDrawComboBox::COwnerDrawComboBox(): m_bLastSearch(false),m_pItemDrawer(NULL)
{
}

COwnerDrawComboBox::~COwnerDrawComboBox()
{

}


BEGIN_MESSAGE_MAP(COwnerDrawComboBox, CComboBox)
	ON_WM_DELETEITEM()
	ON_WM_CREATE()
	ON_WM_CHARTOITEM()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS,OnCBKillFocus)
END_MESSAGE_MAP()

int COwnerDrawComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CComboBox::OnCreate(lpCreateStruct) == -1) return -1;

	ASSERT(lpCreateStruct->style & CBS_OWNERDRAWFIXED);
	ASSERT(!(lpCreateStruct->style & CBS_HASSTRINGS));
	ASSERT(lpCreateStruct->style & CBS_DROPDOWNLIST);
	ASSERT(!(lpCreateStruct->style & CBS_SORT));
	CreateToolTip(GetParent());
	return 0;	
}
void COwnerDrawComboBox::PreSubclassWindow() 
{
	CComboBox::PreSubclassWindow();	

	ASSERT(GetStyle() & CBS_OWNERDRAWFIXED);
	ASSERT(!(GetStyle() & CBS_HASSTRINGS));
	ASSERT(GetStyle() & CBS_DROPDOWNLIST);
	ASSERT(!(GetStyle() & CBS_SORT));

	CreateToolTip(GetParent());
}


void COwnerDrawComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(m_pItemDrawer)
		m_pItemDrawer->Draw(lpDrawItemStruct);
}

void COwnerDrawComboBox::MeasureItem(LPMEASUREITEMSTRUCT)
{
}

int COwnerDrawComboBox::CompareItem(LPCOMPAREITEMSTRUCT)
{
	return 0; 
}



 HBRUSH COwnerDrawComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  if (nCtlColor == CTLCOLOR_LISTBOX)
  {
     //ListBox control
     if (m_listbox.GetSafeHwnd() == NULL)
        m_listbox.SubclassWindow(pWnd->GetSafeHwnd());
  }
  HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
  return hbr;
}

void COwnerDrawComboBox::OnDestroy()
{
  if (m_listbox.GetSafeHwnd() != NULL)
     m_listbox.UnsubclassWindow();
  CComboBox::OnDestroy();
}

int COwnerDrawComboBox::OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex) 
{
	char Char = (char)nChar;
	
	if(Char ==  '\r') return -2;

//	TRACE("OnCharToItem = %c,%d,isgrpah = %d,isspace = %d\n",Char,Char,isgraph(Char),Char == ' ');
	int len = m_itemSearchBuffer.GetLength();
	if((isgraph(Char) || Char == ' ') && len < MAX_SEARCH_BUFFER && ((m_bLastSearch && len > 0) || len == 0))
		m_itemSearchBuffer.AppendChar(Char);

	int foundItem = SearchForItem(m_itemSearchBuffer);
	
	m_bLastSearch = foundItem >= 0; 

	UpdateToolTip();
	
	return foundItem;	
}

void COwnerDrawComboBox::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_BACK)
	{
		if(m_itemSearchBuffer.GetLength() > 0)
			m_itemSearchBuffer.Delete(m_itemSearchBuffer.GetLength()-1);
				
		int foundItem = SearchForItem(m_itemSearchBuffer);
		if(foundItem >= 0)
		{
			SetCurSel(foundItem);
			m_bLastSearch = true;
		}
		else
			m_bLastSearch = false;
		
		UpdateToolTip();
	}
	CComboBox::OnKeyDown (nChar, nRepCnt, nFlags);
	
}

void COwnerDrawComboBox::UpdateToolTip()
{
	CString tipText("Szukaj: ");
	tipText += m_itemSearchBuffer;
	SetToolTipText(tipText,!m_bLastSearch);	
	DisplayToolTip(true);

}

//int COwnerDrawComboBox::SearchForItem(const CString &searchFor)
//{
//	int nItemIndex = 0;
//	PtrFlushArray<ImageComboItem>::ArrayIterator it = m_items.iterator();
//	while(it.hasNext())
//	{
//		CString strItem = (*(it++))->itemText;
//			
//		if (!strItem.IsEmpty())
//		{
//
//			if(strItem.GetLength() > searchFor.GetLength())
//			{					
//				strItem.Delete(searchFor.GetLength(),
//					strItem.GetLength()-searchFor.GetLength());
//			}				
//			
//			if (!searchFor.CompareNoCase(strItem))
//			{
//				return nItemIndex;				
//			}
//		}
//		nItemIndex++;
//	}
//	return  -1;
//}

void COwnerDrawComboBox::OnCloseup() 
{
	DisplayToolTip(false);
	m_bLastSearch = false;
	m_itemSearchBuffer.Empty();
	

}

void COwnerDrawComboBox::OnCBKillFocus()
{
	OnCloseup();	
}

BOOL COwnerDrawComboBox::PreTranslateMessage(MSG* pMsg)
{
	
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if(!GetDroppedState())
		{
			OnCloseup();
			return TRUE;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}


void COwnerDrawComboBox::CreateToolTip (CWnd *pParent)
{
	TOOLINFO ti;
	DWORD dwStyle = TTS_ALWAYSTIP | TTS_NOPREFIX;

	m_ToolTip.Create (pParent, dwStyle);
	m_ToolTip.FillInToolInfo (ti, this, 0);
	ti.uFlags |= TTF_TRACK  | TTF_ABSOLUTE;
	ti.lpszText = (LPTSTR) "";

	m_ToolTip.SendMessage (TTM_ADDTOOL, 0, reinterpret_cast<LPARAM> (&ti));
}

void COwnerDrawComboBox::SetToolTipText (const TCHAR *pszText,bool bRedBackgroud)
{
	if(bRedBackgroud)
		m_ToolTip.SetTipBkColor(RGB(255,0,0));
	else
		m_ToolTip.SetTipBkColor(::GetSysColor(COLOR_WINDOW));
	m_ToolTip.UpdateTipText (pszText, this);
}
void COwnerDrawComboBox::DisplayToolTip (const bool bDisplay)
{
	if (m_ToolTip.m_hWnd)
	{
		CToolInfo ti;

		m_ToolTip.GetToolInfo (ti, this, 0);

		if (bDisplay)
		{
			CRect rectClient;
			GetClientRect (&rectClient);
			ClientToScreen(&rectClient);
			rectClient.left += 1;
			if(GetDroppedState())
			{
				rectClient.top += 3;
			}
			else
			{
				rectClient.top += rectClient.Height();
				//show toop tip under combobox if there is no list showed
			}

			m_ToolTip.SendMessage (TTM_TRACKPOSITION, 0, MAKELPARAM (rectClient.left,
				rectClient.top));
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, TRUE,
				reinterpret_cast<LPARAM> (&ti));
		}
		else
		{
			m_ToolTip.SendMessage (TTM_TRACKACTIVATE, FALSE,
				reinterpret_cast<LPARAM> (&ti));
		}
	}
}




