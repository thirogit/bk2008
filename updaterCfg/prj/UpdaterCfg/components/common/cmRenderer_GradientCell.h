// cmRenderer_CaptionCell.h
//
#pragma once

#include "cmRenderer_CaptionCell.h"
#include "cmFontInfo.h"

class cmRenderer_GradientCell: public cmRenderer_CaptionCell
{
public:
	cmRenderer_GradientCell();
	cmRenderer_GradientCell(const cmRenderer_GradientCell& rend);
	virtual ~cmRenderer_GradientCell();

	void SetGradientTopColor(COLORREF color);
	COLORREF GetGradientTopColor() const;

	void SetGradientBottomColor(COLORREF color);
	COLORREF GetGradientBottomColor() const;
	
	void operator= (const cmRenderer_GradientCell& rend);

protected:

	virtual void Copy(const cmRenderer_GradientCell& rend);
	virtual void DrawBackground(CDC* pDC, cmBaseCell* cell);
	
	COLORREF gradientTopColor_;
	COLORREF gradientBottomColor_;
};
