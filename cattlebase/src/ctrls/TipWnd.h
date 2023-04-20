#ifndef __TIPWND_H__
#define __TIPWND_H__

class CTipWnd : public CWnd
{
public:
	CTipWnd();	
	~CTipWnd();
	
protected:
	
	COLORREF m_rgbTextColour;
	COLORREF m_rgbBorderOuter;
	COLORREF m_rgbBorderInner;
	COLORREF m_rgbBackgroundTop;
	COLORREF m_rgbBackgroundBottom;
	
	CFont m_fntTitleFont;

	bool m_bShowing;
	CSize m_targetSize;
	CSize m_currentSize;
	int m_lastDivRemainder;

		
		

	BOOL RegisterWndClass(HINSTANCE hInstance);
public:
	BOOL Create( CWnd* pParent );
	BOOL Show( CPoint& pPoint, LPCTSTR lpszItemText);	
	BOOL Hide();
	void SetColors(COLORREF inner,COLORREF outer);
	
protected:
	UINT HitTest(CPoint &ptClient);
	void OnMouseMessage(UINT message,UINT ncMessage,UINT nFlags,CPoint point);
	
	
	
	
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nTimerID);


	DECLARE_MESSAGE_MAP();

};
#endif