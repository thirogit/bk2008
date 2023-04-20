#ifndef __DPUS445DRIVER_H__
#define __DPUS445DRIVER_H__

/*	Define callback finction	*/
typedef INT (CALLBACK EXPORT *FNC_CB_STATUS)(DWORD);

typedef const BYTE *LPCBYTE;

/*	Define finction type		*/
typedef DWORD (WINAPI *F_OpenSiiPrinter)(LPCTSTR, LPDWORD);
typedef DWORD (WINAPI *F_CloseSiiPrinter)(DWORD);
typedef DWORD (WINAPI *F_GetSiiPrinterData)(DWORD,LPTSTR,LPBYTE,DWORD,LPDWORD);
typedef DWORD (WINAPI *F_SetSiiPrinterData)(DWORD,LPCBYTE,DWORD,LPDWORD);
typedef DWORD (WINAPI *F_SetSiiPrinterReset)(DWORD);
typedef DWORD (WINAPI *F_GetSiiPrinterAutoStatus)(DWORD,LPDWORD);
typedef DWORD (WINAPI *F_SetSiiPrinterCallbackStatus)(DWORD,FNC_CB_STATUS);

class DPUS445Driver
{
public:
	DPUS445Driver();
	bool LoadDriver();
	DWORD OpenPrinterSession(LPCSTR lpszPrinterName);
	void ClosePrinterSession(DWORD dwSession);
	int SendToPrinter(DWORD dwSession,const BYTE* pBuffer,UINT uiBytesToWrite);
	void UnloadDriver();

private:
	HMODULE	m_hModule;
	
	F_OpenSiiPrinter m_pfnOpenSiiPrinter;
	F_CloseSiiPrinter m_pfnCloseSiiPrinter;
	F_SetSiiPrinterData m_pfnSetSiiPrinterData;
};


#endif