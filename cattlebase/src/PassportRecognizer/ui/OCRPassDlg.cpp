#include "stdafx.h"
#include "OCRPassDlg.h"

#include "../../exceptions/StrException.h"
#include "../../exceptions/DateTimeException.h"
#include "../../classes/types/DateTime.h"
#include "../../ctrls/EANEdit.h"
#include <boost\regex.hpp>
#include "../../missing/util.h"
#include "InputWeight.h"
#include "../../dlgs/NewHentDlg.h"
#include "../../Cow/NewCowDlg.h"
#include "../../Cow/EditCowDlg.h"
#include "../../missing/paths.h"
#include "../../log.h"
#include "MainCfg.h"
#include "../../classes/match/CowMatcher.h"
#include "../../classes/forallactions/CountAction.h"
#include "../../CattleSQL.h"
#include "../../classes/TempStringLoader.h"
#include "../../splash/SplashWnd.h"
#include "../../global_arrays.h"
#include "../engine/abbyy80/Abbyy80RecognitionEngine.h"
#include "../engine/RecognitionException.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PASSPORT_RESOLUTION_DPI_FOR_STORING 100

//#define LOAD_FROM_FILE

#if (!defined(_DEBUG) && defined(LOAD_FROM_FILE))
#error LOAD_FROM_FILE is defined in release version
#endif


COCRPassDlg::COCRPassDlg(CCowArray &outArray,
						 CCowArray &sesArray,
						 CCowDataPassImgMap &passMap,
						 PassportRecognizerSettings* pSettings,
						 CWnd* pParent /*=NULL*/) : 
	CDialog(COCRPassDlg::IDD, pParent), 
	m_outArray(outArray),
	m_foundHent(NULL),
	m_foundStock(NULL),
	m_scanList(&outArray,CFGID_SCANCOWSLIST), 
	m_TwainPP(AfxGetAppName(), &m_TraceFile),
	m_bNoSource(FALSE),
	m_passMap(passMap),
	m_sesArray(sesArray),	
	m_pPassRecognizer(NULL),
	m_pSettings(pSettings),
	m_pRecognitionEngine(NULL)
{

	DataSettings& dataSettings = m_pSettings->GetDataSettings();

	m_passDate.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE));
	m_passNo.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO));
	m_farmNo.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_cowNo.SetEnabled(true);
	m_birthDate.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::BIRTH));
	m_birthPlace.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::BIRTH));	
	m_stockCombo.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::STOCK));
	m_motherNo.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::MOTHER_EAN));
	m_cowSex.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::SEX));
	m_hentName.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_hentStreet.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_hentPobox.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_hentCity.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	m_hentZip.SetEnabled(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER));
	

	m_inputCtrls.Add(&m_passDate);
	m_inputCtrls.Add(&m_passNo);
	m_inputCtrls.Add(&m_farmNo);
	m_inputCtrls.Add(&m_cowNo),
	m_inputCtrls.Add(&m_birthDate);
	m_inputCtrls.Add(&m_birthPlace);	
	m_inputCtrls.Add(&m_stockCombo);
	m_inputCtrls.Add(&m_motherNo);
	m_inputCtrls.Add(&m_cowSex);
	m_inputCtrls.Add(&m_hentName);
	m_inputCtrls.Add(&m_hentStreet);
	m_inputCtrls.Add(&m_hentPobox);
	m_inputCtrls.Add(&m_hentCity);
	m_inputCtrls.Add(&m_hentZip);
	
}

COCRPassDlg::~COCRPassDlg()
{
	if(m_pPassRecognizer)
		delete m_pPassRecognizer;
	if(m_pRecognitionEngine)
	{
		m_pRecognitionEngine->Destroy();
		delete m_pRecognitionEngine;
	}

}

void COCRPassDlg::EnableControls(BOOL bEnable)
{
	
	for(int i = 0;i < m_inputCtrls.GetCount();i++)
		m_inputCtrls[i]->GetFieldWindow()->EnableWindow(bEnable);

	GetDlgItem(IDC_ADDHENT)->EnableWindow(bEnable);
	GetDlgItem(IDC_STOCKADD)->EnableWindow(bEnable);
}

void COCRPassDlg::EnableHentSection(BOOL bEnable)
{
	for(int i = 0;i < m_hentSection.GetCount();i++)
		m_hentSection[i]->EnableWindow(bEnable);
}
void COCRPassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_PASSDATE,m_passDate.GetCtrl());
	DDX_Control(pDX,IDC_PASSNO,m_passNo.GetCtrl());
	DDX_Control(pDX,IDC_EAN,m_cowNo.GetCtrl());
	DDX_Control(pDX,IDC_FARMNO,m_farmNo.GetCtrl());
	DDX_Control(pDX,IDC_BIRTHDATE,m_birthDate.GetCtrl());
	DDX_Control(pDX,IDC_BIRTHPLACE,m_birthPlace.GetCtrl());
	DDX_Control(pDX,IDC_STOCK,m_stockCombo.GetCtrl());
	DDX_Control(pDX,IDC_MOTHERNO,m_motherNo.GetCtrl());
	DDX_Control(pDX,IDC_SEX, m_cowSex.GetCtrl());
	DDX_Control(pDX,IDC_HENTNAME,m_hentName.GetCtrl());
	DDX_Control(pDX,IDC_HENTSTREET,m_hentStreet.GetCtrl());
	DDX_Control(pDX,IDC_HENTPOBOX,m_hentPobox.GetCtrl());
	DDX_Control(pDX,IDC_HENTCITY,m_hentCity.GetCtrl());
	DDX_Control(pDX,IDC_HENTZIP,m_hentZip.GetCtrl());
	DDX_Control(pDX,IDC_SHOWTAB,m_showTab);
	DDX_Control(pDX,IDC_DOSCAN,m_DoScanBtn);
	DDX_Control(pDX,IDC_SCANNINGFINISHED,m_ScanEndBtn);

	DDX_Control(pDX,IDC_ADDCOW,m_AddCowBtn);
	DDX_Control(pDX,IDC_PASSPROFILESLIST,m_passProfiles);
	DDX_Control(pDX,IDC_OCRAGAIN,m_ORCAgain);

}

