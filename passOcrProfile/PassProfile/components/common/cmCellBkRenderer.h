// cmCellBkRenderer.h: interface for the cmCellBkRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCELLBKRENDERER_H_)
#define _CMCELLBKRENDERER_H_

#include "cmCellBaseRenderer.h"

class cmCellBkRenderer : public cmCellBaseRenderer  
{
public:
	cmCellBkRenderer();
	cmCellBkRenderer(const cmCellBkRenderer& rend);
	virtual ~cmCellBkRenderer();

	virtual void Draw(CDC* pDC, cmBaseCell* cell);

	void SetBkColor(COLORREF color);
	COLORREF GetBkColor() const;

	virtual cmCellBaseRenderer& operator=(const cmCellBaseRenderer& rend);

protected:

	virtual cmCellBaseRenderer& Copy(const cmCellBaseRenderer& rend);

	COLORREF bkColor_;
};

#endif // !defined(_CMCELLBKRENDERER_H_)
