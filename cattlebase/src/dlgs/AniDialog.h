// AniDialog.h: interface for the CAniDialog class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ANIDIALOG_H__
#define __ANIDIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAniDialog : public CDialog  
{
public:
	
// pre-define animation styles

// animation styles
#define AS_REFER		0 // animate from/to the refer point
#define AS_CENTER		1 // animate from/to center
#define AS_TOPLEFT		2 // animate from/to corners
#define AS_TOPRIGHT		3
#define AS_BOTTOMLEFT	4
#define AS_BOTTOMRIGHT	5
#define AS_LEFT			6 // animate from/to sides
#define AS_TOP			7
#define AS_RIGHT		8
#define AS_BOTTOM		9
#define AS_RANDOM		10 // animate from/to a random point inside of dialog

// define displaying style
#define DS_WIRE			0
#define DS_CAPTION		1

// default animation data
#define DEF_MILLSEC	10
#define DEF_FRAMES	25

	// construction
	CAniDialog(UINT nDlgID, CWnd* pParent);
	CAniDialog(CPoint pt, UINT nDlgID, CWnd* pParent);

	// operation

	// access frame number
	UINT GetFrameNum() const;
	void SetFrameNum(UINT nFrames);

	// access animation speed
	UINT GetAniSpeed() const;
	void SetAniSpeed(UINT nMillSec);

	// access the reference point
	CPoint GetRefPt() const;
	void SetRefPt(CPoint pt);

	// access animation styles
	WORD GetOpenStyle() const;
	WORD GetCloseStyle() const;
	void SetAniStyles(WORD wOpenStyle, WORD wCloseStyle);
	WORD GetDispStyle() const;
	void SetDispStyle(WORD dispStyle);

	// temporarily enable/disable animation
	void EnableAni(BOOL bEnable = TRUE);

	// check object status
	BOOL IsAniValid() const;

protected:

	// handlers for message WM_CREATE and WM_DESTROY

	// Generated message map functions
	//{{AFX_MSG(CTestAniDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CWnd* m_pParent; // parent Wnd
	BOOL m_bAni; // animation enable/disable flag
	CPoint m_ptRefer; // the reference point
	UINT m_nMillSec; // animation speed
	UINT m_nFrames; // animation frame number
	WORD m_wOpenStyle; // open animation style
	WORD m_wCloseStyle; // close animation style
	WORD m_wDispStyle; // whether to use wire animation

	// calculate rect positions and sizes for animation
	CRect PtToRect(CPoint pt) const;
	CRect PtToRect(int x, int y) const;
	CRect CalcTargetRect(CRect wndRect, WORD wStyle) const;

	// Almond and Maunder's function: DrawWireRects
///////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	DrawWireRects
//
// DESCRIPTION:	Creates exploding wire rectanges
//
//  INPUTS: LPRECT lprcFrom      Source Rectangle
//          LPRECT lprcTo        Destination Rectangle
//          UINT nMilliSecSpeed  Speed in millisecs for animation
//
//	RETURN:	None
//	NOTES:	None
//
//  Maintenance Log
//  Author      Date    Version     Notes
//  NT Almond   011199  1.0         Origin
//  CJ Maunder  010899  1.1         Modified rectangle transition code
//
////////////////////////////////////////////////////////////////////////////////

// Original function was delevoped by Almond and Maunder, however,
// a little modification was needed to be done to make this function cooperate
// with other class members. I tried to keep the function as
// unchanged as possible.
	static void DrawWireRects(LPRECT lprcFrom, LPRECT lprcTo,
		UINT nMilliSecSpeed = DEF_MILLSEC, UINT nFrames = DEF_FRAMES);
	
};

#endif // ifndef __ANIDIALOG_H__