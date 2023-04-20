// AsOptionListWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cmOptionListWnd.h"
#include "MemDC.h"
#include "DrawUtils.h"
#include "ICmCellListener.h"
#include "cmOptionRow.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

#define INFO_BAR_HEIGHT 70

/////////////////////////////////////////////////////////////////////////////
// cmOptionListWnd

cmOptionListWnd::cmOptionListWnd():
	borderColor_(::GetSysColor(COLOR_3DSHADOW)),
	bkColor_(::GetSysColor(COLOR_3DFACE)),
	optionList_(NULL),
	fontInfo_(NULL),
	cellListener_(NULL),
	tooltipIcon_(0)
{
}

cmOptionListWnd::~cmOptionListWnd()
{
	if (optionList_)
		delete optionList_;

	if (fontInfo_)
		delete fontInfo_;
}


BEGIN_MESSAGE_MAP(cmOptionListWnd, CWnd)
	//{{AFX_MSG_MAP(cmOptionListWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// cmOptionListWnd message handlers

int cmOptionListWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	
	fontInfo_ = new cmFontInfo;
	optionList_ = new cmOptionList;
	optionList_->Create(CRect(0,0,0,0), this);
	optionList_->RegisterListener(this);
	optionList_->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	UpdateWindowSize();

	return 0;
}

BOOL cmOptionListWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void cmOptionListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	UpdateWindowSize();
}

void cmOptionListWnd::OnPaint() 
{
	CPaintDC dc(this); 
	CMemDC memDC(&dc);

	DrawBackground(memDC);
	DrawInfo(memDC);
}

void cmOptionListWnd::DrawBackground(CDC* pDC)
{
	CRect rc;
	CBrush b(bkColor_);

	GetClientRect(rc);
	pDC->FillRect(infoRect_, &b);

	rc.DeflateRect(0,0,1,1);
	DrawUtils::DrawRect(pDC, rc, borderColor_);
	
}

void cmOptionListWnd::DrawInfo(CDC* pDC)
{
	CRect tmp;
	CRect textRect;
	int captionLineHeight;
	CFont* oldFont;
	int tooltipHeight = 0;

	oldFont = pDC->SelectObject(fontInfo_->GetFont());
	captionLineHeight = pDC->GetTextExtent(caption_.c_str()).cy;
	pDC->SelectObject(oldFont);

	tmp = infoRect_;
	tmp.DeflateRect(5, 5, 5, 5);

	if (tooltipText_ != _T("") || tooltipIcon_ != 0) {
		// draw tooltip
		//
		CRect tooltipRect;

		tooltipHeight = 20;
		
		tooltipRect = tmp;
		tooltipRect.bottom = tooltipRect.top + tooltipHeight;

		pDC->FillSolidRect(tooltipRect, ::GetSysColor(COLOR_INFOBK));
		DrawUtils::DrawRect(pDC, tooltipRect, ::GetSysColor(COLOR_3DSHADOW));

		if (tooltipIcon_) {
			DrawUtils::DrawIcon(pDC, tooltipIcon_, CPoint(tooltipRect.left + 2, tooltipRect.top + 2), 16);
			tooltipRect.left += 20;
		}

		if (tooltipText_ != _T("")) {
			DrawUtils::DrawString(pDC, 
								  tooltipText_, 
								  tooltipRect, 
								  DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, 
								  fontInfo_->GetFontName(), 
								  -fontInfo_->GetFontSize(), 
								  ::GetSysColor(COLOR_INFOTEXT), 
								  false);
		}

		tmp.top += tooltipHeight + 5;
	}
	textRect = tmp;
	textRect.bottom = textRect.top + captionLineHeight;
	
	DrawUtils::DrawString(pDC, 
						  caption_, 
						  textRect, 
						  DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, 
						  fontInfo_->GetFontName(), 
						  -fontInfo_->GetFontSize(), 
						  ::GetSysColor(COLOR_BTNTEXT), 
						  true);

	textRect = tmp;
	textRect.top += captionLineHeight + 5;
	DrawUtils::DrawString(pDC, 
						  description_, 
						  textRect, 
						  DT_LEFT | DT_WORDBREAK, 
						  fontInfo_->GetFontName(), 
						  -fontInfo_->GetFontSize(), 
						  ::GetSysColor(COLOR_BTNTEXT), 
						  false);
}

void cmOptionListWnd::UpdateWindowSize()
{
	CRect rc;

	GetClientRect(rc);

	infoRect_ = rc;

	infoRect_.DeflateRect(0, rc.bottom - INFO_BAR_HEIGHT, 0, 0);

	rc.DeflateRect(0,0,0, INFO_BAR_HEIGHT);

	optionList_->SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

	
}

cmOptionList* cmOptionListWnd::GetOptionList()
{
	return optionList_;
}

BOOL cmOptionListWnd::Create(const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	BOOL ret;

	ret = CWnd::Create(NULL, _T("cmOptionListWnd"), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rect, pParentWnd, nID, NULL);

	if (ret)
		ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	return ret;
}

void cmOptionListWnd::SetFontInfo(const cmFontInfo& fontInfo)
{
	*fontInfo_ = fontInfo;

	optionList_->SetFontInfo(fontInfo);
}

void cmOptionListWnd::SetFontInfo(const cmString& fontName, int fontSize, bool fontBold)
{
	fontInfo_->SetFontName(fontName);
	fontInfo_->SetFontSize(fontSize);
	fontInfo_->SetFontBold(fontBold);

	optionList_->SetFontInfo(*fontInfo_);
}

BOOL cmOptionListWnd::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_LBUTTONDOWN) {
		CPoint pt(pMsg->pt);
		
		ScreenToClient(&pt);
//		if (infoRect_.PtInRect(pt))
//			optionList_->SetFocus();
	} else if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_F2) {
			cmOptionRow* row;

			row = optionList_->GetSelectedRow();
			if (row)
				row->GetValueCell()->SetFocus(true);
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}


void cmOptionListWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
//	optionList_->SetFocus();	
}

void cmOptionListWnd::OnInfoUpdate(const cmString& caption, 
								   const cmString& description,
								   const cmString& tooltipText,
								   UINT tooltipIcon)
{
	caption_ = caption;
	description_ = description;
	tooltipText_ = tooltipText;
	tooltipIcon_ = tooltipIcon;

	RedrawWindow(infoRect_);
}


void cmOptionListWnd::SetCellListener(ICmCellListener* cellListener)
{
	cellListener_ = cellListener;
}