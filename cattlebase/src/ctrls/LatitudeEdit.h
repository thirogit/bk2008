#ifndef __LATITUDEEDIT_H__
#define __LATITUDEEDIT_H__

#include "GeoCoordinateEdit.h"
#include "../classes/types/Latitude.h"

class CLatitudeEdit : public CGeoCoordinateEdit
{
public:
	CLatitudeEdit();
	void SetLatitude(const  Latitude& latitude);
	Latitude GetLatitude() const;
protected:
	bool IsValidGeoCoordinate();
	virtual CString GetValidationErrorMessage() const;
	virtual CString GetGeoCoordHint() const;

	
};


#endif