#include "stdafx.h"
#include "Sample.h"

static HMODULE	hModule = NULL;		// handle for dll
static DWORD	dwSessionId = 0;	// Session ID for dll's function	
TCHAR pMsg[1024];



//	Callback status function sample
EXPORT INT CALLBACK CBFuncSampProc( DWORD dwStatus )
{
	static	INT n = 0;

	wsprintf( pMsg, TEXT("[%02d] 0x%08x"), n++, dwStatus ); 
	::SendMessage( ::GetDlgItem( AfxGetApp()->m_pMainWnd->m_hWnd, IDC_CB_LIST ), 
						LB_INSERTSTRING, 0, (LPARAM)pMsg );
	return 0;
}



// Load dll
BOOL LoadDll()
{
	if( hModule = LoadLibrary( (LPCTSTR)DLL_FILE_NAME ) )
	{
		if(	PFN_OPEN_PRINTER			&&	
			PFN_GET_PRINTER_DATA		&&
			PFN_CLOSE_PRINTER			&&
			PFN_SET_PRINTER_DATA		&&
			PFN_SET_PRINTER_RESET		&&
			PFN_GET_PRINTER_STATUS		&&
			PFN_SET_CALLBACK_STATUS		)
		{
			return TRUE;
		}
		FreeLibrary( hModule );
	}

	wsprintf( pMsg, TEXT("Loading the [[ %s ]] file was failed."), DLL_FILE_NAME );
	AfxMessageBox( pMsg );
	return FALSE;
}


// Release dll
VOID UnloadDll()
{
	if( dwSessionId )
	{
		PFN_CLOSE_PRINTER( dwSessionId );
		dwSessionId = 0;
	}

	FreeLibrary( hModule );
}




//	Sample of OpenSiiPrinter
BOOL OpenSamp( LPTSTR lpPrinterName )
{
	if( dwSessionId )
	{
		PFN_CLOSE_PRINTER( dwSessionId );
		dwSessionId = 0;
	}

	// Open Printer
	return PFN_OPEN_PRINTER( lpPrinterName, &dwSessionId )? FALSE : TRUE;  
}

//	Sample of SetSiiPrinterData 
VOID WriteSamp()
{
	BYTE	pHeader[]	= HEADER_CMD;
	BYTE	pFooter[]	= FOOTER_CMD;
	BYTE	pSampCmd[]	= SAMPLE_CMD;
	BYTE	pBuf[]		= "Seiko Instruments Inc.\n";
	DWORD	cbWritten	= 0;

	PFN_SET_PRINTER_DATA( dwSessionId, pHeader, sizeof(pHeader), &cbWritten );	// For checking "GET_RAW_DATA_EXEC_RESP"
	PFN_SET_PRINTER_DATA( dwSessionId, pBuf, sizeof(pBuf), &cbWritten );		// Sample Print Data	
	PFN_SET_PRINTER_DATA( dwSessionId, pFooter, sizeof(pFooter), &cbWritten );	// For checking "GET_RAW_DATA_EXEC_RESP"
	PFN_SET_PRINTER_DATA( dwSessionId, pSampCmd, sizeof(pSampCmd), &cbWritten );// For checking "GET_RAW_DATA_CMD_RESP"
}

//	Sample of SetSiiPrinterCallbackStatus
VOID CallbackSamp( BOOL bStart )
{
	if( bStart )
	{
		PFN_SET_CALLBACK_STATUS( dwSessionId, (FNC_CB_STATUS)CBFuncSampProc );
	}
	else
	{
		PFN_SET_CALLBACK_STATUS( dwSessionId, (FNC_CB_STATUS)NULL );
	}
}


//	Message of GetSiiPrinterData 
VOID DispMsg( LPBYTE pRecBuf, DWORD dwReturned )
{
	HWND	hGetDataMsg = ::GetDlgItem( AfxGetApp()->m_pMainWnd->m_hWnd, IDC_GET_DATA_MSG );
	DWORD	dwIndex	= 0;	

	if( dwReturned )
	{
		while( dwReturned-- )
		{
			TCHAR	pTmpMsg[32];
			wsprintf( pTmpMsg, TEXT("0x%02x "), pRecBuf[ dwIndex++ ] ); 
			lstrcat( pMsg, pTmpMsg ); 
			if( !( dwIndex % 6 ) )
			{
				lstrcat( pMsg, TEXT("\n") ); 
			}
			else if( dwIndex > 30 )
			{
				lstrcat( pMsg, TEXT("....") );
				break;
			}
		}
	}
	else
	{
		lstrcat( pMsg, TEXT("NO DATA !!") ); 
	}
	::SetWindowText( hGetDataMsg, pMsg ); 
}


//	Sample of GetSiiPrinterData 
VOID ReadSamp()
{
	static UINT	nIndexCmdTbl = 0;
	DWORD	dwSuccess = 0;
	DWORD	dwReturned = 0;
	BYTE	pRecBuf[256] = {0};	
	HWND	hGetDataMsg = ::GetDlgItem( AfxGetApp()->m_pMainWnd->m_hWnd, IDC_GET_DATA_MSG );

	if( !lstrlen( TBL_CMD[ nIndexCmdTbl ] ) )
	{
		nIndexCmdTbl = 0;
	}

	wsprintf( pMsg, TEXT("[ %s ]\n\n"), TBL_CMD[ nIndexCmdTbl ] ); 
	::SetWindowText( hGetDataMsg, pMsg ); 

	dwSuccess = PFN_GET_PRINTER_DATA( dwSessionId, (LPTSTR)TBL_CMD[ nIndexCmdTbl ], pRecBuf, sizeof(pRecBuf), &dwReturned );
	if( !dwSuccess )
	{
		DispMsg( pRecBuf, dwReturned );
	}
	else
	{
		wsprintf( pMsg, TEXT("Error ( %d ) : [ %s ]"), dwSuccess, TBL_CMD[ nIndexCmdTbl ] ); 
		::SetWindowText( hGetDataMsg, pMsg ); 
	}

	nIndexCmdTbl++;
}