BEGIN_MESSAGE_MAP(COCRPassDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DOSCAN, OnDoScan)
	ON_BN_CLICKED(IDC_ADDCOW, OnAddCow)
	ON_BN_CLICKED(IDC_STOCKADD,OnAddStock)
	ON_NOTIFY(TCN_SELCHANGE,IDC_SHOWTAB, OnSelChange)
	ON_NOTIFY(TCN_SELCHANGING,IDC_SHOWTAB, OnSelChanging)
	ON_CBN_SELCHANGE(IDC_STOCK,OnStockCbnChange)
	ON_WM_CLOSE()
	ON_WM_DESTROY()

	//ON_NOTIFY(IDC_SHOWTAB,NM_DBLCLK, OnDblClick)
	
	
	//ON_BN_CLICKED(IDC_OPTIONS, OnBnClickedOptions)
	ON_BN_CLICKED(IDC_ADDHENT, OnBnClickedAddHent)
	ON_BN_CLICKED(IDC_SCANNINGFINISHED, OnBnClickedScanningFinished)
	ON_NOTIFY(NM_DBLCLK,IDC_COWLIST,OnDblClick)
	//ON_NOTIFY(NM_RCLICK, IDC_COWLIST, OnRClick)
	ON_BN_CLICKED(IDC_OCRAGAIN,OnOCRAgain)
END_MESSAGE_MAP()


BOOL COCRPassDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	EnableControls(FALSE);

	m_hentSection.Add(m_hentName.GetFieldWindow());
	m_hentSection.Add(m_hentStreet.GetFieldWindow());
	m_hentSection.Add(m_hentPobox.GetFieldWindow());
	m_hentSection.Add(m_hentCity.GetFieldWindow());
	m_hentSection.Add(m_hentZip.GetFieldWindow());
	m_hentSection.Add(GetDlgItem(IDC_ADDHENT));
	m_hentSection.Add(m_farmNo.GetFieldWindow());

	m_DoScanBtn.SetButtonImage(IDB_DOSCAN);
	m_AddCowBtn.SetButtonImage(IDB_SCANADDCOW);

	m_ScanEndBtn.SetButtonImage(IDB_DROPBOX);
	m_ORCAgain.SetButtonImage(IDB_REPEAT);


	TCITEM item;
	CString tabText;
	item.mask = TCIF_TEXT|TCIF_PARAM;
	CRect tabItemRect;
	m_showTab.GetClientRect(&tabItemRect);

	tabItemRect.top += 28;
	tabItemRect.bottom -= 4;
	tabItemRect.left += 4;
	tabItemRect.right -= 8;

	m_scanList.Create(LVS_REPORT | LVS_OWNERDATA | WS_BORDER | WS_TABSTOP,tabItemRect,&m_showTab,IDC_COWLIST);

	tabText.LoadString(IDS_OCRTABNAMELIST);
	item.lParam = (LPARAM) &m_scanList;
	item.pszText = tabText.GetBuffer();
	m_showTab.InsertItem(0, &item);
	tabText.ReleaseBuffer();

	m_scanList.SetWindowPos(NULL, 0,0, 0, 0,SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);
	
	m_scanList.ShowWindow(SW_HIDE);

	m_iSelectedTab = 1;

	m_passShow.Create(tabItemRect,&m_showTab);

	tabText.LoadString(IDS_OCRTABNAMEIMG);
	item.lParam = (LPARAM) &m_passShow;
	item.pszText = tabText.GetBuffer();
	m_showTab.InsertItem(1, &item);
	tabText.ReleaseBuffer();

	m_passShow.SetWindowPos(NULL, 0, 0 , 0, 0,SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);
	
	m_passShow.ShowWindow(SW_SHOW);
	m_showTab.SetCurSel(m_iSelectedTab);
	m_scanList.UpdateItemCount();

	m_bNoSource = !(m_TwainPP.Create(m_hWnd, this) && m_TwainPP.OpenSource());
	
	if(m_passProfilesXml.LoadFromXmlFile(paths_ConcatPath(CUtil::GetAppDir(),"passprofiles.xml")))
	{
		PassProfilePassProfileArray& profileArray = m_passProfilesXml.GetPassProfileArray();
		for(int iProfile = 0;iProfile < profileArray.GetSize();iProfile++)
		{
			PassProfilePassProfile& passProfile  = profileArray[iProfile];
			int iProfileItem = m_passProfiles.AddString(passProfile.GetPassProfileName());
			m_passProfiles.SetItemData(iProfileItem,(DWORD)&passProfile);
		}
		m_passProfiles.SetCurSel(0);
	}

	m_pRecognitionEngine = new Abbyy80RecognitionEngine();

	try
	{
		m_pRecognitionEngine->Create(m_pSettings->GetEngineSettings());
		m_pPassRecognizer = new PassportRecognizer(m_pRecognitionEngine);
	}
	catch(RecognitionException* pException)
	{
		CString sMessage;
		sMessage.Format(IDS_OCRERRFAILDTOINITENGINE,(LPCSTR)pException->GetError());
		AfxMessageBox(sMessage);
		pException->Delete();
		EndDialog(IDOK);
	}
	
	EnableRepeatButton(false);
	EnableAddCowButton(false);
			
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COCRPassDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting

	CPen redPen(PS_SOLID,2,RGB(255,0,0));
	CPen blackPen(PS_SOLID,2,RGB(0,0,0));

		
		CRect wndRect;

		for(int i = 0;i<m_inputCtrls.GetCount();i++)
		{
			CInputField *pInputField = (CInputField*)m_inputCtrls[i];
			pInputField->GetFieldWindow()->GetWindowRect(&wndRect);
			ScreenToClient(&wndRect);
			wndRect.top -= 2;
			wndRect.left -= 2;
			wndRect.bottom += 2;
			wndRect.right += 2;	

			if(pInputField->IsInvalid())
			{
				CPen* oldPen = dc.SelectObject(&redPen);			
				dc.Rectangle(wndRect);
				dc.SelectObject(oldPen);
			}
			else if(!pInputField->IsFieldEnabledForInput())
			{
				CPen* oldPen = dc.SelectObject(&blackPen);			
				dc.Rectangle(wndRect);
				dc.SelectObject(oldPen);				
			}
		}

		
	CDialog::OnPaint();		
}

CString COCRPassDlg::RepairDate(const CString& date)
{
	CString _date;	
	_date = RemoveWhitespace(date);
	int len = _date.GetLength(),xchar;
	//min length of date YYYYMMDD
	if(len < 8) return date;

	for(int i = 0;i < len;i++)
	{
		xchar = _date.GetAt(i);
		if(isalpha(xchar) || (!isdigit(xchar) && xchar != '-'))  return date;
	}

	boost::regex e("(\\d{4})[-]?(\\d{2})[-]?(\\d{2})");
	char mergeFmt[] = "\\1-\\2-\\3"; 
	std::string str = (LPCSTR)_date;

	std::string output = boost::regex_replace(str,e,mergeFmt,boost::match_default | boost::format_sed);

	if(output.size() == 0) return date;

	return output.c_str();
}

