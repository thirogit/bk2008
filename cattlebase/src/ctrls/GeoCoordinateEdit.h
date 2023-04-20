#ifndef __GEOCOORDINATEEDIT_H__
#define __GEOCOORDINATEEDIT_H__

#include "tipedit.h"
#include "../classes/types/GeoCoordinate.h"


class CGeoCoordinateEdit : public CTipEdit
{
protected:
	CGeoCoordinateEdit();
	virtual bool IsValidGeoCoordinate();
	void SetGeoCoordinate(const GeoCoordinate& geoCoord);
	bool GetGeoCoordinate(unsigned short& degrees,unsigned short& minutes,unsigned short& seconds,Hemisphere& hemisphere) const;

protected:	
	virtual BOOL OnValidate();
	virtual void PrepareToolTip(CWnd *pParent);
	virtual CString GetValidationErrorMessage() const = 0;
	virtual CString GetGeoCoordHint() const = 0;


	void DrawGetCoordHint();


protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd) ;

	DECLARE_MESSAGE_MAP()

};


#endif