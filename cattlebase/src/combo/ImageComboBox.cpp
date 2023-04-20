#include "stdafx.h"
#include "ImageComboBox.h"
#include "../exceptions/ArrayBoundsException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_SEARCH_BUFFER 20

CImageComboBox::CImageComboBox(): m_pImageList(NULL),m_bLastSearch(false)
{
}

CImageComboBox::~CImageComboBox()
{

}


BEGIN_MESSAGE_MAP(CImageComboBox, CComboBox)
	ON_WM_DELETEITEM()
	ON_WM_CREATE()
	ON_WM_CHARTOITEM()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS,OnCBKillFocus)
END_MESSAGE_MAP()

int CImageComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CComboBox::OnCreate(lpCreateStruct) == -1) return -1;

	ASSERT(lpCreateStruct->style & CBS_OWNERDRAWFIXED);
	ASSERT(!(lpCreateStruct->style & CBS_HASSTRINGS));
	ASSERT(lpCreateStruct->style & CBS_DROPDOWNLIST);
	ASSERT(!(lpCreateStruct->style & CBS_SORT));
	CreateToolTip(GetParent());
	return 0;	
}
void CImageComboBox::PreSubclassWindow() 
{
	CComboBox::PreSubclassWindow();	

	ASSERT(GetStyle() & CBS_OWNERDRAWFIXED);
	ASSERT(!(GetStyle() & CBS_HASSTRINGS));
	ASSERT(GetStyle() & CBS_DROPDOWNLIST);
	ASSERT(!(GetStyle() & CBS_SORT));

	CreateToolTip(GetParent());
}


int CImageComboBox::AddItem(const CString& itemText,int nImageIndex)
{
	ImageComboItem *pItem = new ImageComboItem;
	pItem->bBold = FALSE;
	pItem->crTextColor = RGB(0,0,0);
	pItem->dwItemData = NULL;
	pItem->itemText = itemText;
	pItem->nImageIndex = nImageIndex;
	m_items.Add(pItem);
	return AddString(NULL);

}

void CImageComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (GetCount() == 0 || lpDrawItemStruct->itemID > GetCount()) return;

	ImageComboItem* pItem = m_items.GetAt(lpDrawItemStruct->itemID);

	
	CDC dc;
	BOOL bSelected = FALSE;
	dc.Attach(lpDrawItemStruct->hDC);

	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();

	// If this item is selected, set the background color and the text color to appropriate 
	// values. Erase the rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
		bSelected = TRUE;
	}
	else
	{
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
	}

	CRect rect(lpDrawItemStruct->rcItem);
	rect.DeflateRect(1,0);

	// If we use images, and there is data, and the index is valid:
	if (m_pImageList && pItem->nImageIndex != -1)
	{
		DrawIconEx(dc.GetSafeHdc(),rect.left,rect.top,
			m_pImageList->ExtractIcon(pItem->nImageIndex),0, 0, 0, NULL, DI_NORMAL);
	}

	// If we use images - move text to the right:
	if (m_pImageList)
	{
		IMAGEINFO sImageInfo;
		m_pImageList->GetImageInfo(0, &sImageInfo);
		rect.left += sImageInfo.rcImage.right;
	}

	CFont* pOldFont = NULL;
	CFont boldFont;
	if (pItem->bBold)
	{
		CFont* curFont = dc.GetCurrentFont();
		LOGFONT lf;
		curFont->GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;
		boldFont.CreateFontIndirect(&lf);
		pOldFont = dc.SelectObject(&boldFont);
	}

	// If the item has its own color, replace text color (exception - color is black, and
	// the item is selected - then we leave the highlight text color)
	if (!bSelected || (bSelected && pItem->crTextColor != RGB(0,0,0)))
	{
		dc.SetTextColor(pItem->crTextColor);
	}

	
	dc.DrawText(pItem->itemText, -1, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	

	if (pOldFont)
	{
		dc.SelectObject(pOldFont);
		boldFont.DeleteObject();
	}

	// Reset the background color and the text color back to their original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);

	dc.Detach();
}

void CImageComboBox::SetItemBold(int nItemIndex,  BOOL bBold)
{
	ImageComboItem* pItem = GetImageComboItem(nItemIndex);
	if(pItem)
	{
		pItem->bBold = bBold;
		Invalidate();
	}
	else
		throw new CArrayBoundsException(GetCount(),nItemIndex);
}

void CImageComboBox::SetImageList(CImageList* pImageList) 
{
	ASSERT(pImageList);
	m_pImageList = pImageList; 
}

CString CImageComboBox::GetItemText(int nItemIndex)
{
	ImageComboItem* pItem = GetImageComboItem(nItemIndex);
	if(pItem)
		return pItem->itemText;
	
	throw new CArrayBoundsException(GetCount(),nItemIndex);
}

