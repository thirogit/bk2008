#include "stdafx.h"
#include "BaseCowDlg.h"
#include "../dlgs/NewStockDlg.h"
#include "../datalimits.h"
#include "../missing/util.h"
#include "MainCfg.h"
#include "../dlgs/Stocks.h"
#include "../classes/TempStringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PREDEFSEX_SETTING_NOTIFY_DELAY 1600

BEGIN_MESSAGE_MAP(CBaseCowDlg, CDialog)
			ON_CBN_SELCHANGE(IDC_COMBOSTOCK, OnStockSelChange)
			ON_BN_CLICKED(IDC_ADDSTOCK,OnAddStock)
			ON_CBN_KILLFOCUS(IDC_COMBOSTOCK,OnStockComboLostFocus)
END_MESSAGE_MAP()


CBaseCowDlg::CBaseCowDlg(Cow *pCow,CWnd *pParent/* = NULL*/) : 
	CDialog(CBaseCowDlg::IDD,pParent), m_passdate(false),m_birthdate(false),
		m_myprice(g_szRegExPrice),m_hentowner(true),
		m_pCow(pCow)		
{
	ASSERT(m_pCow);
}

void CBaseCowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control (pDX, IDC_EAN  , m_ean  ) ;
	DDX_Control (pDX, IDC_BIRTHDATE, m_birthdate ) ;
	DDX_Control (pDX, IDC_PASSNO, m_passno  ) ;
	DDX_Control (pDX, IDOK, m_CowAdd  ) ;
	DDX_Control (pDX, IDCANCEL, m_CowCancel) ;
	DDX_Control (pDX, IDC_COMBOSEX,m_combosex);
	DDX_Control (pDX,IDC_HENTOWNER,m_hentowner);
	DDX_Control (pDX,IDC_COMBOSTOCK,m_stock);
	DDX_Control (pDX,IDC_PASSDATE,m_passdate);
	DDX_Control (pDX,IDC_NEWCOWEXTRAS,m_extras);
	DDX_Control (pDX,IDC_MOTHEREAN, m_motherean);
	DDX_Control (pDX,IDC_WEIGHT, m_weight);
	DDX_Text (pDX,IDC_BIRTHPLACE, m_birthplace);
	DDX_Control(pDX,IDC_MYPRICE,m_myprice);
	DDX_Control(pDX,IDC_LONGITUDE,m_Longitude);
	DDX_Control(pDX,IDC_LATITUDE,m_Latitude);
	DDX_Text(pDX,IDC_HEALTHCERTNO,m_HealthCertNo);

}
BOOL  CBaseCowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	SetWindowText(GetWindowCaption());	

	m_passno.SetMask(g_PassMask ) ;
	SendDlgItemMessage(IDC_BIRTHPLACE,EM_LIMITTEXT,MAXBIRTHPLACE);
	SendDlgItemMessage(IDC_HEALTHCERTNO,EM_LIMITTEXT,MAXHEALTHCERTNO);
	m_ean.SetAbortWnd(&m_CowCancel);
	m_motherean.SetAbortWnd(&m_CowCancel);
	EnableToolTips(TRUE);
	m_CowCancel.SetButtonImage( IDB_CANCEL);
	m_CowAdd.SetButtonImage( IDB_ADDCOW );

	SetupControls();	

	m_UsedPreDefinedSexTip.Create(this);
	m_ean.SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_motherean.SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	return TRUE ;
}


