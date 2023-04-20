#ifndef __SCANNERSETTINGSPAGE_H__
#define __SCANNERSETTINGSPAGE_H__

#include "Ctrls/IntValueSliderCtrl.h"
#include "../ScannerSettings.h"

class CScannerSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CScannerSettingsPage)

public:
	enum { IDD = IDD_RECOGNITIONSCANNEROPTS };
	
	CScannerSettingsPage(ScannerSettings *pScannerSettings);
	virtual ~CScannerSettingsPage();

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);

	CIntValueSliderCtrl m_BrightnessSlider;
	CComboBox			m_ResolutionCb;
	CIntValueSliderCtrl m_ContrastSlider;

	ScannerSettings *m_pScannerSettings;
		
protected:	

	DECLARE_MESSAGE_MAP()

};


#endif