#include "stdafx.h"
#include "BackupDlg.h"
#include "../exceptions/StrException.h"
#include "../missing/paths.h"
#include "../classes/BrowseForDir.h"
#include "../PassportStorage/PassImgBase.h"
#include "backupdlg.h"
#include "MainCfg.h"
#include "CattleSQL.h"
#include "../missing/util.h"
#include <boost/regex.hpp>
#include "../mainfrm_ctrl.h"

// CBackupDlg dialog

IMPLEMENT_DYNAMIC(CBackupDlg, CPropertyPage)
CBackupDlg::CBackupDlg()
	: CPropertyPage(CBackupDlg::IDD)
{
	m_ask4BaseDir = AppCfg().GetBool(CFGID_ASK4BASEDIR);
	m_doPassImgBackup = AppCfg().GetBool(CFGID_DOPASSIMGDBBACKUP);
}

CBackupDlg::~CBackupDlg()
{
}

void CBackupDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BACKUPDIR, m_BackupDirectory);
	DDX_Control(pDX,IDC_TIMESPAN,m_timeSpan);
	DDX_Control(pDX,IDC_LASTCOPY,m_lastCopy);
	DDX_Control(pDX,IDC_BACKUPLIST,m_copiesList);
	DDX_Check(pDX,IDC_ASKFORDBLOC,m_ask4BaseDir);
	DDX_Check(pDX,IDC_DOPASSIMGBACKUP,m_doPassImgBackup);
}


BEGIN_MESSAGE_MAP(CBackupDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_MAKECOPY, OnMakeCopy)
	ON_BN_CLICKED(IDC_DELCOPY, OnDelCopy)
	ON_BN_CLICKED(IDC_RESTORECOPY, OnRestoreCopy)
	ON_MESSAGE(EN_CHANGE,OnDirChange)
	ON_BN_CLICKED(IDC_MAKEDBCOPY, OnBnClickedMakeDBCopy)
END_MESSAGE_MAP()

void CBackupDlg::FillListBox()
{
	CString wild;
	wild.LoadString(IDS_BACKUPWILDCARD);
	for(int i =0,c = m_copiesList.GetCount();i<c;i++) m_copiesList.DeleteString(0);
	m_copiesList.Dir(DDL_DIRECTORY,(LPCSTR)paths_ConcatPath(m_BackupDir,wild));
}


void CBackupDlg::OnMakeCopy()
{
	UpdateData();
	AppCfg().SetBool(CFGID_DOPASSIMGDBBACKUP,m_doPassImgBackup);

	MakeBackupCopy(this,m_BackupDir);
	FillListBox();
}

void CBackupDlg::MakeBackupCopy(CWnd *msgWnd,CString &strBackupDir)
{

	CString newBackupDir,checkExists;
	if(paths_IsPathAbsolute(strBackupDir))
	{
		newBackupDir = strBackupDir;
	}
	else
	{
		newBackupDir = paths_ConcatPath(CUtil::GetAppDir(),strBackupDir);
	}

	CDateTime now = CDateTime::now();
	CString backupDirNm("backup_");
	backupDirNm += now.GetDateInFmt(CDateTime::DDMMYYYY);
	
    newBackupDir =  paths_ConcatPath(newBackupDir,backupDirNm);


	checkExists = newBackupDir; 
	int r = 1;
	while(paths_DoesPathExist(checkExists) != DOES_NOT_EXIST)
	{
		checkExists.Format("%s_%d",(LPCSTR)newBackupDir,r);
		r++;
	}
	newBackupDir = checkExists;

	if(!paths_CreateIfNeeded(newBackupDir))
	{
		AfxMessageBox(CStrException::ErrorString(GetLastError()));
		return;
	}

	if(!SQLDB.DoBackup(newBackupDir))
        SQLDB.SQLMessageBox(IDS_ERRDOINGBACKUP);
	else
	{
		if(AppCfg().GetBool(CFGID_DOPASSIMGDBBACKUP))
		{
	
			if(!CPassImgBase::GetInstance()->DoBackup(newBackupDir))
			{
				//error
				AfxMessageBox(CStrException::ErrorString(GetLastError()));				
			}
		}
			
		
		AppCfg().SetString(CFGID_LASTCOPY,now.GetDateInFmt(CDateTime::YYYYMMDD));
		
	}	

}


