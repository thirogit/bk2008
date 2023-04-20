#include "stdafx.h"
#include "Notification.h"

#include "../../consts.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
DisplacementDirection::DisplacementDirection(const Hent* pSrcHent,const Hent* pDestHent) : m_pDestHent(pDestHent),m_pSrcHent(pSrcHent)
{
}

DisplacementDirection::DisplacementDirection(const DisplacementDirection& src)
{
	CopyFrom(src);
}

DisplacementDirection& DisplacementDirection::operator=(const DisplacementDirection& src)
{
	CopyFrom(src);
	return *this;
}

void DisplacementDirection::CopyFrom(const DisplacementDirection& src)
{
	m_pDestHent = src.m_pDestHent;
	m_pSrcHent = src.m_pSrcHent;
}

const Hent* DisplacementDirection::GetDestHent() const
{
	return m_pDestHent;
}

const Hent* DisplacementDirection::GetSrcHent() const
{
	return m_pSrcHent;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
NotificationRecord::NotificationRecord() : m_pSrcHent(NULL),m_pDstHent(NULL),m_Events(CZgloszenieBase::EVENT_NONE)
{
}


void NotificationRecord::SetSrcHent(const Hent* pSrcHent)
{
	m_pSrcHent = pSrcHent;
}

void NotificationRecord::SeDstHent(const Hent* pDstHent)
{
	m_pDstHent = pDstHent;
}

void NotificationRecord::SetEvents(UINT uiEvents)
{
	m_Events = uiEvents;
}

void NotificationRecord::SetEventDate(const CDateTime& eventDt)
{
	m_EventDate = eventDt;
}

void NotificationRecord::SetCows(CCowArray& cows)
{
	m_Cows.RemoveAll();
	m_Cows.Append(cows);
}
	
const Hent* NotificationRecord::GetSrcHent() const
{
	return m_pSrcHent;
}

const Hent* NotificationRecord::GetDstHent() const
{
	return m_pDstHent;
}

const CDateTime& NotificationRecord::GetEventDate() const
{
	return m_EventDate;
}

UINT NotificationRecord::GetEvents() const
{
	return m_Events;
}

int NotificationRecord::GetCowCount() const
{
	return m_Cows.GetCount();
}

CowArrayPtr NotificationRecord::GetCows()
{
	return CowArrayPtr(&m_Cows);
}

		
/////////////////////////////////////////////////////////////////////////////////////////
CZgloszenieBase::CZgloszenieBase(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator) : m_pCurrentRecord(NULL)
{
	m_CustomEvents  = EVENT_NONE;
	m_bUseCustomEvents = FALSE;
	m_pConverter = pConverter;
	m_pNavigator = pNavigator;
}

int CZgloszenieBase::LoadInputData(CPtrArray &inputElements) 
{	
	m_records.FreeAndFlush();
		
	for(int i = 0,s = inputElements.GetCount();i < s;i++)
	{
		m_records.Add(m_pConverter->convert(inputElements.GetAt(i)));
	}
	return Load();
}

int CZgloszenieBase::Load() 
{	
	int size;
	m_nSheets = 0;

	int cowsPerSheet = GetNumberOfRecordsPerSheet();
	for(int i = 0;i < m_records.GetSize();i++)
	{
		size = m_records[i]->GetCowCount();
        m_nSheets += size/cowsPerSheet;
		if (size%cowsPerSheet)  m_nSheets += 1;
	}

	m_pCurrentRecord = NULL;
	m_currStartIdx = 0;
	m_currEndIdx = 0;
	m_currDocPage = 0;
	m_currCows.RemoveAll();


	int cfgId = GetCfgItemID();
//	m_bPutBuyerName = PrintCfg().GetBool(cfgId,CZgloszenieBaseDoc::CFG_PUTBUYERNAME);
	m_bUseCustomEvents = PrintCfg().GetBool(cfgId,CZgloszenieBase::CFG_USE_CUSTOM_EVENTS);
	m_CustomEvents = PrintCfg().GetNum(cfgId,CZgloszenieBase::CFG_CUSTOM_EVENTS);
	m_bDontFillDstHentNo = PrintCfg().GetBool(cfgId,CZgloszenieBase::CFG_DONT_FILL_DEST_HENTNO);
	return RELOAD_OK_CONTINUE;
}	

void CZgloszenieBase::PreparePage(int nPage)
{
	int cowsize,i = 0,page = 0,p,pagesCount;
	int cowsPerSheet = GetNumberOfRecordsPerSheet();
	NotificationRecord *pRecord = NULL;
	do
	{	
		pRecord = m_records[i];
		cowsize = pRecord->GetCowCount();
		p = cowsize/cowsPerSheet;
		if(cowsize%cowsPerSheet) p++;
		pagesCount = page; 
		page += p*GetTemplatePages();
		i++;
	}
	while(page < nPage);


	m_currDocPage=nPage-pagesCount;

	if(m_pCurrentRecord != pRecord)
	{		
		m_pCurrentRecord = pRecord;
		m_currCows.RemoveAll();
		m_currCows.AppendPtr(m_pCurrentRecord->GetCows());

		if(GetSortBy() != (COWDATA_MEMBERS)-1)
			m_currCows.SortBy(GetSortBy(),GetSortOrder());

		
	}
	ComputeIndexes();

}


void CZgloszenieBase::FillEANs(int iStart,int iEnd,int iEANLblStart)
{
	int eanRow = iEANLblStart;
	for(int i = m_currStartIdx;i <= m_currEndIdx;i++)
	{			
		FillCowEANRecord(m_currCows[i]->GetEAN(),eanRow,"EAN%d_%%d");		
		eanRow +=1;
	}
}


void CZgloszenieBase::FillOneEAN(const CString &strEAN,LPCSTR EANLblRowFmt)
{
	CString eanLbl;

	if(strEAN.GetLength() >= 14)
	{
		CString myEAN = strEAN;
		CString sEANCountry = strEAN.Left(2);
		if(sEANCountry == "AT" || sEANCountry == "SK")
		{
			myEAN = sEANCountry + strEAN.Mid(5);
		}

		for(int i = 0,s = myEAN.GetLength(); i < s;i++)
		{
			eanLbl.Format(EANLblRowFmt,i+1);
			PutChar(eanLbl,myEAN.GetAt(i));
		}
	}
}

void CZgloszenieBase::FillCowEANRecord(const CString &strEAN,int iRow,LPCSTR EANLblRowFmt)
{
	CString eanLblFmt;
	eanLblFmt.Format(EANLblRowFmt,iRow);
	FillOneEAN(strEAN,eanLblFmt);
}

void CZgloszenieBase::PutPrintDate(const CDateTime& printDt)
{
	CString dateStr = printDt.GetDateInFmt(CDateTime::DDMMYYYY);
	
	PutChar("PRINT_DATE_DAY1",dateStr.GetAt(0));
	PutChar("PRINT_DATE_DAY2",dateStr.GetAt(1));
	PutChar("PRINT_DATE_MONTH1",dateStr.GetAt(2));
	PutChar("PRINT_DATE_MONTH2",dateStr.GetAt(3));
	PutChar("PRINT_DATE_YEAR1",dateStr.GetAt(4));
	PutChar("PRINT_DATE_YEAR2",dateStr.GetAt(5));
	PutChar("PRINT_DATE_YEAR3",dateStr.GetAt(6));
	PutChar("PRINT_DATE_YEAR4",dateStr.GetAt(7));
}

void CZgloszenieBase::PutEventDate(const CDateTime& eventDt)
{
	CString dateStr = eventDt.GetDateInFmt(CDateTime::DDMMYYYY);
	
	PutChar("EVENT_DATE_DAY1",dateStr.GetAt(0));
	PutChar("EVENT_DATE_DAY2",dateStr.GetAt(1));
	PutChar("EVENT_DATE_MONTH1",dateStr.GetAt(2));
	PutChar("EVENT_DATE_MONTH2",dateStr.GetAt(3));
	PutChar("EVENT_DATE_YEAR1",dateStr.GetAt(4));
	PutChar("EVENT_DATE_YEAR2",dateStr.GetAt(5));
	PutChar("EVENT_DATE_YEAR3",dateStr.GetAt(6));
	PutChar("EVENT_DATE_YEAR4",dateStr.GetAt(7));

}

void CZgloszenieBase::PutEventType(UINT uiEvents)
{
	char cEventCheckMark = 'X';
		
	if(uiEvents & EVENT_BUY)
		PutChar("EVENT_BUY",cEventCheckMark);

	if(uiEvents & EVENT_SELL)
		PutChar("EVENT_SELL",cEventCheckMark);

	if(uiEvents & EVENT_BUY_AND_SELL)
		PutChar("EVENT_BUY_AND_SELL",cEventCheckMark);

	if(uiEvents & EVENT_EXPORT_TO_EU)
	{
		PutChar("EVENT_EXPORT_TO_EU",cEventCheckMark);		
	}
	if(uiEvents & EVENT_EXPORT_NOTTO_EU)
	{
		PutChar("EVENT_EXPORT_NOT_TO_EU",cEventCheckMark);		
	}
	if(uiEvents & EVENT_OTHER)
		PutChar("EVENT_OTHER",cEventCheckMark);

}

void CZgloszenieBase::PutEventCountry(Country* pEventCountry)
{
	CString code2a = pEventCountry->GetCountryCode2A();
	PutChar("COUNTRY2A_1",code2a[0]);
	PutChar("COUNTRY2A_2",code2a[1]);
}

void CZgloszenieBase::FillBottomOfPage(NotificationRecord *pRecord)
{
	const Hent *pDestHent = pRecord->GetDstHent();
	
	if(!m_bDontFillDstHentNo)
	{
		FillOneEAN(pDestHent->GetFarmNo(),"DEST_FARMNO_%d");
	}

		
	PutEventDate(pRecord->GetEventDate());
	PutPrintDate(CDateTime::now());

	UINT uiEvents = EVENT_NONE;
		
	if(m_bUseCustomEvents)
	{
		uiEvents = m_CustomEvents;
	}
	else
	{
		uiEvents = pRecord->GetEvents();
	}

	if(uiEvents & (EVENT_EXPORT_TO_EU | EVENT_EXPORT_NOTTO_EU))
	{
		PutEventCountry(pDestHent->GetCountry());
	}

	PutEventType(uiEvents);

	/*if(m_bPutBuyerName)
	{	
		CString hentStr;
		hentStr.Format("%s\n%s %s\n%s %s",
			(LPCSTR)pHent->GetName(),
			(LPCSTR)pHent->GetStreet(),
			(LPCSTR)pHent->GetPOBox(),
			(LPCSTR)pHent->GetZip(),
			(LPCSTR)pHent->GetCity());
		DrawText("BUYER_SIGN",hentStr);
	}*/

}

void CZgloszenieBase::FillHeaderOfPage()
{
	PutChar("TYPE_COWS",'X'); 
	FillOneEAN(m_pCurrentRecord->GetSrcHent()->GetFarmNo(),"SRC_FARMNO_%d");	
}

CZgloszenieBase::~CZgloszenieBase()
{
	m_records.FreeAndFlush();	
}

void CZgloszenieBase::PutChar(const CString& sLabel,char c)
{
	CString sText = c;
	DrawText(sLabel,sText);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CZgloszenieOptDlg, CCustomPrintDocOptDlg)


BEGIN_MESSAGE_MAP(CZgloszenieOptDlg, CCustomPrintDocOptDlg)
	
	ON_BN_CLICKED(IDC_CUSTOMEVENT_TYPE_GROUP,OnCheckUseCustomEvents)
END_MESSAGE_MAP()


CZgloszenieOptDlg::CZgloszenieOptDlg (CWnd *pParent/* = NULL*/) :  CCustomPrintDocOptDlg(IDD,pParent)
		
{
	
	m_bEventType_Buy = FALSE;
	m_bEventType_Sell = FALSE;
	m_bEventType_Buy_And_Sell = FALSE;
	m_bEventType_Export_To_EU = FALSE;
	m_bEventType_Export_NotTo_EU = FALSE;
	m_bEventType_Other = FALSE;
}

void CZgloszenieOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomPrintDocOptDlg::DoDataExchange(pDX);
	
	DDX_Check(pDX,IDC_EVENT_CHECK_BUY, m_bEventType_Buy);
	DDX_Check(pDX,IDC_EVENT_CHECK_SELL,  m_bEventType_Sell);
	DDX_Check(pDX,IDC_EVENT_CHECK_BUY_AND_SELL,  m_bEventType_Buy_And_Sell);
	DDX_Check(pDX,IDC_EVENT_CHECK_EXPORT_TO_EU,  m_bEventType_Export_To_EU);
	DDX_Check(pDX,IDC_EVENT_CHECK_EXPORT_NOT_TO_EU,  m_bEventType_Export_NotTo_EU);
	DDX_Check(pDX,IDC_EVENT_CHECK_OTHER,  m_bEventType_Other);

	DDX_Control(pDX,IDC_CUSTOMEVENT_TYPE_GROUP,m_CustomEventTypeGroup);
	DDX_Check(pDX,IDC_CUSTOMEVENT_TYPE_GROUP,m_bUseCustomEvents);
	DDX_Check(pDX,IDC_DONTFILLDESTFARMNO,m_bDontFillDestHentNo);
	
}   

