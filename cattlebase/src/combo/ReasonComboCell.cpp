#include "stdafx.h"
#include "ReasonComboCell.h"
#include "global_arrays.h"
#include "datatypes/Reason.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

gxReasonComboCell::gxReasonComboCell (Reason::REASON_INOUT_TYPE inoutcombo,CListCtrl* pCtrl, int iItem, 
									  int iSubItem, Reason *curSel/* = NULL*/) : CBaseReasonCombo(0)
{
	m_bESC = FALSE;
	pListCtrl = pCtrl;
    Item = iItem;
    SubItem = iSubItem;
    initSelection = curSel;

	Reason *pReason = NULL;
	Reason::REASON_INOUT_TYPE reasonType;
	for(int i = 0,s = ary_reasons.GetSize();i < s;i++)
	{
		pReason = ary_reasons[i];
		reasonType = pReason->GetReasonType();
		if(reasonType == inoutcombo || reasonType == Reason::REASON_BOTH)
			m_reasons.Add(pReason);
	}
	UpdateItemCount();
}

int gxReasonComboCell::GetNewItemCount() 
{ 
	return m_reasons.GetSize(); 
}

gxReasonComboCell::~gxReasonComboCell()
{
}


Reason *gxReasonComboCell::GetItemText(int item,CString &itemText)
{
	Reason *itemReason = m_reasons[item];
	itemText = itemReason->GetReasonCode();
	return itemReason;
}
   

BEGIN_MESSAGE_MAP(gxReasonComboCell, CBaseReasonCombo)
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

int gxReasonComboCell::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBaseReasonCombo::OnCreate(lpCreateStruct) == -1)
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

BOOL gxReasonComboCell::PreTranslateMessage(MSG* pMsg) 
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
	
	return CBaseReasonCombo::PreTranslateMessage(pMsg);
}

void gxReasonComboCell::OnKillFocus(CWnd* pNewWnd) 
{
	CBaseReasonCombo::OnKillFocus(pNewWnd);
	DestroyWindow();	
}

void gxReasonComboCell::OnNcDestroy() 
{
    CBaseReasonCombo::OnNcDestroy();    
    delete this;
}


void gxReasonComboCell::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
	
	CBaseReasonCombo::OnChar(nChar, nRepCnt, nFlags);
}

void gxReasonComboCell::OnCloseup() 
{
	// Set the focus to the parent list control
	if(!m_bESC) SetListSubItem();
	GetParent()->SetFocus();
	

}

void gxReasonComboCell::SetListSubItem()
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
	if(curSel >= 0)
	{
		rule = (Rule*)GetItemData(curSel);
	
		dispinfo.item.pszText =  LPTSTR ((LPCTSTR) Text);
		dispinfo.item.cchTextMax = Text.GetLength();
		dispinfo.item.lParam = (LPARAM)rule;

		pListCtrl->GetParent()->SendMessage (WM_NOTIFY, pListCtrl->GetDlgCtrlID(), (LPARAM) &dispinfo);
	}
}


UINT gxReasonComboCell::OnGetDlgCode() 
{
    return CBaseReasonCombo::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}
