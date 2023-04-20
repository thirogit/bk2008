#include "stdafx.h"
#include "EditCowDlg.h"
#include "../classes/TempStringLoader.h"

CEditCowDlg::CEditCowDlg(Cow *pCow,CWnd *pParent/* = NULL*/) : 
	CBaseCowDlg(pCow,pParent), m_bLockEANEdit(false)
{
	ASSERT(m_pCow);
}

CString CEditCowDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_EDITCOWCAPTION);
}

void CEditCowDlg::SetupControls()
{
	m_CowAdd.SetWindowText(CTempStringLoader(IDS_BTNEDIT));
}

BOOL  CEditCowDlg::OnInitDialog()
{
	CBaseCowDlg::OnInitDialog();
	
	m_ean.SetEAN(m_pCow->GetEAN());
	m_motherean.SetEAN(m_pCow->GetMotherEAN());
	m_birthdate.SetDate(m_pCow->GetBirthDate());
	m_passno.SetInputData(m_pCow->GetPassNo());
	m_passdate.SetDate(m_pCow->GetPassDate());
	
	m_weight.SetWindowText(m_pCow->GetWeight().ToString());
	SetDlgItemText(IDC_BIRTHPLACE,m_pCow->GetBirthplace());
	m_extras.SetWindowText(m_pCow->GetExtras());

	m_myprice.SetWindowText(m_pCow->GetMyPrice().ToString());	

	if(m_bLockEANEdit)
		m_ean.ModifyStyle(0,ES_READONLY);

	m_hentowner.ReloadCombo(m_pCow->GetFirstOwner());
	m_combosex.ReloadCombo(m_pCow->GetSex().getCOWSEX());
	m_stock.ReloadCombo(m_pCow->GetStock());
	m_Latitude.SetLatitude(m_pCow->GetLatitude());
	m_Longitude.SetLongitude(m_pCow->GetLongitude());

	SetDlgItemText(IDC_HEALTHCERTNO,m_pCow->GetHealthCertNo());
	

	return TRUE ;
}

void CEditCowDlg::LockEANEdit()
{
	m_bLockEANEdit = true;
}
void CEditCowDlg::UpdateCow(Cow *pCow)
{
	Cow *pNewCow = CreateNewCow();
	pCow->ChangeEAN(pNewCow->GetEAN());	
	pCow->ChangeBirthDate(pNewCow->GetBirthDate());
	pCow->ChangeWeight(pNewCow->GetWeight());
	pCow->ChangeBirthPlace(pNewCow->GetBirthplace());
	pCow->ChangeExtras(pNewCow->GetExtras());
	pCow->ChangePassDate(pNewCow->GetPassDate());
	pCow->ChangePassNo(pNewCow->GetPassNo());
	pCow->ChangeMotherEAN(pNewCow->GetMotherEAN());
	pCow->ChangeStock(pNewCow->GetStock());
	pCow->ChangeFirstOwner(pNewCow->GetFirstOwner());
	pCow->ChangeSex(pNewCow->GetSex());
	pCow->ChangeMyPrice(pNewCow->GetMyPrice());
	pCow->ChangeLatitude(pNewCow->GetLatitude());
	pCow->ChangeLongitude(pNewCow->GetLongitude());
	pCow->ChangeHealthCertNo(pNewCow->GetHealthCertNo());
	
	delete pNewCow;
}

bool CEditCowDlg::CommitCow()
{
	UpdateCow(m_pCow);
	return true;
}


