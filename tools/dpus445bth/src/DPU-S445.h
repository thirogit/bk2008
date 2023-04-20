
// DPU-S445.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CDPUS445App:
// See DPU-S445.cpp for the implementation of this class
//

class CDPUS445App : public CWinApp
{
public:
	CDPUS445App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CDPUS445App theApp;