
#include "stdafx.h"
#include "RuleComboCell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceCombo

gxRuleComboCell::gxRuleComboCell (CListCtrl* pCtrl, int iItem, int iSubItem, Rule *curSel/* = NULL*/)
{
	m_bESC = FALSE;
	pListCtrl = pCtrl;
    Item = iItem;
    SubItem = iSubItem;
    initSelection = curSel;
}

gxRuleComboCell::~gxRuleComboCell()
{
}

   

BEGIN_MESSAGE_MAP(gxRuleComboCell, CComboBox)
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

int gxRuleComboCell::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

BOOL gxRuleComboCell::PreTranslateMessage(MSG* pMsg) 
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

void gxRuleComboCell::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	DestroyWindow();	
}

void gxRuleComboCell::OnNcDestroy() 
{
    CComboBox::OnNcDestroy();    
    delete this;
}


void gxRuleComboCell::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

void gxRuleComboCell::OnCloseup() 
{
	// Set the focus to the parent list control
	if(!m_bESC) SetListSubItem();
	GetParent()->SetFocus();
	

}

void gxRuleComboCell::SetListSubItem()
{
	Rule *rule = NULL;
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
	rule = (Rule*)GetItemData(curSel);
	
    dispinfo.item.pszText =  LPTSTR ((LPCTSTR) Text);
    dispinfo.item.cchTextMax = Text.GetLength();
	dispinfo.item.lParam = (LPARAM)rule;

    pListCtrl->GetParent()->SendMessage (WM_NOTIFY, pListCtrl->GetDlgCtrlID(), (LPARAM) &dispinfo);

}


UINT gxRuleComboCell::OnGetDlgCode() 
{
    return CComboBox::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}
