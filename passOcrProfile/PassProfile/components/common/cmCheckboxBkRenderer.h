// cmCheckBoxBkRenderer.h: interface for the cmCheckboxBkRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCHECKBOXBKRENDERER_H_)
#define _CMCHECKBOXBKRENDERER_H_

#include "cmCellBkRenderer.h"

class cmCheckBoxBkRenderer : public cmCellBkRenderer  
{
public:
	cmCheckBoxBkRenderer();
	virtual ~cmCheckBoxBkRenderer();

	virtual void Draw(CDC* pDC, cmBaseCell* cell);

	void SetIcons(UINT uncheckedIconId, UINT checkedIconId);

protected:
	
	UINT uncheckedIconId_;
	UINT checkedIconId_;
};

#endif // !defined(_CMCHECKBOXBKRENDERER_H_)
