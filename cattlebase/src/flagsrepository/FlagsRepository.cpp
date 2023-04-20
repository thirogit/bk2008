#include "stdafx.h"
#include "FlagsRepository.h"


const _iso3166number2flagimageindex CFlagsRepository::_flagsImagesMap[] =
{
	{0	,0},
	{20 ,1}, 
	{51 ,2}, 
	{32 ,3}, 
	{31 ,4}, 
	{46 ,5}, 
	{100,6}, 
	{112,7}, 
	{756,8}, 
	{891,9}, 
	{196,10},
	{203,11},
	{276,12},
	{208,13},
	{233,14},
	{724,15},
	{246,16},
	{250,17},
	{826,18},
	{300,19},
	{239,20},
	{191,21},
	{348,22},
	{372,23},
	{352,24},
	{380,25},
	{398,26},
	{438,27},
	{440,28},
	{442,29},
	{428,30},
	{492,31},
	{807,32},
	{370,33},
	{528,34},
	{578,35},
	{616,36},
	{620,37},
	{642,38},
	{643,39},
	{752,40},
	{705,41},
	{703,42},
	{674,43},
	{792,44},
	{804,45},
	{(UINT)-1,(UINT)-1},
};        


CFlagsRepository::CFlagsRepository()
{
	m_ImageList.Create(16,16,ILC_COLORDDB | ILC_MASK,22,10);
	CBitmap bmp;
	bmp.LoadBitmap(IDB_FLAGS);
	m_ImageList.Add(&bmp,RGB(255,0,255));
}

CImageList& CFlagsRepository::GetFlagsImageList()
{
	return m_ImageList;
}

UINT CFlagsRepository::GetFlagImageIndex(UINT isoCountryNumber)
{
	const _iso3166number2flagimageindex* pFlagImgMaping = _flagsImagesMap;
	
	while(pFlagImgMaping->flagimgindex != ((UINT)-1) && pFlagImgMaping->countryisonumber != ((UINT)-1) )
	{
		if(pFlagImgMaping->countryisonumber == isoCountryNumber)
			return pFlagImgMaping->flagimgindex;
		pFlagImgMaping++;
	}
	return (UINT)-1;

}

