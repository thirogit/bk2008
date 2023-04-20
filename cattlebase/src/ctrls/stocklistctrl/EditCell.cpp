#include "stdafx.h"
#include "EditCell.h"
gxEditCell::gxEditCell (CListCtrl* pCtrl, int iItem, int iSubItem, CString sInitText)
{
    pListCtrl = pCtrl;
    Item = iItem;
    SubItem = iSubItem;
    InitText = sInitText;
}

gxEditCell::~gxEditCell()
{
}

BEGIN_MESSAGE_MAP(gxEditCell, CEdit)
    //{{AFX_MSG_MAP(gxEditCell)
    ON_WM_KILLFOCUS()
    ON_WM_NCDESTROY()
    ON_WM_CHAR()
    ON_WM_CREATE()
    ON_WM_GETDLGCODE()
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// gxEditCell message handlers

void gxEditCell::SetListText()
{
    CString Text;
    GetWindowText (Text);

    // Send Notification to parent of ListView ctrl
    LV_DISPINFO dispinfo;
    dispinfo.hdr.hwndFrom = pListCtrl->m_hWnd;
    dispinfo.hdr.idFrom = GetDlgCtrlID();
    dispinfo.hdr.code = LVN_ENDLABELEDIT;

    dispinfo.item.mask = LVIF_TEXT;
    dispinfo.item.iItem = Item;
    dispinfo.item.iSubItem = SubItem;
    dispinfo.item.pszText =  LPTSTR ((LPCTSTR) Text);
    dispinfo.item.cchTextMax = Text.GetLength();
	
    pListCtrl->GetParent()->SendMessage (WM_NOTIFY, pListCtrl->GetDlgCtrlID(), (LPARAM) &dispinfo);
}

BOOL gxEditCell::PreTranslateMessage (MSG* pMsg) 
{
    if (pMsg->message == WM_KEYDOWN)
    {
	    if (pMsg->wParam == VK_RETURN || /*pMsg->wParam == VK_DELETE || */
			pMsg->wParam == VK_ESCAPE || /*pMsg->wParam == VK_TAB || 
			pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN ||*/ GetKeyState (VK_CONTROL))
			{
				::TranslateMessage (pMsg);
				::DispatchMessage (pMsg);
				return TRUE;		    	// DO NOT process further
			}
    }

    return CEdit::PreTranslateMessage (pMsg);
}

void gxEditCell::OnKillFocus (CWnd* pNewWnd) 
{
    CEdit::OnKillFocus(pNewWnd);
    DestroyWindow();
}

void gxEditCell::OnNcDestroy() 
{
    CEdit::OnNcDestroy();
    
    delete this;
}
void gxEditCell::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    BOOL Shift = GetKeyState (VK_SHIFT) < 0;
    switch (nChar)
    {
		case VK_RETURN :
			SetListText();
		case VK_ESCAPE :
			pListCtrl->SetFocus();
			return;
   }

    CEdit::OnChar (nChar, nRepCnt, nFlags);

    // Resize edit control if needed

    // Get text extent
  /*  CString Text;

    GetWindowText (Text);
    CWindowDC DC (this);
    CFont *pFont = GetParent()->GetFont();
    CFont *pFontDC = DC.SelectObject (pFont);
    CSize Size = DC.GetTextExtent (Text);
    DC.SelectObject (pFontDC);
    Size.cx += 5;			   	// add some extra buffer

    // Get client rect
    CRect Rect, ParentRect;
    GetClientRect (&Rect);
    GetParent()->GetClientRect (&ParentRect);

    // Transform rect to parent coordinates
    ClientToScreen (&Rect);
    GetParent()->ScreenToClient (&Rect);

    // Check whether control needs to be resized and whether there is space to grow
    if (Size.cx > Rect.Width())
    {
		if (Size.cx + Rect.left < ParentRect.right )
			Rect.right = Rect.left + Size.cx;
		else
			Rect.right = ParentRect.right;
		MoveWindow (&Rect);
    }*/
}

int gxEditCell::OnCreate (LPCREATESTRUCT lpCreateStruct) 
{
    if (CEdit::OnCreate (lpCreateStruct) == -1)
		return -1;

    // Set the proper font
    CFont* Font = pListCtrl->GetFont();
    SetFont (Font);
    SetWindowText (InitText);
    SetFocus();
    SetSel (0, -1);
    return 0;
}

UINT gxEditCell::OnGetDlgCode() 
{
    return CEdit::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}