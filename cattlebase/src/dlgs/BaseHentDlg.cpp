#include "stdafx.h"
#include "BaseHentDlg.h"
#include "MainCfg.h"
#include "../datalimits.h"
#include "../global_arrays.h"

#define DEFAULTCOUNTRY ISO_POL_NUMBER 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CBaseHentDlg, CDialog)

		ON_CBN_SELCHANGE(IDC_COUNTRIES, OnCountryChange)
		ON_EN_SETFOCUS(IDC_ALIAS,OnAliasFocus)
		ON_EN_KILLFOCUS(IDC_ALIAS,OnAliasKillFocus)

END_MESSAGE_MAP()


CBaseHentDlg::CBaseHentDlg(CWnd* pParent/* = NULL*/) : CDialog(CBaseHentDlg::IDD,pParent), 
							   m_regon(g_szREGONRegEx),
							   m_idno(g_szIDNoRegEx),
							   m_issuedate(false)
{
	
}
void CBaseHentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control (pDX, IDC_ZIP  , m_ZipCode  ) ;
	DDX_Control (pDX, IDC_NIP, m_NIP ) ;
	DDX_Control (pDX, IDC_AIMRNO, m_FarmNo  ) ;
	DDX_Control (pDX, IDC_WETNO, m_WetNo  ) ;
	DDX_Control (pDX, IDC_EXTRAS, m_extras  ) ;
	DDX_Control(pDX, IDOK, m_HentAdd);
	DDX_Control(pDX, IDCANCEL, m_HentCancel);
	DDX_Control(pDX,IDC_COUNTRIES,m_Countries);
	DDX_Text(pDX,IDC_PLATE,m_plate);

	DDX_Text(pDX,IDC_NAME,m_name);
	DDX_Text(pDX,IDC_ALIAS,m_alias);
	DDX_Text(pDX,IDC_PHONE,m_phone);
	DDX_Text(pDX,IDC_CITY,m_city);
	DDX_Text(pDX,IDC_STREET,m_street);
	DDX_Text(pDX,IDC_POBOX,m_pobox);
	DDX_Control(pDX,IDC_HTYPE,m_htype);

	DDX_Control(pDX,IDC_PESEL,m_pesel);
	DDX_Control(pDX,IDC_REGON,m_regon);
	DDX_Control(pDX,IDC_IDNUMBER,m_idno);
	DDX_Control(pDX,IDC_ISSUEDATE,m_issuedate);
	DDX_Text(pDX,IDC_ISSUEPOST,m_issuepost);
	DDX_Control(pDX,IDC_ACCOUNTNO,m_accountno);
	DDX_Control(pDX,IDC_LONGITUDE,m_Longitude);
	DDX_Control(pDX,IDC_LATITUDE,m_Latitude);
	DDX_Text(pDX,IDC_BANKNAME,m_bankname);

	DDX_Text(pDX,IDC_WETLICENCENO,m_wetlicenceno);
	DDX_Text(pDX,IDC_CELLPHONENO,m_cellphoneno);
	DDX_Text(pDX,IDC_EMAILADDRESS,m_emailaddress);
		
}

BOOL  CBaseHentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString wndCaption = GetWindowCaption();
	SetWindowText(wndCaption);
	SetupDialogControls();
	InitDialogControls();
	return TRUE ;
}

void CBaseHentDlg::InitDialogControls()
{
	m_Countries.InitCountryCombo(DEFAULTCOUNTRY);
	OnCountryChange(); //change mask for zip and arimrno if nesesery

	HentType *ht = ary_htypes.GetHentTypeByHentTypeId(AppCfg().GetNum(CFG_PRESELHENTTYPE));
	m_htype.ReloadCombo(ht);
}


