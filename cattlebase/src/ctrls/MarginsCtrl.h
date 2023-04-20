#ifndef __MARGINSCTRL_H__
#define __MARGINSCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMarginsCtrl : public CStatic
{
// Construction
public:
	CMarginsCtrl();
	virtual ~CMarginsCtrl();

	virtual void PreSubclassWindow();

	bool SetOffX(int sOffX);
	bool SetOffY(int sOffY);

	inline int GetOffX(void) { return OffX; };
	inline int GetOffY(void) {return OffY ; };

// Attributes
protected:
	CDC* CreatePrinterDC();   


	CSliderCtrl *m_sliderH;
	CSliderCtrl *m_sliderV;
	RECT m_pageRect;

	RECT RtMinMarginMMs;
	SIZE PtPaperSizeMMs;


	int OffX;
	int OffY;

	void PrintPageSetupMargins(HDC hDC,RECT &drawRect);
	BOOL PosCtrl();
	BOOL CreateControls();

protected:
	afx_msg void OnPaint() ;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
};


#endif

