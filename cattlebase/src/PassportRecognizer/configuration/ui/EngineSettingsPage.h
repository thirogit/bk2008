#ifndef __ENGINESETTINGSPAGE_H__
#define __ENGINESETTINGSPAGE_H__

#include "../EngineSettings.h"
#include <afxeditbrowsectrl.h>

class CEngineSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEngineSettingsPage)

public:
	CEngineSettingsPage(EngineSettings* pEngineSettings);
	virtual ~CEngineSettingsPage();

	enum { IDD = IDD_RECOGNITIONENGINECFG };

protected:

	CMFCEditBrowseCtrl m_EnginePath;
	CMFCEditBrowseCtrl m_LicenseFilePath;
	CString m_DeveloperSN;
	CString m_ProjectId;

	EngineSettings* m_pEngineSettings;
protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
		
	DECLARE_MESSAGE_MAP()

};

#endif