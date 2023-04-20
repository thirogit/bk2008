// cmCheckBoxFrRenderer.h: interface for the cmCheckBoxFrRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCHECKBOXFRRENDERER_H_)
#define _CMCHECKBOXFRRENDERER_H_

#include "cmCellFrRenderer.h"

class cmCheckBoxFrRenderer : public cmCellFrRenderer  
{
public:
	cmCheckBoxFrRenderer();
	virtual ~cmCheckBoxFrRenderer();

	virtual void Draw(CDC* pDC, cmBaseCell* cell);
};

#endif // !defined(_CMCHECKBOXFRRENDERER_H_)
