// drawcli.h : main header file for the DRAWCLI application
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __DRAWCLI_H__
#define __DRAWCLI_H__

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif



#include "resource.h"       // main symbols


#include <afxtempl.h>       // drawcli uses collection class templates

/////////////////////////////////////////////////////////////////////////////
// CDrawApp:
// See drawcli.cpp for the implementation of this class
//

class CDrawApp : public CWinApp
{
public:
	CDrawApp();
	~CDrawApp();

// Overrides
	virtual BOOL InitInstance();
	void DoContextHelp();
public:
	afx_msg void OnFilePrintSetup(); //need this public
// Implementation
protected:
	//{{AFX_MSG(CDrawApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDrawApp theApp;

#endif //__DRAWCLI_H__

/////////////////////////////////////////////////////////////////////////////
