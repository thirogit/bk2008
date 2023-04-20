#include "stdafx.h"
#include "subeditlistctrl.h"

BEGIN_MESSAGE_MAP(CSubEditListCtrl, CListCtrl)
    //{{AFX_MSG_MAP(gxListCtrl)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(NM_DBLCLK , OnDblClick)
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CEdit*  CSubEditListCtrl::EditSubItem (int Item, int Column)
{
		
		// The returned pointer should not be saved
		// Make sure that the item is visible
		if (!EnsureVisible (Item, TRUE)) 
    		return NULL;
	    

		// Make sure that nCol is valid
		CHeaderCtrl* pHeader = GetHeaderCtrl();
		int nColumnCount = pHeader->GetItemCount();
		if (Column >= nColumnCount || GetColumnWidth (Column) < 5/*not enough space to room edit ctrl*/)
			return NULL;                 

		// Get the column offset
		int Offset = 0;
		for (int iColumn = 0; iColumn < Column; iColumn++)
			Offset += GetColumnWidth (iColumn);

		CRect Rect;
		GetItemRect (Item, &Rect, LVIR_BOUNDS);

		// Now scroll if we need to expose the column
		CRect ClientRect;
		GetClientRect (&ClientRect);
		if (Offset + Rect.left < 0 || Offset + Rect.left > ClientRect.right)
		{
			CSize Size;
			if (Offset + Rect.left > 0)
				Size.cx = -(Offset - Rect.left);
			else
				Size.cx = Offset - Rect.left;
			Size.cy = 0;
			Scroll (Size);
			Rect.left -= Size.cx;
		}

		// Get Column alignment
		LV_COLUMN lvCol;
		lvCol.mask = LVCF_FMT;
		GetColumn (Column, &lvCol);
		DWORD dwStyle;
		if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
			dwStyle = ES_LEFT;
		else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
			dwStyle = ES_RIGHT;
		else dwStyle = ES_CENTER;

		Rect.left += Offset+4;
		Rect.right = Rect.left + GetColumnWidth (Column) - 3;
		if (Rect.right > ClientRect.right)
			Rect.right = ClientRect.right;

		dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;

		CEdit *pEdit = new gxEditCell (this, Item, Column, GetItemText (Item, Column));

		pEdit->Create (dwStyle, Rect,(CWnd*)this, IDC_EDITCELL);
		return pEdit;

}

void CSubEditListCtrl::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	}

	if(nItem >= 0 && nSubItem >= 0)
	{

		EditSubItem(nItem,nSubItem);
	}

	*pResult = 0;
}


void CSubEditListCtrl::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (GetFocus() != this) 
		SetFocus();
    
    CListCtrl::OnHScroll (nSBCode, nPos, pScrollBar);
}

void CSubEditListCtrl::OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (GetFocus() != this) 
		SetFocus();
    
    CListCtrl::OnVScroll (nSBCode, nPos, pScrollBar);
}
