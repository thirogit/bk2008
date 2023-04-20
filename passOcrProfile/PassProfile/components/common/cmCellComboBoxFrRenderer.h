// cmCellComboBoxFrRenderer.h: interface for the cmCellComboBoxFrRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCELLCOMBOBOXFRRENDERER_H_)
#define _CMCELLCOMBOBOXFRRENDERER_H_


#include "cmCellFrRenderer.h"

class cmCellComboBoxFrRenderer : public cmCellFrRenderer  
{
public:
	cmCellComboBoxFrRenderer();
	virtual ~cmCellComboBoxFrRenderer();

	virtual void Draw(CDC* pDC, cmBaseCell* cell);
};

#endif // !defined(_CMCELLCOMBOBOXFRRENDERER_H_)
