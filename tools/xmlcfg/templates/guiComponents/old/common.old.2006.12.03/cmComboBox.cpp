// AsComboBox.cpp : implementation file
//
#include "stdafx.h"
#include "cmComboBox.h"
#include "MemDC.h"
#include "DrawUtils.h"
#include "resource.h"

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
	basicRect_(0,0,0,0)
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    //HINSTANCE hInst = AfxGetResourceHandle();

    
	if (!(::GetClassInfo(hInst, _T("AsComboBox"), &wndcls)))
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
        wndcls.lpszClassName    = _T("AsComboBox");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return;
        }
    }
}

cmComboBox::~cmComboBox()
{
}


BEGIN_MESSAGE_MAP(cmComboBox, CComboBox)
	//{{AFX_MSG_MAP(cmComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_KILLFOCUS()
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
}

void cmComboBox::DrawComboButton(CDC* pDC, CRect clientRect)
{
	CSize bmpSize;
	CRect bmpRect;

	bmpSize = DrawUtils::GetBitmapSize(IDB_COMBO_BOX_NORMAL);

	bmpRect = clientRect;
	bmpRect.left = bmpRect.right - bmpSize.cx - 2;
	bmpRect.top = (bmpRect.Height() - bmpSize.cy) / 2;

	DrawUtils::DrawBitmap(pDC, IDB_COMBO_BOX_NORMAL, bmpRect.TopLeft(), DrawUtils::enBmp_AlignLeft);

	clientRect.DeflateRect(4,0,bmpRect.Width() + 2, 0);
	DrawUtils::DrawString(pDC, GetSelString(), clientRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE, ::GetSysColor(COLOR_BTNTEXT), GetFont());
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

				//pDC->SetTextColor(RGB(255,255,0));
				pWnd->SetWindowPos(&CWnd::wndTop, 0, 0, rc.Width(), height, SWP_SHOWWINDOW | SWP_NOMOVE); 
			}
		break;

		case CTLCOLOR_STATIC:
			//pDC->SetTextColor(RGB(255,0,0));

		default:
			{
				int i = 0;
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

void cmComboBox::AddItem(const cmString& s)
{
	AddItem(s, (long)sortKeyToValueMap_.size());
}

void cmComboBox::AddItem(const cmString& s, double sortKey)
{
	cmString buf;

	buf.Format(_T("%10.10f"), sortKey);

	AddItem(s, buf);
}

void cmComboBox::AddItem(const cmString& s, int sortKey)
{
	cmString buf;

	buf.Format(_T("%010d"), sortKey);

	AddItem(s, buf);
}

void cmComboBox::AddItem(const cmString& s, const cmString& sortKey)
{
	sortKeyToValueMap_[sortKey] = s;
	valueToSortKeyMap_[s] = sortKey;

	BuildList();
}

void cmComboBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc;

	GetClientRect(rc);

	
		
	DrawBackground(&dc, rc);
	DrawComboButton(&dc, rc);
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
}

void cmComboBox::OnSelchange() 
{
	RedrawWindow();	
}

/*
void cmComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (lpMeasureItemStruct->CtlType ==	ODT_COMBOBOX) {
		lpMeasureItemStruct->itemWidth = 100;
		lpMeasureItemStruct->itemHeight = 20;
	} else if (lpMeasureItemStruct->CtlType ==	ODT_LISTBOX) {
		lpMeasureItemStruct->itemWidth = 100;
		lpMeasureItemStruct->itemHeight = 20;
		MessageBox("PIPPO");
	} else {
		int i;

		i = 0;
	}
}

void cmComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;

	CMemDC memDC(dc.FromHandle(lpDrawItemStruct->hDC));
	
	switch (lpDrawItemStruct->CtlType) {
		case ODT_COMBOBOX:
			DrawComboButton(memDC, lpDrawItemStruct->rcItem);
			//DrawSingleItem(lpDrawItemStruct);		
			break;

		case ODT_LISTBOX:
			MessageBox("ODT_LISTBOX");
			//DrawSingleItem(lpDrawItemStruct);
			break;
	}

	cmString tmp;

	switch (lpDrawItemStruct->CtlType) {
		case ODT_BUTTON:
			tmp = "Owner-drawn button";
			break;

		case ODT_COMBOBOX:
			tmp = "Owner-drawn combo box";
			break;

		case ODT_LISTBOX:   
			tmp = "Owner-drawn list box";
			break;

		case ODT_MENU:   
			tmp = "Owner-drawn menu";
			break;

		case ODT_LISTVIEW:
			tmp = "List view control";
			break;

		case ODT_STATIC:
			tmp = "Owner-drawn static control";
			break;

		case ODT_TAB:
			tmp = "Tab";
			break;

		default:
			tmp = "NIENTE!!!";

	}
	
	//MessageBox(tmp);
}

*/

void cmComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	if (pNewWnd)
		CComboBox::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}
