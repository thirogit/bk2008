#include "stdafx.h"
#include "ZebraPOS.h"
//#include "../Utils/BstrWrap.h"
#include "ZebraException.h"
#include "../Utils/StringHelpers.h"
//#include "../Utils/SafePtr.h"
#include "../Utils/StringFormat.h"
//#include "../log.h"
#include <winspool.h>

#define MAX_PRINTER_JOB_NAME 30

const char* szZPLCommandTemplate =  "^XA"
									"^FS"
									"~JS"
									"^LT0"
									"^MNW"
									"^MTT"
									"^PON"
									"^PMN"
									"^LH0,0"
									"^JMA"
									"^PR6,6"
									"^MD15"
									"^LRN"
									"^CI0"
									"^MMT"
									"^PW400"
									"^LL0400"
									"^LS0"
									"^BY2,3,59"
									"^FT55,220"
									"^B2N,,N,N"
									"^FD{BARCODE}" 
									"^FS"
									"^FT16,357"
									"^A0N,171,172"
									"^FH\\\\"
									"^FD{NEXT4DIGITS}"
									"^FS"
									"^FT338,360"
									"^A0N,112,100"
									"^FH\\\\"
									"^FD{CKSUMDIGIT}" 
									"^FS"
									"^FT163,149"
									"^A0N,68,67"
									"^FH\\\\"
									"^FD{MIDDLE5DIGITS}"
									"^FS"
									"^FT73,149"
									"^A0N,68,67"
									"^FH\\\\"
									"^FD{FIRST2DIGITS}" 
									"^FS"
									"^FT222,73"
									"^A0N,45,45"
									"^FH\\\\"
									"^FD{COUNTRYCODE}" 
									"^FS"
									"^FO124,1"
									"^GB83,77,8"
									"^FS"
									"^XZ";



//ZebraPOS::ZebraPOS()
//{
//	m_pZebraPOS = NULL;
//}

//ZebraPOS::~ZebraPOS()
//{
//	Clean();
//}


//void ZebraPOS::Init()
//{
/*	if(!m_pZebraPOS)
	{
		LOG("ZEBRA-Creating instance");
		 HRESULT hr =  CoCreateInstance( __uuidof(ZebraPOSPrinter),
										  NULL,
										  CLSCTX_INPROC_SERVER,
										  __uuidof(IZebraPOSPrinter),
										 (void**) &m_pZebraPOS
										 );
					 
		if(!SUCCEEDED(hr))
		{
			m_pZebraPOS = NULL;
			throwIfNotSucceeded(hr);
		}
		
		LOG("ZEBRA-Opening printer.");
		long lOpenResult = 0;
		CBstr printerLogicalName("4BK");
		hr = m_pZebraPOS->Open(printerLogicalName,&lOpenResult);

		if(!SUCCEEDED(hr))
		{
			m_pZebraPOS->Release();
			m_pZebraPOS = NULL;
			throwIfNotSucceeded(hr);
		}

		if(lOpenResult)
		{
			LOG("ZEBRA-Open result = %d",lOpenResult);
			m_pZebraPOS->Release();
			m_pZebraPOS = NULL;
			throwIfError(lOpenResult);
		}

	*/

