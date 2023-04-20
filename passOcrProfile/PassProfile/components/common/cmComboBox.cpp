// AsComboBox.cpp : implementation file
//
#include "stdafx.h"
#include "MemDC.h"
#include "cmDrawUtils.h"
#include "resource.h"
#include "cmComboBox.h"
#include "cmComboBoxCell.h"
#include <assert.h>

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
/////////////////////////////////////////////////////////////////////////////
// cmComboBox

cmComboBox::cmComboBox():
	hwndList_(NULL),
	droppedDown_(false),
	dropAreaHeight_(100),
	basicRect_(0,0,0,0),
	parentCell_(NULL),
	borderColor_(::GetSysColor(COLOR_3DSHADOW)),
	drawBorder_(false)
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    //HINSTANCE hInst = AfxGetResourceHandle();

    
	if (!(::GetClassInfo(hInst, _T("cmComboBox"), &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = _T("cmComboBox");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return;
        }
    }
}

cmComboBox::~cmComboBox()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(cmComboBox, CComboBox)
	//{{AFX_MSG_MAP(cmComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_CONTROL_REFLECT(CBN_SELENDOK , OnSelEndOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmComboBox message handlers

void cmComboBox::OnDropdown() 
{
	droppedDown_ = true;
	//MessageBox("DropDown");
}

void cmComboBox::OnCloseup() 
{
	droppedDown_ = false;
	if (parentCell_)
		parentCell_->KillFocus(false, true);
}

void cmComboBox::DrawComboButton(CDC* pDC, CRect clientRect)
{
	CSize bmpSize;
	CRect bmpRect;

	bmpSize = cmDrawUtils::GetBitmapSize(IDB_COMBO_BOX_NORMAL);

	bmpRect = clientRect;
	bmpRect.left = bmpRect.right - bmpSize.cx - 2;
	bmpRect.top = (bmpRect.Height() - bmpSize.cy) / 2;

	cmDrawUtils::DrawBitmap(pDC, IDB_COMBO_BOX_NORMAL, bmpRect.TopLeft(), cmDrawUtils::enBmp_AlignLeft);

	clientRect.DeflateRect(4,0,bmpRect.Width() + 2, 0);
	cmDrawUtils::DrawString(pDC, GetSelString(), clientRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE, ::GetSysColor(COLOR_BTNTEXT), GetFont());
}

BOOL cmComboBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	BOOL ret;

	dwStyle |= (WS_CHILD | CBS_HASSTRINGS | CBS_NOINTEGRALHEIGHT | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST/* | CBS_OWNERDRAWFIXED*/);
	
	//ret = CWnd::Create("ComboBox", "Algostar Combo Box", dwStyle, rect, pParentWnd, nID, pContext);
	
	ret = CComboBox::Create(dwStyle, rect, pParentWnd, nID);
	
	if (ret)
		ModifyStyleEx(0, WS_EX_NOPARENTNOTIFY);

	
	return ret;	
}

HBRUSH cmComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	switch (nCtlColor) {
		case CTLCOLOR_LISTBOX:
			{
				CRect rc;
				int height;

				GetWindowRect(rc);
				if (GetCount())
					height = min(10, sortKeyToValueMap_.size()) * (GetItemHeight(0) + 1);
				else
					height = 100;

				pWnd->SetWindowPos(&CWnd::wndTop, 0, 0, rc.Width(), height, SWP_SHOWWINDOW | SWP_NOMOVE);
			}
			break;

		case CTLCOLOR_STATIC:
			//pDC->SetTextColor(RGB(255,0,0));
			break;

		case CTLCOLOR_EDIT:
			{
				CEdit tmp;

				tmp.SubclassWindow(pWnd->GetSafeHwnd());
				tmp.SetMargins(0, 0);

				tmp.Detach();
			}
			break;
	}
		
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void cmComboBox::Clear()
{
	sortKeyToValueMap_.clear();
	valueToSortKeyMap_.clear();

	ResetContent();
}

void cmComboBox::AddItem(const cmString& s, DWORD itemData)
{
	AddItem(s, (long)sortKeyToValueMap_.size(), itemData);
}

void cmComboBox::AddItem(const cmString& s, double sortKey, DWORD itemData)
{
	cmString buf;

	buf.Format(_T("%10.10f"), sortKey);

	AddItem(s, buf, itemData);
}

void cmComboBox::AddItem(const cmString& s, int sortKey, DWORD itemData)
{
	cmString buf;

	buf.Format(_T("%010d"), sortKey);

	AddItem(s, buf, itemData);
}

