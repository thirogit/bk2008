#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		
#include <afxmt.h>
#include "../UpdateInfoEdit/src/VersionInfoStructFileTypes.h"
#include "VersionInfoList.h"
#include "VersionFileList.h"
#include "../src/taskprogress/ProgressIFace.h"

class CUpdaterApp : public CWinApp
{
public:
	CUpdaterApp();
	
	~CUpdaterApp();
public:
	virtual BOOL InitInstance();
private:
	UINT GetCattlebaseExeVersion();
	void CreateVersionList(VERSION_INFO_HEADER* pVIHeader,CVersionInfoList &versionInfoList);
	void InstallVersionUpdates(CVersionInfoList &versionInfoList,ITaskProgres* pProgress);
	bool CheckCanUpdate(CVersionFileList& verFileList);
	bool BackupFiles(CVersionFileList& verFileList);


	CMutex * m_pSIMutex;

	DECLARE_MESSAGE_MAP()
};

extern CUpdaterApp theApp;