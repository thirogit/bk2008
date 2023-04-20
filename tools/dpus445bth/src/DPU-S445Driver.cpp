#include "stdafx.h"
#include "DPU-S445Driver.h"


#define	DLL_FILE_NAME		TEXT("SII_DPUS_API.DLL")


/*	Define function macros		*/
#if defined(UNICODE)
#define PFN_OPEN_PRINTER		((F_OpenSiiPrinter)GetProcAddress( hModule, "OpenSiiPrinterW"))
#define PFN_GET_PRINTER_DATA	((F_GetSiiPrinterData)GetProcAddress( hModule, "GetSiiPrinterDataW"))
#else
#define PFN_OPEN_PRINTER		((F_OpenSiiPrinter)GetProcAddress( hModule, "OpenSiiPrinterA"))
#define PFN_GET_PRINTER_DATA	((F_GetSiiPrinterData)GetProcAddress( hModule, "GetSiiPrinterDataA"))
#endif

#define PFN_CLOSE_PRINTER		((F_CloseSiiPrinter)GetProcAddress( hModule, "CloseSiiPrinter"))
#define PFN_SET_PRINTER_DATA	((F_SetSiiPrinterData)GetProcAddress( hModule, "SetSiiPrinterData"))
#define PFN_SET_PRINTER_RESET	((F_SetSiiPrinterReset)GetProcAddress( hModule, "SetSiiPrinterReset"))
#define PFN_GET_PRINTER_STATUS	((F_GetSiiPrinterAutoStatus)GetProcAddress( hModule, "GetSiiPrinterAutoStatus"))
#define PFN_SET_CALLBACK_STATUS	((F_SetSiiPrinterCallbackStatus)GetProcAddress( hModule, "SetSiiPrinterCallbackStatus"))


DPUS445Driver::DPUS445Driver() : m_hModule(NULL)
{
}

bool DPUS445Driver::LoadDriver()
{
	if(m_hModule == NULL && (m_hModule = LoadLibrary( (LPCTSTR)DLL_FILE_NAME )))
	{
		m_pfnOpenSiiPrinter = (F_OpenSiiPrinter)GetProcAddress( m_hModule, "OpenSiiPrinterA");
		m_pfnCloseSiiPrinter = (F_CloseSiiPrinter)GetProcAddress( m_hModule, "CloseSiiPrinter");
		m_pfnSetSiiPrinterData = (F_SetSiiPrinterData)GetProcAddress( m_hModule, "SetSiiPrinterData");	

		if(m_pfnOpenSiiPrinter && m_pfnCloseSiiPrinter && m_pfnSetSiiPrinterData)
			return true;
		else
		{
			UnloadDriver();
			return false;
		}
	}

	return false;
}

DWORD DPUS445Driver::OpenPrinterSession(LPCSTR lpszPrinterName)
{	
	DWORD dwSessionId = 0;
	
	if(m_hModule != NULL && m_pfnOpenSiiPrinter( lpszPrinterName, &dwSessionId ) == 0)
		return dwSessionId;

	return 0;
}

void DPUS445Driver::ClosePrinterSession(DWORD dwSession)
{
	if(m_hModule != NULL)
		m_pfnCloseSiiPrinter(dwSession);
}

int DPUS445Driver::SendToPrinter(DWORD dwSession,const BYTE* pBuffer,UINT uiBytesToWrite)
{
	if(dwSession != 0 && m_hModule != NULL)
	{
		DWORD dwWritten = 0;
		m_pfnSetSiiPrinterData( dwSession, pBuffer, uiBytesToWrite, &dwWritten );
		return (int)dwWritten;
	}
	return -1;
}

void DPUS445Driver::UnloadDriver()
{
	if(m_hModule != NULL)
		FreeLibrary( m_hModule );
}

	


			 



