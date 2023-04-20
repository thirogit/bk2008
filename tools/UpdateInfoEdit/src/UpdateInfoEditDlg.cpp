#include "stdafx.h"
#include "UpdateInfoEdit.h"
#include "UpdateInfoEditDlg.h"
#include "NewVersionDlg.h"
#include "VerInfoEditProperties.h"
#include "EditVersionDlg.h"
#include "../../../cattlebase/src/Utils/StringFormat.h"
#include "../zip/ZipArchive/ZipArchive.h"
#include "MainCfg.h"
#include "../../../cattlebase/src/missing/paths.h"
#include "common/files.h"
#include "BufferTransfer.h"
#include "../ftp/FTPDataTypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define VERSION_INFO_FILE_REMOTEPATH "/version.info"

CUpdateInfoEditDlg::CUpdateInfoEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateInfoEditDlg::IDD, pParent),m_verFileCount(0),m_verFilesListCtrl(m_verFiles)
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CUpdateInfoEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_VERNO_EDIT,m_versionNumber);
	DDX_Text(pDX,IDC_ARCHNAME_EDIT,m_archiveName);
	DDX_Text(pDX,IDC_VERDESC_EDIT,m_versionDesc);
	DDX_Control(pDX,IDC_VERSIONLIST,m_verListBox);
	DDX_Text(pDX,IDC_FILECOUNT,m_verFileCount);
	DDX_Control(pDX,IDC_VERFILELIST,m_verFilesListCtrl);
	DDX_Control(pDX, IDC_FTPPROTOCOLOUTPUT, m_ProtocolOutput);
	
}

BEGIN_MESSAGE_MAP(CUpdateInfoEditDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_BUILD,OnBuild)
	ON_COMMAND(ID_PROPERTIES,OnProperties)
	ON_COMMAND(ID_ADDVERSION,OnAddVersion)
	ON_LBN_SELCHANGE(IDC_VERSIONLIST,OnLBSelChange)
	ON_COMMAND(ID_CREATEEMPTY,OnCreateEmpty)
	ON_COMMAND(ID_UPLOAD,OnUpload)
	ON_COMMAND(ID_DOWNLOAD,OnDownload)
	ON_COMMAND(ID_EXITVERDLG,OnExit)
	ON_COMMAND(ID_REMOVEVERSION,OnRemoveVersion)
	ON_COMMAND(ID_EDITVERSION,OnEditVersion)
	ON_WM_CLOSE()

END_MESSAGE_MAP()

BOOL CUpdateInfoEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	UpdateMenu(true,false,false,true);
	return TRUE;  
}

void CUpdateInfoEditDlg::OnClose()
{
	OnExit();
}

void CUpdateInfoEditDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CUpdateInfoEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUpdateInfoEditDlg::OnExit()
{
	m_verList.FreeAndFlush();
	EndDialog(IDOK);
}

void CUpdateInfoEditDlg::OnAddVersion()
{
	UINT maxVerNo = m_verList.GetMaxVerNo();
	if(maxVerNo == 0)
		maxVerNo = MIN_VER_NO;
	CVersionInfo *pVI = CNewVersionDlg::createNewVersion(maxVerNo,this);
	if(pVI != NULL)
	{
		while(m_verList.ContainsVerNo(pVI->GetVerNo()))
		{
			AfxMessageBox(CStringFormat("Already got version with ver. no %d",pVI->GetVerNo()));
			CEditVersionDlg editVersionDlg(pVI,maxVerNo,this);
			if(editVersionDlg.DoModal() != IDOK)
			{
				delete pVI;
				return;
			}
		}
		while(m_verList.ContainsArchiveName(pVI->GetArchiveName()))
		{
			AfxMessageBox(CStringFormat("Already got version with archive name %s",(LPCSTR)pVI->GetArchiveName()));
			CEditVersionDlg editVersionDlg(pVI,maxVerNo,this);
			if(editVersionDlg.DoModal() != IDOK)
			{
				delete pVI;
				return;
			}
		}	
				
		AddVersion(new CSaveVersionInfo(*pVI));
		
		delete pVI;
	}
}

