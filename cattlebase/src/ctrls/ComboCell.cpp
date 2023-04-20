#include "stdafx.h"
#include "ComboCell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_TEMPLATE_MESSAGE_MAP(<class BASECLASS>,CComboCellTemplate<BASECLASS>,BASECLASS)
	ON_WM_KILLFOCUS()
    ON_WM_NCDESTROY()
    ON_WM_CHAR()
    ON_WM_CREATE()
    ON_WM_GETDLGCODE()
END_MESSAGE_MAP()


template<class BASECLASS>
CComboCellTemplate<BASECLASS>::CComboCellTemplate (CListCtrl* pCtrl, int iItem, int iSubItem, void *curSel = NULL)
{
	m_bESC = FALSE;
	pListCtrl = pCtrl;
	Item = iItem;
	SubItem = iSubItem;
	initSelection = curSel;
}


template<class BASECLASS>    
void  CComboCellTemplate<BASECLASS>::SetListSubItem()
{
	RuleData *rule = NULL;
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
	rule = (RuleData*)GetItemData(curSel);
	
	dispinfo.item.pszText =  LPTSTR ((LPCTSTR) Text);
	dispinfo.item.cchTextMax = Text.GetLength();
	dispinfo.item.lParam = (LPARAM)rule;

	pListCtrl->GetParent()->SendMessage (WM_NOTIFY, pListCtrl->GetDlgCtrlID(), (LPARAM) &dispinfo);

}

template<class BASECLASS>
BOOL CComboCellTemplate<BASECLASS>::PreTranslateMessage(MSG* pMsg)
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
	
	return BASECLASS::PreTranslateMessage(pMsg);
}

template<class BASECLASS>
void CComboCellTemplate<BASECLASS>::OnKillFocus(CWnd* pNewWnd)
{
	CComboBox::OnKillFocus(pNewWnd);
	DestroyWindow();
}
template<class BASECLASS>
void CComboCellTemplate<BASECLASS>::OnNcDestroy()
{
	CComboBox::OnNcDestroy();    
	delete this;
}


template<class BASECLASS>
void CComboCellTemplate<BASECLASS>::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	BASECLASS::OnChar(nChar, nRepCnt, nFlags);
}

template<class BASECLASS>
int CComboCellTemplate<BASECLASS>::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (BASECLASS::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	
	// Set the proper font
	CFont* pFont = GetParent()->GetFont();
	SetFont(pFont);
	
	SetFocus();
//		InitCombo(initSelection);
	//BringWindowToTop();
	return 0;
}
template<class BASECLASS>
UINT CComboCellTemplate<BASECLASS>::OnGetDlgCode()
{
	return BASECLASS::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;
}
template<class BASECLASS>
void CComboCellTemplate<BASECLASS>::OnCloseup()
{
	// Set the focus to the parent list control
	if(!m_bESC) SetListSubItem();
	GetParent()->SetFocus();
}



