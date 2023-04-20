#include "stdafx.h"
#include "swiadzdrowia.h"
#include "../Utils/StringFormat.h"
#include "../classes/IntString.h"

IMPLEMENT_DYNCREATE(CHealthCertDoc, CCowDrwDoc)

CHealthCertDoc::CHealthCertDoc() : m_currDoc(NULL),starti(0),endi(0),m_nPages(0),m_currDocPageSize(0)
{    
}


int CHealthCertDoc::GetPrintPages() const
{
	return m_nPages;
}

int CHealthCertDoc::LoadInputData(CPtrArray &docInputData)
{
	m_docs.RemoveAll();
	m_docs.Append((CDocDataArray&)docInputData);

	int pages = 0,size,x;
	size = m_docs.GetSize();
	for(int i = 0;i < size;i++)
	{
		x = m_docs[i]->GetCowCount();
		pages += x/HEALTHCERTDOCSIZE;
			if(x%HEALTHCERTDOCSIZE) pages++;
	}
    m_nPages = pages;

	m_currDoc = NULL;
	m_currDocPageSize = -1;
	m_currDocCows.RemoveAll();

	return RELOAD_OK_CONTINUE;

}

void CHealthCertDoc::PreparePage(int nPage)
{

	int p = 0,i = 0,x,y,fp,size,grp;
	Doc *pDoc = NULL;
	int nDocPageSize = -1;
			
	do
	{
		p++;
		fp  = p;
		pDoc = m_docs[i];
		x =  pDoc->GetCowCount()-HEALTHCERTDOCSIZE;
		nDocPageSize = 1;
		if(x > 0) 
		{
			y = x/HEALTHCERTDOCSIZE;
			p += y;
			nDocPageSize += y;

			if(x%HEALTHCERTDOCSIZE)
			{
				p++;
				nDocPageSize++;
			}
		}
		i++;
	}
	while(p < nPage);


	if(m_currDoc != pDoc)
	{
		m_currDoc = pDoc;
		m_currDocPageSize = nDocPageSize;
		m_currDocCows.RemoveAll();
		m_currDocCows.AppendPtr(m_currDoc->GetCows());
		
		if(GetSortBy() != (COWDATA_MEMBERS)-1)
			m_currDocCows.SortBy(GetSortBy(),GetSortOrder());
	}


	size = m_currDocCows.GetSize();
	
	ResetGroupsVisibility(false);
	CString grpname;

	pageOff = nPage-fp;
	starti = HEALTHCERTDOCSIZE*(pageOff);
	endi = starti + HEALTHCERTDOCSIZE - 1;
	
	grp = 0;    
						
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,grp++)
	{
		grpname.Format("%d",grp);
		SetGroupVisible(grpname,true);
	}
			

}


void CHealthCertDoc::PrintPage(int page)
{
	CString fld,str;
	int i,row;
    
	DrawText(CStringFormat("PAGES","%d/%d",pageOff+1),CIntString(m_currDocPageSize));    
	Cow *cow;
	row = 0;
	for(i = starti;i <= endi;i++,row++)
	{
	    cow = m_currDocCows[i];
		DrawText(CStringFormat("EAN%d",row),cow->GetEAN());
		DrawText(CStringFormat("PASSNO%d",row),cow->GetPassNo());
		DrawText(CStringFormat("LP%d",row),CIntString(i+1));
	}	
			
}