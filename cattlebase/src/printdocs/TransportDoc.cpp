#include "stdafx.h"
#include "TransportDoc.h"
#include "PrintDocsCfg.h"
#include "../logo/Logo.h"
#include "datatypes/CompanyInfo.h"
#include "../Utils/StringFormat.h"
#include "../classes/IntString.h"

IMPLEMENT_DYNCREATE(CTransportDocOptDlg, CCustomPrintDocOptDlg)
CTransportDocOptDlg::CTransportDocOptDlg (CWnd *pParent/* = NULL*/) : 
	CCustomPrintDocOptDlg(IDD,pParent) 
{
}	

BOOL CTransportDocOptDlg::OnInitDialog()
{
	CCustomPrintDocOptDlg::OnInitDialog();
	m_bDoNotFillCarrier = PrintCfg().GetBool(GetCfgItemID(),TRANSPORTDOC_DONOTFILLCARIER);
	UpdateData(FALSE);
	return TRUE;
}

void CTransportDocOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomPrintDocOptDlg::DoDataExchange(pDX);
	DDX_Check(pDX,IDC_DONTFILLCARRIER,m_bDoNotFillCarrier);				
}
void CTransportDocOptDlg::Save()
{
	UpdateData();
	PrintCfg().SetBool(GetCfgItemID(),TRANSPORTDOC_DONOTFILLCARIER,m_bDoNotFillCarrier);
}

BEGIN_MESSAGE_MAP(CTransportDocOptDlg, CCustomPrintDocOptDlg)
	
END_MESSAGE_MAP()

//#############################################################################

CTransportDoc::CTransportDoc() : currDoc(NULL),starti(0),endi(0),m_nPages(0),m_bDontFillCarrier(FALSE)
{
   
}

int CTransportDoc::GetPrintPages() const
{
	return m_nPages;
}

int CTransportDoc::LoadInputData(CPtrArray &docInputData)
{
	m_bDontFillCarrier = PrintCfg().GetBool(GetCfgItemID(),TRANSPORTDOC_DONOTFILLCARIER);

	m_docs.RemoveAll();
	m_docs.Append((CDocDataArray&)docInputData);

	int pages = 0,size,x;
	size = m_docs.GetSize();
	pages = size;
	for(int i = 0;i < size;i++)
	{
		x = m_docs[i]->GetCowCount()-TRANSPORTDOCPAGE_1_SIZE;
		if(x > 0)
		{
			pages += x/TRANSPORTDOCPAGE_2_SIZE;
			if(x%TRANSPORTDOCPAGE_2_SIZE) pages++;
		}
	}
    m_nPages = pages;
	m_currDocCows.RemoveAll();
	currDoc = NULL;
	currDocLastPage = -1;
	
	return RELOAD_OK_CONTINUE;

}

void CTransportDoc::PreparePage(int nPage)
{
	int p = 0,i = 0,x,fp,grp,nDocLastPage = -1,newCurDocPageSize = 0;
	Doc *pDoc = NULL;
	CString grpname;
		
	do
	{
		p++;
		fp  = p;
		pDoc = m_docs[i];
		x =  pDoc->GetCowCount() - TRANSPORTDOCPAGE_1_SIZE;
		if(x > 0) 
		{
			p += x/TRANSPORTDOCPAGE_2_SIZE;
			if(x%TRANSPORTDOCPAGE_2_SIZE) p++;
		}
		newCurDocPageSize = p-fp+1;
		nDocLastPage = p;
		i++;
	}
	while(p < nPage);

	if(currDoc != pDoc)
	{
		curDocPageSize = newCurDocPageSize;
		currDoc = pDoc;
		currDocLastPage = nDocLastPage;
				
		m_currDocCows.RemoveAll();
		m_currDocCows.AppendPtr(currDoc->GetCows());

		if(GetSortBy() != (COWDATA_MEMBERS)-1)
			m_currDocCows.SortBy(GetSortBy(),GetSortOrder());		
	}

	int size = m_currDocCows.GetSize();
	
	ResetGroupsVisibility(false);
	if(nPage == fp) 
	{
		starti = 0;
		endi = TRANSPORTDOCPAGE_1_SIZE - 1;
		SetTemplatePage(0);
		pageOff = 0;
		grp = 1;
		
		COleStreamFile logoStream;
		if(CLogoImage::GetLogo()->GetOleLogoImage(logoStream))
			DrawImage("LOGO",logoStream);		

	}
	else
	{
		starti = TRANSPORTDOCPAGE_1_SIZE + (nPage-fp-1)*TRANSPORTDOCPAGE_2_SIZE;
		endi = starti + TRANSPORTDOCPAGE_2_SIZE - 1;
		SetTemplatePage(1);
		pageOff = nPage-fp;
		grp = TRANSPORTDOCPAGE_1_SIZE+1;    
		
	}
				
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,grp++)
	{
		grpname.Format("%d",grp);
		TRACE("Group = %s\n",(LPCSTR)grpname);
		SetGroupVisible(grpname,true);
	}		

}