int COCRPassDlg::ParseLines(CStringList &output,const CString &input)
{
	int lfPos = 0,lfPos2 = 0,cnt = 0;
	CString _input(input);
	_input.Remove('\r');

	while(TRUE)
	{
		lfPos2 = _input.Find('\n',lfPos);
		if(lfPos2 < 0)
		{
			output.AddTail(_input.Mid(lfPos,_input.GetLength()-lfPos));
			cnt++;
			break;
		}
		else
		{
			output.AddTail(_input.Mid(lfPos,lfPos2-lfPos));
			cnt++;
			lfPos = lfPos2+1; //+1 to skip \n
		}
	}
	return cnt;

}


BOOL COCRPassDlg::ParseHent(const CStringList &hentAddrList)
{
	if(hentAddrList.GetCount() < 3) return FALSE;
	POSITION pos = hentAddrList.GetHeadPosition();
	int iIndex = 0,c,l;
	CString addrLine,inputData1,inputData2;
	bool makeUpper = m_pSettings->GetDataSettings().GetDoUppercaseHent();
	
	while(pos)
	{
		addrLine = hentAddrList.GetNext(pos);
		switch(iIndex)
		{
		case 0:
			
			inputData1 = RepairName(addrLine);
			LOG("hentName = %s",(LPCSTR)inputData1);
			if(makeUpper)
				inputData1.MakeUpper();

			m_hentName->SetText(inputData1);

			break;
		case 1:
			l = addrLine.GetLength();
			if(l < 3) 
			{
				m_hentStreet.SetInvalid(true);
				m_hentPobox.SetInvalid(true);
				break;
			}

			c = l-1;
			while(c > 2 && !(isalpha(addrLine.GetAt(c-2)) && isalpha(addrLine.GetAt(c-1)) && addrLine.GetAt(c) == ' ')) c--;
				
			//c = addrLine.ReverseFind(' ');
			ASSERT(c >= 0);
			inputData1 = RepairName(addrLine.Left(c));  
			inputData2 = RemoveWhitespace(addrLine.Mid(c+1,l-c-1));
			if(makeUpper)
			{
				inputData1.MakeUpper();
				inputData2.MakeUpper();
			}

			LOG("hentStreet = %s,hentPobox = %s",(LPCSTR)inputData1,(LPCSTR)inputData2);
			m_hentStreet->SetText(inputData1);
			m_hentPobox->SetText(inputData2);
			break;
		case 2:

			l = addrLine.GetLength();
			if(l < 8) 
			{
				m_hentZip.SetInvalid(true);
				m_hentCity.SetInvalid(true);
				break;
			}
 
			c = 0;
			while(c < (l-2) && !(addrLine.GetAt(c) == ' ' && isalpha(addrLine.GetAt(c+1)))) c++;
			
			inputData1 = RemoveWhitespace(addrLine.Left(c));
			inputData2 = RepairName(addrLine.Mid(c+1,l-c-1));
			if(makeUpper)
			{
				inputData1.MakeUpper();
				inputData2.MakeUpper();
			}

			LOG("hentZip = %s,hentCity = %s",(LPCSTR)inputData1,(LPCSTR)inputData2);
			
	
			m_hentZip->SetText(inputData1);
			m_hentCity->SetText(inputData2);
			break;
		}
		iIndex++;
	}

	return TRUE;
}


void COCRPassDlg::OnDoScan()
{

	AMLOGINFO("OnDoScan()");
#ifdef LOAD_FROM_FILE

	ClearFields();
	EnableControls(FALSE);
	GetDlgItem(IDC_STOCKADD)->EnableWindow(FALSE);
	OnTwainBitmap(NULL);
	return;
#endif

	if(m_bNoSource)
	{
		AfxMessageBox(IDS_OCRERRNOSOURCE,MB_OK,0);
		AMLOGINFO("Source not choosen.");
		return;
	}

	if(m_passProfiles.GetCurSel() < 0)
	{
		AfxMessageBox(IDS_OCRERRNOPASSPROFILESELECTED);
		return;
	}

	ClearFields();
	EnableControls(FALSE);
	GetDlgItem(IDC_STOCKADD)->EnableWindow(FALSE);
	
	if(!m_TwainPP.HasPendingTransfers())
	{
		CTwainPP::SBitmapOptions Options;
		Options.eImageType = CTwainPP::itGray;
		Options.nResolution = m_pSettings->GetScannerSettings().GetResolution();
		Options.nBrightness = m_pSettings->GetScannerSettings().GetBrightness();
		Options.nContrast = m_pSettings->GetScannerSettings().GetContrast();

		Options.fSizeX = A4_SizeX/Inch;
		Options.fSizeY = (A4_SizeY/2)/Inch;

		m_TwainPP.GetBitmap(FALSE, Options,FALSE);
	}
	else
		m_TwainPP.DoPendingTransfer();


}


