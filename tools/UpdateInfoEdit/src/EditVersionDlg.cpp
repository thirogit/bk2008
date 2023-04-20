#include "stdafx.h"
#include "EditVersionDlg.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CEditVersionDlg,CVersionDlg)


CEditVersionDlg::CEditVersionDlg(CVersionInfo* pVI,UINT minVerNo,CWnd* pParent /*=NULL*/)
	: CVersionDlg(minVerNo,pParent)
{
	ASSERT(pVI);
	m_description = pVI->GetVersionDesc();
	m_archiveName = pVI->GetArchiveName();
	m_versionNo = pVI->GetVerNo();
	m_files.Append(pVI->GetVerFiles());
	m_pVI = pVI;
}

CEditVersionDlg::~CEditVersionDlg()
{

}

void CEditVersionDlg::SetupDialog()
{
	m_bOWArchName = GetArchiveName(m_pVI->GetVerNo()) != m_pVI->GetArchiveName();
}

CString CEditVersionDlg::GetWindowCaption()
{
	CString newVerCaption;
	newVerCaption.LoadString(IDS_EDITVERCAPTION);
	return newVerCaption;

}
CString CEditVersionDlg::GetOKBtnText()
{
	CString newVerOKBtnText;
	newVerOKBtnText.LoadString(IDS_EDITVEROKBTNTEXT);
	return newVerOKBtnText;

}

bool CEditVersionDlg::CommitVersionInfo()
{
	CVersionInfo *pVI = CreateNewVersionInfo();
	m_pVI->CopyFrom(*pVI);
	return true;
}