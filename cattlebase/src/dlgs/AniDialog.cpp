// AniDialog.cpp: implementation of the CAniDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AniDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAniDialog::CAniDialog(UINT nDlgID, CWnd* pParent)
: m_bAni(TRUE), m_nFrames(DEF_FRAMES), m_nMillSec(DEF_MILLSEC),
  m_wOpenStyle(AS_REFER), m_wCloseStyle(AS_REFER), m_pParent(pParent),
  m_ptRefer(0, 0), m_wDispStyle(DS_WIRE), CDialog(nDlgID, pParent)
{}

CAniDialog::CAniDialog(CPoint pt, UINT nDlgID, CWnd* pParent) 
: m_ptRefer(pt), m_nMillSec(DEF_MILLSEC), m_nFrames(DEF_FRAMES),
  m_wOpenStyle(AS_REFER), m_wCloseStyle(AS_REFER), m_pParent(pParent),
  m_bAni(TRUE), m_wDispStyle(DS_WIRE), CDialog(nDlgID, pParent)
{}

BEGIN_MESSAGE_MAP(CAniDialog, CDialog)
	//{{AFX_MSG_MAP(CTestAniDlg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CAniDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	srand((unsigned)time(NULL));

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!IsAniValid())
		return 0;

	CenterWindow(m_pParent);

	// rect for the dialog to appear from
	CRect rectTo;
	GetWindowRect(&rectTo);

	CRect rectFrom = CalcTargetRect(rectTo, m_wOpenStyle);
	
	if (m_wDispStyle == DS_CAPTION)
		DrawAnimatedRects( IDANI_CAPTION, rectFrom, rectTo);
	else
		DrawWireRects(rectFrom, rectTo, m_nMillSec, m_nFrames);

	return 0;
}

void CAniDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (!IsAniValid())
		return;

	CRect rectFrom;
	GetWindowRect(&rectFrom);

	CRect rectTo = CalcTargetRect(rectFrom, m_wCloseStyle);

	if (m_wDispStyle == DS_CAPTION)
		DrawAnimatedRects( IDANI_CAPTION, rectFrom, rectTo);
	else
		DrawWireRects(rectFrom, rectTo, m_nMillSec, m_nFrames);

}

BOOL CAniDialog::IsAniValid() const
{
	return (m_bAni && m_nMillSec > 0 && m_nFrames > 0 );
}

// Almond and Maunder's function.
void CAniDialog::DrawWireRects(LPRECT lprcFrom, LPRECT lprcTo, UINT nMilliSecSpeed, UINT nFrames)
{
	GdiFlush();
    Sleep(50);  // Let the desktop window sort itself out

	// if hwnd is null - "you have the CON".
	HDC hDC = ::GetDC(NULL);

	// Pen size, urmmm not too thick
    HPEN hPen = ::CreatePen(PS_SOLID, 2, RGB(0,0,0));

	int nMode = ::SetROP2(hDC, R2_NOT);
	HPEN hOldPen = (HPEN) ::SelectObject(hDC, hPen);

	for (int i = 0; i < (int)nFrames; i++)
	{
        double dFraction = (double) i / (double) nFrames;

        RECT transition;
        transition.left   = lprcFrom->left + (int)((lprcTo->left - lprcFrom->left) * dFraction);
        transition.right  = lprcFrom->right + (int)((lprcTo->right - lprcFrom->right) * dFraction);
        transition.top    = lprcFrom->top + (int)((lprcTo->top - lprcFrom->top) * dFraction);
        transition.bottom = lprcFrom->bottom + (int)((lprcTo->bottom - lprcFrom->bottom) * dFraction);

		POINT pt[5];
		pt[0] = CPoint(transition.left, transition.top);
		pt[1] = CPoint(transition.right,transition.top);
		pt[2] = CPoint(transition.right,transition.bottom);
		pt[3] = CPoint(transition.left, transition.bottom);
		pt[4] = CPoint(transition.left, transition.top);

		// We use Polyline because we can determine our own pen size
		// Draw Sides
		::Polyline(hDC,pt,5);

		GdiFlush();

		Sleep(nMilliSecSpeed);

		// UnDraw Sides
		::Polyline(hDC,pt,5);

		GdiFlush();
	}

	::SetROP2(hDC, nMode);
	::SelectObject(hDC, hOldPen);

	::ReleaseDC(NULL,hDC);
}

CPoint CAniDialog::GetRefPt() const
{
	return m_ptRefer;
}

void CAniDialog::SetRefPt(CPoint pt)
{
	m_ptRefer = pt;
}

UINT CAniDialog::GetAniSpeed() const
{
	return m_nMillSec;
}

void CAniDialog::SetAniSpeed(UINT nMillSec)
{
	m_nMillSec = nMillSec;
}

UINT CAniDialog::GetFrameNum() const
{
	return m_nFrames;
}

void CAniDialog::SetFrameNum(UINT nFrames)
{
	m_nFrames = nFrames;
}

void CAniDialog::EnableAni(BOOL bEnable)
{
	m_bAni = bEnable;
}

WORD CAniDialog::GetOpenStyle() const
{
	return m_wOpenStyle;
}

WORD CAniDialog::GetCloseStyle() const
{
	return m_wCloseStyle;
}

void CAniDialog::SetAniStyles(WORD wOpenStyle, WORD wCloseStyle)
{
	m_wOpenStyle = wOpenStyle;
	m_wCloseStyle = wCloseStyle;
}

CRect CAniDialog::PtToRect(CPoint pt) const
{
	return CRect(pt, CSize(1, 1));
}

CRect CAniDialog::PtToRect(int x, int y) const
{
	return CRect(CPoint(x, y), CSize(1, 1));
}


CRect CAniDialog::CalcTargetRect(CRect wndRect, WORD wStyle) const
{
	if (wndRect.IsRectEmpty())
		return wndRect;

	CRect targetRect;
	int x, y;

	switch (wStyle)
	{
	case AS_CENTER:
		targetRect = PtToRect(wndRect.CenterPoint());
		break;

	case AS_TOPLEFT:
		targetRect = PtToRect(wndRect.TopLeft());
		break;

	case AS_TOPRIGHT:
		targetRect = PtToRect(wndRect.right, wndRect.top);
		break;

	case AS_BOTTOMLEFT:
		targetRect = PtToRect(wndRect.left, wndRect.bottom);
		break;

	case AS_BOTTOMRIGHT:
		targetRect = PtToRect(wndRect.BottomRight());
		break;

	case AS_LEFT:
		targetRect = CRect(wndRect.TopLeft(),
			CPoint(wndRect.left + 1, wndRect.bottom));
		break;

	case AS_TOP:
		targetRect = CRect(wndRect.TopLeft(),
			CPoint(wndRect.right, wndRect.top + 1));
		break;

	case AS_RIGHT:
		targetRect = CRect(CPoint(wndRect.right - 1, wndRect.top),
			wndRect.BottomRight());
		break;

	case AS_BOTTOM:
		targetRect = CRect(CPoint(wndRect.left, wndRect.bottom - 1),
			wndRect.BottomRight());
		break;

	case AS_RANDOM:
		x = wndRect.left + rand() % wndRect.Width();
		y = wndRect.top + rand() % wndRect.Height();
		targetRect = PtToRect(x, y);
		break;

	default:
		targetRect = PtToRect(m_ptRefer);
		break;
	}
	
	return targetRect;
}

void CAniDialog::SetDispStyle(WORD dispStyle)
{
	m_wDispStyle = dispStyle;
}

WORD CAniDialog::GetDispStyle() const
{
	return m_wDispStyle;
}
