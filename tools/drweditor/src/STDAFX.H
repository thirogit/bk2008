// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifndef _UNICODE
#define VC_EXTRALEAN
#endif

#define _SECURE_ATL 1

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions (including VB)
#include <afxole.h>         // MFC OLE 2.0 support
#include <afxodlgs.h>       // MFC OLE 2.0 dialog support
#include <afxcmn.h>         // MFC common control support
#include <afxpriv2.h>


#define YAXISRISESUP

#if (!defined __PROF_UIS_H)
	#include <Prof-UIS.h>
#endif // (!defined __PROF_UIS_H)


#include "resource.h"