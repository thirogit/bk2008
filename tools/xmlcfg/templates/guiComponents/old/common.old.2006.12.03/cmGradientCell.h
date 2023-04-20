// AsGradientCell.h: interface for the cmGradientCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CM_GRADIENTCELL_H_)
#define _CM_GRADIENTCELL_H_

#include "cmFontInfo.h"
#include "cmBaseCell.h"
#include "cmString.h"

class cmGradientCell: public cmBaseCell 
{
public:
	cmGradientCell();
	virtual ~cmGradientCell();

	virtual void Draw(CDC* pDC);

	void SetTopColor(COLORREF topColor);
	COLORREF GetTopColor() const;

	void SetMidTopColor(COLORREF midTopColor);
	COLORREF GetMidTopColor() const;

	void SetMidBottomColor(COLORREF midBottomColor);
	COLORREF GetMidBottomColor() const;

	void SetBottomColor(COLORREF bottomColor);
	COLORREF GetBottomColor() const;

protected:

	COLORREF topColor_;
	COLORREF midTopColor_;
	COLORREF midBottomColor_;
	COLORREF bottomColor_;
	
};

#endif // !defined(_CM_GRADIENTCELL_H_)
