// cmInfoArea.cpp : implementation file
//

#include "stdafx.h"
#include "cmInfoArea.h"
#include "cmDrawUtils.h"
#include "MemDC.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT cmInfoArea::infoExtraIconId_ = 0;
UINT cmInfoArea::warningExtraIconId_ = 0;
UINT cmInfoArea::questionExtraIconId_ = 0;
UINT cmInfoArea::errorExtraIconId_ = 0;

/////////////////////////////////////////////////////////////////////////////
// cmInfoArea

cmInfoArea::cmInfoArea():
	bkColor_(::GetSysColor(COLOR_BTNFACE)),
	textColor_(::GetSysColor(COLOR_BTNTEXT)),
	extraMsgColor_(::GetSysColor(COLOR_BTNTEXT)),
	currentExtraIconId_(0)
{
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
    
    if (!(::GetClassInfo(hInst, _T("CodeMachines.com InfoArea"), &wndcls)))
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
        wndcls.lpszClassName    = _T("cmInfoArea");

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return;
        }
    }
}

cmInfoArea::~cmInfoArea()
{
}


BEGIN_MESSAGE_MAP(cmInfoArea, CWnd)
	//{{AFX_MSG_MAP(cmInfoArea)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
//	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// cmInfoArea message handlers
int cmInfoArea::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->dwExStyle |= WS_EX_CONTROLPARENT;
	lpCreateStruct->style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN/* | WS_VSCROLL | WS_HSCROLL*/);

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ModifyStyleEx(0, WS_EX_CONTROLPARENT | WS_EX_NOPARENTNOTIFY);
	
	return 0;
}

void cmInfoArea::SetBkColor(COLORREF bkColor)
{
	bkColor_ = bkColor;
}

COLORREF cmInfoArea::GetBkColor() const
{
	return bkColor_;
}

void cmInfoArea::SetTextColor(COLORREF textColor)
{
	textColor_ = textColor;
}

COLORREF cmInfoArea::GetTextColor() const
{
	return textColor_;
}

void cmInfoArea::SetExtraMsgTextColor(COLORREF textColor)
{
	extraMsgColor_ = textColor;
}

COLORREF cmInfoArea::GetExtraMsgTextColor() const
{
	return extraMsgColor_;
}

void cmInfoArea::SetTitle(const cmString& title)
{
	title_ = title;
}

cmString cmInfoArea::GetTitle() const
{
	return title_;
}

void cmInfoArea::SetDescription(const cmString& description)
{
	description_ = description;
}

cmString cmInfoArea::GetDescription() const
{
	return description_;
}

void cmInfoArea::SetExtraMsg(const cmString& msg)
{
	extraMsg_ = msg;
}

cmString cmInfoArea::GetExtraMsg() const
{
	return extraMsg_;
}


void cmInfoArea::DrawText(CDC* pDC)
{
	CRect tmp;
	CRect textRect;
	int captionLineHeight;
	CFont* oldFont;
	int extraBoxHeight = 0;
	CRect rc;

	GetClientRect(rc);
	oldFont = pDC->SelectObject(GetFont());
	captionLineHeight = pDC->GetTextExtent(title_.c_str()).cy;
	pDC->SelectObject(oldFont);

	//tmp = rc;
	//tmp.DeflateRect(3, 3, 3, 0);
	rc.DeflateRect(3, 3, 3, 0);
	if (!extraMsg_.IsEmpty() || currentExtraIconId_) {
		// draw tooltip
		//
		CRect extraMsgRect(rc);

		tmp = rc;
		if (!extraMsg_.IsEmpty()) {
			if (currentExtraIconId_)
				tmp.left += 20;

			pDC->DrawText(extraMsg_, tmp, DT_CALCRECT | DT_WORDBREAK | DT_TOP);
			extraBoxHeight = max(tmp.Height(), 19);
		} else {
			extraBoxHeight = 19;
		}
		
		extraMsgRect = rc;
		extraMsgRect.bottom = extraMsgRect.top + extraBoxHeight;

		//pDC->FillSolidRect(tooltipRect, ::GetSysColor(COLOR_INFOBK));

		cmDrawUtils::DrawVerticalGradient(pDC, extraMsgRect, ::GetSysColor(COLOR_INFOBK) - RGB(30,30,30), ::GetSysColor(COLOR_INFOBK), 1);
		cmDrawUtils::DrawRect(pDC, extraMsgRect, ::GetSysColor(COLOR_3DSHADOW));

		if (currentExtraIconId_) {
			cmDrawUtils::DrawIcon(pDC, currentExtraIconId_, CPoint(extraMsgRect.left + 2, extraMsgRect.top + 2), 16);
			extraMsgRect.left += 20;
		}

		if (!extraMsg_.IsEmpty()) {
			cmDrawUtils::DrawString(pDC, 
								  extraMsg_, 
								  extraMsgRect, 
								  DT_LEFT | DT_TOP | DT_WORDBREAK, 
								  _T("tahoma"), 
								  -12, 
								  textColor_, 
								  false);
		}

		tmp.top += extraBoxHeight + 5;
	}
	
	textRect = rc;
	textRect.DeflateRect(0, extraBoxHeight + 3, 0, 0); 
	//textRect.bottom = textRect.top + captionLineHeight;
	
	cmDrawUtils::DrawString(pDC, 
						  title_, 
						  textRect, 
						  DT_LEFT | DT_TOP | DT_SINGLELINE | DT_END_ELLIPSIS, 
						  _T("tahoma"), 
						  -12, 
						  textColor_, 
						  true);

	textRect.DeflateRect(0, captionLineHeight + 3, 0, 0); 
	cmDrawUtils::DrawString(pDC, 
						  description_, 
						  textRect, 
						  DT_TOP | DT_WORDBREAK, 
						  _T("tahoma"), 
						  -12, 
						  textColor_, 
						  false);

//	cmDrawUtils::DrawRect(pDC, infoRect_, borderColor_);
}

