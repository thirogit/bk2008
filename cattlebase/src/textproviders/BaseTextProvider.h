#ifndef __BASETEXTPROVIDER_H__
#define __BASETEXTPROVIDER_H__

template<class DATA>
class BaseTextProvider
{
public:
	virtual CString GetDataText(DATA *pData,UINT memberID) = 0;
	

};

#endif