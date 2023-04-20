#include "stdafx.h"
#include "UpdateInfoEdit.h"
#include "VersionDlg.h"
#include "VersionInfoStructFileTypes.h"
#include "MainCfg.h"
#include "common/PathUtil.h"
#include "../../../cattlebase/src/Utils/StringFormat.h"


IMPLEMENT_DYNAMIC(CVersionDlg, CDialog)

CVersionDlg::CVersionDlg(UINT minVerNo,CWnd* pParent /*=NULL*/)
	: CDialog(CVersionDlg::IDD, pParent),m_fileListCtrl(m_files)
{
	m_minVerNo = minVerNo;
	m_versionNo = m_minVerNo+1;
}

CVersionDlg::~CVersionDlg()
{
	
}

void CVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_VERNO_EDIT,m_versionNo);
	DDX_Text(pDX,IDC_ARCHNAME_EDIT,m_archiveName);
	DDX_Text(pDX,IDC_VERDESC_EDIT,m_description);
	DDX_Control(pDX,IDC_FILELIST,m_fileListCtrl);
	DDX_Check(pDX,IDC_OW_ARCHNAME,m_bOWArchName);
}

BOOL CVersionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(GetWindowCaption());
	SetDlgItemText(IDOK,GetOKBtnText());

	SendDlgItemMessage(IDC_ARCHNAME_EDIT,EM_LIMITTEXT,MAX_ARCHIVE_NAME);
	SendDlgItemMessage(IDC_VERDESC_EDIT,EM_LIMITTEXT,MAX_DESCRIPTION);


	SetupDialog();

	UpdateData(FALSE);
	
	
	OnOverrideArchName();

	return TRUE;
}



BEGIN_MESSAGE_MAP(CVersionDlg, CDialog)
	ON_BN_CLICKED(ID_ADDFILE,OnAddFile)
	ON_BN_CLICKED(ID_REMFILE,OnRemFile)
	ON_BN_CLICKED(IDC_OW_ARCHNAME,OnOverrideArchName)
	ON_EN_KILLFOCUS(IDC_VERNO_EDIT,OnOverrideArchName)
END_MESSAGE_MAP()

void CVersionDlg::SetupDialog()
{
	m_bOWArchName = FALSE;
}

void CVersionDlg::OnAddFile()
{
	char anyfileFilter[] = "Any file  (*.*)|*.*||";
	CFileDialog anyFileDlg(FALSE,NULL,NULL,OFN_FILEMUSTEXIST,anyfileFilter,this);
	if(anyFileDlg.DoModal() == IDOK)
	{
		CString verRootDir = AppCfg().GetString(CFGID_BASEDIR);
		CString newVerFile = anyFileDlg.GetPathName();
		if(CPathUtil::IsSubPath(newVerFile,verRootDir))
		{
			CString relPath = CPathUtil::GetRelativePath(newVerFile,verRootDir);
			m_files.Add(relPath);
			m_fileListCtrl.UpdateItemCount();
		}		
	}
}

void CVersionDlg::OnRemFile()
{	
	int curSel = m_fileListCtrl.GetCurSel();
	if(curSel >= 0)
	{
		m_fileListCtrl.SetRedraw(FALSE);
		m_files.RemoveAt(curSel);
		m_fileListCtrl.UpdateItemCount();
		m_fileListCtrl.SetRedraw(TRUE);
	}
}

void CVersionDlg::OnOverrideArchName()
{
	UpdateData();
	GetDlgItem(IDC_ARCHNAME_EDIT)->EnableWindow(m_bOWArchName);
	if(!m_bOWArchName)
	{
		CString archName = GetArchiveName(m_versionNo);
		SetDlgItemText(IDC_ARCHNAME_EDIT,archName);
	}
}
CString CVersionDlg::GetArchiveName(UINT verNo)
{
	return CStringFormat("VER%d.ZIP",verNo);
}

void CVersionDlg::OnOK()
{
	UpdateData();

	if(m_versionNo < m_minVerNo)
	{
		AfxMessageBox(CStringFormat("Version number should be greather than %d",m_minVerNo));
		return;
	}

	if(m_files.GetSize() == 0)
	{
		AfxMessageBox(IDS_NOVERFILES);
		return;
	}
	if(CommitVersionInfo())
		EndDialog(IDOK);
}

CVersionInfo* CVersionDlg::CreateNewVersionInfo()
{
	CVersionInfo *pVI = new CVersionInfo();
	pVI->SetArchiveName(m_archiveName);
	pVI->SetVerNo(m_versionNo);
	pVI->SetVersionDesc(m_description);
	pVI->AddVerFiles(m_files);
	pVI->SetOWArchName(m_bOWArchName == TRUE);
	pVI->SetVersionDate(CDateTime::now());
	return pVI;
}