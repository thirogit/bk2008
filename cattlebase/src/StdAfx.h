#if !defined(AFX_STDAFX_H__B1E1677E_1136_4F48_AD58_5C4B71AFF432__INCLUDED_)
#define AFX_STDAFX_H__B1E1677E_1136_4F48_AD58_5C4B71AFF432__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WINVER 
#define WINVER 0x0502
#endif

//#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#define _CRTDBG_MAP_ALLOC
#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <afxpriv2.h>
#include <afxtempl.h>
#include <afxdlgs.h>
#include <afxcview.h>
#endif // _AFX_NO_AFXCMN_SUPPORT


#include "resource.h"

#define FILTER_EDIT_BOOST


#endif // !defined(AFX_STDAFX_H__B1E1677E_1136_4F48_AD58_5C4B71AFF432__INCLUDED_)
