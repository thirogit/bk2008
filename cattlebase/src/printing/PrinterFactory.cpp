#include "stdafx.h"
#include "PrinterFactory.h"
#include "printers/SeikoUSBPrinter.h"
#include "printers/DYMOPrinter.h"
#include "printers/SeikoBluetoothPrinter.h"

CPrinter* CPrinterFactory::CreatePrinter(const CString& sDeviceName,const CString& sDriverName,const CString& sAddress)
{

	if(IsSeikoPrinter(sDriverName))
	{
		return new CSeikoUSBPrinter(sDeviceName,sAddress,sDriverName);
	}
	else if(IsDYMOPrinter(sDriverName))
	{
		return new CDYMOPrinter(sDeviceName,sAddress,sDriverName);
	}
	else if(IsSeikoBluetoothPrinter(sDeviceName))
	{
		return new CSeikoBluetoothPrinter(sDeviceName,sAddress,sDriverName);
	}
	return NULL;
}

bool  CPrinterFactory::IsDYMOPrinter(const CString& sDriverName)
{
	const char szDYMODriverPrefix[] = "DYMO";	
	return strncmp(szDYMODriverPrefix,sDriverName,strlen(szDYMODriverPrefix)) == 0;
}
bool  CPrinterFactory::IsSeikoPrinter(const CString& sDriverName)
{
	const char szDriverPrefix[] = "SII";
	return strncmp(szDriverPrefix,sDriverName,strlen(szDriverPrefix)) == 0;
}

bool CPrinterFactory::IsSeikoBluetoothPrinter(const CString& sDeviceName)
{
	const char szDeviceNamePrefix[] = "DPU-S445";
	return strncmp(szDeviceNamePrefix,sDeviceName,strlen(szDeviceNamePrefix)) == 0;

}