bool COCRPassDlg::ValidateDataFields()
{
	bool bValidationResult = true;
	BOOL bCheckEANs = AppCfg().GetBool(CFG_CHECKEANS); 
	DataSettings& dataSettings = m_pSettings->GetDataSettings();

	//PASSNO
	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO))
	{			
		CString sPassNo;
		m_passNo->GetWindowText(sPassNo);
		m_passNo.SetInvalid(!ValidatePassNo(sPassNo));
		bValidationResult = bValidationResult && !m_passNo.IsInvalid();		
	}

	//COWNO
	CString sCowEAN;
	m_cowNo->GetWindowText(sCowEAN);
	if(ValidateEAN(sCowEAN))
	{
		if(bCheckEANs)
		{
			if(!CUtil::InCowEANValid(sCowEAN))
			{
				if(AfxMessageBox(IDS_YNBADCTRLDIGIT,MB_YESNO,0) != IDYES)
				{
					m_cowNo.SetInvalid(true);
				}
			}
		}
		m_cowNo.SetInvalid(false);
	}
	else
	{
		m_cowNo.SetInvalid(true);		
	}

	bValidationResult = bValidationResult && !m_cowNo.IsInvalid();

	//PASSDATE	
	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE))
	{
		CDateTime passDate;
		CString sPassDate;
		m_passDate->GetWindowText(sPassDate);
		m_passDate.SetInvalid(!passDate.ParseDate(CDateTime::YYYYdashMMdashDD,(LPCSTR)sPassDate,false));		
		bValidationResult = bValidationResult && !m_passDate.IsInvalid();
	}

	//MOTHERNO
	if(dataSettings.IsFieldEnabled(DataSettings::MOTHER_EAN))
	{
		CString sMotherEAN;
		m_motherNo->GetWindowText(sMotherEAN);
		if(ValidateEAN(sMotherEAN))
		{
			if(bCheckEANs)
			{
				if(!CUtil::InCowEANValid(sMotherEAN))
				{
					if(AfxMessageBox(IDS_YNBADCTRLDIGIT,MB_YESNO,0) != IDYES)
					{
						m_motherNo.SetInvalid(true);
					}
				}
			}
			m_motherNo.SetInvalid(false);
		}
		else
		{
			m_motherNo.SetInvalid(true);
		}
		bValidationResult = bValidationResult && !m_motherNo.IsInvalid();
	}

	CString sFarmNo;
	m_farmNo->GetWindowText(sFarmNo);
	if(!m_foundHent && sFarmNo.GetLength() > 0 && ValidateEAN(sFarmNo))
	{
		m_foundHent = ary_hents.GetHentByHentNo(sFarmNo);
		if(m_foundHent)
		{
			SetHentFields(m_foundHent);

			m_farmNo.SetInvalid(false);
			m_hentName.SetInvalid(false);
			m_hentStreet.SetInvalid(false);
			m_hentPobox.SetInvalid(false);
			m_hentCity.SetInvalid(false);
			m_hentZip.SetInvalid(false);

			EnableHentSection(FALSE);
		}
	}

	

	//FSTOWNR
	if(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER) && !m_foundHent)
	{
		AfxMessageBox(IDS_REQUIREFSTOWNER);
		bValidationResult = false;	
	}
	
	
	if(dataSettings.IsFieldEnabled(DataSettings::BIRTH))
	{
		CDateTime birthDate;
		CString sBirthPlace,sBirthDate;

		//BIRTHDATE
		m_birthDate->GetWindowText(sBirthDate);
		m_birthDate.SetInvalid(!birthDate.ParseDate(CDateTime::YYYYdashMMdashDD,(LPCSTR)sBirthDate,false));
			
		//BIRTHPLACE
		m_birthPlace->GetWindowText(sBirthPlace);
		m_birthPlace.SetInvalid(sBirthPlace.IsEmpty());

		bValidationResult = bValidationResult && !m_birthDate.IsInvalid() && !m_birthPlace.IsInvalid();
	}

	
	Invalidate();
	return bValidationResult;
}

Cow* COCRPassDlg::CreateCowFromFields()
{
	Cow* pNewCow = new Cow(NULL_COWID);
	std::auto_ptr<Cow> autoDeleteCow(pNewCow);
	DataSettings& dataSettings = m_pSettings->GetDataSettings();
	
	if(!m_foundStock)
	{
		Stock *pStock = ary_stocks.GetStockDataByStockCode(m_cowStock);
		if(pStock)
		{
			m_foundStock = pStock;
			pNewCow->SetStock(m_foundStock);				
		}
		else
		{
			do 
			{				
				if(dataSettings.GetUseDefaultStock())
				{
					Stock * defaultStock =
						ary_stocks.GetStockDataByStockId(dataSettings.GetDefaultStockId());
					if(defaultStock)
					{
						pNewCow->SetStock(defaultStock);
						break;
					}						
				}								
				AfxMessageBox(IDS_ERROCRNOSTOCK); 
				GetDlgItem(IDC_STOCKADD)->SetFocus();
				m_stockCombo.SetInvalid(true);
				Invalidate();
				return NULL;

			} while(FALSE);
			
		}
	}
	else
	{
		pNewCow->SetStock(m_foundStock);	
	}

	//COWNO
	CString sCowEAN;
	m_cowNo->GetWindowText(sCowEAN);
	pNewCow->SetEAN(sCowEAN);

	//PASSNO
	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO))
	{			
		CString sPassNo;
		m_passNo->GetWindowText(sPassNo);
		pNewCow->SetPassNo(sPassNo);
	}

	//PASSDATE	
	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE))
	{
		CDateTime passDate;
		CString sPassDate;
		m_passDate->GetWindowText(sPassDate);
		passDate.ParseDate(CDateTime::YYYYdashMMdashDD,(LPCSTR)sPassDate,false);		
		pNewCow->SetPassDate(passDate);
	}

	//MOTHERNO
	if(dataSettings.IsFieldEnabled(DataSettings::MOTHER_EAN))
	{
		CString sMotherEAN;
		m_motherNo->GetWindowText(sMotherEAN);
		pNewCow->SetMotherEAN(sMotherEAN);
	}

	pNewCow->SetFirstOwner(m_foundHent);
	
	if(dataSettings.IsFieldEnabled(DataSettings::BIRTH))
	{
		CDateTime birthDate;
		CString sBirthPlace,sBirthDate;

		//BIRTHDATE
		m_birthDate->GetWindowText(sBirthDate);
		birthDate.ParseDate(CDateTime::YYYYdashMMdashDD,(LPCSTR)sBirthDate,false);
		pNewCow->SetBirthDate(birthDate);

		//BIRTHPLACE
		m_birthPlace->GetWindowText(sBirthPlace);
		pNewCow->SetBirthPlace(sBirthPlace);
	}
	autoDeleteCow.release();
	return pNewCow;

}

void COCRPassDlg::OnAddCow()
{
	LRESULT lRes;
	Cow *pCow = NULL;
	if(ValidateDataFields())
	{
		pCow = CreateCowFromFields();

		if(pCow)
		{
			if(!Look4Duplicates(pCow))
			{
				delete pCow;
				return;
			}			
			CInputWeight inputWeight(this);			
			do
			{
				NullDouble weight = inputWeight.AskForWeight();
			
				if(!weight.IsNull() && weight > 0)
				{
					pCow->SetWeight(Weight(weight.GetDouble()));
					break;
				}				
				AfxMessageBox(IDS_ERRREQWEIGHT,MB_OK,0);
				delete pCow;
				return;
			} 
			while(FALSE);
			

			m_outArray.Add(pCow);		
			SavePassImg(pCow);		
			m_showTab.SetCurSel(0);	
			OnSelChange(NULL,&lRes);
			OnSelChanging(NULL,&lRes);
			m_scanList.UpdateItemCount();
		}
	}
	
}

