#include "stdafx.h"
#include "RegistrationDoc.h"
#include "../../datatypes/CompanyInfo.h"
#include "../../consts.h"
#include "../../Utils/StringFormat.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
RegistrationRecord::RegistrationRecord() 
{
	m_Cows = new CCowArray();
}

void RegistrationRecord::SetCows(CowArrayPtr cows)
{
	m_Cows->RemoveAll();
	m_Cows->AppendPtr(cows);
}
	

int RegistrationRecord::GetCowCount()
{
	return m_Cows->GetSize();
}

CowArrayPtr RegistrationRecord::GetCows()
{
	return m_Cows;
}

		
/////////////////////////////////////////////////////////////////////////////////////////

RegistrationDoc::RegistrationDoc() : m_pCurrentRecord(NULL)
{
}

int RegistrationDoc::LoadInputData(CPtrArray &inputElements) 
{	
	m_records.FreeAndFlush();
		
	for(int i = 0,s = inputElements.GetCount();i < s;i++)
	{
		InDoc *pDoc = (InDoc*)inputElements.GetAt(i);
		RegistrationRecord* record = new RegistrationRecord();
		record->SetCows(pDoc->GetCows());
		m_records.Add(record);
	}
	return Load();
}

int RegistrationDoc::Load() 
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
//	m_bPutBuyerName = PrintCfg().GetBool(cfgId,RegistrationDocDoc::CFG_PUTBUYERNAME);
//	m_bUseCustomEvents = PrintCfg().GetBool(cfgId,RegistrationDoc::CFG_USE_CUSTOM_EVENTS);
//	m_CustomEvents = PrintCfg().GetNum(cfgId,RegistrationDoc::CFG_CUSTOM_EVENTS);
	
	return RELOAD_OK_CONTINUE;
}	

void RegistrationDoc::PreparePage(int nPage)
{
	int cowsize,i = 0,page = 0,p,pagesCount;
	int cowsPerSheet = GetNumberOfRecordsPerSheet();
	RegistrationRecord *pRecord = NULL;
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

void RegistrationDoc::FillEANs(int iStart,int iEnd,int iEANLblStart)
{
	int eanRow = iEANLblStart;
	for(int i = m_currStartIdx;i <= m_currEndIdx;i++)
	{			
		FillCowRecord(m_currCows[i],eanRow);		
		eanRow +=1;
	}
}


void RegistrationDoc::FillOneEAN(const CString &strEAN,LPCSTR EANLblRowFmt)
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

void RegistrationDoc::PutDate(const CDateTime& dt,LPCSTR prefix,int row)
{
	CString dateStr = dt.GetDateInFmt(CDateTime::DDMMYYYY);
	
	PutChar(CStringFormat("%s_DAY1_%d",prefix,row),dateStr.GetAt(0));
	PutChar(CStringFormat("%s_DAY2_%d",prefix,row),dateStr.GetAt(1));
	PutChar(CStringFormat("%s_MONTH1_%d",prefix,row),dateStr.GetAt(2));
	PutChar(CStringFormat("%s_MONTH2_%d",prefix,row),dateStr.GetAt(3));
	PutChar(CStringFormat("%s_YEAR1_%d",prefix,row),dateStr.GetAt(4));
	PutChar(CStringFormat("%s_YEAR2_%d",prefix,row),dateStr.GetAt(5));
	PutChar(CStringFormat("%s_YEAR3_%d",prefix,row),dateStr.GetAt(6));
	PutChar(CStringFormat("%s_YEAR4_%d",prefix,row),dateStr.GetAt(7));
}

void RegistrationDoc::FillCowRecord(Cow* pCow,int iRow)
{
	
	FillOneEAN(pCow->GetEAN(),CStringFormat("EAN%d_%%d",iRow));

	CString motherEan = pCow->GetMotherEAN();
	if(!motherEan.IsEmpty())
	{
		FillOneEAN(motherEan,CStringFormat("MOTHEREAN%d_%%d",iRow));
	}

	CDateTime birthDt = pCow->GetBirthDate();
	if(!birthDt.IsNull())
	{
		PutDate(birthDt,"BIRTHDATE",iRow);
	}

	CDateTime inDt = pCow->GetInDoc()->GetLoadDate();
	PutDate(birthDt,"INDATE",iRow);
		
	CowSex sex = pCow->GetSex();
	if(sex == SEX_XX)
	{
		PutChar(CStringFormat("SEX_XX_%d",iRow),'X');
	}
	else if (sex == SEX_XY)
	{
		PutChar(CStringFormat("SEX_XY_%d",iRow),'X');
	}
}

void RegistrationDoc::PutPrintDate(const CDateTime& printDt)
{
	CString dateStr = printDt.GetDateInFmt(CDateTime::DDMMYYYY);
         
	PutChar("PRINTDATE_DAY1",dateStr.GetAt(0));
	PutChar("PRINTDATE_DAY2",dateStr.GetAt(1));
	PutChar("PRINTDATE_MONTH1",dateStr.GetAt(2));
	PutChar("PRINTDATE_MONTH2",dateStr.GetAt(3));
	PutChar("PRINTDATE_YEAR1",dateStr.GetAt(4));
	PutChar("PRINTDATE_YEAR2",dateStr.GetAt(5));
	PutChar("PRINTDATE_YEAR3",dateStr.GetAt(6));
	PutChar("PRINTDATE_YEAR4",dateStr.GetAt(7));
}

//void RegistrationDoc::PutCountry(Country* pEventCountry)
//{
//	CString code2a = pEventCountry->GetCountryCode2A();
//	PutChar("COUNTRY2A_1",code2a[0]);
//	PutChar("COUNTRY2A_2",code2a[1]);
//}

void RegistrationDoc::FillBottomOfPage(RegistrationRecord *pRecord)
{
	PutPrintDate(CDateTime::now());
}

void RegistrationDoc::FillHeaderOfPage()
{	
	FillOneEAN(Company::GetCompany()->GetFarmNo(),"FARMNO_%d");	
}

RegistrationDoc::~RegistrationDoc()
{
	m_records.FreeAndFlush();	
}

void RegistrationDoc::PutChar(const CString& sLabel,char c)
{
	CString sText = c;
	DrawText(sLabel,sText);
}

