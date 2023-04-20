#include "stdafx.h"
#include "SeikoUSBPrinter.h"
#include "../../log.h"

CSeikoUSBPrinter::CSeikoUSBPrinter(const CString& sPrinterName,const CString& sPrinterAddress,const CString& sDriverName) : 
		CSeikoPrinter(sPrinterName,sPrinterAddress,PRINTER_USB,sDriverName),m_hDLL(NULL), m_dwSessionId(0)
{
	if( m_hDLL = LoadLibrary( "SII_DPUS_API.DLL" ))
	{
		if(!(	PFN_OPEN_PRINTER			&&	
				PFN_GET_PRINTER_DATA		&&
				PFN_CLOSE_PRINTER			&&
				PFN_SET_PRINTER_DATA		&&
				PFN_SET_PRINTER_RESET		&&
				PFN_GET_PRINTER_STATUS		&&
				PFN_SET_CALLBACK_STATUS		))
		{
			LOG("SEIKO USB PRINTER DRIVER DLL SEEMS TO BE INVALID");
		}
	}
	else
	{
		LOG("FAILD TO LOAD SEIKO USB PRINTER DRIVER DLL");
	}
}
CSeikoUSBPrinter::~CSeikoUSBPrinter()
{
	if( m_dwSessionId )
	{
		PFN_CLOSE_PRINTER( m_dwSessionId );
		m_dwSessionId = 0;
	}

	if(m_hDLL)
		FreeLibrary( m_hDLL );
}
void CSeikoUSBPrinter::DoPrint(CBaseDocView* pDocView,UINT nNumOfCopeis)
{
	if( !m_dwSessionId )
	{
		DWORD dwOpenPrinterResult = PFN_OPEN_PRINTER( (LPCSTR)GetPrinterName(), &m_dwSessionId );
		if(dwOpenPrinterResult != 0)
		{
			LOG("FAIL TO OPEN SEIKO USB PRINTER: %s.",(LPCSTR)GetPrinterName());
			return;
		}
	}
	CSeikoPrinter::DoPrint(pDocView,nNumOfCopeis);
}

DWORD CSeikoUSBPrinter::WritePrinter(BYTE *pBuffer,DWORD bytesToWrite)
{
	DWORD	cbWritten	= 0;
	if(m_dwSessionId)
	{
		PFN_SET_PRINTER_DATA( m_dwSessionId, pBuffer, bytesToWrite, &cbWritten );
		return cbWritten;
	}
	return (DWORD)(-1);
	
}



