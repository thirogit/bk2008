#include "stdafx.h"
#include "EditHentDlg.h"
#include "../CattleSQL.h"
#include "../dispacher/NewHentDispatcher.h"

CEditHentDlg::CEditHentDlg(Hent *pHent2Edit,CWnd* pParent/* = NULL*/) :	CBaseHentDlg(pParent),m_pHent2Edit(pHent2Edit)
{
}

CString CEditHentDlg::GetWindowCaption()
{
	CString wndCaption;
	wndCaption.LoadString(IDS_HENTCAPTIONEDIT);
	return wndCaption;
}

void CEditHentDlg::SetupDialogControls()
{
	CString btnCaption;
	btnCaption.LoadString(IDS_BTNEDIT);
	
	CBaseHentDlg::SetupDialogControls();
	
	m_HentAdd.SetWindowText(btnCaption);
}

void CEditHentDlg::InitDialogControls()
{
	m_Countries.InitCountryCombo(m_pHent2Edit->GetCountry()->GetCountryISONumber());
	OnCountryChange();
	m_ZipCode.SetWindowText((LPCSTR)m_pHent2Edit->GetZip());
	m_NIP.SetInputData((LPCSTR)m_pHent2Edit->GetNIP());
	m_FarmNo.SetInitialValue(m_pHent2Edit->GetFarmNo());
	m_WetNo.SetInputData((LPCSTR)m_pHent2Edit->GetWetNo());
	m_extras.SetWindowText((LPCSTR)m_pHent2Edit->GetExtras());
	m_alias = m_pHent2Edit->GetAlias();
	m_pesel.SetInputData(m_pHent2Edit->GetPESEL());
	m_regon.SetWindowText(m_pHent2Edit->GetREGON());
	m_idno.SetWindowText(m_pHent2Edit->GetIdNo());
	m_issuedate.SetDate(m_pHent2Edit->GetIssueDate());
	m_accountno.SetInputData(m_pHent2Edit->GetAccountNo());
	m_plate = m_pHent2Edit->GetPlate();
	m_name = m_pHent2Edit->GetName();
	m_phone = m_pHent2Edit->GetPhone();
	m_pobox = m_pHent2Edit->GetPOBox();
	m_city = m_pHent2Edit->GetCity();
	m_street = m_pHent2Edit->GetStreet();
	m_bankname = m_pHent2Edit->GetBankName();
	m_issuepost = m_pHent2Edit->GetIssuePost();
	m_htype.ReloadCombo(m_pHent2Edit->GetHentType());
	m_wetlicenceno = m_pHent2Edit->GetWetLicenceNo();
	m_cellphoneno = m_pHent2Edit->GetCellPhoneNo();
	m_emailaddress = m_pHent2Edit->GetEmailAddress();
	m_Latitude.SetLatitude(m_pHent2Edit->GetLatitude());
	m_Longitude.SetLongitude(m_pHent2Edit->GetLongitude());
	
	UpdateData(FALSE);

}


void CEditHentDlg::UpdateHent(Hent *pHent)
{
	Hent *pNewHent = CreateHent();

	pHent->ChangeCountry(pNewHent->GetCountry());
	pHent->ChangeZip(pNewHent->GetZip());
	pHent->ChangeFarmNo(pNewHent->GetFarmNo());
	pHent->ChangeWetNo(pNewHent->GetWetNo());
	pHent->ChangeExtras(pNewHent->GetExtras());
	pHent->ChangeAlias(pNewHent->GetAlias());
	pHent->ChangeREGON(pNewHent->GetREGON());
	pHent->ChangeIdNo(pNewHent->GetIdNo());
	pHent->ChangeIssueDate(pNewHent->GetIssueDate());
	pHent->ChangeAccountNo(pNewHent->GetAccountNo());
	pHent->ChangePlate(pNewHent->GetPlate());
	pHent->ChangeName(pNewHent->GetName());
	pHent->ChangePhone(pNewHent->GetPhone());
	pHent->ChangePOBox(pNewHent->GetPOBox());
	pHent->ChangeCity(pNewHent->GetCity());
	pHent->ChangeStreet(pNewHent->GetStreet());
	pHent->ChangeBankName(pNewHent->GetBankName());
	pHent->ChangeIssuePost(pNewHent->GetIssuePost());
	pHent->ChangeHentType(pNewHent->GetHentType());
	pHent->ChangePESEL(pNewHent->GetPESEL());
	pHent->ChangeNIP(pNewHent->GetNIP());

	pHent->ChangeWetLicenceNo(pNewHent->GetWetLicenceNo());
	pHent->ChangeCellPhoneNo(pNewHent->GetCellPhoneNo());
	pHent->ChangeEmailAddress(pNewHent->GetEmailAddress());
	pHent->ChangeLatitude(pNewHent->GetLatitude());
	pHent->ChangeLongitude(pNewHent->GetLongitude());
	

	delete pNewHent;
}

bool CEditHentDlg::CommitHent()
{
	HentType* pCurrentHentType = m_pHent2Edit->GetHentType();
	UpdateHent(m_pHent2Edit);

	if(!m_pHent2Edit->UpdateThisHent())
	{
		SQLDB.SQLMessageBox(IDS_ERRUPDATINGHENT);
		m_pHent2Edit->RollbackChanges();
		return false;
	}
	m_pHent2Edit->CommitChanges();
	
	if(m_pHent2Edit->GetHentType() != pCurrentHentType)
		NewHentDispatcher::ReDispatch(m_pHent2Edit);			
		
	return true;	
}