void CUpdateInfoEditDlg::OnBuild()
{
	CVersionInfoList::ArrayIterator it = m_verList.iterator();
	while(it.hasNext())
	{
		CSaveVersionInfo *pVer = *(it++);
		if(!pVer->IsSaved())
		{
			
			CString archiveDir = AppCfg().GetString(CFGID_ARCHIVESDIR);
			if(archiveDir.GetLength() == 0 || IS_EXISTING_DIR != paths_DoesPathExist((LPCSTR)archiveDir))
			{
				AfxMessageBox("Archive directory does not exist.");
				return;
			}			
			CString tempZipFilePath = GetTempZipFileName(archiveDir);

			CZipArchive zip;
			CString baseDir = AppCfg().GetString(CFGID_BASEDIR);
			
			

			if(!zip.Open(tempZipFilePath,CZipArchive::zipCreate))
			{
				AfxMessageBox("Faild to create temporary zip archive.");
				return;
			}
			
			zip.SetRootPath(baseDir);
			
			CStringArray files;
			files.Copy(pVer->GetVerFiles());

			for(int iFile = 0;iFile < files.GetCount();iFile++)
			{
				CString filePath = paths_ConcatPath(baseDir,files[iFile]);
				zip.AddNewFile((LPCSTR)filePath,CZipCompressor::levelDefault,false);
			}
			CZipString zipFile = zip.Close();
			pVer->SetTempZipFilePath(zipFile);
			pVer->SetSave(true);
		}
	}
	ReloadList();
	UpdateMenu(false,true,false,false);
}

CString CUpdateInfoEditDlg::GetTempZipFileName(const CString& zipTempDir)
{
	CString fileNamePattern = "_zip%d";
	CString zipFileName,zipFilePath;
	int patternIndex = 1001;
	do
	{
		zipFileName.Format((LPCSTR)fileNamePattern,patternIndex);
		patternIndex++;
		zipFilePath = paths_ConcatPath(zipTempDir,zipFileName);
	}
	while(IsFileExists((LPCSTR)zipFilePath));
	return zipFilePath;
}

void CUpdateInfoEditDlg::ReloadList()
{
	m_verListBox.ResetContent();	

	CVersionInfoList::ArrayIterator it = m_verList.iterator();
	while(it.hasNext())
	{
		CSaveVersionInfo *pVer = *(it++);
		CString flags;

		if(!pVer->IsSaved())
			flags.AppendChar('+');
		if(!pVer->IsUploaded())
			flags.AppendChar('*');

		int iItem = m_verListBox.AddString(CStringFormat("%d%s",pVer->GetVerNo(),(LPCSTR)flags));
		m_verListBox.SetItemData(iItem,(DWORD_PTR)pVer);
	}
}

void CUpdateInfoEditDlg::AddVersion(CSaveVersionInfo *newVersion)
{
	newVersion->SetSave(false);
	newVersion->SetUpload(false);
	m_verList.Add(newVersion);
	//m_verList.Sort();
	ReloadList();
	

}

void CUpdateInfoEditDlg::OnProperties()
{
	CVerInfoEditProperties propDlg(this);
	propDlg.DoModal();
}

void CUpdateInfoEditDlg::OnLBSelChange()
{
	int iSel = m_verListBox.GetCurSel();
	CSaveVersionInfo* viewVersion = (CSaveVersionInfo*)m_verListBox.GetItemData(iSel);

	m_versionNumber = (LPCSTR)CStringFormat("%d",viewVersion->GetVerNo()); 
	m_archiveName = viewVersion->GetArchiveName();
	m_versionDesc = viewVersion->GetVersionDesc();
	m_verFileCount = viewVersion->GetVerFilesCount();
	SetDlgItemText(IDC_VERDATE,viewVersion->GetVersionDate().GetDayDate());

	m_verFilesListCtrl.Lock();
	m_verFiles.Copy(viewVersion->GetVerFiles());
	m_verFilesListCtrl.UpdateItemCount();
	m_verFilesListCtrl.Unlock();
	UpdateData(FALSE);

}

