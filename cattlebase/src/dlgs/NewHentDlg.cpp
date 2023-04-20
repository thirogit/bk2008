#include "stdafx.h"
#include "NewHentDlg.h"
#include "../CattleSQL.h"
#include "../dispacher/NewHentDispatcher.h"

CNewHentDlg::CNewHentDlg(CWnd* pParent/* = NULL*/) : 
							CBaseHentDlg(pParent),
							m_NewHent(NULL),
							m_setHent(NULL_HENTID)
{
}

CNewHentDlg::CNewHentDlg(Hent* pSetHent,CWnd* pParent/* = NULL*/) : 
							m_setHent(NULL_HENTID),
							m_NewHent(NULL)
{
	m_setHent.CopyFrom(*pSetHent);
}

Hent *CNewHentDlg::GetNewHent()
{
	return m_NewHent;
}

void CNewHentDlg::InitDialogControls()
{
	CBaseHentDlg::InitDialogControls();

	CString sName = m_setHent.GetName();
	if(!sName.IsEmpty())
		SetDlgItemText(IDC_NAME,sName);

	CString sAlias = m_setHent.GetAlias();
	if(!sAlias.IsEmpty())
		SetDlgItemText(IDC_ALIAS,sAlias);
	else
		OnAliasFocus();
	
	CString sStreet = m_setHent.GetStreet();
	if(!sStreet.IsEmpty())
		SetDlgItemText(IDC_STREET,sStreet);

	CString sPOBox = m_setHent.GetPOBox();
	if(!sPOBox.IsEmpty())
		SetDlgItemText(IDC_POBOX,sPOBox);

	CString sCity = m_setHent.GetCity();
	if(!sCity.IsEmpty())
		SetDlgItemText(IDC_CITY,sCity);
	
	CString sPhone = m_setHent.GetPhone();
	if(!sPhone.IsEmpty())
		SetDlgItemText(IDC_PHONE,sPhone);
	
	CString sPlate = m_setHent.GetPlate();
	if(!sPlate.IsEmpty())
		SetDlgItemText(IDC_PLATE,sPlate);

	CString sIssuePost = m_setHent.GetIssuePost();
	if(!sIssuePost.IsEmpty())
		SetDlgItemText(IDC_ISSUEPOST,sIssuePost);

	CString sBankName = m_setHent.GetBankName();
	if(!sBankName.IsEmpty())
		SetDlgItemText(IDC_BANKNAME,sBankName);

	Country* pCountry = m_setHent.GetCountry();
	if(pCountry)
		m_Countries.InitCountryCombo(pCountry->GetCountryISONumber());
	
	HentType* pHentType = m_setHent.GetHentType();
	if(pHentType)
		m_htype.ReloadCombo(pHentType);

	CDateTime dtIssueDate = m_setHent.GetIssueDate();
	if(!dtIssueDate.IsNull())
		m_issuedate.SetDate(dtIssueDate);

	CString sZip = m_setHent.GetZip();
	if(!sZip.IsEmpty())
		m_ZipCode.SetWindowText(sZip);

	CString sNIP = m_setHent.GetNIP();
	if(!sNIP.IsEmpty())
		m_NIP.SetInputData(sNIP);
	
	CString sFarmNo = m_setHent.GetFarmNo();
	if(!sFarmNo.IsEmpty())
		m_FarmNo.SetInitialValue(sFarmNo);
	
	CString sWetNo = m_setHent.GetWetNo();
	if(!sWetNo.IsEmpty())
		m_WetNo.SetInputData(sWetNo);

	CString sExtras = m_setHent.GetExtras(); 
	if(!sExtras.IsEmpty())
		m_extras.SetWindowText(sExtras);
	
	CString sPESEL = m_setHent.GetPESEL();
	if(!sPESEL.IsEmpty())
		m_pesel.SetInputData(sPESEL);
	
	CString sREGON = m_setHent.GetREGON();
	if(!sREGON.IsEmpty())
		m_regon.SetWindowText(sREGON);
	
	CString sIdNo = m_setHent.GetIdNo();
	if(!sIdNo.IsEmpty())
		m_idno.SetWindowText(sIdNo);
	
	CString sAccountNo = m_setHent.GetAccountNo();
	if(!sAccountNo.IsEmpty())
		m_accountno.SetInputData(sAccountNo);

	CString sWetLicenceNo = m_setHent.GetWetLicenceNo();
	if(!sWetLicenceNo.IsEmpty())
		m_wetlicenceno = sWetLicenceNo;


	CString sCellPhoneNo = m_setHent.GetCellPhoneNo();
	if(!sCellPhoneNo.IsEmpty())
		m_cellphoneno = sCellPhoneNo;

	CString sEmailAddress = m_setHent.GetEmailAddress();
	if(!sEmailAddress.IsEmpty())
		m_emailaddress = sEmailAddress;

	Latitude latitude = m_setHent.GetLatitude();
	if(!latitude.IsNull())
		m_Latitude.SetLatitude(latitude);

	Longitude longitude = m_setHent.GetLongitude();
	if(!longitude.IsNull())
		m_Longitude.SetLongitude(longitude);


}

CString CNewHentDlg::GetWindowCaption()
{
	CString wndCaption;
	wndCaption.LoadString(IDS_HENTCAPTIONNEW);
	return wndCaption;

}

bool CNewHentDlg::CommitHent()
{
	Hent *pNewHent = CreateHent();
	if(!pNewHent->InsertThisHent())
	{
		SQLDB.SQLMessageBox(IDS_ERRINSERTINGHENT);
		delete pNewHent;
		return false;
	}	
	m_NewHent = pNewHent;
	NewHentDispatcher::Dispatch(pNewHent);		
	return true;	
}

