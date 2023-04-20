#include "stdafx.h"
#include "LatitudeEdit.h"
#include "../classes/types/Latitude.h"


CLatitudeEdit::CLatitudeEdit()
{
}
bool CLatitudeEdit::IsValidGeoCoordinate()
{
	if(IsInputEmpty())
		return true;

	if(CGeoCoordinateEdit::IsValidGeoCoordinate())
	{
		Latitude latitue;
		unsigned short degrees;
		unsigned short minutes;
		unsigned short seconds;
		Hemisphere hemisphere = NullHemisphere();
		if(GetGeoCoordinate(degrees,minutes,seconds,hemisphere))
		{
			return latitue.Set(degrees,minutes,seconds,hemisphere);
		}
	}
	return false;
}
void CLatitudeEdit::SetLatitude(const  Latitude& latitude)
{
	if(latitude.IsNull())
	{
		EmptyData(TRUE);
	}
	else
	{
		SetGeoCoordinate(latitude);
	}
}
Latitude CLatitudeEdit::GetLatitude() const
{
	Latitude latitude;
	if(!IsInputEmpty() && IsFullData())
	{	
		unsigned short degrees;
		unsigned short minutes;
		unsigned short seconds;
		Hemisphere hemisphere = NullHemisphere();
		if(GetGeoCoordinate(degrees,minutes,seconds,hemisphere))
		{
			latitude.Set(degrees,minutes,seconds,hemisphere);
		}
	}
	return latitude;
}

CString CLatitudeEdit::GetValidationErrorMessage() const
{
	Latitude minLat,maxLat;
	minLat.SetAngle(minLat.GetMinAngle());
	maxLat.SetAngle(maxLat.GetMaxAngle());

	return "Wpisz wartoœæ od: " + minLat.ToString() + " do " + maxLat.ToString();

}
CString CLatitudeEdit::GetGeoCoordHint() const
{
	return "90N/S";
}
