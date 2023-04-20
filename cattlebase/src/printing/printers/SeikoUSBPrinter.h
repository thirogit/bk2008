#ifndef __SEIKOUSBPRINTER_H__
#define __SEIKOUSBPRINTER_H__

#include "SeikoPrinter.h"

/*	Define callback finction	*/
typedef INT (CALLBACK EXPORT *FNC_CB_STATUS)(DWORD);

/*	Define finction type		*/
typedef DWORD (WINAPI *F_OpenSiiPrinter)(LPCTSTR, LPDWORD);
typedef DWORD (WINAPI *F_CloseSiiPrinter)(DWORD);
typedef DWORD (WINAPI *F_GetSiiPrinterData)(DWORD,LPTSTR,LPBYTE,DWORD,LPDWORD);
typedef DWORD (WINAPI *F_SetSiiPrinterData)(DWORD,LPBYTE,DWORD,LPDWORD);
typedef DWORD (WINAPI *F_SetSiiPrinterReset)(DWORD);
typedef DWORD (WINAPI *F_GetSiiPrinterAutoStatus)(DWORD,LPDWORD);
typedef DWORD (WINAPI *F_SetSiiPrinterCallbackStatus)(DWORD,FNC_CB_STATUS);
			 

/*	Define function macros		*/
#if defined(UNICODE)
#define PFN_OPEN_PRINTER		((F_OpenSiiPrinter)GetProcAddress( hModule, "OpenSiiPrinterW"))
#define PFN_GET_PRINTER_DATA	((F_GetSiiPrinterData)GetProcAddress( hModule, "GetSiiPrinterDataW"))
#else
#define PFN_OPEN_PRINTER		((F_OpenSiiPrinter)GetProcAddress( m_hDLL, "OpenSiiPrinterA"))
#define PFN_GET_PRINTER_DATA	((F_GetSiiPrinterData)GetProcAddress( m_hDLL, "GetSiiPrinterDataA"))
#endif

#define PFN_CLOSE_PRINTER		((F_CloseSiiPrinter)GetProcAddress( m_hDLL, "CloseSiiPrinter"))
#define PFN_SET_PRINTER_DATA	((F_SetSiiPrinterData)GetProcAddress( m_hDLL, "SetSiiPrinterData"))
#define PFN_SET_PRINTER_RESET	((F_SetSiiPrinterReset)GetProcAddress( m_hDLL, "SetSiiPrinterReset"))
#define PFN_GET_PRINTER_STATUS	((F_GetSiiPrinterAutoStatus)GetProcAddress( m_hDLL, "GetSiiPrinterAutoStatus"))
#define PFN_SET_CALLBACK_STATUS	((F_SetSiiPrinterCallbackStatus)GetProcAddress( m_hDLL, "SetSiiPrinterCallbackStatus"))


class CSeikoUSBPrinter : public CSeikoPrinter
{
public:
	CSeikoUSBPrinter(const CString& sPrinterName,const CString& sPrinterAddress,const CString& sDriverName);
	virtual ~CSeikoUSBPrinter();
protected:
	virtual void DoPrint(CBaseDocView* pDocView,UINT nNumOfCopeis);
	virtual DWORD WritePrinter(BYTE *pBuffer,DWORD bytesToWrite);


	DWORD	m_dwSessionId;
	HMODULE	m_hDLL;
};

#endif