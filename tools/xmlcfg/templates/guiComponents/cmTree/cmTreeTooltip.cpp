// cmTreeTooltip.cpp : implementation file
//

#include "stdafx.h"
#include "cmTreeTooltip.h"
#include "memDC.h"
#include "cmDrawUtils.h"
#include "cmTreeEnum.h"
#include "cmTreeNodeDescriptor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BORDER 2

#define TIP_WIDTH 300

/////////////////////////////////////////////////////////////////////////////
// cmTreeTooltip

cmTreeTooltip::cmTreeTooltip():
	nodeDescriptor_(NULL)
{
}

cmTreeTooltip::~cmTreeTooltip()
{
}


BEGIN_MESSAGE_MAP(cmTreeTooltip, CToolTipCtrl)
	//{{AFX_MSG_MAP(cmTreeTooltip)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY_REFLECT(TTN_SHOW, OnShow)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cmTreeTooltip message handlers

void cmTreeTooltip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc);
	CRect rc;
	CRect iconRect;
	CRect textRect;
	CRect descRect;
	CRect extraRect;
	UINT icon;
	int height = 0;
	int textHeight = 0;
	int descHeight = 0;
	int extraHeight = 0;

	if (nodeDescriptor_ == NULL) {
		// background
		//
		//memDC.FillSolidRect(rc, RGB(255, 0, 0));

	} else {
		
		// define tooltip size
		//
		CRect tmp;

		memDC.SelectObject(&font_);
		tmp = CRect(0, 0, 0, 0);
		tmp.right = TIP_WIDTH;
		if (nodeDescriptor_->GetText().GetLength()) {
			height += BORDER; // upper distance from border

			memDC.DrawText(nodeDescriptor_->GetText(), tmp, DT_LEFT | DT_CALCRECT);

			if (tmp.Height() < 18) {
				height += 18;
				textHeight = 18;
			} else {
				height += tmp.Height();
				textHeight = tmp.Height();
			}
		}

		if (nodeDescriptor_->GetTooltipDescription().GetLength()) {
			tmp.bottom = 0;
			memDC.DrawText(nodeDescriptor_->GetTooltipDescription(), tmp, DT_LEFT | DT_CALCRECT | DT_WORDBREAK);

			height += BORDER; // upper distance from text
			height += tmp.Height();
			descHeight = tmp.Height();
		}

		if (nodeDescriptor_->GetTooltipExtraMsg().GetLength()) {
			tmp.bottom = 0;
			if (nodeDescriptor_->GetTooltipExtraMsgIcon())
				tmp.right = tmp.left + TIP_WIDTH - 16;

			memDC.DrawText(nodeDescriptor_->GetTooltipExtraMsg(), tmp, DT_LEFT | DT_CALCRECT | DT_WORDBREAK);

			height += BORDER; // upper distance from text
			if (tmp.Height() < 18) {
				height += 18;
				extraHeight = 18;
			} else {
				height += tmp.Height();
				extraHeight = tmp.Height();
			}
		}

		SetWindowPos(&CWnd::wndTop, 0, 0, TIP_WIDTH, height + 3 * BORDER, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		GetClientRect(rc);

		// background
		//
		memDC.FillSolidRect(rc, ::GetSysColor(COLOR_INFOBK));
	
		// item icon
		//
		iconRect.left = rc.left + BORDER;
		iconRect.top = rc.top + BORDER;
		iconRect.right = rc.left + 16;
		iconRect.bottom = rc.top + 16;

		icon = nodeDescriptor_->GetTreeIcon(enCmTreeIcon_NotSelCollapsed);
		if (icon)
			cmDrawUtils::DrawIcon(memDC, 
								icon, 
								iconRect.TopLeft(), 
								16);

		// item text
		//
		textRect.left = iconRect.right + 8;
		textRect.top = BORDER;
		textRect.right = TIP_WIDTH - 2 * BORDER;
		textRect.bottom = textRect.top + textHeight;
		cmDrawUtils::DrawString(memDC, 
								nodeDescriptor_->GetText(), 
								textRect, 
								DT_LEFT | DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE, 
								nodeDescriptor_->GetFontName(), nodeDescriptor_->GetFontSize(), 
								::GetSysColor(COLOR_INFOTEXT), 
								true);
		//DrawUtils::DrawRect(memDC, textRect, RGB(0x80,0x80,0x80));

		// item description
		//
		if (nodeDescriptor_->GetTooltipDescription().GetLength()) {
			descRect.left = iconRect.right + 8;
			descRect.top = textRect.bottom + BORDER;
			descRect.right = TIP_WIDTH - 2 * BORDER;
			descRect.bottom = descRect.top + descHeight;

			cmDrawUtils::DrawString(memDC, 
								  nodeDescriptor_->GetTooltipDescription(), 
								  descRect, 
								  DT_LEFT | DT_WORDBREAK, 
								  nodeDescriptor_->GetFontName(), nodeDescriptor_->GetFontSize(), 
								  ::GetSysColor(COLOR_INFOTEXT), 
								  false);
			//DrawUtils::DrawRect(memDC, descRect, RGB(0x80,0x80,0x80));
		} else {
			descRect = textRect;
		}

		// extra msg
		//
		if (nodeDescriptor_->GetTooltipExtraMsg().GetLength()) {
			extraRect.left = iconRect.right + 8;
			extraRect.top = descRect.bottom + 2 * BORDER;
			extraRect.right = TIP_WIDTH - 2 * BORDER;
			extraRect.bottom = extraRect.top + extraHeight;

			if (extraRect.Height() < 18)
				extraRect.bottom = extraRect.top + 18;

			cmDrawUtils::DrawString(memDC, 
								  nodeDescriptor_->GetTooltipExtraMsg(), 
								  extraRect, 
								  DT_LEFT | DT_WORDBREAK, 
								  nodeDescriptor_->GetFontName(), nodeDescriptor_->GetFontSize(), 
								  RGB(200, 0, 0), //::GetSysColor(COLOR_INFOTEXT), 
								  false);
			
			extraRect.left = BORDER;
			
			cmDrawUtils::DrawRect(memDC, extraRect, RGB(0x80,0x80,0x80));

			// extra icon
			//
			if (nodeDescriptor_->GetTooltipExtraMsgIcon()) {
				CPoint pt;
				pt = extraRect.TopLeft();
				pt.Offset(1,1);

				cmDrawUtils::DrawIcon(memDC, nodeDescriptor_->GetTooltipExtraMsgIcon(), pt, 16);
			}
		} else {
			extraRect = descRect;
		}
	}
}

