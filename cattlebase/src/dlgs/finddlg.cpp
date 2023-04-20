#include "stdafx.h"
#include "finddlg.h"
#include "mainfrm_ctrl.h"
#include "MainCfg.h"

BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	//{{AFX_MSG_MAP(CFindDlg)

	ON_BN_CLICKED(IDC_GOSEARCH,OnGoSearch)
	ON_BN_CLICKED(IDC_NEWSEARCH,OnSetNewSearch)
	ON_BN_CLICKED(IDC_ADDTOFOUND,OnSetAddToFound)
	ON_BN_CLICKED(IDC_INRESULT,OnSetInResult)
	ON_BN_CLICKED(IDC_BYCREATION, OnCheckClicked)
	ON_BN_CLICKED(IDC_BYMODIFICATION, OnCheckClicked)
	ON_BN_CLICKED(IDOK,OnOK)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFindDlg::CFindDlg() : CDialog(CFindDlg::IDD)
{
m_bByCreatTime = FALSE;
m_bByModifTime = FALSE;	
m_bNewSearch = TRUE;
m_bInResult = FALSE;
m_bAddToFound = FALSE;	

m_bSortFound = AppCfg().GetBool(CFG_SORTFOUND);
m_bChangeGridTab  = AppCfg().GetBool(CFG_CHANGETAB);

}


CFindDlg::~CFindDlg()
{
	
}

void CFindDlg::OnDestroy()
{
	UpdateData();
	AppCfg().SetBool(CFG_SORTFOUND,m_bSortFound);
	AppCfg().SetBool(CFG_CHANGETAB,m_bChangeGridTab);
}


void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	
	DDX_Control(pDX, IDC_GOSEARCH, m_gosearch);
	DDX_Control(pDX, IDCANCEL, m_cancel);

	DDX_Text(pDX, IDC_FINDEXTRAS, m_extras);
	
	DDX_Control(pDX, IDC_CREATTIME, m_creattimespan);
	DDX_Control(pDX, IDC_MODIFTIME, m_modiftimespan);

	DDX_Check(pDX,IDC_BYCREATIONCHECK,m_bByCreatTime);
	DDX_Check(pDX,IDC_BYMODIFICATIONCHECK,m_bByModifTime);

	DDX_Control(pDX,IDC_CREATUSER,m_creator);
	DDX_Control(pDX,IDC_MODIFYUSER,m_modifier);

	
	DDX_Check(pDX,IDC_SORTFOUND,m_bSortFound);
	DDX_Check(pDX,IDC_CHNGTAB,m_bChangeGridTab);


	DDX_Check(pDX,IDC_NEWSEARCH,m_bNewSearch);
	DDX_Check(pDX,IDC_INRESULT,m_bInResult);
	DDX_Check(pDX,IDC_ADDTOFOUND,m_bAddToFound);
	


		
	
}


BOOL CFindDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
	OnCheckClicked();

	m_gosearch.SetButtonImage( IDB_SEARCH);
	m_cancel.SetButtonImage( IDB_CANCEL );

	
	m_dlgPropSheet.AddPage(&m_findcow);
	m_dlgPropSheet.AddPage(&m_findhent);
	m_dlgPropSheet.AddPage(&m_findindoc);
	m_dlgPropSheet.AddPage(&m_findoutdoc);
	m_dlgPropSheet.AddPage(&m_findinv);
	m_dlgPropSheet.AddPage(&m_findexpense);


	m_dlgPropSheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_dlgPropSheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_dlgPropSheet.ModifyStyle( 0, WS_TABSTOP );

	CRect rcSheet;
	GetDlgItem( IDC_CREATTIME )->GetWindowRect( &rcSheet );
	ScreenToClient( &rcSheet );

	int top = rcSheet.top;

	GetDlgItem( IDC_GOSEARCH )->GetWindowRect( &rcSheet );
	ScreenToClient( &rcSheet );

	m_dlgPropSheet.SetWindowPos( NULL,0,0, rcSheet.left-10,top-10,
				SWP_NOZORDER |SWP_NOACTIVATE );

	m_creator.InitFindCombo();
	m_modifier.InitFindCombo();

	UpdateData(FALSE);

	OnSetNewSearch();
  	
	return TRUE;
}


void CFindDlg::OnOK()
{
	OnGoSearch();
}

void CFindDlg::OnGoSearch(void)
{
	UpdateData();	

	int found = ((CFindBaseDlg*)m_dlgPropSheet.GetActivePage())->GoSearch(*this);
	
	if(found >= 0) //negative when validation error
	{		
		if(m_bSortFound)
			SendMainFrmMessage(WM_COMMAND,IDM_GRID_SORTMARKED,0);

		CString msg;	
		msg.Format(IDS_FOUNDCAPTION,found);
		SetWindowText(msg);
	}
}


void CFindDlg::OnSetNewSearch(void)
{
	m_bNewSearch = TRUE;
	m_bInResult = FALSE;
	m_bAddToFound = FALSE;
	UpdateData(FALSE);
}
void CFindDlg::OnSetAddToFound(void)
{
	m_bNewSearch = FALSE;
	m_bInResult = FALSE;
	m_bAddToFound = TRUE;
	UpdateData(FALSE);
}
void CFindDlg::OnSetInResult(void)
{
	m_bNewSearch = FALSE;
	m_bInResult = TRUE;
	m_bAddToFound = FALSE;
	UpdateData(FALSE);
}

void CFindDlg::OnCheckClicked(void)
{
	UpdateData();
	m_modifier.EnableWindow(m_bByModifTime);
	m_modiftimespan.EnableWindow(m_bByModifTime);
	
	m_creator.EnableWindow(m_bByCreatTime);
	m_creattimespan.EnableWindow(m_bByCreatTime);
}

bool CFindDlg::GetAddToResult()
{
	return m_bAddToFound == TRUE;
}
bool CFindDlg::GetNewSearch()
{
	return m_bNewSearch == TRUE;
}
bool CFindDlg::GetInResult()
{
	return m_bInResult == TRUE;
}

bool CFindDlg::ByCreationTime()
{
	return m_bByCreatTime == TRUE;
}
TIMESPAN CFindDlg::CreationTimeSpan(CDateTime &time1,CDateTime &time2)
{
	return m_creattimespan.GetSpan(time1,time2);
}
bool CFindDlg::ByLastModificationTime()
{
	return m_bByModifTime == TRUE;
}
TIMESPAN CFindDlg::LastModificationTimeSpan(CDateTime &time1,CDateTime &time2)
{
	return m_modiftimespan.GetSpan(time1,time2);
}

UserData* CFindDlg::SelectedCreator()
{
	return m_creator.GetSelItemData();
}
UserData* CFindDlg::SelectedLastModifier()
{
	return m_modifier.GetSelItemData();
}

bool CFindDlg::ByExtras()
{
	return m_extras.GetLength() > 0;
}
CString CFindDlg::GetExtras()
{
	return m_extras;
}

void CFindDlg::ChangeMainGrid(UINT gridId)
{
	UpdateGrid(gridId);
	if(m_bChangeGridTab)
		ChangeGrid(gridId);        
}
