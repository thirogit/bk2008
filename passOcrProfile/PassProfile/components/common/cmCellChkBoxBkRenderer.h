// cmCellChkBoxBkRenderer.h: interface for the cmCellChkBoxBkRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCELLCHKBOXBKRENDERER_H_)
#define _CMCELLCHKBOXBKRENDERER_H_

#include "cmCellBkRenderer.h"

class cmCellChkBoxBkRenderer : public cmCellBkRenderer  
{
public:
	cmCellChkBoxBkRenderer();
	virtual ~cmCellChkBoxBkRenderer();

	void Draw(CDC* pDC, cmBaseCell* cell);

	void SetIcons(UINT uncheckedIconId, UINT checkedIconId);

protected:
	
	UINT uncheckedIconId_;
	UINT checkedIconId_;
};

#endif // !defined(_CMCELLCHKBOXBKRENDERER_H_)
