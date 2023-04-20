// cmCellBaseRenderer.h: interface for the cmCellBaseRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCELLBASERENDERER_H_)
#define _CMCELLBASERENDERER_H_

#include "cmFontInfo.h"

class cmBaseCell;

class cmCellBaseRenderer : public CObject 
{
public:
	cmCellBaseRenderer();
	cmCellBaseRenderer(const cmCellBaseRenderer& rend);
	virtual ~cmCellBaseRenderer();

	virtual void Draw(CDC* pDC, cmBaseCell* cell) = 0;
	virtual void CleanUp(){};

	cmFontInfo& GetFontInfo(){return fontInfo_;}

	virtual cmCellBaseRenderer& operator=(const cmCellBaseRenderer& rend);

protected:
	virtual cmCellBaseRenderer& Copy(const cmCellBaseRenderer& rend);

	cmFontInfo fontInfo_;
};

#endif // !defined(_CMCELLBASERENDERER_H_)
