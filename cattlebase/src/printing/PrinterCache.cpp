#include "stdafx.h"
#include "PrinterCache.h"
#include <winspool.h>
#include "PrinterFactory.h"
#include "../BtInterface/BtInterface.h" 
#include "../BtInterface/Bluetooth.h"

CPrintersCache::CPrintersCache()
{
	

			
}

CPrintersCache::~CPrintersCache()
{	
	m_cache.FreeAndFlush();	
}


void CPrintersCache::Refresh()
{
	m_cache.FreeAndFlush();
	EnumLocalPrinters();
	EnumBTPrinters();
}
int CPrintersCache::PrintersCount() const
{
	return m_cache.GetSize();
}
PtrFlushArray<CPrinter>::ArrayIterator CPrintersCache::GetIterator()
{
	return m_cache.iterator();
}


void CPrintersCache::EnumLocalPrinters()
{
	BYTE		*pPrinter;
 	DWORD		cbNeedPr, cbReturnPr;
	DWORD		dwIndex;
	
	//Get size of structure
 	EnumPrinters( PRINTER_ENUM_LOCAL, NULL,	2, NULL, 0, &cbNeedPr, &cbReturnPr );

	//Get printer info
 	pPrinter = (PBYTE) new BYTE[ cbNeedPr ];
	if( pPrinter )
	{
		if( EnumPrinters( PRINTER_ENUM_LOCAL, NULL, 2, pPrinter, cbNeedPr, &cbNeedPr, &cbReturnPr ) )
		{
			PRINTER_INFO_2* pPrinterInfo2 = (PRINTER_INFO_2 *) pPrinter;
			for( dwIndex=0 ; dwIndex < cbReturnPr; dwIndex++, pPrinterInfo2++ ) 
			{
				CPrinter* pPrinter = CPrinterFactory::CreatePrinter(pPrinterInfo2->pPrinterName,
																	pPrinterInfo2->pDriverName,
																	pPrinterInfo2->pPortName);
				if(pPrinter)
				{
					m_cache.Add(pPrinter);
				}
 			}
		}
		delete [] pPrinter;
	}
}

void CPrintersCache::EnumBTPrinters()
{
	CBluetooth bt;
	if(	bt.InitBluetooth())
	{
		BtDeviceIterator btIt = bt.EnumBtPrinters();
		while(btIt.Next())
		{
			BtDevice btDevice = *btIt;
			CPrinter* pPrinter = CPrinterFactory::CreatePrinter(btDevice.GetDeviceName(),"BT",btDevice.GetDeviceBtAddress());
			if(pPrinter)
			{
				m_cache.Add(pPrinter);
			}
		}
	}

		
}