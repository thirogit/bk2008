#include "stdafx.h"
#include "RegistrationDuplex.h"


IMPLEMENT_DYNCREATE(RegistrationDuplex,RegistrationDoc);

RegistrationDuplex::RegistrationDuplex() 
{
}

int RegistrationDuplex::GetPrintPages() const
{
	return m_nSheets*2;
}

int RegistrationDuplex::GetNumberOfRecordsOnBackPage()
{
	return GetNumberOfRecordsPerSheet()-GetNumberOfRecordsOnFrontPage();
}

int RegistrationDuplex::GetNumberOfRecordsPerSheet()
{
	return 6;
}

int RegistrationDuplex::GetNumberOfRecordsOnFrontPage()
{
	return 3;
}

void RegistrationDuplex::ComputeIndexes()
{
	int cowSize = m_currCows.GetSize();
	int cowsPerSheet = GetNumberOfRecordsPerSheet();
	int cowsOnFrontPage = GetNumberOfRecordsOnFrontPage();
	int cowsOnBackPage = GetNumberOfRecordsOnBackPage();
	if(m_currDocPage&1)
	{
		m_currStartIdx = (m_currDocPage/2)*cowsPerSheet;
	          
		if (m_currStartIdx+(cowsOnFrontPage-1) >= cowSize)
			m_currEndIdx = cowSize-1;
		else
			m_currEndIdx = m_currStartIdx+(cowsOnFrontPage-1);	
	}
	else
	{
		m_currStartIdx = ((m_currDocPage-1)/2)*cowsPerSheet;
		if(m_currStartIdx+cowsOnFrontPage < cowSize)
		{			
			m_currStartIdx += cowsOnFrontPage;
			
			if ((m_currStartIdx+(cowsOnBackPage-1)) >= cowSize)	
				m_currEndIdx = cowSize-1;
			else
				m_currEndIdx = m_currStartIdx+cowsOnBackPage-1;		 
		}
		else
			m_currEndIdx = -1;
	}
}

void RegistrationDuplex::PrintPage(int nPage)
{
	if(m_currDocPage&1)
	{
		FillHeaderOfPage();
		FillEANs(m_currStartIdx,m_currEndIdx,1);	
	}
	else
	{
		FillEANs(m_currStartIdx,m_currEndIdx, GetNumberOfRecordsOnFrontPage()+1);
		FillBottomOfPage(m_pCurrentRecord);
	}	
	
}

void RegistrationDuplex::PreparePage(int nPage)
{
	SetTemplatePage(!(nPage%2));
	RegistrationDoc::PreparePage(nPage);
}