void cmComboBox::AddItem(const cmString& s, const cmString& sortKey, DWORD itemData)
{
	int index;
	map<cmString, cmString>::iterator it;
	
	sortKeyToValueMap_[sortKey] = s;
	valueToSortKeyMap_[s] = sortKey;

	index = 0;
	for (it = sortKeyToValueMap_.begin(); it != sortKeyToValueMap_.end(); it++) {
		if (it->second == s) { 
			InsertString(index, s);
			SetItemData(index, itemData);
		}
		index++;
	}
}

void cmComboBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc;

	GetClientRect(rc);
		
	Draw(&dc, rc);
}

void cmComboBox::DrawBackground(CDC* pDC, CRect clientRect)
{
	// draw rect
	//
	CBrush h;
	
	//if (GetFocus() != this)
		h.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
	//else
	//	h.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
	
	pDC->FillRect(clientRect, &h);
}

void cmComboBox::BuildList()
{
	cmString selcmString;
	map<cmString, cmString>::iterator it;

	if (GetCurSel() != CB_ERR) {
		GetLBText(GetCurSel(), selcmString);
	}

	ResetContent();

	for (it = sortKeyToValueMap_.begin(); it != sortKeyToValueMap_.end(); it++) {
		AddString(it->second.c_str());
	}
}

void cmComboBox::SelectItem(double sortKey)
{
	cmString buf;

	buf.Format(_T("%10.10f"), sortKey);

	SelectItem(buf);
}

void cmComboBox::SelectItem(int sortKey)
{
	cmString buf;

	buf.Format(_T("%010d"), sortKey);

	SelectItem(buf);
}

void cmComboBox::SelectItem(const cmString& sortKey)
{
	map<cmString, cmString>::iterator it;

	it = sortKeyToValueMap_.find(sortKey);
	if (it != sortKeyToValueMap_.end())
		SelectString(-1, it->second.c_str());
}

bool cmComboBox::GetSelKey(double& sortKey)
{
	bool ret;
	cmString s;

	ret = GetSelKey(s);
	sortKey = (double)s;

	return ret;
}

bool cmComboBox::GetSelKey(int& sortKey)
{
	bool ret;
	cmString s;

	ret = GetSelKey(s);
	sortKey = (int)s;

	return ret;
}

bool cmComboBox::GetSelKey(cmString& sortKey)
{
	if (GetCurSel() == CB_ERR)
		return false;

	cmString tmp;
	map<cmString, cmString>::iterator it;

	GetLBText(GetCurSel(), tmp);
	
	it = valueToSortKeyMap_.find(tmp);
	if (it != valueToSortKeyMap_.end()) {
		sortKey = it->second;	
		return true;
	}
	
	return false;
}

cmString cmComboBox::GetSelString()
{
	if (GetCurSel() == CB_ERR)
		return _T("");

	cmString tmp;

	GetLBText(GetCurSel(), tmp);
	
	return tmp;
}

void cmComboBox::Draw(CDC* pDC, CRect clientRect)
{
	DrawBackground(pDC, clientRect);
	DrawComboButton(pDC, clientRect);
	DrawBorder(pDC, clientRect);
}

void cmComboBox::OnSelEndOk() 
{
	if (parentCell_) {
		ShowWindow(SW_HIDE);
		parentCell_->KillFocus(true, true);
	} else {
		GetParent()->SetFocus();
	}
}

void cmComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	//parentCell_->KillFocus(false);
}

BOOL cmComboBox::PreTranslateMessage(MSG* pMsg)
{
/***
	if (pMsg->message == WM_KEYDOWN) {
		lastChar_ = pMsg->wParam;
		switch (pMsg->wParam) {
			case VK_DELETE:
				break;

			case VK_BACK:
				break;

			case VK_ESCAPE:
				break;
		}
	}
***/
	
	return CComboBox::PreTranslateMessage(pMsg);
} 

void cmComboBox::SetParentCell(cmComboBoxCell* parent)
{
	parentCell_ = parent;
}

UINT cmComboBox::GetLastChar() const
{
	return lastChar_;
}

void cmComboBox::SetBorderColor(COLORREF color)
{
	borderColor_ = color;
}

COLORREF cmComboBox::GetBorderColor() const
{
	return borderColor_;
}

void cmComboBox::EnableBorder(bool enable)
{
	drawBorder_ = enable;
}

void cmComboBox::DrawBorder(CDC* pDC, CRect clientRect)
{
	if (drawBorder_)
		cmDrawUtils::DrawRect(pDC, clientRect, borderColor_);
}

void cmComboBox::SelectItemByItemData(DWORD itemData)
{
	for (int i = 0; i < GetCount(); i++) {
		if (GetItemData(i) == itemData) {
			SetCurSel(i);
			break;
		}
	}

	RedrawWindow();
}