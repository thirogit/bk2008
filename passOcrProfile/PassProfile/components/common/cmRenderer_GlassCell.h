// cmRenderer_GlassCell.h
//
#pragma once

#include "cmRenderer_CaptionCell.h"
#include "cmFontInfo.h"

class cmRenderer_GlassCell: public cmRenderer_CaptionCell
{
public:
	cmRenderer_GlassCell();
	cmRenderer_GlassCell(const cmRenderer_GlassCell& rend);
	virtual ~cmRenderer_GlassCell();
	
	void SetBorderEnable(bool enable = true);
	bool GetBorderEnable() const;

	void operator= (const cmRenderer_GlassCell& rend);

	void SetTextFlags(DWORD flags);
	DWORD GetTextFlags() const;

	virtual void EnableTextShadow(bool e);

protected:

	virtual void Copy(const cmRenderer_GlassCell& rend);
	virtual void DrawBackground(CDC* pDC, cmBaseCell* cell);
	virtual void DrawForeground(CDC* pDC, cmBaseCell* cell);

	DWORD textFlags_;
	bool textShadowEnabled_;
	bool borderEnabled_;
};