void COCRPassDlg::SavePassImg(Cow *pCow)
{
	CString passImgPath;
	CString tmpPassImgPath;
	
	CSplashWnd wait;
	wait.AddTextLine(CTempStringLoader(IDS_PLEASEWAIT));

	CString tmpPassStorageDir = CUtil::GetAppDir() + "\\PASSTMPSTRG";
	paths_CreateIfNeeded(tmpPassStorageDir);

	tmpPassImgPath = tmpPassStorageDir + "\\" + pCow->GetEAN() + ".JPG";

	int saveResolution = PASSPORT_RESOLUTION_DPI_FOR_STORING;	
	int curResolution = m_pSettings->GetScannerSettings().GetResolution();

	int newWidth = (m_passportImg.getWidth()*saveResolution)/curResolution;
	int newHeight = (m_passportImg.getHeight()*saveResolution)/curResolution;

	m_passportImg.rescale(newWidth,newHeight,FILTER_BOX);

	if(m_passportImg.save(tmpPassImgPath))
	{
		passImgPath = tmpPassImgPath;			
	}	
	
	m_passMap.SetAt(pCow,passImgPath);
}



bool COCRPassDlg::Look4Duplicates(Cow *cowdata)
{
	CCowDataMatcher cowMatchEAN;
	cowMatchEAN.FindEAN(cowdata->GetEAN());
	CCountAction<CCowArray::ArrayAction,CCowDataMatcher,Cow> duplicateEANs(&cowMatchEAN);
	
	
	m_outArray.ForAllAction(duplicateEANs);
	if(duplicateEANs.GetCount() > 0)
	{
		AfxMessageBox(IDS_DUPLEANONLIST);
		return false;
	}
	duplicateEANs.Reset();

	m_sesArray.ForAllAction(duplicateEANs);
	if(duplicateEANs.GetCount() > 0)
	{
		AfxMessageBox(IDS_SESIONLISTDUPL);
		return false;
	}
	duplicateEANs.Reset();
	
	ary_cows.ForAllAction(duplicateEANs);
	if(duplicateEANs.GetCount() > 0)
	{

		duplicateEANs.Reset();
		ary_incows.ForAllAction(duplicateEANs);
		if(duplicateEANs.GetCount() > 0)
		{
			AfxMessageBox(IDS_DUPLEANNOTSOLD);
			return false;
		}
		return AfxMessageBox(IDS_DUPLEANINDB,MB_YESNO,0) == IDYES;
	}
	return true;
}

BOOL COCRPassDlg::ValidateEAN(LPCSTR szEAN)
{
	boost::regex rexp("[A-Z]{2}[0-9]{12}");
	return boost::regex_match(string(szEAN),rexp);
}


void COCRPassDlg::OnAddStock()
{
	
	if(!m_foundStock)
	{
		Stock * newStock = new Stock();
		m_stockCombo->GetWindowText(m_cowStock);		
		newStock->SetStockCode(m_cowStock);
		if(!newStock->InsertThisStock())
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGSTOCK);
			m_stockCombo->SetFocus();
			delete newStock;
			m_stockCombo.SetInvalid(true);
			return;
		}
		else
		{
			m_foundStock = newStock;
			ary_stocks.Add(newStock);
			m_stockCombo.SetInvalid(false);
			m_stockCombo->UpdateItemCount();
			ReloadStockCombo(false);
			GetDlgItem(IDC_STOCKADD)->EnableWindow(FALSE);
			AfxMessageBox(IDS_STOCKADDED,MB_OK,0);
			Invalidate();
		}

	}

}


void COCRPassDlg::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
		int iNewTab = m_showTab.GetCurSel();

		TCITEM item;
		CWnd* pWnd;

		item.mask = TCIF_PARAM;
		
		//** hide the current tab ---------
		m_showTab.GetItem(m_iSelectedTab, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);
		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_HIDE);

		//** show the selected tab --------
		m_showTab.GetItem(iNewTab, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);
		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_SHOW);

	

	*pResult = 0;
}

void COCRPassDlg::OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_iSelectedTab = m_showTab.GetCurSel();

	*pResult = 0;
}


void COCRPassDlg::OnTwainImageInfo(int nSizeX, int nSizeY)
{
	LOG("OnTwainImageInfo(%d,%d)",nSizeX,nSizeY);
}

void COCRPassDlg::ProcessCowSex(const CString& sOCRCowSex)
{
	
	COWSEX cowSex = SEX_UNK;
	if(sOCRCowSex.Find("XX",0) > 0) 
		cowSex = SEX_XX;
	else if(sOCRCowSex.Find("XY",0) > 0) 
		cowSex =  SEX_XY;
	else
		m_cowSex.SetInvalid(true);

	m_cowSex->ReloadCombo(cowSex);
						
}

void COCRPassDlg::ProcessCowEAN(const CString& sOCRCowEANText,const CString& sOCRCowEANBC)
{
	BOOL bCheckEANs = AppCfg().GetBool(CFG_CHECKEANS); 
	CString cowNo = sOCRCowEANBC;

	if(cowNo.IsEmpty())
	{
		cowNo = sOCRCowEANText; //if empty use other number
	}

	LOG("cowNoBC = %s",(LPCSTR)cowNo);
					
	if(bCheckEANs)
		m_cowNo.SetInvalid(!CUtil::InCowEANValid(cowNo));
	else
		m_cowNo.SetInvalid(!ValidateEAN(cowNo));
												
	m_cowNo->SetText(cowNo);

	LOG("m_cowNoErr = %d",m_cowNo.IsInvalid());

}

void COCRPassDlg::ProcessMotherEAN(const CString& sOCRMotherEAN)
{
	BOOL bCheckEANs = AppCfg().GetBool(CFG_CHECKEANS); 
	CString motherNo = RepairEAN(sOCRMotherEAN);
	LOG("repaired motherNo = %s",(LPCSTR)motherNo);

	if(bCheckEANs)
		m_motherNo.SetInvalid(!CUtil::InCowEANValid(motherNo));
	else
		m_motherNo.SetInvalid(!ValidateEAN(motherNo));
		 
	m_motherNo->SetText(motherNo);
	LOG("m_motherNoErr = %d",m_motherNo.IsInvalid());
}

void COCRPassDlg::ProcessPassDate(const CString& sOCRPassDate)
{
	CString sPassDate = RepairDate(sOCRPassDate);
	CDateTime passDate;

	bool bDateParse = passDate.ParseDate(CDateTime::YYYYdashMMdashDD,sPassDate,false);
	bDateParse = bDateParse && !passDate.IsNull();


	m_passDate.SetInvalid(!bDateParse);
	m_passDate->SetText(sPassDate);
}