void CUpdateInfoEditDlg::UpdateMenu(bool bDownloadEnabled,bool bUploadEnabled,bool bBuildEnabled,bool bCreateEmptyEnabled)
{
	CMenu *pMenu = GetMenu();
	ASSERT(pMenu);

	EnableMenuItem(pMenu,ID_DOWNLOAD,bDownloadEnabled);
	EnableMenuItem(pMenu,ID_BUILD,bBuildEnabled);
	EnableMenuItem(pMenu,ID_UPLOAD,bUploadEnabled);
	EnableMenuItem(pMenu,ID_CREATEEMPTY,bCreateEmptyEnabled);
	EnableVersionMenu(bBuildEnabled);
}

void CUpdateInfoEditDlg::EnableVersionMenu(bool bEnable)
{
	CMenu *pMenu = GetMenu();
	ASSERT(pMenu);

	EnableMenuItem(pMenu,ID_ADDVERSION,bEnable);
	EnableMenuItem(pMenu,ID_REMOVEVERSION,bEnable);
    EnableMenuItem(pMenu,ID_EDITVERSION,bEnable);

}

void CUpdateInfoEditDlg::EnableMenuItem(CMenu* pMenu,UINT nCommand,bool bEnable)
{
	pMenu->EnableMenuItem(nCommand,MF_BYCOMMAND | (bEnable ? MF_ENABLED : (MF_DISABLED | MF_GRAYED)));
}



void CUpdateInfoEditDlg::OnCreateEmpty()
{
	UpdateMenu(false,true,false,false);
}

bool CUpdateInfoEditDlg::LoginFTP(nsFTP::CFTPClient& FTPClient)
{
	nsFTP::CLogonInfo logonInfo;
	//m_pasv   = AppCfg().GetBool(CFGID_PASSIVEMODE);
		
	logonInfo.SetHost(static_cast<LPCTSTR>(AppCfg().GetString(CFGID_FTPHOST)), 
					  static_cast<USHORT>(AppCfg().GetNum(CFGID_FTPPORT)), 
					  static_cast<LPCTSTR>(AppCfg().GetString(CFGID_FTPUSERNAME)),
					  static_cast<LPCTSTR>(AppCfg().GetString(CFGID_FTPPASSWORD)),
					  "");
	
	FTPClient.AttachObserver(&m_ProtocolOutput);
	return FTPClient.Login(logonInfo);
}

