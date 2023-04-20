#include "stdafx.h"
#include "HerdBook.h"
//#include "../classes/types/CowSex.h"

//#include "../consts.h"
#include "../../datatypes/CompanyInfo.h"
#include "../../Utils/StringFormat.h"



CHerdBook::CHerdBook()
{
  m_PageNum = 1;
  m_SerialNum = 1; 
}



int CHerdBook::LoadInputData(CPtrArray &docInputData)
{
	m_cows.RemoveAll();
	m_cows.Append((CCowArray&)docInputData);


	COWDATA_MEMBERS sortByMember = GetSortBy();
	if(sortByMember == (COWDATA_MEMBERS)-1)
		sortByMember = COWS_SORT_BY_INDATE;


	DataMemberSortParamsArray sortby;

	sortby.Add(DataMemberSortParams(sortByMember,GetSortOrder()));
	sortby.Add(DataMemberSortParams(COWS_SORT_BY_EAN,true));

	m_cows.SortBy(sortby);	
	
	m_PageNum = PrintCfg().GetNum(GetCfgItemID(),CHerdBook::CFG_PAGENUM);
	m_SerialNum = PrintCfg().GetNum(GetCfgItemID(),CHerdBook::CFG_SERIALNUM);
	return RELOAD_OK_CONTINUE;
}
int CHerdBook::GetPrintPages() const
{
	int cowsOnPage = GetCowsOnPage();
	int size = m_cows.GetSize();
	int pages = size/cowsOnPage;
        if (size%cowsOnPage)  pages += 1;
     	return pages;
}

void CHerdBook::PrintPage(int page)
{
	int cowsOnPage = GetCowsOnPage();
	int startidx = (page-1)*cowsOnPage,
		endidx  = (startidx+cowsOnPage)-1,
		size = m_cows.GetSize(),			
		i,
		row;
			
	PrintHerdNo(Company::GetCompany()->GetFarmNo(),"HERDNO_%d");
	
	DrawText("PAGE",CStringFormat("%d",m_PageNum+page-1));

	if (endidx >= size)	
		endidx = size-1;
	
	for(i=startidx,row=1;i<=endidx;i++,row++)	
	{
		DrawCowRow(row,m_SerialNum+i,m_cows[i]);
	}
}
void CHerdBook::PrintHerdNo(const CString& sHerdNo,const CString& sLabelFmt)
{
	PrintEAN(sHerdNo,sLabelFmt);
}

void CHerdBook::PrintEAN(const CString& sEAN,const CString& sLabelFmt)
{
	for(int j = 0; j < 14 && j < sEAN.GetLength();j++)
	{		
		DrawText(CStringFormat(sLabelFmt,j+1),CStringFormat("%c",sEAN.GetAt(j)));
	}	
}

CString CHerdBook::FormatHent(const Hent* pHent)
{
	const char szAddressFmt[] = "%s, %s\n%s %s,%s %s";

	return CStringFormat(szAddressFmt,
						(LPCSTR)pHent->GetFarmNo(),
						(LPCSTR)pHent->GetName(),						
						(LPCSTR)pHent->GetStreet(),
						(LPCSTR)pHent->GetPOBox(),
						(LPCSTR)pHent->GetZip(),
						(LPCSTR)pHent->GetCity());
}


	
IMPLEMENT_DYNCREATE(CHerdBookOptDlg, CCustomPrintDocOptDlg)


CHerdBookOptDlg::CHerdBookOptDlg (CWnd *pParent/* = NULL*/) : CCustomPrintDocOptDlg(IDD,pParent),
		m_nPageNum(1),m_nSerialNum(1)
{
}

void CHerdBookOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomPrintDocOptDlg::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_SERIALNUM,m_nSerialNum);
	DDX_Text(pDX,IDC_PAGENUM,m_nPageNum);	
}   


BOOL CHerdBookOptDlg::OnInitDialog()
{
	CCustomPrintDocOptDlg::OnInitDialog();

	m_nPageNum = PrintCfg().GetNum(m_nCfgID,CHerdBook::CFG_PAGENUM);
	m_nSerialNum = PrintCfg().GetNum(m_nCfgID,CHerdBook::CFG_SERIALNUM);
	
	UpdateData(FALSE);
	return TRUE;
}

void CHerdBookOptDlg::Save()
{
	UpdateData();
	PrintCfg().SetNum(m_nCfgID,CHerdBook::CFG_PAGENUM,m_nPageNum);
	PrintCfg().SetNum(m_nCfgID,CHerdBook::CFG_SERIALNUM,m_nSerialNum);
	
}