#include "stdafx.h"
#include "NotificationDuplex.h"
#include "../../datatypes/CompanyInfo.h"

CNotificationDuplex::CNotificationDuplex(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator) :
	CZgloszenieBase(pConverter,pNavigator)
{
}

int CNotificationDuplex::GetPrintPages() const
{
	return m_nSheets*2;
}

int CNotificationDuplex::GetNumberOfRecordsOnBackPage()
{
	return GetNumberOfRecordsPerSheet()-GetNumberOfRecordsOnFrontPage();
}

int CNotificationDuplex::Load()
{
	int iRet = CZgloszenieBase::Load();
	return iRet;
}

void CNotificationDuplex::ComputeIndexes()
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

void CNotificationDuplex::PrintPage(int nPage)
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

void CNotificationDuplex::PreparePage(int nPage)
{
	SetTemplatePage(!(nPage%2));
	CZgloszenieBase::PreparePage(nPage);
}