void CUpdateInfoEditDlg::OnUpload()
{
	nsFTP::CFTPClient                 FTPClient;

    LoginFTP(FTPClient);

	bool bPassiveMode = AppCfg().GetBool(CFGID_PASSIVEMODE) == TRUE;

	size_t bufferSize = sizeof(VERSION_INFO_HEADER) + m_verList.GetAllVInfosSize();
	UCHAR *pMemoryBuffer = new UCHAR[bufferSize];
	std::auto_ptr<UCHAR> deleteBuffer(pMemoryBuffer);
	memset(pMemoryBuffer,0,bufferSize);

	VERSION_INFO_HEADER *versionInfoFile = (VERSION_INFO_HEADER*)pMemoryBuffer;
	m_verList.CreateVIHeader(versionInfoFile);


	VERSION_INFO *pVI = (VERSION_INFO*)(pMemoryBuffer+sizeof(VERSION_INFO_HEADER));

	CVersionInfoList::ArrayIterator it = m_verList.iterator();
	
	while(it.hasNext())
	{
		CSaveVersionInfo *pSaveVI = *(it++);
		pSaveVI->CreateVersionInfo(pVI);
		pVI = (VERSION_INFO*)(((UCHAR*)pVI) + pVI->size);
	}

	//first upload zip archives.

	it = m_verList.iterator();
	while(it.hasNext())
	{
		CSaveVersionInfo *pSaveVI = *(it++);
		if(!pSaveVI->IsUploaded())
		{
			CString zipRemotePath = AppCfg().GetString(CFGID_SERVERROOT) + "/zips/" + pSaveVI->GetArchiveName();

			if(FTPClient.UploadFile((LPCSTR)pSaveVI->GetTempZipFilePath(),(LPCSTR)zipRemotePath,
				false,nsFTP::CRepresentation(nsFTP::CType::Image()),bPassiveMode) )
			{
				pSaveVI->SetUpload(true);
			}
			else
			{
				AfxMessageBox(CStringFormat("Faild to upload: %s",(LPCSTR)pSaveVI->GetArchiveName()));
				return;
			}
		}
	}

	CString versionInfoFileRemotePath = AppCfg().GetString(CFGID_SERVERROOT) + VERSION_INFO_FILE_REMOTEPATH;
	CBufferTransfer bufferTransfer(versionInfoFile,versionInfoFile->size);

	if(!FTPClient.Upload(&bufferTransfer,(LPCSTR)versionInfoFileRemotePath,bPassiveMode))
	{
		AfxMessageBox("Faild to upload version info file.");
		return;
	}


	FTPClient.Logout();
	
	UpdateMenu(true,false,false,true);
	ReloadList();

}

void CUpdateInfoEditDlg::OnDownload()
{
	m_verList.FreeAndFlush();
	nsFTP::CFTPClient                 FTPClient;
	LoginFTP(FTPClient);
	long lSize = 0;

	CString versionInfoFileRemotePath = AppCfg().GetString(CFGID_SERVERROOT) + VERSION_INFO_FILE_REMOTEPATH;

	if(FTPClient.FileSize((LPCSTR)versionInfoFileRemotePath,lSize) == nsFTP::FTP_ERROR)
	{
		AfxMessageBox("Error while checking size of version info file.");
	}
	else
	{
		if(lSize < sizeof(VERSION_INFO_HEADER))
		{
			AfxMessageBox("Version file is corrupted.");
		}
		else
		{
			UCHAR *pBuffer = new UCHAR[lSize];
			CBufferTransfer bufferTransfer(pBuffer,lSize);
			if(FTPClient.Download((LPCSTR)versionInfoFileRemotePath,&bufferTransfer,false))
			{
			
				VERSION_INFO_HEADER* pVIHeader = (VERSION_INFO_HEADER*)pBuffer;
				if(!strcmp(VERSION_INFO_HEADER_ID,pVIHeader->id))
				{
					VERSION_INFO *pVI = (VERSION_INFO*)(pVIHeader+1);

					for(int i = 0; i < pVIHeader->version_info_count;i++)
					{
						CVersionInfo *pVInfo = new CVersionInfo(pVI);
						CSaveVersionInfo *pSaveVI = new CSaveVersionInfo(*pVInfo);
						pSaveVI->SetSave(true);
						pSaveVI->SetUpload(true);
						m_verList.Add(pSaveVI);
						
						delete pVInfo;
						pVI = (VERSION_INFO*)(((UCHAR*)pVI)+pVI->size);
					}

					ReloadList();
					UpdateMenu(false,false,true,false);
					
				}
				else
				{
					AfxMessageBox("Invalid version of version info file.");
				}
			}
			else
			{
				AfxMessageBox("Error while downloading version info file.");
			}
			delete pBuffer;
		}
	}

	FTPClient.Logout();

}

void CUpdateInfoEditDlg::OnRemoveVersion()
{
}
void CUpdateInfoEditDlg::OnEditVersion()
{
}