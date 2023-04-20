// cmCellBkGradient.h: interface for the cmCellBkGradient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMCELLBKGRADIENT_H_)
#define _CMCELLBKGRADIENT_H_

#include "cmCellBkRenderer.h"

class cmCellBkGradient : public cmCellBkRenderer  
{
public:
	cmCellBkGradient();
	cmCellBkGradient(const cmCellBkGradient& rend);
	virtual ~cmCellBkGradient();
	
	virtual void Draw(CDC* pDC, cmBaseCell* cell);

	void SetGradientTopColor(COLORREF color);
	COLORREF GetGradientTopColor() const;

	void SetGradientBottomColor(COLORREF color);
	COLORREF GetGradientBottomColor() const;

	void SetGradientPercentage(int perc);
	int GetGradientPercentage() const;

	virtual cmCellBaseRenderer& operator=(const cmCellBaseRenderer& rend);

protected:

	virtual cmCellBaseRenderer& Copy(const cmCellBaseRenderer& rend);

	COLORREF gradientTopColor_;
	COLORREF gradientBottomColor_;
	float gradientPercentage_;
};

#endif // !defined(_CMCELLBKGRADIENT_H_)
