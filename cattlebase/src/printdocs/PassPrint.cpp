#include "stdafx.h"
#include "PassPrint.h"
#include "PrintDocsCfg.h"
#include "../PassportStorage/PassImgBase.h"


IMPLEMENT_DYNCREATE(CPassPrintDoc, CCowDrwDoc)

CPassPrintDoc::CPassPrintDoc() : m_nPages(0)
{    
}
int CPassPrintDoc::LoadInputData(CPtrArray &docInputData)
{
	m_cows.RemoveAll();
	m_cows.Append((CCowArray&)docInputData);

	int s,i,s2;
	
	s = m_cows.GetSize();
	s2 = s;
	for(i = 0;i < s;i++)
	{
		if(!m_cows[i]->HasPassImg())
		{
			m_cows.RemoveAt(i);
			i--;
			s--;
		}
	}

	if(!s) return RELOAD_NO_CONTINUE;

	if(GetSortBy() != (COWDATA_MEMBERS)-1)
		m_cows.SortBy(GetSortBy(),GetSortOrder());

	

	m_nPages = s/2;
    if (s&1)  m_nPages += 1;
		
	

	return s2 == s ? RELOAD_OK_CONTINUE : RELOAD_CAN_CONTINUE;
}

int CPassPrintDoc::GetPrintPages() const
{	
    return m_nPages;		
}

void CPassPrintDoc::PreparePage(int nPage)
{
	const char pass2Label[] = "PASS2";

	int imgIndex = (nPage-1)*2;
	CPassImgBase *pPassDB = CPassImgBase::GetInstance();
	COleStreamFile passPic1;
	COleStreamFile passPic2;

	
	pPassDB->GetPassImageOle(passPic1,m_cows[imgIndex]);
	DrawImage("PASS1",passPic1);	
	passPic1.Close();

	imgIndex++;

	if(imgIndex < m_cows.GetSize())
	{
		
		pPassDB->GetPassImageOle(passPic2,m_cows[imgIndex]);
		DrawImage(pass2Label,passPic2);
		passPic2.Close();
		SetGroupVisible(pass2Label,true);
	}
	else
	{
		SetGroupVisible(pass2Label,false);
	}

}

void CPassPrintDoc::PrintPage(int page)
{
	
}
