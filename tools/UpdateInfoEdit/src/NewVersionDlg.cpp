#include "stdafx.h"
#include "NewVersionDlg.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CNewVersionDlg,CVersionDlg)

CVersionInfo* CNewVersionDlg::createNewVersion(UINT minVerNo,CWnd *pParent)
{
	CNewVersionDlg newVersionDlg(minVerNo,pParent);
	if(newVersionDlg.DoModal() == IDOK)
	{
		CVersionInfo *pVI = newVersionDlg.CreateNewVersionInfo();
		return pVI;
	}
	return NULL;

}



CNewVersionDlg::CNewVersionDlg(UINT minVerNo,CWnd* pParent /*=NULL*/)
	: CVersionDlg(minVerNo,pParent)
{
	
}

CNewVersionDlg::~CNewVersionDlg()
{

}


CString CNewVersionDlg::GetWindowCaption()
{
	CString newVerCaption;
	newVerCaption.LoadString(IDS_NEWVERCAPTION);
	return newVerCaption;

}
CString CNewVersionDlg::GetOKBtnText()
{
	CString newVerOKBtnText;
	newVerOKBtnText.LoadString(IDS_NEWVEROKBTNTEXT);
	return newVerOKBtnText;

}
bool CNewVersionDlg::CommitVersionInfo()
{
	return true;
}