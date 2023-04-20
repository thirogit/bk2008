#pragma once

#include "VersionDlg.h"
#include "VersionInfoClassTypes.h"

class CNewVersionDlg  : public CVersionDlg
{
	DECLARE_DYNAMIC(CNewVersionDlg)
public:
	static CVersionInfo* createNewVersion(UINT minVerNo,CWnd *pParent);

protected:
	CNewVersionDlg(UINT minVerNo,CWnd* pParent = NULL);
	virtual ~CNewVersionDlg();
protected:
	virtual CString GetWindowCaption();
	virtual CString GetOKBtnText();
	virtual bool CommitVersionInfo();
	

};
