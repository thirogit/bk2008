#include "stdafx.h"
#include "StringHelpers.h"

//----------------------------------------------------------------------

//
// This function converts BSTR string to C string (null-ternimated array of chars).
// Use SysFreeString to free memory allocated for BSTR.
//
BSTR StringToBstr( const CString& str ) 
{
	int nConvertedLen = MultiByteToWideChar( CP_ACP, 0, (LPCSTR)str, -1, 0, 0 );
	BSTR bstr = SysAllocStringLen( 0, nConvertedLen ); 
	if( bstr != 0 ) {
		MultiByteToWideChar( CP_ACP, 0, (LPCSTR)str, -1, bstr, nConvertedLen );
	}
	return bstr;
}

//
// This function converts C string (null-ternimated array of chars) to BSTR string.
// Use delete[] to free memory allocated for converted string.
//
CString BstrToString( BSTR bstr )
{
	int length = 0;
	if( bstr != 0 ) {
		length = WideCharToMultiByte( CP_ACP, 0, bstr, -1, 0, 0, 0, 0 );
	}

	CString str;
	LPSTR pBuffer = str.GetBuffer(length + 1);

	//LPSTR str = new char[length + 1];
	if( bstr != 0 ) 
	{
		WideCharToMultiByte( CP_ACP, 0, bstr, -1, pBuffer, length, 0, 0 ); 
	} 
	
	str.ReleaseBuffer();

	return str;
}

//
// This function concatenates two wide-character strings (e.g. BSTRs)
// and returns result as BSTR. Use SysFreeString to free memory allocated for BSTR.
//
BSTR Concatenate( LPCOLESTR str1, LPCOLESTR str2 )
{
	BSTR bstr = SysAllocStringLen( 0, wcslen( str1 ) + wcslen( str2 ) ); 
	if( bstr != 0 ) {
		wcscpy( bstr, str1 );
		wcscat( bstr, str2 );
	}
	return bstr;
}

//----------------------------------------------------------------------