BOOL cmTreeTooltip::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void cmTreeTooltip::SetTreeNodeDescriptor(cmTreeNodeDescriptor* desc)
{
	nodeDescriptor_ = desc;

	UpdateFont();

	SetFont(&font_, FALSE);
}

int cmTreeTooltip::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolTipCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//SetWindowPos(&CWnd::wndTop, 0, 0, 400, 200, SWP_NOMOVE | SWP_HIDEWINDOW);
	
	return 0;
}

void cmTreeTooltip::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CToolTipCtrl::OnGetMinMaxInfo(lpMMI);
}

void cmTreeTooltip::OnShow(NMHDR* pNMHDR, LRESULT* pResult)
{
	
}

void cmTreeTooltip::UpdateFont()
{
	if (nodeDescriptor_ == NULL)
		return;

	if (font_.GetSafeHandle())
		font_.DeleteObject();

	font_.CreateFont(-nodeDescriptor_->GetFontSize(), 
					 0,										/* width */ 
					 0,										/* escapement */
					 0,										/* orientation */
					 nodeDescriptor_->GetFontBold() ? FW_BOLD : FW_NORMAL,		/* weight*/
					 0,										/* italic*/
					 0,										/* underline */
					 0,										/* strikeout */
					 ANSI_CHARSET,							/* charset */
					 OUT_DEFAULT_PRECIS,					/* out precision */
					 CLIP_DEFAULT_PRECIS,					/* clip precision */
					 DEFAULT_QUALITY,						/* quality */
					 DEFAULT_PITCH,							/* pitch family */
					 nodeDescriptor_->GetFontName());	
}