#include "stdafx.h"
#include "updater.h"
#include "log.h"
#include "../src/missing/util.h"
#include "../src/missing/paths.h"
#include "../src/WaitDialog/WaitDlg.h"
#include "../src/http_update/HttpVersionCfg.h"
#include "../src/httpdownloader/HttpFileGetter.h"
#include "../src/httpdownloader/TransferBuffer.h"
#include "../src/WaitDialog/Wait.h"
#include "../src/classes/TempStringLoader.h"
#include <memory>
#include "../src/CattlebaseMutex.h"
#include "../src/httpdownloader/DownloadProgress.h"
#include "CmdLineParser.h"
#include "VersionFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CUpdaterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CUpdaterApp::CUpdaterApp() : m_pSIMutex(NULL)
{
	
}

CUpdaterApp::~CUpdaterApp() 
{
	if (m_pSIMutex != NULL)
	{	
		m_pSIMutex->Unlock();
		delete m_pSIMutex;
		m_pSIMutex = NULL;
	}
}

CUpdaterApp theApp;

UINT CUpdaterApp::GetCattlebaseExeVersion()
{
	CString cattlebaseExePath = paths_ConcatPath(CUtil::GetAppDir(),CString("cattlebase.exe"));

	DWORD   dwVerHnd = 0;
	DWORD dwVerInfoSize = GetFileVersionInfoSize (cattlebaseExePath, &dwVerHnd);
    if (!dwVerInfoSize)    
        return 0;

    LPSTR lpstrVffInfo =  new CHAR[dwVerInfoSize+1];

    if (!GetFileVersionInfo(cattlebaseExePath, dwVerHnd, dwVerInfoSize, lpstrVffInfo)) 
    {
        delete (lpstrVffInfo);
        return 0;  
	}

	UINT    uBuildNoLen = 0;
    LPVOID lpBuildNo = NULL;
    BOOL retVal = VerQueryValue ( lpstrVffInfo, "\\BuildInformation\\BuildNo", &lpBuildNo, (UINT *)&uBuildNoLen);
	LPCSTR szBuildNo = (LPCSTR)lpBuildNo;

	UINT versionExtend = atoi(szBuildNo);
	delete lpstrVffInfo;
	return versionExtend;
}



BOOL CUpdaterApp::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("Cattlebase Updater"));

	m_pSIMutex = new CMutex(FALSE,MUTEX_NAME);
	if(!m_pSIMutex->Lock(1))
	{
		LOG("ANOTHER CATTLEBASE OR UPDATER INSTANCE IS RUNNING.");
		AfxMessageBox(IDS_ERRMUTEXLOCK,MB_OK,0);
		return FALSE;
	}

	CString logPath = paths_ConcatPath(CUtil::GetAppDir(),_T("UPDATERLOG.log"));

	AMLOG_SETFILENAME((LPCSTR)logPath);
	AMLOG_SETLOGLEVEL_INFO;


	CWait waitDlg;
	waitDlg.SetWaitText(CTempStringLoader(IDS_DOWNLOADINGUPDATES));
	waitDlg.Show();

	UINT cattlebaseVersion = 0;
	
	CCmdLineParser cmdLineParser(::GetCommandLine());
	if(cmdLineParser.HasVal("version"))
	{
		cattlebaseVersion =  atoi(cmdLineParser.GetVal("version"));
		LOG("CMD LINE PARAMETER VERSION %d",cattlebaseVersion);
	}
	else
	{	
		cattlebaseVersion = GetCattlebaseExeVersion();
		LOG("EXISTING CATTLEBASE.EXE VERSION %d",cattlebaseVersion);
	}

	if(cattlebaseVersion == 0)
	{
		LOG("CANT FIND CATTLEBASE.EXE OR INVALID VERSION PASSED IN CMD LINE PARAMETER");
		waitDlg.Close();
		return FALSE;
	}

	
	
	CHttpVersionCfg *cfg = CHttpVersionCfg::GetInstance();
	if(!cfg)
	{
		LOG("FAILD TO GET VERSION INFO CONFIGURATION!");
		waitDlg.Close();
		return FALSE;
	}
	DownloadProgress downloadProgress(cfg->GetHttpVersionInfoURL(),&waitDlg);
	CHttpFileGetter verGetter(cfg->GetHttpVersionInfoURL(),&downloadProgress);
	LOG("DOWNLOADING VERSION INFO FILE.");
	long lContentLength = verGetter.GetContentLength();
	if(lContentLength > 0)
	{
		UINT uContentLength = (UINT)lContentLength;
		BYTE *pBuffer = new BYTE[uContentLength];
		std::auto_ptr<BYTE> autoBuffer(pBuffer);
		CTransferBuffer transferBuffer(pBuffer,uContentLength);
		
		if(verGetter.Download(transferBuffer))
		{
			VERSION_INFO_HEADER* pVIHeader = (VERSION_INFO_HEADER*)pBuffer;
			if(!strcmp(VERSION_INFO_HEADER_ID,pVIHeader->id))
			{
				if(pVIHeader->size == uContentLength)
				{
					if(cattlebaseVersion < pVIHeader->max_version_no)
					{
						LOG("UPDATING. OLD VERSION %d,  NEW %d.",cattlebaseVersion,pVIHeader->max_version_no);
						CVersionInfoList versionInfoList,newVersionsList;
						CreateVersionList(pVIHeader,versionInfoList);
						
						CVersionInfoList::ArrayIterator viIt = versionInfoList.iterator();
						while(viIt.hasNext())
						{
							CVersionInfo *pVI = *(viIt++);
							if(pVI->GetVerNo() > cattlebaseVersion)
							{
								newVersionsList.Add(pVI);
							}
						}
						InstallVersionUpdates(newVersionsList,&waitDlg);
						newVersionsList.RemoveAll();
						versionInfoList.FreeAndFlush();				
					}
					else
					{
						LOG("NO NEED FOR UPDATE, CURRENT CATTLEBASE.EXE VERSION %d, MAX UPDATE VERSION %d.",cattlebaseVersion,pVIHeader->max_version_no);
					}
				}	
				else
				{
					LOG("VERSION INFO FILE IS CORUPTED. (SIZE)");
				}
			}
			else
			{
				LOG("VERSION INFO FILE IS CORUPTED. (HEADER)");
			}
		}
		else
		{
			LOG("FAILD TO GET VERSION INFO CONFIGURATION!");
		}
	}
	else
	{
		LOG("FAILD TO GET VERSION INFO FILE SIZE.");
	}

	
	waitDlg.Close();
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CUpdaterApp::CreateVersionList(VERSION_INFO_HEADER* pVIHeader,CVersionInfoList &versionInfoList)
{

	VERSION_INFO *pVI = (VERSION_INFO*)(pVIHeader+1);
	for(UINT v = 0; v < pVIHeader->version_info_count;v++)
	{
		versionInfoList.Add(new CVersionInfo(pVI));
		pVI = (VERSION_INFO*)(((BYTE*)pVI) + pVI->size);
	}
}

