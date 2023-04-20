#ifndef __ZEBRAPOS_H__
#define __ZEBRAPOS_H__

//#include "zebraposprinter.h"
#include "../Mustache/Mustache.h"

class ZebraPOS
{
public:
	//ZebraPOS();
	//~ZebraPOS();
	
	//void Init();
	
	void Print(const CString& sEAN);
	
	//void Clean();
protected:
//	void throwIfNotSucceeded(HRESULT hResult);
//	void throwIfError(long lResult);
	MustacheVariables CreateTemplateVariables(const CString& sEAN);
	void throwIfFailed(const CString& sTaskName,bool bResult);
	
private:
//	IZebraPOSPrinter* m_pZebraPOS;
	
	

};

#endif