void CBaseHentDlg::SetupDialogControls()
{

	m_HentCancel.SetButtonImage( IDB_CANCEL );
	m_HentAdd.SetButtonImage(IDB_HENTADD);

	m_ZipCode .SetMask(g_ZipMask) ;
	m_NIP .SetMask(g_NIPMask) ;
	m_WetNo .SetMask(g_WetNoMask) ;	
	m_pesel.SetMask("###########");
	
	m_FarmNo.SetAbortWnd(&m_HentCancel);	
	m_accountno.SetAbortWnd(&m_HentCancel);

	SendDlgItemMessage(IDC_NAME,EM_LIMITTEXT,MAXNAME);	
	SendDlgItemMessage(IDC_ALIAS,EM_LIMITTEXT,MAXALIAS);
	SendDlgItemMessage(IDC_PHONE,EM_LIMITTEXT,MAXPHONE);
	SendDlgItemMessage(IDC_CITY,EM_LIMITTEXT,MAXCITY);
	SendDlgItemMessage(IDC_STREET,EM_LIMITTEXT,MAXSTREET);
	SendDlgItemMessage(IDC_POBOX,EM_LIMITTEXT,MAXPOBOX);
	SendDlgItemMessage(IDC_ISSUEPOST,EM_LIMITTEXT,MAXISSUEPOST);
	SendDlgItemMessage(IDC_BANKNAME,EM_LIMITTEXT,MAXBANKNAME);
	SendDlgItemMessage(IDC_PLATE,EM_LIMITTEXT,MAXPLATE);

	SendDlgItemMessage(IDC_WETLICENCENO,EM_LIMITTEXT,MAXWETLICENCENO);
	SendDlgItemMessage(IDC_CELLPHONENO,EM_LIMITTEXT,MAXCELLPHONE);
	SendDlgItemMessage(IDC_EMAILADDRESS,EM_LIMITTEXT,MAXEMAILADDRESS);

	m_Longitude.SetAbortWnd(&m_HentCancel);
	m_Latitude.SetAbortWnd(&m_HentCancel);


}

bool CBaseHentDlg::ValidateFields()
{

	UpdateData();

	if(m_FarmNo.IsInputEmpty()) 
	{
		AfxMessageBox(IDS_ERRFILLFARMNO);
		m_FarmNo.SetFocus();
		return false;
	}

	if(!m_FarmNo.IsFullData()) 
	{
		AfxMessageBox(IDS_PARTIALFARMNO);
		m_FarmNo.SetFocus();
		return false;
	}
	
	if(m_name.IsEmpty())
    {
		AfxMessageBox(IDS_ERREMPTYNAME);

		return false;
	}

	if(m_alias.IsEmpty()) 
	{
		AfxMessageBox(IDS_ERREMPTYALIAS);
		return false;
	}
	
	if(m_street.IsEmpty()) 
	{
		AfxMessageBox(IDS_ERREMPTYSTREET);
		return false;
	}

	if(m_pobox.IsEmpty()) 
	{
		AfxMessageBox(IDS_ERREMPTYPOBOX);
		return false;
	}

	if(m_city.IsEmpty()) 
	{
		AfxMessageBox(IDS_ERREMPTYCITY);
		return false;
	}

	if(m_ZipCode.IsInputEmpty()) 
	{
		AfxMessageBox(IDS_ERREMPTYZIP);
		m_ZipCode.SetFocus();
		return false;
	}	

	if(m_FarmNo.IsInputEmpty() ) 
	{
		AfxMessageBox(IDS_ERREMPTYARIMRNO);
		m_FarmNo.SetFocus();
		return false;
	}

	
	if(!m_htype.GetSelItemData())
	{
		AfxMessageBox(IDS_ERRSELHTYPE);
		m_htype.SetFocus();
		return false;
	}

	if(!m_regon.IsInputEmpty() && !m_regon.IsInputValid())
	{
		AfxMessageBox(IDS_ERRBADREGON);
		m_regon.SetFocus();
		return false;
	}
	if(!m_idno.IsInputEmpty() && !m_idno.IsInputValid())
	{
		AfxMessageBox(IDS_ERRBADIDNO);
		m_idno.SetFocus();
		return false;
	}

	if(!m_issuedate.IsInputEmpty() && !m_issuedate.IsDateValid())
	{
		AfxMessageBox(IDS_ERRBADISSUEDATE);
		m_issuedate.SetFocus();
		return false;
	}
	


	return true;
}