bool CBackupDlg::GetChoosenBackupDir(CString &choosenBackupDir)
{
	int currItem;
	CString backupDirName,backupPath,newBackupDir;
	boost::regex dirNameExtract("\\[(\\w+)\\]");
	boost::cmatch matches;

	
	if((currItem = m_copiesList.GetCurSel()) >= 0)
	{
		m_copiesList.GetText(currItem,backupDirName);
	
		if(boost::regex_match((LPCSTR)backupDirName,matches,dirNameExtract))
		{
			if(matches.size() > 1)
			{
				string match(matches[1].first, matches[1].second);

				if(paths_IsPathAbsolute(m_BackupDir))
				{
					newBackupDir = m_BackupDir;
				}
				else
				{
					newBackupDir = paths_ConcatPath(CUtil::GetAppDir(),m_BackupDir);
				}
				backupPath = paths_ConcatPath(newBackupDir,CString(match.c_str()));
				choosenBackupDir = backupPath;
				return true;

			}
		}
	}

	return false;
}

void CBackupDlg::OnDelCopy()
{
	CString backupPath;
	if(GetChoosenBackupDir(backupPath))
	{
		if(CUtil::DeleteDirectory((LPCSTR)backupPath))
			FillListBox();
		else
			AfxMessageBox(CStrException::ErrorString(GetLastError()),MB_OK,0);
	}

}



void CBackupDlg::OnRestoreCopy()
{
	CString backupPath;
	if(GetChoosenBackupDir(backupPath))
	{
		if(AfxMessageBox(IDS_WARNRESTOREBACKUP,MB_YESNO,0) == IDYES)
		{	
			SendMainFrmMessage(WM_RESTORECOPY,(WPARAM)(LPCSTR)backupPath,(LPARAM)0);
			OnOK();
			EndDialog(IDOK);

		}
	}
	else AfxMessageBox(IDS_ERRNOSELBACKUP,MB_OK,0);
}

BOOL CBackupDlg::OnInitDialog()
{

	UINT backupTimes[][2] =
	{
		{IDS_BACKUPTIME1,1},
		{IDS_BACKUPTIME3,3},
		{IDS_BACKUPTIME7,7},
		{IDS_BACKUPTIME14,14},
		{IDS_BACKUPTIME30,30},
		{IDS_BACKUPTIME61,61},
		{IDS_BACKUPTIME183,183},
		{IDS_BACKUPTIME365,365}
	};

	CString backupTimeStr,noLastBackup;

	CPropertyPage::OnInitDialog();

	int item;

	int defdays = AppCfg().GetNum(CFGID_MAKECOPY),defitem  = 0;

	for(int i =0;i < 7;i++)
	{
		backupTimeStr.LoadString(backupTimes[i][0]);
		item = m_timeSpan.AddString(backupTimeStr);
		m_timeSpan.SetItemData(item,backupTimes[i][1]);
		if(backupTimes[i][1] == defdays) defitem = item;
	}

	m_timeSpan.SetCurSel(defitem);

	m_BackupDirectory.EnableFolderBrowseButton();
	
	m_BackupDir = AppCfg().GetString(CFGID_BACKUPDIR);
	if(m_BackupDir.GetLength() > 0)
        m_BackupDirectory.SetWindowText(m_BackupDir);
	
	FillListBox();
	

	CDateTime last;

	if(!last.ParseDate(CDateTime::YYYYMMDD,(LPCSTR)AppCfg().GetString(CFGID_LASTCOPY)))
	{
		noLastBackup.LoadString(IDS_NOLASTBACKUP);
		m_lastCopy.SetWindowText(noLastBackup);
		last.MakeNull();

	}
	else m_lastCopy.SetWindowText(last.GetDayDate());
	m_lastCopyTime = last;

return TRUE;
}


BOOL CBackupDlg::OnApply()
{
	UpdateData();
	AppCfg().SetString(CFGID_BACKUPDIR,m_BackupDir);
	AppCfg().SetNum(CFGID_MAKECOPY,m_timeSpan.GetItemData(m_timeSpan.GetCurSel()));
	AppCfg().SetBool(CFGID_ASK4BASEDIR,m_ask4BaseDir);
	AppCfg().SetBool(CFGID_DOPASSIMGDBBACKUP,m_doPassImgBackup);
	return TRUE;
}


LRESULT CBackupDlg::OnDirChange(WPARAM wParam,LPARAM lParam) 
{
	m_BackupDirectory.GetWindowText(m_BackupDir);
	FillListBox();
	return (LRESULT)0;
}


	
void CBackupDlg::OnBnClickedMakeDBCopy()
{
	CString copyDir,caption;

	UpdateData();
	caption.LoadString(IDS_CHOOSEDESTDIR);
	if(CBrowseForDir::BrowseForDir(copyDir,(LPCSTR)caption,this))
	{

		if(!SQLDB.DoBackup(copyDir))
			SQLDB.SQLMessageBox(IDS_ERRDOINGDBCOPY);
		else
		{
			if(m_doPassImgBackup)
			{
				if(!CPassImgBase::GetInstance()->DoBackup(copyDir))
				{
					//error
					AfxMessageBox(CStrException::ErrorString(GetLastError()));
				
				}
			}
		}
					
	

	}
	
}