bool CBaseCowDlg::Validate()
{
	UpdateData();
	
	if(!m_ean.IsFullData()) 
	{
		AfxMessageBox(IDS_ERREMPTYEAN,MB_OK,0);
		m_ean.SetFocus();
		return false;
	}
	else if(!m_ean.IsValidEAN())
	{
		if(AfxMessageBox(IDS_YNBADCTRLDIGIT,MB_YESNO,0) == IDNO)
		{
			m_ean.SetFocus();
			return false;
		}
	}

	NullDouble weight = m_weight.GetWeight();

	if(weight.IsNull())
	{
		AfxMessageBox(IDS_ERRREQWEIGHT,MB_OK,0);
		m_weight.SetFocus();
		return false;
	}
	
	

	if(!m_stock.GetSelItemData())
	{		
		AfxMessageBox(IDS_ERRSELSTOCK,MB_OK,0);
		m_stock.SetFocus();
		return false;
	}


	if(!m_passno.IsFullData()) 
	{		
		if(AfxMessageBox(IDS_ASKEMPTYPASSNO,MB_YESNO,0) != IDYES)
		{	
			m_passno.SetFocus();
			return false;
		}
	}
	
	if(m_motherean.IsFullData()) 
	{
		if(!m_motherean.IsValidEAN())
		{
			if(AfxMessageBox(IDS_YNBADCTRLDIGIT,MB_YESNO,0) == IDNO)
			{
				m_motherean.SetFocus();
				return false;
			}
		}
	}
	else
	{
		if(!m_motherean.IsInputEmpty())
		{
			AfxMessageBox(IDS_INVALIDMOTHEREAN,MB_OK,0);		
			m_motherean.SetFocus();
			return false;
		}
	}


	if(!m_birthdate.IsInputEmpty() && !m_birthdate.IsDateValid())
	{
		AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
		return false;
	}

	if(!m_passdate.IsInputEmpty() && !m_passdate.IsDateValid())
	{
		AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
		return false;
	}	

	return true;
}

Cow* CBaseCowDlg::CreateNewCow()
{
	Cow *pNewCow = new Cow(NULL_COWID);
	
	pNewCow->SetEAN(m_ean.GetEAN());
	
	
	pNewCow->SetWeight(Weight(m_weight.GetWeight().GetDouble()));			 
		

	if(!m_passno.IsInputEmpty())
	{
		CString passno;
		m_passno.GetWindowText(passno);
		pNewCow->SetPassNo(passno);
	}	
	pNewCow->SetStock(m_stock.GetSelItemData());		

	CDateTime birthdate;
	m_birthdate.GetDate(birthdate);
	pNewCow->SetBirthDate(birthdate);

	pNewCow->SetBirthPlace(m_birthplace);
	
	CString extras;
	m_extras.GetWindowText(extras);
	pNewCow->SetExtras(extras);
		
	pNewCow->SetFirstOwner(m_hentowner.GetSelectedHent());

	CDateTime passdate;
	m_passdate.GetDate(passdate );
	pNewCow->SetPassDate(passdate);

	if(!m_motherean.IsInputEmpty())
	{		
		pNewCow->SetMotherEAN(m_motherean.GetEAN());
	}

	pNewCow->SetSex(m_combosex.GetSelSex());

	CString mypriceStr;
	m_myprice.GetWindowText(mypriceStr);
	pNewCow->SetMyPrice(Money(CUtil::myatof((LPCSTR)mypriceStr)));


	pNewCow->SetLatitude(m_Latitude.GetLatitude());
	pNewCow->SetLongitude(m_Longitude.GetLongitude());
	pNewCow->SetHealthCertNo(m_HealthCertNo);

	return pNewCow;
}

void CBaseCowDlg::OnOK() 
{
	if(!Validate()) return;
	if(CommitCow())
		EndDialog(IDOK);
}

void CBaseCowDlg::OnStockSelChange()
{
	Stock *pStock = m_stock.GetSelItemData();
	if(pStock && !pStock->GetPredefSex().IsUNK())
	{
		m_combosex.SelSex(pStock->GetPredefSex().getCOWSEX());

		CWnd* sexCombo = GetDlgItem(IDC_COMBOSEX);
		CRect rect;
		sexCombo->GetClientRect(&rect);
		sexCombo->ClientToScreen(&rect);

		m_UsedPreDefinedSexTip.Show(CPoint(rect.left,rect.bottom),
									CTempStringLoader(IDS_USEDPREDEFSEX),
									PREDEFSEX_SETTING_NOTIFY_DELAY);

	}
}

void CBaseCowDlg::OnStockComboLostFocus()
{
	m_UsedPreDefinedSexTip.Hide();
}

void CBaseCowDlg::OnAddStock()
{
	Stock* pNewStock = CStocksDlg::AddNewStock(this);
	if(pNewStock != NULL)
	{
		m_stock.UpdateItemCount();
		m_stock.ReloadCombo(pNewStock);
	}
}


