#include "stdafx.h"
#include "ArchiveOpt.h"
#include "MainCfg.h"
#include "../missing/paths.h"
#include "../missing/util.h"
#include "../exceptions/StrException.h"
#include <boost/regex.hpp>
#include <string>
#include "../msg_defs.h"

IMPLEMENT_DYNAMIC(CArchiveOpt, CPropertyPage)
CArchiveOpt::CArchiveOpt()	: CPropertyPage(CArchiveOpt::IDD)
{
	m_archiveDirStr = AppCfg().GetString(CFG_ARCHIVEDIR);
	m_doPassDBArchive = AppCfg().GetBool(CFG_DOPASSIMGDBARCHIVE);
}

CArchiveOpt::~CArchiveOpt()
{
}

void CArchiveOpt::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ARCHIVEDIR, m_archiveDir);
	DDX_Control(pDX,IDC_ARCHIVELIST,m_archivesList);
	DDX_Check(pDX,IDC_DOPASSDBARCHIVE,m_doPassDBArchive);
	
}

BEGIN_MESSAGE_MAP(CArchiveOpt, CPropertyPage)
	ON_BN_CLICKED(IDC_DOARCHIVE, OnBnClickedDoArchive)
	ON_BN_CLICKED(IDC_DELARCHIVEBTN, OnBnClickedDelArchiveBtn)
	ON_MESSAGE(EN_CHANGE,OnDirChange)
END_MESSAGE_MAP()



BOOL CArchiveOpt::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_archiveDir.EnableFolderBrowseButton();

	if(!paths_IsPathAbsolute(m_archiveDirStr))
		m_archiveDirStr = paths_ConcatPath(CUtil::GetAppDir(),m_archiveDirStr);

	m_archiveDir.SetWindowText(m_archiveDirStr);
	FillListBox();

	return TRUE;
}

void CArchiveOpt::OnBnClickedDoArchive()
{
	CString backupPath;
	
	if(AfxMessageBox(IDS_DOARCHIVEWARN,MB_YESNO,0) == IDYES)
	{	
		OnApply();
		AfxGetMainWnd()->PostMessage(WM_DOARCHIVE);		
		EndDialog(IDOK);
	}	
}

void CArchiveOpt::FillListBox()
{
	CString wild;
	wild.LoadString(IDS_ARCHIVEWILDCARD);
	for(int i =0,c = m_archivesList.GetCount();i<c;i++) m_archivesList.DeleteString(0);
	m_archivesList.Dir(DDL_DIRECTORY,(LPCSTR)paths_ConcatPath(m_archiveDirStr,wild));
}


void CArchiveOpt::OnBnClickedDelArchiveBtn()
{
	CString archiveName;
	CString archivePath;
	int currItem;
	boost::regex dirNameExtract("\\[(\\w+)\\]");
	boost::cmatch matches;
	if((currItem = m_archivesList.GetCurSel()) >= 0)
	{
		m_archivesList.GetText(currItem,archiveName);

		if(boost::regex_match((LPCSTR)archiveName,matches,dirNameExtract))
		{
			if(matches.size() > 1)
			{
				std::string match(matches[1].first, matches[1].second);				
				archivePath = paths_ConcatPath(m_archiveDirStr,CString(match.c_str()));

				if(CUtil::DeleteDirectory((LPCSTR)archivePath))
					FillListBox();
				else
					AfxMessageBox(CStrException::ErrorString(GetLastError()),MB_OK,0);
			}
		}
					
	}
}
LRESULT CArchiveOpt::OnDirChange(WPARAM wParam,LPARAM lParam) 
{
	m_archiveDir.GetWindowText(m_archiveDirStr);
	FillListBox();
	return (LRESULT)0;
}
BOOL CArchiveOpt::OnApply()
{
	UpdateData();

	m_archiveDir.GetWindowText(m_archiveDirStr);

	if(!paths_IsPathAbsolute(m_archiveDirStr))
		m_archiveDirStr = paths_ConcatPath(CUtil::GetAppDir(),m_archiveDirStr);
		
	AppCfg().SetString(CFG_ARCHIVEDIR,m_archiveDirStr);
	AppCfg().SetBool(CFG_DOPASSIMGDBARCHIVE,m_doPassDBArchive);
	
	return TRUE;
}