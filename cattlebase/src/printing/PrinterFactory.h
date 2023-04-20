#ifndef __PRINTERFACTORY_H__
#define __PRINTERFACTORY_H__

#include "printers/Printer.h"

class CPrinterFactory 
{
public:
	static CPrinter* CreatePrinter(const CString& sDeviceName,const CString& sDriverName,const CString& sAddress);
	static bool IsDYMOPrinter(const CString& sDriverName);
	static bool IsSeikoPrinter(const CString& sDriverName);
	static bool IsSeikoBluetoothPrinter(const CString& sDeviceName);
	
};

#endif