// cmOptionGroup_Style1.h
//

#pragma once

#include "cmOptionGroup.h"
#include "cmRendererFactory.h"

class cmRenderer_GradientCell;

class cmOptionGroup_Style1: public cmOptionGroup
{
public:
	cmOptionGroup_Style1();
	virtual ~cmOptionGroup_Style1();

	virtual void Draw(CDC* pDC, const CPoint& mousePos);

	virtual cmOptionRow* AddRow(const cmString& name, const cmString& caption, enCmCellType valueCellType);

	void SetHeaderBkColor(COLORREF color, bool redraw = true);
	COLORREF GetHeaderBkColor() const;

	void SetHeaderTextColor(COLORREF color, bool redraw = true);
	COLORREF GetHeaderTextColor() const;

	void SetHeaderShadow(bool shadowEnabled, bool redraw = true);
	bool GetHeaderShadow() const;

protected:

	virtual void UpdateHeaderDrawArea(const CRect& drawArea);
	virtual void DrawExpandButton(CDC* pDC);
	virtual void DrawGrid(CDC* pDC);

	virtual void InitializeHeaderLookAndFeel();
	virtual void InitializeCellLookAndFeel(cmBaseCell* cell);
	virtual void InitGlassRenderer(cmRenderer_GlassCell* rend);

	cmRendererFactory rendFactory_;

	COLORREF headerBkColor_;
	COLORREF headerTextColor_;
	bool headerTextShadow_;
};