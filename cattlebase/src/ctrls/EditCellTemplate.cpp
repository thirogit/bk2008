#include "stdafx.h"
#include "EditCellTemplate.h"
#include "../consts.h"

template<class BASECLASS>
CBaseEditCell<BASECLASS>::CBaseEditCell (CListCtrl* pCtrl, int iItem, int iSubItem/*, CString &sInitText*/)
{
    pListCtrl = pCtrl;
    Item = iItem;
    SubItem = iSubItem;
    //InitText = sInitText;
}

BEGIN_TEMPLATE_MESSAGE_MAP(CBaseEditCell,BASECLASS,BASECLASS)
    //{{AFX_MSG_MAP(gxEditCell)
    ON_WM_KILLFOCUS()
    ON_WM_NCDESTROY()
    ON_WM_CHAR()
    ON_WM_CREATE()
    ON_WM_GETDLGCODE()
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

template<class BASECLASS>
void CBaseEditCell<BASECLASS>::SetListText()
{
    CString Text;
    GetWindowText (Text);

    // Send Notification to parent of ListView ctrl
    LV_DISPINFO dispinfo;
    dispinfo.hdr.hwndFrom = pListCtrl->m_hWnd;
    dispinfo.hdr.idFrom = pListCtrl->GetDlgCtrlID();
    dispinfo.hdr.code = LVN_ENDLABELEDIT;

    dispinfo.item.mask = LVIF_TEXT;
    dispinfo.item.iItem = Item;
    dispinfo.item.iSubItem = SubItem;
    dispinfo.item.pszText =  LPTSTR ((LPCTSTR) Text);
    dispinfo.item.cchTextMax = Text.GetLength();
	
    pListCtrl->GetParent()->SendMessage (WM_NOTIFY, pListCtrl->GetDlgCtrlID(), (LPARAM) &dispinfo);
}

template<class BASECLASS>
BOOL CBaseEditCell<BASECLASS>::PreTranslateMessage (MSG* pMsg) 
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

    return BASECLASS::PreTranslateMessage (pMsg);
}

template<class BASECLASS>
void CBaseEditCell<BASECLASS>::OnKillFocus (CWnd* pNewWnd) 
{
    BASECLASS::OnKillFocus(pNewWnd);
    DestroyWindow();
}

template<class BASECLASS>
void CBaseEditCell<BASECLASS>::OnNcDestroy() 
{
    BASECLASS::OnNcDestroy();
    delete this;
}

template<class BASECLASS>
void CBaseEditCell<BASECLASS>::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    //BOOL Shift = GetKeyState (VK_SHIFT) < 0;
    switch (nChar)
    {
		case VK_RETURN :
			SetListText();
		case VK_ESCAPE :
			pListCtrl->SetFocus();
			return;
   }

    BASECLASS::OnChar (nChar, nRepCnt, nFlags);
}
template<class BASECLASS>
int CBaseEditCell<BASECLASS>::OnCreate (LPCREATESTRUCT lpCreateStruct) 
{
    if (BASECLASS::OnCreate (lpCreateStruct) == -1)
		return -1;

    // Set the proper font
    CFont* Font = pListCtrl->GetFont();
    SetFont (Font);

    //SetWindowText (InitText);
    
	SetFocus();
    //SetSel (0, -1);
    return 0;
}
template<class BASECLASS>
UINT CBaseEditCell<BASECLASS>::OnGetDlgCode() 
{
    return BASECLASS::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}
template<class BASECLASS>
int CBaseEditCell<BASECLASS>::Create(const RECT &rect,CWnd *pParentWnd,UINT nID)
{
	// Get Column alignment
		LV_COLUMN lvCol;
		DWORD dwStyle;

		lvCol.mask = LVCF_FMT;
		pListCtrl->GetColumn (SubItem, &lvCol);
		if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
			dwStyle = ES_LEFT;
		else if ((lvCol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
			dwStyle = ES_RIGHT;
		else dwStyle = ES_CENTER;

		dwStyle |= /*WS_BORDER |*/ WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
		return BASECLASS::Create(dwStyle,rect,pParentWnd,nID);
}


CFloatEditCell::CFloatEditCell(CListCtrl* pCtrl, int iItem, int iSubItem, double initFloat,int prec) : 
			m_initFloat(initFloat),CBaseEditCell<CFilterEdit>(pCtrl,iItem,iSubItem),m_prec(max(2,prec))
{
	CFilterEdit::SetRegEx(g_szRegExUnsigned);
}

int CFloatEditCell::Create(const RECT &rect,CWnd *pParentWnd,UINT nID)
{
	if(CBaseEditCell<CFilterEdit>::Create(rect,pParentWnd,nID) == -1) return -1;
	char floatBuff[20],floatFmt[] = "%.*f";
	sprintf(floatBuff,floatFmt,m_prec,m_initFloat);
	SetWindowText(floatBuff);
	SetSel (0, -1);
	return 0;
}


CIntEditCell::CIntEditCell(CListCtrl* pCtrl, int iItem, int iSubItem, int initValue) :
	 m_initValue(initValue),CBaseEditCell<CFilterEdit>(pCtrl,iItem,iSubItem)
{
	CFilterEdit::SetRegEx("[0-9]{0,19}");
}
int CIntEditCell::Create(const RECT &rect,CWnd *pParentWnd,UINT nID)
{
	if(CBaseEditCell<CFilterEdit>::Create(rect,pParentWnd,nID) == -1) return -1;
	char intBuff[20];
	_snprintf(intBuff,sizeof(intBuff),"%d",m_initValue);
	SetWindowText(intBuff);
	SetSel (0, -1);
	return 0;
}