void COCRPassDlg::ProcessPassNo(const CString& sOCRPassNo)
{
	LOG("PASSNO = %s",(LPCSTR)sOCRPassNo);
	CString sPassNo = RepairPassNo(sOCRPassNo);
	LOG("REPAIRED PASSNO = %s",(LPCSTR)sPassNo);
	m_passNo.SetInvalid(!ValidatePassNo(sPassNo));
	m_passNo->SetText(sPassNo);
	LOG("m_passNoErr = %d",m_passNo.IsInvalid());
}

void COCRPassDlg::ProcessBirthDatePlace(const CString& sOCRBirthDatePlace)
{

#ifdef DO_LOGGING
	CString logBirthDatePlace(sOCRBirthDatePlace);
	logBirthDatePlace.Remove('\r');
	logBirthDatePlace.Replace('\n','|');
	LOG("BIRTHDATEPLACE = '%s'",(LPCSTR)logBirthDatePlace);
#endif

	CString birthDatePlace = sOCRBirthDatePlace;
	birthDatePlace.Remove('\r');

	int lfPos = birthDatePlace.Find('\n'),lfPos2 = -1;
	if(lfPos >= 0)
	{
		CString sBirthDate(birthDatePlace.Left(lfPos));
		lfPos += 1; //skip LF
		lfPos2 = birthDatePlace.Find('\n',lfPos);
		if(lfPos2 < 0)
			lfPos2 = birthDatePlace.GetLength();
		else
			lfPos2 = min(lfPos2,birthDatePlace.GetLength()-lfPos);

		CString sBirthPlace(birthDatePlace.Mid(lfPos,lfPos2));
		sBirthPlace = RemoveWhitespace(sBirthPlace);
	
		LOG("birthDate = '%s'",(LPCSTR)sBirthDate);
		sBirthDate = RepairDate(sBirthDate);

		LOG("repaired birthDate = '%s'",(LPCSTR)sBirthDate);
								
		CDateTime birthDate;
		m_birthDate.SetInvalid(!birthDate.ParseDate(CDateTime::YYYYdashMMdashDD,(LPCSTR)sBirthDate,false));
		m_birthDate->SetText(sBirthDate);
				
		LOG("m_birthDateErr = %d",m_birthDate.IsInvalid());

		LOG("birthPlace = '%s'",(LPCSTR)sBirthPlace);
		m_birthPlace->SetText(sBirthPlace);
	}
						
}


void COCRPassDlg::ProcessFirstOwner(const CString& sOCRFirstOwnerFarmNo,const CString& sOCRFirstOwnerNameAddress)
{
	Hent *pHent = NULL;
	CString sFarmNo = sOCRFirstOwnerFarmNo;
	LOG("FARMNO = '%s'",(LPCSTR)sFarmNo);
	if(!sFarmNo.IsEmpty())
	{		
		sFarmNo = RepairEAN(sFarmNo);
		m_farmNo.SetInvalid(!ValidateEAN(sFarmNo));

		m_farmNo->SetText(sFarmNo);
		
		LOG("Repaired FARMNO = '%s', m_farmNoErr = %d",(LPCSTR)sFarmNo,m_farmNo.IsInvalid());
		
		
		if(!m_farmNo.IsInvalid())
		{
			pHent = ary_hents.GetHentByHentNo(sFarmNo);
		}
	}
	else
	{
		m_farmNo.SetInvalid(true);	
		m_farmNo->EnableWindow(TRUE);							
	}

	if(pHent)
	{
		SetHentFields(pHent);
		EnableHentSection(FALSE);
		m_foundHent = pHent;
		LOG("FOUNDHENT");
	}
	else
	{
		CString sHentNameAddress(sOCRFirstOwnerNameAddress);
#ifdef DO_LOGGING
		CString logHentNameAddress(sHentNameAddress);
		logHentNameAddress.Remove('\r');
		logHentNameAddress.Replace('\n','|');
		LOG("sHentNameAddress = '%s'",(LPCSTR)sHentNameAddress);
#endif						

		CStringList addr;
		ParseLines(addr,sHentNameAddress);
#ifdef DO_LOGGING
		POSITION pos = addr.GetHeadPosition();
		int _i = 0;
		while(pos) 
		{
			LOG("hentAddr[] = %s",(LPCSTR)addr.GetNext(pos));
		}
#endif						
		if(!ParseHent(addr))
		{
			m_hentName.SetInvalid(true);
			m_hentStreet.SetInvalid(true);
			m_hentPobox.SetInvalid(true);
			m_hentCity.SetInvalid(true);
			m_hentZip.SetInvalid(true);
			ClearFirstOwnerFields(false);
		}
	}
	



}
void COCRPassDlg::ProcessCowStock(const CString& sOCRCowStock)
{
	CString sCowStockCode = RemoveWhitespace(sOCRCowStock);
	
	Stock *pStock = ary_stocks.GetStockDataByStockCode(sCowStockCode);
	if(pStock)
	{
		GetDlgItem(IDC_STOCKADD)->EnableWindow(FALSE);
		m_cowStock = pStock->GetStockCode();
		m_foundStock = pStock;
	}
	else
	{
		m_cowStock = sCowStockCode;
		m_stockCombo.SetInvalid(true);
	}
	
	ReloadStockCombo(m_stockCombo.IsInvalid());
}


void COCRPassDlg::ProcessRecognitionResult(const PassportRecognitionResult& recognitionResult)
{
	DataSettings& dataSettings = m_pSettings->GetDataSettings();

	ProcessCowEAN(recognitionResult.GetCowNo(),recognitionResult.GetCowNoBC());					

	if(dataSettings.IsFieldEnabled(DataSettings::FIRST_OWNER))
	{
		ProcessFirstOwner(recognitionResult.GetFarmNoBC(),recognitionResult.GetFstOwner());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::MOTHER_EAN))
	{
		ProcessMotherEAN(recognitionResult.GetMotherNo());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::SEX))
	{
		ProcessCowSex(recognitionResult.GetCowSex());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_DATE))
	{
		ProcessPassDate(recognitionResult.GetPassDate());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::PASSPORT_NO))
	{					
		ProcessPassNo(recognitionResult.GetPassNo());
	}

	if(dataSettings.IsFieldEnabled(DataSettings::STOCK))
	{
		ProcessCowStock(recognitionResult.GetCowStock());
	}
	else
		ReloadStockCombo(false);


	if(dataSettings.IsFieldEnabled(DataSettings::BIRTH))
	{
		ProcessBirthDatePlace(recognitionResult.GetBirthDatePlace());
	}
}

