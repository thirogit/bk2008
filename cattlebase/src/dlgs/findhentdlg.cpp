#include "stdafx.h"
#include "findhentdlg.h"
#include "finddlg.h"
#include "mainfrm_ctrl.h"
#include "../global_arrays.h"
#include "../classes/match/HentMatcher.h"
#include "../classes/forallactions/FindAction.h"


BEGIN_MESSAGE_MAP(CFindHentDlg, CFindBaseDlg)
	//{{AFX_MSG_MAP(CFindHentDlg)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFindHentDlg::CFindHentDlg() : CFindBaseDlg(CFindHentDlg::IDD), 
							m_arimrno(2,12),m_wetno(0,8),m_nip(0,10),
							m_pesel(0,11),m_regon(0,9),m_idno(3,7),m_accountno(0,26),
							m_issuedate(false)
{

}

void CFindHentDlg::DoDataExchange(CDataExchange* pDX)
{
	CFindBaseDlg::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CFindHentDlg)
	DDX_Text (pDX, IDC_BYNAME  , m_name ) ;
	DDX_Text (pDX, IDC_BYALIAS  , m_alias ) ;
	DDX_Control (pDX, IDC_BYARIMRNO  , m_arimrno ) ;
	DDX_Control (pDX, IDC_BYWETNO  , m_wetno ) ;
	DDX_Control (pDX, IDC_BYNIP  , m_nip ) ;
	DDX_Text (pDX, IDC_BYSTREET  , m_street ) ;
	DDX_Text (pDX, IDC_BYPOBOX  , m_pobox ) ;
	DDX_Text (pDX, IDC_BYCITY  , m_city ) ;
	DDX_Text (pDX, IDC_BYZIP  , m_zip ) ;
	DDX_Text (pDX, IDC_BYPHONE  , m_phone ) ;
	DDX_Control (pDX, IDC_BYCOUNTRY  , m_country ) ;
	DDX_Text (pDX, IDC_BYPLATE  , m_plate ) ;
	DDX_Control (pDX, IDC_BYHTYPE  , m_htype ) ;


	DDX_Control (pDX, IDC_BYPESEL, m_pesel);
	DDX_Control (pDX, IDC_BYREGON, m_regon);
	DDX_Control (pDX, IDC_BYIDNO, m_idno);
	DDX_Control (pDX, IDC_BYISSUEDATE, m_issuedate);
	DDX_Text (pDX, IDC_BYISSUEPOST, m_issuepost);
	DDX_Control (pDX, IDC_BYACCOUNTNO,m_accountno);
	DDX_Text (pDX, IDC_BYBANKNAME, m_bankname);
				
	//}}AFX_DATA_MAP
}


BOOL CFindHentDlg::OnInitDialog()
{

	CFindBaseDlg::OnInitDialog();

	int i,s,item;
	char str[40];

	Country *cc;

	item =  m_country.AddString("*");
	m_country.SetItemData(item,ITEM_NEUTRAL);
	m_country.SetCurSel(item);

	for(i=0,s=ary_countries.GetSize();i<s;i++)
	{
		cc = ary_countries[i];
		sprintf(str,"%-.32s (%.2s)",(LPCSTR)(cc->GetCountryName()),(LPCSTR)cc->GetCountryCode2A());
		item = m_country.AddString(str);
		m_country.SetItemData(item,(DWORD_PTR)cc);
	}

	m_htype.InitFindCombo();

	return TRUE;
}

int CFindHentDlg::GoSearch(CFindSetup& setup)
{
	UpdateData();

	CHentMatcher hentMatcher;

	if(!m_issuedate.IsInputEmpty())
	{
		CDateTime issuedate;
		if(!m_issuedate.GetDate(issuedate))
		{
			AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
			return (-1);
		}
		else
			hentMatcher.FindIssueDate(issuedate,issuedate,SPAN_BETWEEN);
	}


	if(m_name.GetLength() > 0)
		hentMatcher.FindName(m_name);

	if(m_alias.GetLength() > 0)
		hentMatcher.FindAlias(m_alias);

	if(m_street.GetLength() > 0)
		hentMatcher.FindStreet(m_street);

	if(m_pobox.GetLength() > 0)
		hentMatcher.FindPoBox(m_pobox);

	if(m_phone.GetLength() > 0)
		hentMatcher.FindPhoneNo(m_phone);
	
	if(m_city.GetLength() > 0)
		hentMatcher.FindCity(m_city);

	if(m_zip.GetLength() > 0)
		hentMatcher.FindZip(m_zip);

	if(m_issuepost.GetLength() > 0)
		hentMatcher.FindIssuePost(m_issuepost);

	if(m_bankname.GetLength() > 0)
		hentMatcher.FindBankName(m_bankname);

	if(setup.ByExtras())
		hentMatcher.FindExtras(setup.GetExtras());

	if(m_plate.GetLength() > 0)
		hentMatcher.FindPlate(m_plate);

	if(m_arimrno.GetWindowTextLength() > 0)
		hentMatcher.FindARiMRNo(m_arimrno.GetWindowText());

	if(m_wetno.GetWindowTextLength() > 0)
		hentMatcher.FindWetNo(m_wetno.GetWindowText());

	if(m_nip.GetWindowTextLength() > 0)
		hentMatcher.FindNIP(m_nip.GetWindowText());

	if(!m_htype.IsNeutral())
		hentMatcher.FindHentType(m_htype.GetSelItemData());
	
	if(m_pesel.GetWindowTextLength() > 0)
		hentMatcher.FindPesel(m_pesel.GetWindowText());
	
	if(m_regon.GetWindowTextLength() > 0)
		hentMatcher.FindRegon(m_regon.GetWindowText());

	if(m_idno.GetWindowTextLength() > 0)
		hentMatcher.FindIdNo(m_idno.GetWindowText());

	


	if(m_accountno.GetWindowTextLength() > 0)
		hentMatcher.FindAccountNo(m_accountno.GetWindowText());
	
	Country * country = (Country*)m_country.GetItemData(m_country.GetCurSel());
	if((int)country != ITEM_NEUTRAL)
		hentMatcher.FindCountry(country);
	
	if(setup.ByCreationTime())
	{
		CDateTime time1,time2;
		TIMESPAN creationSearchSpan = setup.CreationTimeSpan(time1,time2);
		hentMatcher.FindCreationTime(time1,time2,creationSearchSpan);
	}
	
	if(setup.ByLastModificationTime())
	{
		CDateTime time1,time2;
		TIMESPAN lastModificationSearchSpan = setup.LastModificationTimeSpan(time1,time2);
		hentMatcher.FindLastModificationTime(time1,time2,lastModificationSearchSpan);
	}

	UserData *pCreator = setup.SelectedCreator();
	if(pCreator)
		hentMatcher.FindCreator(pCreator);
		
	UserData* pLastModifier = setup.SelectedLastModifier();
	if(pLastModifier)
		hentMatcher.FindLastModifier(pLastModifier);

	if(setup.GetInResult())
		hentMatcher.FindMarked();

	CFindAction<CHentArray::ArrayAction,CHentMatcher,Hent> findHent(setup.GetAddToResult(),&hentMatcher);	
	ary_hents.ForAllAction(findHent);
	
	setup.ChangeMainGrid(HENTSGRID);
	
	return findHent.GetFoundCount();

}