void CUpdaterApp::InstallVersionUpdates(CVersionInfoList &versionInfoList,ITaskProgres* pProgress)
{
	versionInfoList.Sort(false);
	CVersionFileList versionFiles;
	CVersionFileList::AutoArray autoDeleteVersionFiles(versionFiles);
	CString basePath = CUtil::GetAppDir();

	CStringArray files;
	CVersionInfoList::ArrayIterator vIt = versionInfoList.iterator();
	while(vIt.hasNext())
	{
		CVersionInfo *pVI = *(vIt++);		
		files.Copy(pVI->GetVerFiles());
		for(int iFile = 0;iFile < files.GetCount();iFile++)
		{
			CString file = files[iFile];
			if(!versionFiles.HasThisFile(file))
				versionFiles.Add(new CVersionFile(basePath,file,pVI));
		}
	}

	if(!CheckCanUpdate(versionFiles))
	{
		AfxMessageBox(IDS_ERR_FILES_ARE_IN_USE);
		LOG("UPDATE CHECK FAILD.");
		return;
	}

	if(!BackupFiles(versionFiles))
	{
		AfxMessageBox(IDS_ERR_FAILD_TO_BACKUP_FILES);
		LOG("FAILD TO BACKUP FILES TO UPDATE.");
		return;
	}

	pProgress->SetTask((UINT)(LPCSTR)CTempStringLoader(IDS_INSTALINGVERUPDATES),versionFiles.GetCount());
	CVersionTmpArchiveFileCache archiveCache;
	CVersionFileList::ArrayIterator vfIt = versionFiles.iterator();


	while(vfIt.hasNext())
	{
		CVersionFile* pVersionFile = *(vfIt++);
		LOG("UPDAING %s",(LPCSTR)pVersionFile->GetFilePath());
		bool bUpdated = pVersionFile->UpdateFile(&archiveCache,false);
		pProgress->OnStep(vfIt.index());
		if(!bUpdated)
		{
			while(!vfIt.isFirst())
				(*(vfIt--))->RolbackUpdate();
			break;
		}
	}

	return;


}

bool CUpdaterApp::BackupFiles(CVersionFileList& verFileList)
{
	CString backupBasePath = paths_ConcatPath(CUtil::GetAppDir(),CString("update_backup"));
	CString thisBackupPath = paths_ConcatPath(backupBasePath,
											  CString(CDateTime::now().GetDateInFmt(CDateTime::YYYYMMDDHHMISS)));

	CVersionFileList::ArrayIterator vfIt = verFileList.iterator();
	while(vfIt.hasNext())
	{
		CVersionFile* pVersionFile = *(vfIt++);
		LOG("BACKING UP %s.",(LPCSTR)pVersionFile->GetFilePath());
		bool bBackedup = pVersionFile->BackupOldFile(thisBackupPath);
		if(!bBackedup)
		{
			LOG("BACKUP FAILD FOR FILE: %s",(LPCSTR)pVersionFile->GetFilePath());
			return false;
		}
	}

	return true;
}

bool CUpdaterApp::CheckCanUpdate(CVersionFileList& verFileList)
{
	CVersionFileList::ArrayIterator vfIt = verFileList.iterator();
	while(vfIt.hasNext())
	{
		CVersionFile* pVersionFile = *(vfIt++);

		LOG("CHECKING IF GOOD FOR UPDATE: %s.",(LPCSTR)pVersionFile->GetFilePath());
		bool bChecked = pVersionFile->UpdateFile(NULL,true);
		if(!bChecked)
		{
			LOG("UPDATE CHECK FAILD FOR FILE: %s",(LPCSTR)pVersionFile->GetFilePath());
			return false;
		}
	}
	return true;
}