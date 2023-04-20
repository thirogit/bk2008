#ifndef __LONGITUDEEDIT_H__
#define __LONGITUDEEDIT_H__

#include "GeoCoordinateEdit.h"
#include "../classes/types/Longitude.h"

class CLongitudeEdit : public CGeoCoordinateEdit
{
public:
	CLongitudeEdit();
	void SetLongitude(const  Longitude& longitude);
	Longitude GetLongitude() const;
	
protected:
	bool IsValidGeoCoordinate();
	virtual CString GetValidationErrorMessage() const;
	virtual CString GetGeoCoordHint() const;

	
};


#endif