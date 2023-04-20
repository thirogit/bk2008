// cmGlassButton.h
//

#pragma once

#include "cmBaseCell.h"
#include "cmRenderer_GlassCell.h"

class cmGlassButton  : public CButton
{

public:
	
	

	cmGlassButton();
	virtual ~cmGlassButton();
	
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void SetBaseColor(COLORREF color);
	
	virtual void SetCaptionColor(COLORREF color);
	virtual void EnableCaptionShadow(bool e);
	virtual void EnableBorder(bool e);

	virtual cmFontInfo& GetFontInfo();

protected:

	enum enButtonStatus 
	{
		enButtonStatus_Normal,
		enButtonStatus_Hover,
		enButtonStatus_Down
	};


	BOOL IsInRect(CRect &rect, CPoint &point);

	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message );

	DECLARE_MESSAGE_MAP()

	
	enButtonStatus status_;
	enButtonStatus prevStatus_;

	void SetStatus(enButtonStatus status);
	BOOL OnEraseBkgnd(CDC* pDC);

	COLORREF baseColor_;
	COLORREF captionColor_;
	cmRenderer_GlassCell renderer_;
	cmBaseCell cell_;
	bool captionShadowEnabled_;
};