void CTransportDoc::PrintPage(int page)
{
	CString fld,str,seller_str,buyer_str;
	int i,row;
	const Hent *transportCompany = GetTransportCompany(),*destinationHent = GetDestination();
		
	DrawText("DOCNUMBER",currDoc->GetDocNoStr());
	DrawText("PAGE",CStringFormat("%d/%d",pageOff+1,curDocPageSize));

    if(pageOff > 0)
	{
		row = TRANSPORTDOCPAGE_1_SIZE+1;	
	}
	else
	{		
		if(!m_bDontFillCarrier)
		{
			DrawText("TRANSPORTCOMPANY",FormatHent(transportCompany));
		}
		DrawText("DESTINATION",FormatHent(destinationHent));
		
		DrawText("DOCDATE",currDoc->GetDocDate().GetDayDate());
		DrawText("TRANSPORTDATE",currDoc->GetLoadDate().GetDayDate());
		DrawText("PLATENO",currDoc->GetPlateNo());
		DrawText("LOADSTARTTIME",currDoc->GetLoadStartTime().GetTime(Time::HHcolonMI));
		DrawText("LOADENDTIME",currDoc->GetLoadEndTime().GetTime(Time::HHcolonMI));
		DrawText("EXTRAS",currDoc->GetExtras());

		row = 1;
	}

		
	for(i = starti;i <= endi;i++,row++)
	{
        PrintRecord(m_currDocCows[i],row,i);			
	}
}

void CTransportDoc::PrintRecord(Cow *pCow,int iRow,int iIndex)
{
	DrawText(CStringFormat("EAN%d",iRow),pCow->GetEAN());	
	
	DrawText(CStringFormat("LP%d",iRow),CIntString(iIndex+1));

	if(pCow->HasFirstOwner())
		DrawText(CStringFormat("FSTOWNR%d",iRow),FormatFirstOwner(pCow->GetFirstOwner()));
	
}

CString CTransportDoc::FormatHent(const Hent* hent)
{
	CString outStr;
		outStr.Format("%s\n%s %s, %s %s\n%s",
			(LPCSTR)hent->GetName(),
			(LPCSTR)hent->GetStreet(),
			(LPCSTR)hent->GetPOBox(),
			(LPCSTR)hent->GetZip(),
			(LPCSTR)hent->GetCity(),
			(LPCSTR)hent->GetFarmNo());
		if(hent->HasNIP())
			outStr.AppendFormat(", NIP: %s",(LPCSTR)hent->GetNIP());
		if(hent->HasREGON())
			outStr.AppendFormat(",REGON: %s",(LPCSTR)hent->GetREGON());
		if(hent->HasWetNo())
			outStr.AppendFormat("\nNrWet: %s",(LPCSTR)hent->GetWetNo());


	return outStr;
	
}

CString CTransportDoc::FormatFirstOwner(const Hent* hent)
{
	CString	outStr;
	outStr.Format("%s, %s %s, %s %s, %s",
			(LPCSTR)hent->GetName(),
			(LPCSTR)hent->GetStreet(),
			(LPCSTR)hent->GetPOBox(),
			(LPCSTR)hent->GetZip(),
			(LPCSTR)hent->GetCity(),
			(LPCSTR)hent->GetFarmNo());
	return outStr;	
	
}

IMPLEMENT_DYNCREATE(CPzTransportDoc, CTransportDoc)
CPzTransportDoc::CPzTransportDoc()
{
}

const Hent* CPzTransportDoc::GetTransportCompany()
{
	return currDoc->GetHent();	
}

const Hent* CPzTransportDoc::GetDestination()
{
	return Company::GetCompany()->GetAsHent();
}

IMPLEMENT_DYNCREATE(CWzTransportDoc, CTransportDoc)
CWzTransportDoc::CWzTransportDoc()
{
}

const Hent* CWzTransportDoc::GetDestination()
{
	return currDoc->GetHent();			
}

const Hent* CWzTransportDoc::GetTransportCompany()
{
	return Company::GetCompany()->GetAsHent();
}