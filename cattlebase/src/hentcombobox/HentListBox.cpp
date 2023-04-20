
#include "stdafx.h"
#include "HentListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CHentListBox, CListBox)
	ON_WM_CREATE()
	
END_MESSAGE_MAP()

CHentListBox::CHentListBox()
{

}

int CHentListBox::GetItemCount()
{
	return GetCount();
}

int CHentListBox::SetItemCount( int nCount )
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, LB_SETCOUNT, nCount, 0);
}


int CHentListBox::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	OnInitListBox();
	return 0;
}

void CHentListBox::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();
//	OnInitListBox();

}

void CHentListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
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
}