//}

	
void ZebraPOS::Print(const CString& sEAN)
{
//	Init();

	/*LOG("ZEBRA-Claiming printer");
	long lClaimResult = 0;
	HRESULT hr = m_pZebraPOS->ClaimDevice(1000,&lClaimResult);
	
	throwIfNotSucceeded(hr);

	throwIfError(lClaimResult);

	MustacheContext context = Mustache::Parse(szZPLCommandTemplate);
	MustacheVariables variables = CreateTemplateVariables(sEAN);

	CString sZPLCommand = Mustache::Format(context,variables);
	CBstr bstrZPLCommand((LPCSTR)sZPLCommand);
	long lCommandLen = bstrZPLCommand.Length();
	long lDirectIOResult = 0;
	hr = m_pZebraPOS->put_DeviceEnabled(TRUE);
	throwIfNotSucceeded(hr);
	
	hr = m_pZebraPOS->DirectIO(1,&lCommandLen,bstrZPLCommand.GetBuffer(),&lDirectIOResult);
	throwIfNotSucceeded(hr);
	throwIfError(lDirectIOResult);

	hr = m_pZebraPOS->put_DeviceEnabled(FALSE);
	throwIfNotSucceeded(hr);
	
	long lReleaseResult = 0;
	hr = m_pZebraPOS->ReleaseDevice(&lReleaseResult);
	throwIfNotSucceeded(hr);
*/
	HANDLE hPrinter = NULL;

	throwIfFailed("OPEN_PRINTER",OpenPrinter("4BK",&hPrinter,NULL) == TRUE);

	char szJobName[MAX_PRINTER_JOB_NAME+1];

	strncpy(szJobName,(LPCSTR)sEAN,MAX_PRINTER_JOB_NAME);

	DOC_INFO_1 docInfo;
	docInfo.pDocName = szJobName;
	docInfo.pOutputFile = NULL;
	docInfo.pDatatype = "RAW";

	if(!StartDocPrinter(hPrinter,1,(BYTE*)&docInfo))
	{
		ClosePrinter(hPrinter);
		throwIfFailed("START_DOC",false);
	}
	
	MustacheContext context = Mustache::Parse(szZPLCommandTemplate);
	MustacheVariables variables = CreateTemplateVariables(sEAN);

	CString sZPLCommand = Mustache::Format(context,variables);
	DWORD bytesWritten = 0;
	WritePrinter(hPrinter,(void*)sZPLCommand.GetBuffer(),sZPLCommand.GetLength(),&bytesWritten);
	sZPLCommand.ReleaseBuffer();
	
	EndDocPrinter(hPrinter);
	ClosePrinter(hPrinter);


}

MustacheVariables ZebraPOS::CreateTemplateVariables(const CString& sEAN)
{
//	0  1  2  3  4  5  6  7  8  9  10 11 12 13
//	P  L  1  1  1  1  1  1  1  1  1  1  1  1
//  _________________________________________
//  A  A  B  B  C  C  C  C  C  D  D  D  D  E
//
//
//
	CString sCountryCode = sEAN.Left(2);
	CString sFirst2Digits = sEAN.Mid(2,2);
	CString sMiddle5Digits = sEAN.Mid(4,5);
	CString sNext4Digits = sEAN.Mid(9,4);
	CString sChkSumDigit = sEAN.Right(1);
	CString sBarcode = sEAN.Mid(2);


	MustacheVariables variables;
	variables.Put(MustacheVariable("BARCODE",sBarcode));
	variables.Put(MustacheVariable("NEXT4DIGITS",sNext4Digits));
	variables.Put(MustacheVariable("MIDDLE5DIGITS",sMiddle5Digits));
	variables.Put(MustacheVariable("COUNTRYCODE",sCountryCode));
	variables.Put(MustacheVariable("FIRST2DIGITS",sFirst2Digits));
	variables.Put(MustacheVariable("CKSUMDIGIT",sChkSumDigit));

	return variables;

	

}

/*
void ZebraPOS::throwIfNotSucceeded(HRESULT hResult)
{
	if( FAILED( hResult ) ) 
	{
		CSafePtr<IErrorInfo> errorInfo;
		if( GetErrorInfo( 0, errorInfo.GetBuffer() ) == S_OK ) 
		{
			CBstr description;
			errorInfo->GetDescription( &description );
			LOG("ZEBRA-FAILED: %s",(LPCSTR)BstrToString(description));
			throw new ZebraException(BstrToString(description));			
		} 
		else 
		{
			LOG("ZEBRA-FAILED: HRESULT = %08X",hResult);
			throw new ZebraException(CStringFormat("Failed with result %08X",hResult));
		}
	}
	
}

void  ZebraPOS::throwIfError(long lResult)
{
	if(lResult)
	{
		LOG("ZEBRA-FAILED: result = %d",lResult);
		throw new ZebraException(CStringFormat("Failed with result %08X",lResult));
	}
}
*/

void ZebraPOS::throwIfFailed(const CString& sTaskName,bool bResult)
{
	if(!bResult)
	{
		throw new ZebraException(CString("FAILED: ") + sTaskName);
	}
}

//void ZebraPOS::Clean()
//{
/*	if(m_pZebraPOS)
	{
		long lCloseResult = 0;
		m_pZebraPOS->Close(&lCloseResult);
		m_pZebraPOS->Release();
		m_pZebraPOS = NULL;
	}
*/
//}