void COCRPassDlg::SetPassportImageFromRecognitionContext(boost::shared_ptr<const PassportRecognitionContext> ptrRecognitionContext,const PassProfilePassProfile* pPassProfile)
{
	if(pPassProfile)
	{
		CSize size(pPassProfile->GetPassDimX(),pPassProfile->GetPassDimY());
		CPoint org = ptrRecognitionContext->GetOrg();
		UINT  dpiResolution =  ptrRecognitionContext->GetResolution();

		int right = org.x + (size.cx*dpiResolution)/254;
		int	bottom = org.y + (size.cy*dpiResolution)/254;
	
		ptrRecognitionContext->GetPreparedPassportImage().copySubImage(m_passportImg,org.x,org.y,right,bottom);
		
		m_passShow.UpdateOCRImage(m_passportImg,*pPassProfile);
	}
	else
	{
		m_passportImg = ptrRecognitionContext->GetRawPassportImage();
		m_passShow.UpdateImage(m_passportImg);
	}
}

BOOL COCRPassDlg::OnTwainBitmap(HDIB hDIB)
{
	LOG("OnTwainBitmap()");

#ifdef LOAD_FROM_FILE
	CFileDialog l_openimg(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,
		  "Windows Bitmap (*.bmp)|*.bmp|JPEG Compressed Image (*.jpg)|*.jpg|TrueVision Targa (*.tga)|*.tga|TIFF (*.tif)|*.tga||");
	if(l_openimg.DoModal() != IDOK)
	{
		return TRUE;
	}
#endif

	m_foundStock = NULL;
	m_foundHent = NULL;
	ClearInvalid();
	ClearFields();
	PassProfilePassProfile* pProfile = (PassProfilePassProfile*)m_passProfiles.GetItemData(m_passProfiles.GetCurSel());
	HCURSOR hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	try
	{
#ifndef LOAD_FROM_FILE
		PassportRecognitionResult result = m_pPassRecognizer->Recognize(hDIB,m_pSettings->GetScannerSettings().GetResolution(),*pProfile);
#else
		PassportRecognitionResult result = m_pPassRecognizer->Recognize(l_openimg.GetPathName(),m_pSettings->GetScannerSettings().GetResolution(),*pProfile);
#endif

		const RecognitionOperationResult& operationResult = result.GetOperationResult();
		m_ptrRecognitionContext = result.GetRecognitionContext();

		if(operationResult.GetStatus() == RecognitionStatus_Success)
		{
			EnableControls(TRUE);
			EnableHentSection(TRUE);
			EnableRepeatButton(true);
			EnableAddCowButton(true);

			ProcessRecognitionResult(result);		
			SetPassportImageFromRecognitionContext(m_ptrRecognitionContext,pProfile);	

			Invalidate();
			LRESULT lRes;
			m_showTab.SetCurSel(1);					
			OnSelChange(NULL,&lRes);
			OnSelChanging(NULL,&lRes);	
		}
		else
		{
			
			if(m_ptrRecognitionContext.get() == NULL)
			{
				AfxMessageBox(operationResult.GetOperationMessage());						
			}
			else
			{
				EnableRepeatButton(false);
				EnableAddCowButton(false);
		
				CString errorMsg =  operationResult.GetOperationMessage();
				errorMsg += "\n";
				errorMsg += CTempStringLoader(IDS_ASKMANUALINPUT);

				if(AfxMessageBox(errorMsg,MB_YESNO,0) == IDYES)
				{
					SetPassportImageFromRecognitionContext(m_ptrRecognitionContext,NULL);	
				
					Invalidate();
					LRESULT lRes;
					m_showTab.SetCurSel(1);						
					OnSelChange(NULL,&lRes);
					OnSelChanging(NULL,&lRes);

					if(DoManualInput())
					{
						m_showTab.SetCurSel(0);
						OnSelChange(NULL,&lRes);
						OnSelChanging(NULL,&lRes);
						m_scanList.UpdateItemCount();
					}
				}
			}
		}
		
		SetCursor(hCursor);
	}
	catch(RecognitionException* e)
	{
		e->Delete();
	}



	return TRUE;
}


void COCRPassDlg::OnTwainError(CTwainPP::ETWainError eTwainError)
{
	switch (eTwainError)
	{
	case CTwainPP::eNoTwain:
		AfxMessageBox(IDS_SCANNERNOTFOUND,MB_OK,0);
		break;
	case CTwainPP::eCancel:
		AfxMessageBox(IDS_SCANCANCELED,MB_OK,0);
		break;
	case CTwainPP::eGetCapability:
		AfxMessageBox(IDS_ERRORGETOPTION,MB_OK,0);
		break;
	case CTwainPP::eSetCapability:
		AfxMessageBox(IDS_ERRORSETOPTION,MB_OK,0);
		break;
	case CTwainPP::eDeviceNotReady:
		AfxMessageBox(IDS_ERRORDEVICENOTREADY,MB_OK,0);
		break;
	case CTwainPP::eErrorReading:
		AfxMessageBox(IDS_ERRORREADING,MB_OK,0);
		break;
	}
}


void COCRPassDlg::OnClose()
{
	POSITION pos = m_passMap.GetStartPosition();
	Cow *pCow = NULL;
    CString passImage;

	while(pos)
	{
		m_passMap.GetNextAssoc(pos,pCow,passImage);
		delete pCow;
	}
	m_passMap.RemoveAll();

	CDialog::OnClose();
}


void COCRPassDlg::OnDestroy()
{
	m_TwainPP.Close();
	
}

void COCRPassDlg::OnBnClickedAddHent()
{
	Hent *pHent = new Hent(NULL_HENTID);

	CString hentName;
	m_hentName->GetWindowText(hentName);
	pHent->SetName(hentName);

	CString hentStreet;
	m_hentStreet->GetWindowText(hentStreet);
	pHent->SetStreet(hentStreet);

	CString hentPOBox;
	m_hentPobox->GetWindowText(hentPOBox);
	pHent->SetPOBox(hentPOBox);

	CString hentCity;
	m_hentCity->GetWindowText(hentCity);
	pHent->SetCity(hentCity);

		
	CString hentZip;
	m_hentZip->GetWindowText(hentZip);
	pHent->SetZip(hentZip);

	CString hentFarmNo;
	m_farmNo->GetWindowText(hentFarmNo);
	pHent->SetFarmNo(hentFarmNo);

	CNewHentDlg newHent(pHent);

	if(newHent.DoModal() == IDOK)
	{
		m_foundHent = newHent.GetNewHent();
		EnableHentSection(FALSE);
	}
	delete pHent;
}

