
#include "stdafx.h"
#include "SexComboCell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceCombo

gxSexComboCell::gxSexComboCell (CListCtrl* pCtrl, int iItem, int iSubItem, COWSEX curSel/* = SEX_UNK*/)
{
	m_bESC = FALSE;
	pListCtrl = pCtrl;
    Item = iItem;
    SubItem = iSubItem;
    initSelection = curSel;
}

gxSexComboCell::~gxSexComboCell()
{
}

   

BEGIN_MESSAGE_MAP(gxSexComboCell, CComboBox)
	//{{AFX_MSG_MAP(CInPlaceCombo)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_WM_NCDESTROY()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceCombo message handlers

int gxSexComboCell::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	// Set the proper font
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);
	
	SetFocus();
	ReloadCombo(initSelection);
	//BringWindowToTop();
	return 0;
}

BOOL gxSexComboCell::PreTranslateMessage(MSG* pMsg) 
{
	// If the message if for "Enter" or "Esc"
	// Do not process
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			// DO NOT process further
			return TRUE;				
		}
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}

void gxSexComboCell::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	DestroyWindow();	
}

void gxSexComboCell::OnNcDestroy() 
{
    CComboBox::OnNcDestroy();    
    delete this;
}


void gxSexComboCell::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// If the key is "Esc" set focus back to the list control
	switch(nChar)
	{
	case VK_RETURN:
			SetListSubItem();
			GetParent()->SetFocus();
	return ;
	case VK_ESCAPE:
			GetParent()->SetFocus();
			m_bESC = TRUE;
	return;
	}
	
	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void gxSexComboCell::OnCloseup() 
{
	// Set the focus to the parent list control
	if(!m_bESC) SetListSubItem();
	GetParent()->SetFocus();
	

}

void gxSexComboCell::SetListSubItem()
{
	COWSEX sex = SEX_UNK;
	CString Text;
	int curSel = -1;
	
    LV_DISPINFO dispinfo;
    dispinfo.hdr.hwndFrom = pListCtrl->m_hWnd;
    dispinfo.hdr.idFrom = GetDlgCtrlID();
    dispinfo.hdr.code = LVN_ENDLABELEDIT;

    dispinfo.item.mask = LVIF_TEXT;
    dispinfo.item.iItem = Item;
    dispinfo.item.iSubItem = SubItem;

	curSel = GetCurSel();
	ASSERT(curSel >= 0);
	sex = (COWSEX)GetItemData(curSel);
	
    dispinfo.item.pszText =  LPTSTR ((LPCTSTR) Text);
    dispinfo.item.cchTextMax = Text.GetLength();
	dispinfo.item.lParam = (LPARAM)sex;

    pListCtrl->GetParent()->SendMessage (WM_NOTIFY, pListCtrl->GetDlgCtrlID(), (LPARAM) &dispinfo);

}


UINT gxSexComboCell::OnGetDlgCode() 
{
    return CComboBox::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}
