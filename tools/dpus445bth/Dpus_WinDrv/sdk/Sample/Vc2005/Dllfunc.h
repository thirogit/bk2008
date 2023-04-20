/*++

Copyright (c) 2008 Seiko Instruments Inc.
All rights reserved.

Module Name:

    DllFunc.h

Abstract:

    Header file for dynamic library functions

Author:

Revision History:

--*/

#pragma once



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
typedef DWORD (WINAPI *F_SetSiiPrinterTimeout)(DWORD,DWORD);			 

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
#define PFN_SET_PRINTER_TIMEOUT	((F_SetSiiPrinterTimeout)GetProcAddress( hModule, "SetSiiPrinterTimeout"))





BOOL LoadDll();
VOID UnloadDll();

BOOL OpenSamp(LPTSTR);
VOID WriteSamp();
VOID ReadSamp();
VOID CallbackSamp( BOOL bStart );


