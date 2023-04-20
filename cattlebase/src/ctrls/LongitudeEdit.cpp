#include "stdafx.h"
#include "LongitudeEdit.h"
#include "../classes/types/Longitude.h"


CLongitudeEdit::CLongitudeEdit()
{
}
bool CLongitudeEdit::IsValidGeoCoordinate()
{
	if(IsInputEmpty())
		return true;

	if(CGeoCoordinateEdit::IsValidGeoCoordinate())
	{
		Longitude longitude;
		unsigned short degrees;
		unsigned short minutes;
		unsigned short seconds;
		Hemisphere hemisphere = NullHemisphere();
		if(GetGeoCoordinate(degrees,minutes,seconds,hemisphere))
		{
			return longitude.Set(degrees,minutes,seconds,hemisphere);
		}
	}
	return false;
}



void CLongitudeEdit::SetLongitude(const  Longitude& longitude)
{
	if(longitude.IsNull())
	{
		EmptyData(TRUE);
	}
	else
	{
		SetGeoCoordinate(longitude);
	}
}

Longitude CLongitudeEdit::GetLongitude() const
{
	Longitude longitude;
	if(!IsInputEmpty() && IsFullData())
	{	
		unsigned short degrees;
		unsigned short minutes;
		unsigned short seconds;
		Hemisphere hemisphere = NullHemisphere();
		if(GetGeoCoordinate(degrees,minutes,seconds,hemisphere))
		{
			longitude.Set(degrees,minutes,seconds,hemisphere);
		}
	}
	return longitude;
}

CString CLongitudeEdit::GetValidationErrorMessage() const
{
	Longitude minLong,maxLong;
	minLong.SetAngle(minLong.GetMinAngle());
	maxLong.SetAngle(maxLong.GetMaxAngle());

	return "Wpisz wartoœæ od: " + minLong.ToString() + " do " + maxLong.ToString();

}
CString CLongitudeEdit::GetGeoCoordHint() const
{
	return "180W/E";
}
