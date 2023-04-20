#ifndef __STRINGENCODER_H__
#define __STRINGENCODER_H__

#include <iconv.h>

class StringEncoder 
{
public:	
	StringEncoder(const CString& sFromEncoding,const CString& toEncoding);
	~StringEncoder();
	CString Encode(const CString& s);
private:
	iconv_t m_ConversionDescriptor;
};

#endif