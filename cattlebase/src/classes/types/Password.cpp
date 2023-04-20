#include "stdafx.h"
#include "Password.h"

CPassword::CPassword() : CString()
{
}
CPassword::CPassword(LPCSTR passStr,bool bEncrypted/* = false*/) : CString(passStr)
{
	if(!bEncrypted) Encrypt();
	
}
void CPassword::Encrypt()
{
  /*unsigned long aulDigest[ 4 ];  struct MD5Context stMD5;
   MD5Init( &stMD5 );
   MD5Update( &stMD5, ( unsigned char * )GetBuffer(), GetLength() );
   MD5Final( ( unsigned char * )aulDigest, &stMD5 );   Format("%08lx%08lx%08lx%08lx",aulDigest[ 0 ], aulDigest[ 1 ], aulDigest[ 2 ], aulDigest[ 3 ] );*/

}
CPassword& CPassword::operator=(const CString& right)
{
	CString::operator =(right);
	return *this;
}
