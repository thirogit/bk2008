#include "stdafx.h"
#include "PassDBMngmntOpt.h"
#include "../PassportStorage/PassImgBase.h"
#include "MainCfg.h"
#include "missing/util.h"
#include "missing/paths.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "../exceptions/StrException.h"
#include "../consts.h"

using namespace boost::filesystem;

//#######################################################################################
const GRIDHEADERS CMountedFilesList::m_mountedPassListCols[] =
{
	{IDS_HEADERMOUNTSTATUS,COL_STATUS},
	{IDS_HEADERDOMOUNT,COL_DOMOUNT},
	{IDS_HEADERMOUTFILEPATH,COL_FILEPATH},
	{0,0}	
};


CMountedFilesList::CMountedFilesList(CMountedFilesArray& mountedFiles) : m_mountedFiles(mountedFiles),
				CBaseListCtrl(m_mountedPassListCols,CFGID_MOUNTEDFILESLIST)
{


}
void CMountedFilesList::UpdateItemCount()
{
	SetItemCount(m_mountedFiles.GetSize());
}
void CMountedFilesList::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	const char *pszText = NULL;
	
	switch (GetColID(nSubItem))
	{
	case COL_FILEPATH:
		pszText = (LPCSTR)m_mountedFiles[nItem]->m_filePath;
		break;
	case COL_STATUS:
		pszText = g_szYesNo[m_mountedFiles[nItem]->m_bMounted ? 1 : 0];
		break;
	case COL_DOMOUNT:
		pszText = g_szYesNo[m_mountedFiles[nItem]->m_bDoMount ? 1 : 0];
		break;
	}
	strcpy(pszItemText,pszText);
}

bool CMountedFilesList::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	if(GetColID(nSubItem) == COL_STATUS)
	{
		*cellBkg = m_mountedFiles[nItem]->m_bMounted ? RGB(0,159,0) : RGB(198,0,0);
		return true;
	}
	return false;
}

	

	

//#######################################################################################
IMPLEMENT_DYNAMIC(CPassDBMngmntOpt, CPropertyPage)
CPassDBMngmntOpt::CPassDBMngmntOpt() : CPropertyPage(CPassDBMngmntOpt::IDD),m_mountedList(m_mountedFiles)
{

	m_bAskBeforePassFileMove = AppCfg().GetBool(CFG_ASKBEFOREPASSMOVE);
	m_passMoveDir = AppCfg().GetString(CFG_PASSMOVEDIRECTORY);
}

CPassDBMngmntOpt::~CPassDBMngmntOpt()
{
}

void CPassDBMngmntOpt::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PASSDBMOVEDIR, m_movePassFileDir);
	DDX_Control(pDX,IDC_PASSDBMAXSIZE,m_fileSizes);
	DDX_Check(pDX,IDC_ASKBEFOREMOVE,m_bAskBeforePassFileMove);
	DDX_Control(pDX,IDC_LABELWARN,m_warnLabel);
	DDX_Control(pDX,IDC_MOUNTEDLIST,m_mountedList);
}


