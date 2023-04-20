#include "stdafx.h"
#include "StringEncoder.h"

	
StringEncoder::StringEncoder(const CString& sFromEncoding,const CString& toEncoding)
{
	m_ConversionDescriptor = iconv_open(toEncoding,sFromEncoding);
}

CString StringEncoder::Encode(const CString& s)
{
	CString sEncoded;
	int sLen = s.GetLength();
	int maxEncodedLen = sLen*2; 

	LPSTR pEncodingBuffer = sEncoded.GetBuffer(maxEncodedLen + 1);
	LPCSTR pToEncode = (LPCSTR)s;

	size_t inbufleft = sLen;
	size_t outbufleft = maxEncodedLen;
	LPSTR outbuf = pEncodingBuffer;

	size_t result = iconv(m_ConversionDescriptor,&pToEncode,&inbufleft,&outbuf,&outbufleft);

	pEncodingBuffer[maxEncodedLen-outbufleft] = 0;
	sEncoded.ReleaseBuffer();

	return sEncoded;
		
}
StringEncoder::~StringEncoder()
{
	iconv_close(m_ConversionDescriptor);
}
	
