#ifndef _PASSWORD_H_
#define _PASSWORD_H_

#include "md5.h"

class CPassword : public CString  
{
public:
	CPassword();
	CPassword(LPCSTR passStr,bool bEncrypted = false);
	void Encrypt();
	CPassword& operator=(const CString& right);

};

#endif