BEGIN_MESSAGE_MAP(CPassDBMngmntOpt, CPropertyPage)
//	ON_BN_CLICKED(IDC_MOUNT, OnBnClickedMount)
//	ON_BN_CLICKED(IDC_UNMOUNT, OnBnClickedUnmount)
	ON_BN_CLICKED(IDC_MOVENOW, OnBnClickedMoveNow)
	ON_NOTIFY(NM_RCLICK, IDC_MOUNTEDLIST, OnRClick)
	ON_COMMAND(IDM_MOUNT_UNMOUNT,OnUnmount)
	ON_COMMAND(IDM_MOUNT_MOUNT,OnMount)
	ON_COMMAND(IDM_MOUNT_CHNGPATH,OnChangePath)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CPassDBMngmntOpt::OnInitDialog()
{
	CDialog::OnInitDialog();

	UINT moveSizesComboMB[] = 
	{
		256,
		512,
		650,
		700,
		1024,
		2048,
		4096,
		8192,
		0
	};

	
	int cfgSize = AppCfg().GetNum(CFG_MAXPASS_SIZE_MB),item,defItem = -1;
	UINT *moveSize = moveSizesComboMB;
	ULONGLONG byteSize;
	while(*moveSize)
	{
		byteSize = (*moveSize)*1024*1024;
		item = m_fileSizes.AddString((LPCSTR)CUtil::ByteSize2Str(byteSize));

		if(cfgSize == *moveSize) defItem = item;
		else
		{
			if(item <= defItem) defItem++;
		}
		m_fileSizes.SetItemData(item,*moveSize);
		moveSize++;
	}
	m_fileSizes.SetCurSel(defItem);

	m_movePassFileDir.EnableFolderBrowseButton();

	SetDlgItemText(IDC_PASSIMGBDSIZE,(LPCSTR)CUtil::ByteSize2Str(CPassImgBase::GetInstance()->GetPassDbFileSize()));
	m_movePassFileDir.SetWindowText((LPCSTR)m_passMoveDir);


	CPassImgBase *passDB = CPassImgBase::GetInstance();

	passDB->GetMountList(m_mountedFiles);
	m_mountedList.UpdateItemCount();


	CString loadedDBFile;
	bool bLoaded = passDB->GetLoadedPassDBFilePath(loadedDBFile);
	if(!bLoaded || passDB->GetPrimaryPassDBFilePath().CompareNoCase(loadedDBFile) != 0)
	{

		GetDlgItem(IDC_MOVENOW)->EnableWindow(FALSE);
		m_warnLabel.SetBackgroundColor(RGB(255,0,0));
		m_warnLabel.ShowWindow(SW_SHOW);
	}

	VERIFY(m_mountlistMenu.LoadMenu(IDR_MOUNTMENU));

	return TRUE;
}



void CPassDBMngmntOpt::OnMount()
{
	int iSel = m_mountedList.GetCurSel();
	if(iSel >= 0)
		m_mountedFiles[iSel]->m_bDoMount = true;
	m_mountedList.Invalidate(FALSE);

}

void CPassDBMngmntOpt::OnUnmount()
{
	int iSel = m_mountedList.GetCurSel();
	if(iSel >= 0)
		m_mountedFiles[iSel]->m_bDoMount = false;
	m_mountedList.Invalidate(FALSE);
}

void CPassDBMngmntOpt::OnChangePath()
{
	int iSel = m_mountedList.GetCurSel();
	if(iSel >= 0)
	{
		
		CFileDialog passFilePathDlg(TRUE,NULL,m_mountedFiles[iSel]->m_filePath,OFN_OVERWRITEPROMPT,"*.bin|*.bin||");
		if(passFilePathDlg.DoModal() == IDOK)
		{

			CString passFile2Mount = passFilePathDlg.GetPathName();
					
			if(passFile2Mount.CompareNoCase(CPassImgBase::GetInstance()->GetPrimaryPassDBFilePath()) != 0)
			{
				for(int i = 0; i <  m_mountedFiles.GetSize();i++)
				{
					if(i == iSel) continue;
					if(m_mountedFiles[i]->m_filePath.CompareNoCase(passFile2Mount) == 0)
					{
						AfxMessageBox(IDS_ERRFILEALREADYMOUNTED);
						return;
					}
				}

				m_mountedFiles[iSel]->m_filePath = passFile2Mount;
				m_mountedList.Invalidate(FALSE);
			}
			else
			{
				AfxMessageBox(IDS_ERRCANTMOUNTPRIMARYPASSDBFILE);
			}
		}
	}
}

