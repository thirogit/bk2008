#include "stdafx.h"
#include "WeightingDoc.h"
#include "PrintDocsCfg.h"
#include "../logo/Logo.h"
#include "datatypes/CompanyInfo.h"
#include "../Utils/StringFormat.h"
#include "../classes/IntString.h"


CWeightingDoc::CWeightingDoc() : currDoc(NULL),starti(0),endi(0),m_nPages(0)
{
   
}

int CWeightingDoc::GetPrintPages() const
{
	return m_nPages;
}

int CWeightingDoc::LoadInputData(CPtrArray &docInputData)
{
	m_docs.RemoveAll();
	m_docs.Append((CDocDataArray&)docInputData);

	int pages = 0,size,x;
	size = m_docs.GetSize();
	pages = size;
	for(int i = 0;i < size;i++)
	{
		x = m_docs[i]->GetCowCount()-WEIGHTINGDOCPAGE_1_SIZE;
		if(x > 0)
		{
			pages += x/WEIGHTINGDOCPAGE_2_SIZE;
			if(x%WEIGHTINGDOCPAGE_2_SIZE) pages++;
		}
	}
    m_nPages = pages;
	m_currDocCows.RemoveAll();
	currDoc = NULL;
	currDocLastPage = -1;
	
	return RELOAD_OK_CONTINUE;

}

void CWeightingDoc::PreparePage(int nPage)
{
	int p = 0,i = 0,x,fp,grp,nDocLastPage = -1,newCurDocPageSize = 0;
	Doc *pDoc = NULL;
	CString grpname;
		
	do
	{
		p++;
		fp  = p;
		pDoc = m_docs[i];
		x =  pDoc->GetCowCount() - WEIGHTINGDOCPAGE_1_SIZE;
		if(x > 0) 
		{
			p += x/WEIGHTINGDOCPAGE_2_SIZE;
			if(x%WEIGHTINGDOCPAGE_2_SIZE) p++;
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
		endi = WEIGHTINGDOCPAGE_1_SIZE - 1;
		SetTemplatePage(0);
		pageOff = 0;
		grp = 0;
		
		COleStreamFile logoStream;
		if(CLogoImage::GetLogo()->GetOleLogoImage(logoStream))
			DrawImage("LOGO",logoStream);		

	}
	else
	{
		starti = WEIGHTINGDOCPAGE_1_SIZE + (nPage-fp-1)*WEIGHTINGDOCPAGE_2_SIZE;
		endi = starti + WEIGHTINGDOCPAGE_2_SIZE - 1;
		SetTemplatePage(1);
		pageOff = nPage-fp;
		grp = WEIGHTINGDOCPAGE_1_SIZE;    
		
	}
				
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,grp++)
	{
		grpname.Format("%d",grp);		
		SetGroupVisible(grpname,true);
	}		

}

void CWeightingDoc::PrintPage(int page)
{
	CString fld,str,seller_str,buyer_str;
	int i,row;
	const Hent *sourceHent = GetSource(),*destinationHent = GetDestination();
		
	DrawText("DOCNUMBER",currDoc->GetDocNoStr());
	DrawText("PAGE",CStringFormat("%d/%d",pageOff+1,curDocPageSize));

    if(pageOff > 0)
	{
		row = WEIGHTINGDOCPAGE_1_SIZE;	
	}
	else
	{		
		
		DrawText("SOURCE",FormatHent(sourceHent));
		DrawText("DESTINATION",FormatHent(destinationHent));
		
		DrawText("DOCDATE",currDoc->GetDocDate().GetDayDate());
		DrawText("TRANSPORTDATE",currDoc->GetLoadDate().GetDayDate());
		DrawText("PLATENO",currDoc->GetPlateNo());
		DrawText("LOADSTARTTIME",currDoc->GetLoadStartTime().GetTime(Time::HHcolonMI));
		DrawText("LOADENDTIME",currDoc->GetLoadEndTime().GetTime(Time::HHcolonMI));
		
		row = 0;
	}

		
	for(i = starti;i <= endi;i++,row++)
	{
        PrintRecord(m_currDocCows[i],row,i);			
	}
}

void CWeightingDoc::PrintRecord(Cow *pCow,int iRow,int iIndex)
{
	DrawText(CStringFormat("LP%d",iRow),CIntString(iIndex+1));
	
	DrawText(CStringFormat("EAN%d",iRow),pCow->GetEAN());	

	DrawText(CStringFormat("SEX%d",iRow),(LPCSTR)pCow->GetSex());

	DrawText(CStringFormat("STOCK%d",iRow),pCow->GetStock()->GetStockCode());

	DrawText(CStringFormat("WEIGHT_DECLARED%d",iRow),pCow->GetWeight().ToString(GetWeightFormat()));
	
}

CString CWeightingDoc::FormatHent(const Hent* hent)
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



IMPLEMENT_DYNCREATE(CWeightingInDoc, CWeightingDoc)
CWeightingInDoc::CWeightingInDoc()
{
}

const Hent* CWeightingInDoc::GetSource()
{
	return currDoc->GetHent();	
}

const Hent* CWeightingInDoc::GetDestination()
{
	return Company::GetCompany()->GetAsHent();
}

IMPLEMENT_DYNCREATE(CWeightingOutDoc, CWeightingDoc)
CWeightingOutDoc::CWeightingOutDoc()
{
}

const Hent* CWeightingOutDoc::GetDestination()
{
	return currDoc->GetHent();			
}

const Hent* CWeightingOutDoc::GetSource()
{
	return Company::GetCompany()->GetAsHent();
}