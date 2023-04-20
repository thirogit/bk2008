#include "stdafx.h"
#include "NewCowDlg.h"

#include "../barcodescanner/BCScannerDrv.h"
#include "../dlgs/BarcodeEntryDlg.h"
#include "../classes/validators/CowEanValidator.h"
#include "../classes/validators/FarmNoValidator.h"
#include "../classes/TempStringLoader.h"

#include "MainCfg.h"

#include "../global_arrays.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_EVENT_ID 1
#define BARCODEENTRY_DLG_SHOW_DELAY_MS 500

BEGIN_MESSAGE_MAP(CNewCowDlg, CBaseCowDlg)
			ON_WM_TIMER()
END_MESSAGE_MAP()

CNewCowDlg::CNewCowDlg(Cow *pCow,Hent* pSelOwner/* = NULL*/,CWnd *pParent/* = NULL*/) : 
	CBaseCowDlg(pCow,pParent),m_pSelOwner(pSelOwner)
{
	ASSERT(m_pCow);
}

CString CNewCowDlg::GetWindowCaption()
{
	return CTempStringLoader(IDS_NEWCOWCAPTION);
}

void CNewCowDlg::SetupControls()
{
	m_CowAdd.SetButtonImage( IDB_ADDCOW );
	m_CowAdd.SetWindowText(CTempStringLoader(IDS_BTNADD));
}

BOOL  CNewCowDlg::OnInitDialog()
{
	CBaseCowDlg::OnInitDialog();	

	Hent *pHentSel = m_pSelOwner;

	COWSEX cowsex = (COWSEX)AppCfg().GetNum(CFG_PRESELSEX);
	Stock *pSelStock = ary_stocks.GetStockDataByStockId(AppCfg().GetNum(CFG_PRESELSTOCK));

	if(pSelStock)
	{
		if(!pSelStock->GetPredefSex().IsUNK()) 
			cowsex = pSelStock->GetPredefSex().getCOWSEX();
	}

	m_hentowner.ReloadCombo(pHentSel);
	m_combosex.ReloadCombo(cowsex);
	m_stock.ReloadCombo(pSelStock);	

	if(CBarcodeScannerDriver::IsScannerOnline())
	{
		EnableAllCtrls(false);
		SetTimer(TIMER_EVENT_ID,BARCODEENTRY_DLG_SHOW_DELAY_MS,NULL);
	}

	return TRUE ;
}

void CNewCowDlg::EnableAllCtrls(bool bEnable)
{
	UINT allCtrlsIds[] = 
	{
		IDC_EAN,
		IDC_COMBOSEX,
		IDC_WEIGHT,
		IDC_COMBOSTOCK,
		IDC_ADDSTOCK,
		IDC_PASSNO,
		IDC_PASSDATE,
		IDC_HENTOWNER,
		IDC_BIRTHPLACE,
		IDC_BIRTHDATE,
		IDC_MOTHEREAN,
		IDC_MYPRICE,
		IDC_NEWCOWEXTRAS,
		IDC_LONGITUDE,
		IDC_LATITUDE,
		IDC_HEALTHCERTNO,
		IDOK,
		IDCANCEL,
		0
	};

	UINT *pId = allCtrlsIds;
	while(*pId)
	{
		GetDlgItem(*pId)->EnableWindow(bEnable);
		pId++;
	}

}

bool CNewCowDlg::CommitCow()
{
	Cow *pTmpCow = CreateNewCow();	
	m_pCow->CopyFrom(*pTmpCow);
	delete pTmpCow;
	return true;
}

void CNewCowDlg::OnTimer(UINT nIDEvent)
{	
	KillTimer(TIMER_EVENT_ID);
	CString fstOwnerNo;
	CBarcodeEntryDlg bcEntryDlg(this);
	CCowEanValidator cowEanValidator;
	CFarmNoValidator farmNoValidator;
	bcEntryDlg.SetValidator(&cowEanValidator);
	bcEntryDlg.SetEntryText(CTempStringLoader(IDS_SCANCOWEAN));
	//bcEntryDlg.SetAniStyles(AS_CENTER, AS_CENTER);
	//bcEntryDlg.SetDispStyle(DS_WIRE);
	if(bcEntryDlg.DoModal() == IDOK)
	{
		m_ean.SetEAN(bcEntryDlg.GetBarcode());
		bcEntryDlg.SetEntryText(CTempStringLoader(IDS_SCANFARMNODARCODE));
		bcEntryDlg.SetValidator(&farmNoValidator);
		if(bcEntryDlg.DoModal() == IDOK)
		{
			fstOwnerNo = bcEntryDlg.GetBarcode();

			
			if(!m_hentowner.SelectHent(fstOwnerNo))
			{
				Hent newHent(NULL_HENTID);
				newHent.SetFarmNo(fstOwnerNo);
				CNewHentDlg newHentDlg(&newHent);
				if(newHentDlg.DoModal() == IDOK)
				{
					Hent* pNewlyAddedHent = newHentDlg.GetNewHent();
					if((m_hentowner.GetShowOnlyCompanies() && pNewlyAddedHent->GetHentType()->IsIndividual()) ||
						(m_hentowner.GetShowOnlyIndividuals() && pNewlyAddedHent->GetHentType()->IsCompany()))
					{
						m_hentowner.SetShowOnlyCompanies(pNewlyAddedHent->GetHentType()->IsCompany());
						m_hentowner.SetShowOnlyIndividuals(pNewlyAddedHent->GetHentType()->IsIndividual());
					}
					m_hentowner.ReloadCombo(pNewlyAddedHent);
					m_hentowner.NotifyReload();

				}
			}
		}
	}
	EnableAllCtrls(true);

}