bool CPassDBMngmntOpt::MovePassDBFile(const CString & passMoveDir)
{
	CString moveDir,passFileName,finalPassFileName,
		moveFilePath,uniqueStamp;

	char stampBuf[30];
	time_t now = time(0);
	struct tm *t1;
	t1  = localtime(&now);

	strftime(stampBuf,sizeof(stampBuf),"_%Y%m%d",t1);


	if(!paths_IsPathAbsolute(passMoveDir))
		moveDir = paths_ConcatPath(CUtil::GetAppDir(),passMoveDir);
	else
		moveDir = passMoveDir;

	paths_CreateIfNeeded(moveDir);

	passFileName.LoadString(IDS_PASSSTORAGEFILE);
	int extDot = passFileName.ReverseFind('.');

	int i = 0;
	do 
	{
		finalPassFileName = passFileName;
		if(i > 0)
			uniqueStamp.Format("%s_%d",stampBuf,i+1);
		else
			uniqueStamp = stampBuf;

		finalPassFileName.Insert(extDot,(LPCSTR)uniqueStamp);
		moveFilePath = paths_ConcatPath(moveDir,finalPassFileName);
		i++;
	} while (exists(path((LPCSTR)moveFilePath)));

	CString loadedPassDDFile,primaryPassDBFile;
	CPassImgBase *passImgDB = CPassImgBase::GetInstance();
	bool bOpenPrimaryPassDB = false;

	primaryPassDBFile = passImgDB->GetPrimaryPassDBFilePath();

	if(passImgDB->GetLoadedPassDBFilePath(loadedPassDDFile))
	{
		if(loadedPassDDFile.CompareNoCase((LPCSTR)primaryPassDBFile) == 0)
		{
			passImgDB->ClosePassImgBase();
			bOpenPrimaryPassDB = true;
		}		
	}
	if(!MoveFile((LPCSTR)primaryPassDBFile,(LPCSTR)moveFilePath))
		AfxMessageBox(CStrException::ErrorString(GetLastError()));

	
	if(bOpenPrimaryPassDB)
		passImgDB->OpenPassImgBase(NULL,true);
	return true;
}


void CPassDBMngmntOpt::OnBnClickedMoveNow()
{
	MovePassDBFile(m_passMoveDir);
	ULONGLONG newSize = 0;
	newSize = CPassImgBase::GetInstance()->GetPassDbFileSize();	
	SetDlgItemText(IDC_PASSIMGBDSIZE,(LPCSTR)CUtil::ByteSize2Str(newSize));
}



void CPassDBMngmntOpt::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;		
		if(nItem >= 0)
		{
			CPoint ptCursor;
			GetCursorPos(&ptCursor);
			CMenu* popup = m_mountlistMenu.GetSubMenu(0);
			popup->TrackPopupMenu(TPM_LEFTALIGN,ptCursor.x,ptCursor.y,this,NULL);
		}		
	}
	*pResult = 0;
}

BOOL CPassDBMngmntOpt::OnApply()
{
	UINT mountedFilesCfgItems[MAX_MOUNTED_FILES] =
	{
			CFG_MOUNT1_ITEM,
			CFG_MOUNT2_ITEM,
			CFG_MOUNT3_ITEM,
			CFG_MOUNT4_ITEM,
			CFG_MOUNT5_ITEM
	};
	CFileMountedPath *mountedPath = NULL;
	int i,iSel = m_fileSizes.GetCurSel();
	if(iSel < 0)
	{
		AfxMessageBox(IDS_ERRCHOOSEMAXPASSFILESIZE);
		return FALSE;
	}
	UpdateData();

	AppCfg().SetBool(CFG_ASKBEFOREPASSMOVE,m_bAskBeforePassFileMove);
	AppCfg().SetString(CFG_PASSMOVEDIRECTORY,m_passMoveDir);	
	AppCfg().SetNum(CFG_MAXPASS_SIZE_MB,m_fileSizes.GetItemData(iSel));


	for(i = 0;i < MAX_MOUNTED_FILES;i++)
	{
		mountedPath = m_mountedFiles[i];
		AppCfg().SetString(mountedFilesCfgItems[i],0,mountedPath->m_filePath);
		AppCfg().SetBool(mountedFilesCfgItems[i],1,mountedPath->m_bDoMount);		
	}

	CPassImgBase::GetInstance()->LoadMounts();
	return TRUE;

}

void CPassDBMngmntOpt::OnDestroy()
{
	m_mountedFiles.FreeAndFlush();

}