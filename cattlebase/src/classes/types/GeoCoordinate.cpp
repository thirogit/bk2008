#include "stdafx.h"
#include "GeoCoordinate.h"
//#include "../StringFormat.h"
#include <limits>
#include <float.h>

#define NORTH_HEMISPHERE_CHAR 'N'
#define SOUTH_HEMISPHERE_CHAR 'S'
#define WEST_HEMISPHERE_CHAR 'W'
#define EAST_HEMISPHERE_CHAR 'E'


Hemisphere::Hemisphere(int sign,char suffix) : m_sign(sign),m_suffix(suffix)
{
}

Hemisphere::Hemisphere(const Hemisphere& copy)
{
	CopyFrom(copy);
}


Hemisphere& Hemisphere::operator=(const Hemisphere& right)
{
	CopyFrom(right);
	return *this;
}

void Hemisphere::CopyFrom(const Hemisphere& right)
{
	m_sign = right.m_sign;
	m_suffix = right.m_suffix;
}


int Hemisphere::GetSign() const
{
	return m_sign;
}
char Hemisphere::GetSuffix() const
{
	return m_suffix;
}
/////////////////////////////////////////////////////////////////////////////////////////

Hemisphere HemisphereFactory::GetHemisphere(char cHemisphere)
{

	switch( cHemisphere )
	{
		case NORTH_HEMISPHERE_CHAR:
			return N();			
		case SOUTH_HEMISPHERE_CHAR:
			return S();
		case WEST_HEMISPHERE_CHAR:
			return W();
		case EAST_HEMISPHERE_CHAR:
			return E();
	}
	return NullHemisphere();
}

NullHemisphere::NullHemisphere() : Hemisphere(0,0)
{
}

N::N() : Hemisphere(1,NORTH_HEMISPHERE_CHAR)
{
}

S::S() : Hemisphere(-1,SOUTH_HEMISPHERE_CHAR)
{
}

E::E() : Hemisphere(1,EAST_HEMISPHERE_CHAR)
{
}

W::W() : Hemisphere(-1,WEST_HEMISPHERE_CHAR)
{
}

double operator*(const Hemisphere& hemi,double angle)
{
	return angle*hemi.GetSign();
}
double operator*(double angle,const Hemisphere& hemi)
{
	return angle*hemi.GetSign();
}


GeoCoordinate::GeoCoordinate()
{
	
}

GeoCoordinate::GeoCoordinate(double dAngle) : NullDouble(dAngle)
{
	Validate();
}

GeoCoordinate::GeoCoordinate(const GeoCoordinate& copy)
{
	CopyFrom(copy);
}

void GeoCoordinate::CopyFrom(const NullDouble& copy)
{
	NullDouble::CopyFrom(copy);
	Validate();
}

bool GeoCoordinate::Validate()
{
	if(!(m_dValue <= GetMaxAngle() && GetMinAngle() <= m_dValue))
	{
		MakeNull();
		return false;
	}
	return true;
}

bool GeoCoordinate::Set(unsigned short degrees,unsigned short minutes,unsigned short seconds,const Hemisphere& hemisphere)
{
	int hemisign = hemisphere.GetSign();
	if(!(hemisign == -1 || hemisign == 1)) return false;
	SetAngle((degrees + minutes/60.0 + seconds/3600.0)*hemisphere);
	return Validate();
}

void GeoCoordinate::SetAngle(double dAngle)
{
	SetDouble(dAngle);
	Validate();
}

double  GeoCoordinate::GetAngle(double dIfNull/* = 0.0*/) const
{
	return GetDouble();
}


CString GeoCoordinate::ToString(LPCSTR ifNull/* = NULL*/,unsigned int precision/* = NULLDOUBLE_DEFAULT_PREC*/) const
{
	CString sGeoCoord;

	if(IsNull())
	{
		if(ifNull != NULL)
			sGeoCoord = ifNull;
	}
	else
	{
		sGeoCoord.Format("%03d\xB0 %02d' %02d\" %c",GetDegrees(),GetMinutes(),GetSeconds(),GetHemisphere().GetSuffix());

	}
	return sGeoCoord;
}

unsigned short GeoCoordinate::GetDegrees() const
{
	double degreePart;
	modf (m_dValue , &degreePart);
	return (int)abs(degreePart);
}
unsigned short GeoCoordinate::GetMinutes() const
{
	double fractpart, intpart;
	fractpart = modf (m_dValue , &intpart);
	return ((int)abs((fractpart*3600.0)))/60;

}
unsigned short GeoCoordinate::GetSeconds() const
{
	double fractpart, intpart;
	fractpart = modf (m_dValue , &intpart);
	return ((int)abs((fractpart*3600.0)))%60;
}

bool GeoCoordinate::Equals(const GeoCoordinate& geoCoord) const
{
	if(IsNull() && geoCoord.IsNull()) return true;
	if(geoCoord.IsNull() || IsNull()) return false;

	double sub = GetAngle() - geoCoord.GetAngle();
	return (sub <= (1/3600.0));

}

