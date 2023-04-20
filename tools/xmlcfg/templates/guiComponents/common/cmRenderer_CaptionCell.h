// cmRenderer_CaptionCell.h
//
#pragma once

#include "cmCellBaseRenderer.h"
#include "cmFontInfo.h"

class cmRenderer_CaptionCell: public cmCellBaseRenderer
{
public:
	cmRenderer_CaptionCell();
	cmRenderer_CaptionCell(cmRenderer_CaptionCell& rend);
	virtual ~cmRenderer_CaptionCell();

	virtual void Draw(CDC* pDC, cmBaseCell* cell);

	void SetBkColor(COLORREF color);
	COLORREF GetBkColor() const;

	void SetTextColor(COLORREF color);
	COLORREF GetTextColor() const;

	void SetIconId(UINT icon);
	UINT GetIconId() const;

	void SetLeftMargin(int margin);
	int GetLeftMargin() const;

	virtual cmCellBaseRenderer& operator= (const cmRenderer_CaptionCell& rend);

protected:

	virtual cmCellBaseRenderer& Copy(const cmRenderer_CaptionCell& rend);
	virtual void DrawBackground(CDC* pDC, cmBaseCell* cell);
	virtual void DrawForeground(CDC* pDC, cmBaseCell* cell);

	COLORREF bkColor_;
	COLORREF txtColor_;

	UINT iconId_;
	int leftMargin_;
};
