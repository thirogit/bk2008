#ifndef __COMPANYDLG_H__
#define __COMPANYDLG_H__

#include "maskededit\OXMaskedEdit.h"
#include "global_arrays.h"
#include "../ctrls/AccountNoEdit.h"
#include "../FilterEdit/FilterEdit.h"
#include "../ctrls/LongitudeEdit.h"
#include "../ctrls/LatitudeEdit.h"

class CFirmInfDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CFirmInfDlg)

public:
	CFirmInfDlg();
	virtual ~CFirmInfDlg();

	enum { IDD = IDD_COMPANY };

	BOOL OnApply();
protected:
	CString m_sCompanyName;
	CString m_sCompanyStreet;
	CString m_sCompanyPOBox;
	CString m_sCompanyCity;

	COXMaskedEdit	m_CompanyZipCode;
	COXMaskedEdit	m_CompanyFarmNo;
	COXMaskedEdit	m_CompanyWetIdNo;

	CAccountNoEdit	m_CompanyAccountNo;
	CString			m_sCompanyBankName;

	COXMaskedEdit	m_NIP;
	CFilterEdit		m_REGON;
	CString			m_WetLicenceNo;
	CLongitudeEdit	m_Longitude;
	CLatitudeEdit	m_Latitude;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

#endif 