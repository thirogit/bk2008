// © ABBYY. 2010.
// SAMPLES code is property of ABBYY, exclusive rights are reserved. 
//
// DEVELOPER is allowed to incorporate SAMPLES into his own APPLICATION and modify it under 
// the  terms of  License Agreement between  ABBYY and DEVELOPER.


// ABBYY FineReader Engine 10 Sample

// Helper functions for BSTR handling

#ifndef StringHelpers_h
#define StringHelpers_h

#include <wtypes.h>

//----------------------------------------------------------------------

//
// This function converts BSTR string to C string (null-ternimated array of chars).
// Use SysFreeString to free memory allocated for BSTR.
//
BSTR StringToBstr( const CString& str );

//
// This function converts C string (null-ternimated array of chars) to BSTR string.
// Use delete[] to free memory allocated for converted string.
//
CString BstrToString( BSTR bstr );

//
// This function concatenates two wide-character strings (e.g. BSTRs)
// and returns result as BSTR. Use SysFreeString to free memory allocated for BSTR.
//
BSTR Concatenate( LPCOLESTR str1, LPCOLESTR str2 );

//----------------------------------------------------------------------

#endif // StringHelpers_h