/*********************************************************************************************/
void CImageComboBox::SetItemImage(int nItemIndex, int nImageIndex)
{
	ImageComboItem* pItem = GetImageComboItem(nItemIndex);
	if(pItem)	
	{
		pItem->nImageIndex = nImageIndex;
		Invalidate();
	}
	else
		throw new CArrayBoundsException(GetCount(),nItemIndex);
}

/*********************************************************************************************/
void CImageComboBox::SetItemColor(int nItemIndex, COLORREF rcTextColor)
{
	ImageComboItem* pItem = GetImageComboItem(nItemIndex);
	if(pItem)	
	{
		pItem->crTextColor = rcTextColor;
		Invalidate();
	}
	else
		throw new CArrayBoundsException(GetCount(),nItemIndex);
}

/*********************************************************************************************/
void CImageComboBox::SetItemText(int nItemIndex,const CString& itemText)
{
	ImageComboItem* pItem = GetImageComboItem(nItemIndex);
	if(pItem)	
	{
		pItem->itemText = itemText;
	}
	else
		throw new CArrayBoundsException(GetCount(),nItemIndex);
}
/*********************************************************************************************/
void CImageComboBox::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	delete m_items.GetAt(lpDeleteItemStruct->itemID);
	m_items.RemoveAt(lpDeleteItemStruct->itemID);
	CComboBox::OnDeleteItem(nIDCtl, lpDeleteItemStruct);
}
/*********************************************************************************************/
CImageComboBox::ImageComboItem* CImageComboBox::GetImageComboItem(int nItemIndex)
{
	if (nItemIndex < 0 || nItemIndex >= GetCount()) return NULL;
	return m_items.GetAt(nItemIndex);
}
/*********************************************************************************************/
void CImageComboBox::SetItemData(int nItemIndex, DWORD dwData)
{
	ImageComboItem* pItem = GetImageComboItem(nItemIndex);
	if(pItem)	
	{
		pItem->dwItemData = dwData;
	}
	else
		throw new CArrayBoundsException(GetCount(),nItemIndex);
}
/*********************************************************************************************/
DWORD CImageComboBox::GetItemData(int nItemIndex)
{
	ImageComboItem* pItem = GetImageComboItem(nItemIndex);
	if(pItem)	
	{
		return pItem->dwItemData;
	}
	throw new CArrayBoundsException(GetCount(),nItemIndex);	
}
/*********************************************************************************************/
void CImageComboBox::MeasureItem(LPMEASUREITEMSTRUCT)
{
}

int CImageComboBox::CompareItem(LPCOMPAREITEMSTRUCT)
{
	return 0; 
}
int CImageComboBox::OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex) 
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

void CImageComboBox::OnKeyDown (UINT nChar, UINT nRepCnt, UINT nFlags)
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

void CImageComboBox::UpdateToolTip()
{
	CString tipText("Szukaj: ");
	tipText += m_itemSearchBuffer;
	SetToolTipText(tipText,!m_bLastSearch);	
	DisplayToolTip(true);

}
int CImageComboBox::SearchForItem(const CString &searchFor)
{
	int nItemIndex = 0;
	PtrFlushArray<ImageComboItem>::ArrayIterator it = m_items.iterator();
	while(it.hasNext())
	{
		CString strItem = (*(it++))->itemText;
			
		if (!strItem.IsEmpty())
		{

			if(strItem.GetLength() > searchFor.GetLength())
			{					
				strItem.Delete(searchFor.GetLength(),
					strItem.GetLength()-searchFor.GetLength());
			}				
			
			if (!searchFor.CompareNoCase(strItem))
			{
				return nItemIndex;				
			}
		}
		nItemIndex++;
	}
	return  -1;
}
void CImageComboBox::OnCloseup() 
{
	DisplayToolTip(false);
	m_bLastSearch = false;
	m_itemSearchBuffer.Empty();
	

}

void CImageComboBox::OnCBKillFocus()
{
	OnCloseup();	
}

BOOL CImageComboBox::PreTranslateMessage(MSG* pMsg)
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


void CImageComboBox::CreateToolTip (CWnd *pParent)
{
	TOOLINFO ti;
	DWORD dwStyle = TTS_ALWAYSTIP | TTS_NOPREFIX;

	m_ToolTip.Create (pParent, dwStyle);
	m_ToolTip.FillInToolInfo (ti, this, 0);
	ti.uFlags |= TTF_TRACK  | TTF_ABSOLUTE;
	ti.lpszText = (LPTSTR) "";

	m_ToolTip.SendMessage (TTM_ADDTOOL, 0, reinterpret_cast<LPARAM> (&ti));
}

void CImageComboBox::SetToolTipText (const TCHAR *pszText,bool bRedBackgroud)
{
	if(bRedBackgroud)
		m_ToolTip.SetTipBkColor(RGB(255,0,0));
	else
		m_ToolTip.SetTipBkColor(::GetSysColor(COLOR_WINDOW));
	m_ToolTip.UpdateTipText (pszText, this);
}
void CImageComboBox::DisplayToolTip (const bool bDisplay)
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




