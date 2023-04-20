#ifndef __FLAGSREPOSITORY_H__
#define __FLAGSREPOSITORY_H__

#include "../consts.h"


struct _iso3166number2flagimageindex
{
	UINT countryisonumber;
	UINT flagimgindex;
};

class CFlagsRepository
{
public:
	CFlagsRepository();
	CImageList& GetFlagsImageList();
	UINT GetFlagImageIndex(UINT isoCountryNumber);
protected:
	
	CImageList m_ImageList;
	static const _iso3166number2flagimageindex _flagsImagesMap[];
	
};
#endif