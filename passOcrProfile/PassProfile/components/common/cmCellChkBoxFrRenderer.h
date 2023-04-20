// cmCellChkBoxFrRenderer.h: interface for the cmCellChkBoxFrRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCELLCHKBOXFRRENDERER_H_)
#define _CMCELLCHKBOXFRRENDERER_H_


#include "cmCellFrRenderer.h"

class cmCellChkBoxFrRenderer : public cmCellFrRenderer  
{
public:
	cmCellChkBoxFrRenderer();
	virtual ~cmCellChkBoxFrRenderer();

	virtual void Draw(CDC* pDC, cmBaseCell* cell);
};

#endif // !defined(_CMCELLCHKBOXFRRENDERER_H_)
