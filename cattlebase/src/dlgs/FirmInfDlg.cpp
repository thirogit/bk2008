#include "stdafx.h"
#include "FirmInfDlg.h"
#include <boost/regex.hpp>
#include "../datalimits.h"
#include "../consts.h"
#include "../CattleSQL.h"
#include "datatypes/CompanyInfo.h"

// CFirmInfDlg dialog

IMPLEMENT_DYNAMIC(CFirmInfDlg, CPropertyPage)
CFirmInfDlg::CFirmInfDlg()	: CPropertyPage(CFirmInfDlg::IDD),m_REGON(g_szREGONRegEx)
{
}

CFirmInfDlg::~CFirmInfDlg()
{
}

BEGIN_MESSAGE_MAP(CFirmInfDlg, CPropertyPage)
	

END_MESSAGE_MAP()

void CFirmInfDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text (pDX, IDC_COMPANYNAME  , m_sCompanyName  ) ;
	DDX_Text (pDX, IDC_COMPANYSTREET, m_sCompanyStreet ) ;
	DDX_Text (pDX, IDC_COMPANYPOBOX, m_sCompanyPOBox  ) ;
	DDX_Text (pDX, IDC_COMPANYCITY, m_sCompanyCity  ) ;
	DDX_Control (pDX, IDC_COMPANYZIP, m_CompanyZipCode  ) ;
	DDX_Control(pDX, IDC_COMPANYFARMNO, m_CompanyFarmNo);
	DDX_Control(pDX, IDC_COMPANYWETIDNO, m_CompanyWetIdNo);
	DDX_Control(pDX,IDC_COMPANYACCOUNTNO,m_CompanyAccountNo);
	DDX_Text(pDX,IDC_COMPANYBANK,m_sCompanyBankName);
	DDX_Control(pDX,IDC_COMPANYNIP,m_NIP);
	DDX_Control(pDX,IDC_COMPANYREGON,m_REGON);
	DDX_Text(pDX,IDC_COMPANYWETLICENCENO,m_WetLicenceNo);
	DDX_Control(pDX,IDC_LONGITUDE,m_Longitude);
	DDX_Control(pDX,IDC_LATITUDE,m_Latitude);
}


BOOL CFirmInfDlg::OnInitDialog()
{
	CString temp;

	if(!CPropertyPage::OnInitDialog()) return FALSE;

	SendDlgItemMessage(IDC_COMPANYNAME  , EM_LIMITTEXT,MAXNAME  ) ;
	SendDlgItemMessage(IDC_COMPANYSTREET, EM_LIMITTEXT,MAXSTREET ) ;
	SendDlgItemMessage(IDC_COMPANYPOBOX, EM_LIMITTEXT,MAXPOBOX) ;
	SendDlgItemMessage(IDC_COMPANYCITY, EM_LIMITTEXT,MAXCITY ) ;
	SendDlgItemMessage(IDC_COMPANYBANK,EM_LIMITTEXT,MAXBANKNAME);
	SendDlgItemMessage(IDC_COMPANYWETLICENCENO,EM_LIMITTEXT,MAXWETLICENCENO);	

	m_CompanyFarmNo .SetMask(g_EANMaskPL) ;
	m_CompanyWetIdNo .SetMask(g_WetNoMask) ;
	m_CompanyZipCode .SetMask(g_ZipMask) ;
	m_NIP .SetMask(g_NIPMask) ;

	
	const Company *pCompany = Company::GetCompany();

	m_sCompanyName = pCompany->GetCompanyName();
	m_sCompanyStreet = pCompany->GetStreet();
	m_sCompanyPOBox = pCompany->GetPOBox();
	m_sCompanyCity = pCompany->GetCity();
	m_sCompanyBankName = pCompany->GetBank();
	
	m_CompanyAccountNo.SetInputData(pCompany->GetAccountNo());
	
	m_NIP.SetWindowText(pCompany->GetNIP());
	m_CompanyZipCode.SetWindowText(pCompany->GetZipCode());
	m_CompanyFarmNo.SetWindowText(pCompany->GetFarmNo());
    m_CompanyWetIdNo.SetWindowText(pCompany->GetWetIdNo());
	m_REGON.SetWindowText(pCompany->GetREGON());
	m_WetLicenceNo = pCompany->GetWetLicNo();
//	m_Longitude.SetLongitude(pCompany->GetLongitude());
//	m_Latitude.SetLatitude(pCompany->GetLatitude());

	UpdateData(FALSE);


	return TRUE;
}
BOOL CFirmInfDlg::OnApply()
{	
	BOOL bApplyResult = TRUE;
#ifndef TRIAL_HENT
	UpdateData(FALSE);
	
	if(m_sCompanyName.IsEmpty()) {
		AfxMessageBox(IDS_ERRFILLFIRMNAME);
		return FALSE;
	}

	CString sFarmNo;
	CString sNIP;
	CString sZipCode;

	if(m_NIP.IsFullData()) {		
		m_NIP.GetWindowText(sNIP);		
	} 

	if(m_CompanyFarmNo.IsFullData()) {		
		m_CompanyFarmNo.GetWindowText(sFarmNo);		
	}
	else {
		AfxMessageBox(IDS_ERRFILLFARMNO);
		return FALSE;
	}

	if(m_CompanyZipCode.IsFullData()) {
		m_CompanyZipCode.GetWindowText(sZipCode);
	} 


	Company::Lock();
	CompanyForChanging *pCompany = Company::GetCompanyForChanging();	

	pCompany->SetCompanyName(m_sCompanyName);
	pCompany->SetStreet(m_sCompanyStreet);
	pCompany->SetPOBox(m_sCompanyPOBox);
	pCompany->SetCity(m_sCompanyCity);
	pCompany->SetNIP(sNIP);
	pCompany->SetFarmNo(sFarmNo);
	pCompany->SetZipCode(sZipCode);

		
	CString sWetIdNo;
	if(!m_CompanyWetIdNo.IsInputEmpty()) 
	{
		m_CompanyWetIdNo.GetWindowText(sWetIdNo);
	}

	pCompany->SetWetIdNo(sWetIdNo);
	
	CString sAccountNo;
	if(!m_CompanyAccountNo.IsInputEmpty()) 
	{
		sAccountNo = m_CompanyAccountNo.GetInputData();
	}

	pCompany->SetAccountNo(sAccountNo);
	
	pCompany->SetBank(m_sCompanyBankName);
	
	
	CString sREGON;
	m_REGON.GetWindowText(sREGON);
	pCompany->SetREGON(sREGON);

	pCompany->SetWetLicNo(m_WetLicenceNo);
	
	//cinfo->ChangeLongitude(m_Longitude.GetLongitude());
	//cinfo->ChangeLatitude(m_Latitude.GetLatitude());
	
	if(!pCompany->Save())
	{
		SQLDB.SQLMessageBox(IDS_ERRUPDATINGFIRMINFO);
		bApplyResult = FALSE;
	}

	Company::Unlock();
#endif
	return bApplyResult;
}