bool COCRPassDlg::ValidatePassNo(CString &passNo)
{
	boost::regex passMatch("[A-Z]{2}[0-9]{8}");
	return boost::regex_match(std::string((LPCSTR)passNo),passMatch);
				
}

CString COCRPassDlg::RepairName(const CString &inputName)
{
	CString outName(inputName);
	//outName.Replace('1','l'); //typical OCR error;
	return outName;
}

CString COCRPassDlg::RepairEAN(const CString &inputNumber)
{
	CString outNumber(inputNumber);
	outNumber.Remove(' '); 
	outNumber.Replace('S','5');
	outNumber.Replace('O','0');
	return outNumber;
	
}
CString COCRPassDlg::RemoveWhitespace(const CString &input)
{
	CString output(input);
	output.Remove(' '); 
	return output;
}


char COCRPassDlg::Digit2AlphaReplace(char inputChar)
{
	char Digit2AlphaReplaceArray[][2]  =
	{
		{'0','D'},
		{'l','I'},
		{'5','S'},
		{0  ,0  } 
	};

	char (*replaceRow)[2] = Digit2AlphaReplaceArray;
	
	while(replaceRow[0][0] && replaceRow[0][1])
	{
		if(inputChar == replaceRow[0][0]) return replaceRow[0][1];
		replaceRow++;
	}

	return inputChar;
}

CString COCRPassDlg::RepairPassNo(const CString &inputNumber)
{
	CString outPassNo;
	outPassNo = RemoveWhitespace(inputNumber);
	
	if(outPassNo.GetLength() == 12)
	{
		for(int i = 2;i < 12;i++)
		{
			if(!isdigit(outPassNo[i]))
				return outPassNo; //jezeli ktoras nie jest cyfra, znaczy blad, uzytkownik musi poprawic 
		}

		if(!isalpha(outPassNo[0]) || isalpha(outPassNo[1]))
		{
			outPassNo.SetAt(0,Digit2AlphaReplace(outPassNo[0]));
			outPassNo.SetAt(1,Digit2AlphaReplace(outPassNo[1]));							
		}

	}
	else
		outPassNo = outPassNo.Left(12);

	return outPassNo;
}


void COCRPassDlg::ReloadStockCombo(bool bAddStockStr)
{

	m_stockCombo->ReloadCombo(m_foundStock);
	if(bAddStockStr)
	{
		int item = m_stockCombo->AddString(m_cowStock);
		m_stockCombo->SetItemData(item,NULL);
		m_stockCombo->SetCurSel(item);
	}
}

void COCRPassDlg::OnStockCbnChange()
{
	m_foundStock = (Stock*)m_stockCombo->GetItemData(m_stockCombo->GetCurSel());
	GetDlgItem(IDC_STOCKADD)->EnableWindow(m_foundStock == NULL);
}

bool COCRPassDlg::DoManualInput()
{

	Cow *pCow = new Cow(NULL_COWID);
	CNewCowDlg newCowDlg(pCow);
	if(newCowDlg.DoModal() == IDOK)
	{
		if(!Look4Duplicates(pCow))
		{
			delete pCow;								
		}
		else
		{
			m_outArray.Add(pCow);
			m_scanList.UpdateItemCount();
			SavePassImg(pCow);			
		}						
		return true;
	}
	return false;
}
void COCRPassDlg::OnBnClickedScanningFinished()
{
	EndDialog(IDOK);
}

void COCRPassDlg::OnOK()
{
	OnAddCow();	
}

void COCRPassDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	int nSubItem = -1;
	if (pNMIA)
	{
		
		nItem = pNMIA->iItem;

		if(nItem >= 0)
		{
			if(EditCow(m_outArray[nItem]))
			m_scanList.Invalidate();
		}
	}

	*pResult = 0;
}


bool COCRPassDlg::EditCow(Cow *pCow)
{
	CEditCowDlg editCowDlg(pCow,this);
	editCowDlg.LockEANEdit();
	if(editCowDlg.DoModal() == IDOK)
	{
		pCow->CommitChanges();
		return true;
	}
	return false;
}

void COCRPassDlg::ClearFields()
{
	m_passDate->Clear();
	m_passNo->Clear();	
	m_cowNo->Clear();
	m_birthDate->Clear();
	m_birthPlace->Clear();	
	m_stockCombo->ResetContent();
	m_motherNo->Clear();
	m_cowSex->ResetContent();
	ClearFirstOwnerFields(true);
	
}

void COCRPassDlg::ClearFirstOwnerFields(bool bClearFarmNo)
{
	if(bClearFarmNo)
		m_farmNo->Clear();

	m_hentName->Clear();
	m_hentStreet->Clear();
	m_hentPobox->Clear();
	m_hentCity->Clear();
	m_hentZip->Clear();
}

void COCRPassDlg::SetHentFields(Hent* pHent)
{
	m_hentName->SetText(pHent->GetName());
	m_hentStreet->SetText(pHent->GetStreet());
	m_hentPobox->SetText(pHent->GetPOBox());
	m_hentZip->SetText(pHent->GetZip());
	m_hentCity->SetText(pHent->GetCity());
}

void COCRPassDlg::ClearInvalid()
{
	for(int i = 0;i<m_inputCtrls.GetCount();i++)
	{
		m_inputCtrls[i]->SetInvalid(false);
	}
}

void COCRPassDlg::OnOCRAgain()
{
	PassProfilePassProfile* pPassProfile = 
		(PassProfilePassProfile*)m_passProfiles.GetItemData(m_passProfiles.GetCurSel());
	
	m_foundStock = NULL;
	m_foundHent = NULL;
	ClearInvalid();
	ClearFields();

	HCURSOR hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));

	PassportRecognitionResult result = m_pPassRecognizer->RecognizeAgain(m_ptrRecognitionContext,*pPassProfile);
	RecognitionOperationResult operationResult = result.GetOperationResult();
	if(operationResult.GetStatus() == RecognitionStatus_Success)
	{
		ProcessRecognitionResult(result);		
		SetPassportImageFromRecognitionContext(m_ptrRecognitionContext,pPassProfile);	
		
		Invalidate();
		LRESULT lRes;
		m_showTab.SetCurSel(1);					
		OnSelChange(NULL,&lRes);
		OnSelChanging(NULL,&lRes);
	
	}
	else
	{
		AfxMessageBox(operationResult.GetOperationMessage());
	}

}

void COCRPassDlg::EnableRepeatButton(bool bEnable)
{
	m_ORCAgain.EnableWindow(bEnable);
}
void COCRPassDlg::EnableAddCowButton(bool bEnable)
{
	m_AddCowBtn.EnableWindow(bEnable);
}