#pragma once

#include "VersionDlg.h"
#include "VersionInfoClassTypes.h"

class CEditVersionDlg  : public CVersionDlg
{
	DECLARE_DYNAMIC(CEditVersionDlg)
public:
	
	CEditVersionDlg(CVersionInfo* pVI,UINT minVerNo,CWnd* pParent = NULL);
	virtual ~CEditVersionDlg();
protected:
	virtual CString GetWindowCaption();
	virtual CString GetOKBtnText();
	virtual void SetupDialog();
	virtual bool CommitVersionInfo();
	CVersionInfo* m_pVI;

};