BOOL CZgloszenieOptDlg::OnInitDialog()
{
	if(!CCustomPrintDocOptDlg::OnInitDialog()) return FALSE;
	m_CustomEventTypeGroup.SetTitleStyle(BS_AUTOCHECKBOX);

	m_bUseCustomEvents = PrintCfg().GetBool(m_nCfgID,CZgloszenieBase::CFG_USE_CUSTOM_EVENTS);
	UINT uiEvents = PrintCfg().GetNum(m_nCfgID,CZgloszenieBase::CFG_CUSTOM_EVENTS);
	m_bDontFillDestHentNo = PrintCfg().GetBool(m_nCfgID,CZgloszenieBase::CFG_DONT_FILL_DEST_HENTNO);


	m_bEventType_Buy = (uiEvents & CZgloszenieBase::EVENT_BUY) != 0;
	m_bEventType_Sell = (uiEvents & CZgloszenieBase::EVENT_SELL) != 0;
	m_bEventType_Buy_And_Sell = (uiEvents & CZgloszenieBase::EVENT_BUY_AND_SELL) != 0;
	m_bEventType_Export_To_EU = (uiEvents & CZgloszenieBase::EVENT_EXPORT_TO_EU) != 0;
	m_bEventType_Export_NotTo_EU = (uiEvents & CZgloszenieBase::EVENT_EXPORT_NOTTO_EU) != 0;
	m_bEventType_Other = (uiEvents & CZgloszenieBase::EVENT_OTHER) != 0;
	m_bDontFillDestHentNo = PrintCfg().GetNum(m_nCfgID,CZgloszenieBase::CFG_CUSTOM_EVENTS);
	UpdateData(FALSE);

	
	//m_CustomEventTypeGroup.SetCheck(bUseCustomEvents);
	EnableCustomEvents(m_bUseCustomEvents);
	
	return TRUE;
	
}

