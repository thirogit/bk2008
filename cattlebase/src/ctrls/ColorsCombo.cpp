#include "stdafx.h"
#include "ColorsCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColorsCombo::CColorsCombo()
{
}

CColorsCombo::~CColorsCombo()
{
}


BEGIN_MESSAGE_MAP(CColorsCombo, CComboBox)
	
END_MESSAGE_MAP()


void CColorsCombo::DrawItem(LPDRAWITEMSTRUCT dis) 
{
    CRect r = dis->rcItem;
    CDC * dc = CDC::FromHandle(dis->hDC);
    COLORREF txcolor;
    COLORREF bkcolor;

 //   CSize sz = dc->GetTextExtent(CString(_T("255,255,255  ")));

    int saved = dc->SaveDC();

    if(dis->itemState & ODS_SELECTED)
       { /* selected */
	if(::GetFocus() == m_hWnd)
	   { /* has focus */
	    bkcolor = ::GetSysColor(COLOR_HIGHLIGHT);
	    txcolor = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	   } /* has focus */	    
	else
	   { /* no focus */
	    bkcolor = ::GetSysColor(COLOR_3DFACE);
	    txcolor = ::GetSysColor(COLOR_WINDOWTEXT);
	   } /* no focus */
       } /* selected */
    else
       { /* unselected */
	if(dis->itemState & (ODS_DISABLED | ODS_GRAYED))
	   txcolor = ::GetSysColor(COLOR_GRAYTEXT);
	else
	   txcolor = ::GetSysColor(COLOR_WINDOWTEXT);

	bkcolor = ::GetSysColor(COLOR_WINDOW);
       } /* unselected */

    dc->FillSolidRect(&r, bkcolor);
    CRect focus;

    if(dis->itemID != (UINT)-1 
       && (dis->itemState & (ODS_DISABLED | ODS_GRAYED)) == 0)
       { /* has item */
	CRect block = r;
	block.right = r.Height();
	dc->FillSolidRect(&block, (COLORREF)dis->itemData);
	r.left += r.Height();

	focus = r;
    dc->SetBkColor(bkcolor);
	dc->SetTextColor(txcolor);
	CString s;

	r.left += ::GetSystemMetrics(SM_CXBORDER);
//	s.Format(_T("%d,%d,%d"), GetRValue(dis->itemData),
//		                 GetGValue(dis->itemData),
//		                 GetBValue(dis->itemData));
//	dc->TextOut(r.left, r.top, s);

//	r.left += sz.cx;
	GetLBText(dis->itemID, s);
	dc->TextOut(r.left, r.top, s); 
    } /* has item */

    if(dis->itemState & ODS_FOCUS && dis->itemAction != ODA_SELECT)
       dc->DrawFocusRect(&focus);

    dc->RestoreDC(saved);
}
