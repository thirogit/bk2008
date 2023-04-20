// cmCellFrRenderer.h: interface for the cmCellFrRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCELLFRRENDERER_H_)
#define _CMCELLFRRENDERER_H_

#include "cmCellBaseRenderer.h"
#include "cmFontInfo.h"

class cmBaseCell;

class cmCellFrRenderer : public cmCellBaseRenderer  
{
public:
	cmCellFrRenderer();
	virtual ~cmCellFrRenderer();

	virtual void Draw(CDC* pDC, cmBaseCell* cell);

	void SetTextColor(COLORREF color);
	COLORREF GetTextColor() const;

	cmFontInfo& GetFontInfo();

protected:
	
	COLORREF txtColor_;
	cmFontInfo fontInfo_;
};

#endif // !defined(_CMCELLFRRENDERER_H_)