void cmInfoArea::DrawTooltip(CDC* pDC)
{

}

BOOL cmInfoArea::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

/**
void cmInfoArea::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	UpdateWindowSize();
}
*/
void cmInfoArea::OnPaint() 
{
	CPaintDC dc(this); 
	CMemDC memDC(&dc);

	DrawBackground(memDC);
	DrawText(memDC);
	DrawTooltip(memDC);
}

void cmInfoArea::DrawBackground(CDC* pDC)
{
	CRect rc;
	
	GetClientRect(rc);
		
	pDC->FillSolidRect(rc, bkColor_);	
}

/*
oid cmInfoArea::UpdateWindowSize()
{
	RedrawWindow();
}
*/

BOOL cmInfoArea::Create(const CRect& rc, CWnd* parentWnd, DWORD style, UINT nID) 
{
	BOOL ret;

	style |= WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	ret = CWnd::Create(NULL, _T("cmInfoArea"), style, rc, parentWnd, nID, NULL);

	if (ret)
		ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	return ret;
}

UINT cmInfoArea::OnGetDlgCode() 
{
	return DLGC_WANTARROWS|DLGC_WANTALLKEYS|DLGC_WANTCHARS;
}

void cmInfoArea::SetCellInfo(cmBaseCellInfo* info)
{
	if (info) {
		title_ = info->GetTitle();
		description_ = info->GetDescription();
		extraMsg_ = info->GetExtraMsg();
		
		switch (info->GetExtraMsgType()) {
			case enCmCellExtraMsgType_None:
				currentExtraIconId_ = 0;
				break;

			case enCmCellExtraMsgType_Info:
				currentExtraIconId_ = infoExtraIconId_;
				break;

			case enCmCellExtraMsgType_Warning:
				currentExtraIconId_ = warningExtraIconId_;
				break;

			case enCmCellExtraMsgType_Question:
				currentExtraIconId_ = questionExtraIconId_;
				break;

			case enCmCellExtraMsgType_Error:
				currentExtraIconId_ = errorExtraIconId_;
				break;

			default:
				assert(false);
				currentExtraIconId_ = 0;
}

		RedrawWindow();
	}
}
	
void cmInfoArea::SetExtraIcons(UINT infoIconId, UINT warningIconId, UINT questionIconId, UINT errorIconId)
{
	infoExtraIconId_ = infoIconId;
	warningExtraIconId_ = warningIconId;
	questionExtraIconId_ = questionIconId;
	errorExtraIconId_ = errorIconId;
}

UINT cmInfoArea::GetExtraIcon(enCmCellExtraMsgType msgType)
{
	UINT iconId;

	switch (msgType) {
		case enCmCellExtraMsgType_None:
			iconId = 0;
			break;

		case enCmCellExtraMsgType_Info:
			iconId = infoExtraIconId_;
			break;

		case enCmCellExtraMsgType_Warning:
			iconId = warningExtraIconId_;
			break;

		case enCmCellExtraMsgType_Error:
			iconId = errorExtraIconId_;
			break;

		default:
			assert(false);
			iconId = 0;
}

	return iconId;
}