void CZgloszenieOptDlg::OnCheckUseCustomEvents()
{
	UpdateData();
	EnableCustomEvents(m_bUseCustomEvents);
}

void CZgloszenieOptDlg::EnableCustomEvents(BOOL bEnable)
{
	UINT customEventsCheckBoxIds[] = 
	{
		IDC_EVENT_CHECK_BUY, 
		IDC_EVENT_CHECK_SELL,
		IDC_EVENT_CHECK_BUY_AND_SELL,
		IDC_EVENT_CHECK_EXPORT_TO_EU,
		IDC_EVENT_CHECK_EXPORT_NOT_TO_EU,
		IDC_EVENT_CHECK_OTHER, 
		0
	};

	UINT* pCheckBoxId = customEventsCheckBoxIds;
	while(*pCheckBoxId)
	{
		GetDlgItem(*pCheckBoxId)->EnableWindow(bEnable);
		pCheckBoxId++;
	}
}

void CZgloszenieOptDlg::Save()
{
	UpdateData();

	UINT uiEvents = CZgloszenieBase::EVENT_NONE;

	uiEvents |= m_bEventType_Buy ? CZgloszenieBase::EVENT_BUY : CZgloszenieBase::EVENT_NONE;
	uiEvents |= m_bEventType_Sell ? CZgloszenieBase::EVENT_SELL : CZgloszenieBase::EVENT_NONE;
	uiEvents |= m_bEventType_Buy_And_Sell ? CZgloszenieBase::EVENT_BUY_AND_SELL : CZgloszenieBase::EVENT_NONE;
	uiEvents |= m_bEventType_Export_To_EU ? CZgloszenieBase::EVENT_EXPORT_TO_EU : CZgloszenieBase::EVENT_NONE;
	uiEvents |= m_bEventType_Export_NotTo_EU ? CZgloszenieBase::EVENT_EXPORT_NOTTO_EU : CZgloszenieBase::EVENT_NONE;
	uiEvents |= m_bEventType_Other ? CZgloszenieBase::EVENT_OTHER : CZgloszenieBase::EVENT_NONE;

	PrintCfg().SetBool(m_nCfgID,CZgloszenieBase::CFG_USE_CUSTOM_EVENTS,m_bUseCustomEvents);
	PrintCfg().SetNum(m_nCfgID,CZgloszenieBase::CFG_CUSTOM_EVENTS,uiEvents);
	PrintCfg().SetBool(m_nCfgID,CZgloszenieBase::CFG_DONT_FILL_DEST_HENTNO,m_bDontFillDestHentNo);
	
	
}
