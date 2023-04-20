#ifndef __DRWEDITOR_H__
#define __DRWEDITOR_H__

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <afxtempl.h>       // drawcli uses collection class templates

class CDrwEditorApp : public CWinApp
{
public:
	CDrwEditorApp();
	~CDrwEditorApp();

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

extern CDrwEditorApp theApp;

#endif 