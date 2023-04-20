// cattlebasefrm.h : main header file for the CATTLEBASEFRM application
//
#if !defined(AFX_CATTLEBASEFRM_H__0130D286_28EB_42CE_AE6B_C7B07B361518__INCLUDED_)
#define AFX_CATTLEBASEFRM_H__0130D286_28EB_42CE_AE6B_C7B07B361518__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "afxmt.h"
#include "MainCfg.h"
#include "CrashDumpHandler.h"
#include <afxwinappex.h>
#include "hkey/KeyManager.h"

KeyManager* GetApplicationKeyManager();

class CCattlebaseApp : public CWinAppEx
{
public:
	CCattlebaseApp();
	~CCattlebaseApp();
	KeyManager* GetKeyManager();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
	CMutex * m_pSIMutex;
	CCrashDumpHandler m_crashHandler;
	KeyManager* m_pKeyManagerThread;
	ULONG_PTR m_gdiplusToken;


	BOOL CheckForNewVersion();
	bool RunUpdater();
protected:	
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};





/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATTLEBASEFRM_H__0130D286_28EB_42CE_AE6B_C7B07B361518__INCLUDED_)