Hent* CBaseHentDlg::CreateHent()
{
	Hent* pNewHent = new Hent(NULL_HENTID);
	
	pNewHent->SetName(m_name);	
	pNewHent->SetAlias(m_alias);
	pNewHent->SetStreet(m_street);
	pNewHent->SetPOBox(m_pobox);
	pNewHent->SetCity(m_city);

	CString zip;
	if(!m_ZipCode.IsInputEmpty())
	{
		m_ZipCode.GetWindowText(zip);
		pNewHent->SetZip(zip);
	}

	pNewHent->SetCountry(m_Countries.GetSelectedCountry());
	pNewHent->SetPhone(m_phone);
	pNewHent->SetNIP(m_NIP.GetInputData());

	CString sFarmNo;
	m_FarmNo.GetWindowText(sFarmNo);
	pNewHent->SetFarmNo(sFarmNo);
	
	pNewHent->SetWetNo(m_WetNo.GetInputData());
	pNewHent->SetPlate(m_plate);

	CString sExtras;
	m_extras.GetWindowText(sExtras);
	pNewHent->SetExtras(sExtras);

	pNewHent->SetHentType(m_htype.GetSelItemData());
	pNewHent->SetAccountNo(m_accountno.GetInputData()); 
	pNewHent->SetBankName(m_bankname);
	pNewHent->SetPESEL(m_pesel.GetInputData());
	
	CString sREGON,sIdNo;

	m_regon.GetWindowText(sREGON);
	pNewHent->SetREGON(sREGON);

	m_idno.GetWindowText(sIdNo);
	pNewHent->SetIdNo(sIdNo);
	
	pNewHent->SetIssuePost(m_issuepost);

	CDateTime dtIssueDate;
	if(m_issuedate.GetDate(dtIssueDate))
		pNewHent->SetIssueDate(dtIssueDate);

	pNewHent->SetWetLicenceNo(m_wetlicenceno);
	pNewHent->SetCellPhoneNo(m_cellphoneno);
	pNewHent->SetEmailAddress(m_emailaddress);
	
	pNewHent->SetLatitude(m_Latitude.GetLatitude());
	pNewHent->SetLongitude(m_Longitude.GetLongitude());

	return pNewHent;

}

void  CBaseHentDlg::OnOK()
{
	if(!HAS_PRIVLEDGE(ADDMODOWN))
	{
		AfxMessageBox(IDS_NOPRIVS);
		return;
	}

	if(!ValidateFields()) 
		return;

	if(CommitHent())
		EndDialog(IDOK); 
}


void CBaseHentDlg::OnCountryChange()
{
	char szMask[15];
	int currCountry = m_Countries.GetCurSel();
	Country *cc = (Country*)m_Countries.GetItemData(currCountry);
	if(cc)
	{
		CString code2a = cc->GetCountryCode2A();
		_snprintf(szMask,sizeof(szMask),g_EANMaskXX,code2a[0],code2a[1]);
	
		CString input;
		bool setInput = !m_ZipCode.IsInputEmpty();
		
		input = m_FarmNo.GetInputData();
		
		m_FarmNo.SetMask(szMask);	
		
		m_FarmNo.SetInputData(input);
		
		if(setInput) m_ZipCode.GetWindowText(input);

		if(cc->GetCountryISONumber() == ISO_POL_NUMBER)
			m_ZipCode .SetMask (g_ZipMask);
		else
			m_ZipCode.SetMask();
			
		if(setInput) m_ZipCode.SetWindowText(input);
	}
	
}

void CBaseHentDlg::OnAliasFocus()
{
	if(GetDlgItem(IDC_ALIAS)->GetWindowTextLength() == 0)
	{
		CString l_alias;
		GetDlgItemText(IDC_NAME,l_alias);
		l_alias.Remove(' ');
		l_alias.MakeUpper();
		SetDlgItemText(IDC_ALIAS,l_alias);
	}
}

void CBaseHentDlg::OnAliasKillFocus()
{

}


