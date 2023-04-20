#include "stdafx.h"
#include "NotificationSimplex.h"
#include "../../datatypes/CompanyInfo.h"

CNotificationSimplex::CNotificationSimplex(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator) : 
		CZgloszenieBase(pConverter,pNavigator)
{
}

int CNotificationSimplex::GetPrintPages() const
{
	return m_nSheets;
}

void CNotificationSimplex::ComputeIndexes()
{
	int cowsPerSheet = GetNumberOfRecordsPerSheet();
	int cowSize = m_currCows.GetSize();
	m_currStartIdx = (m_currDocPage-1)*cowsPerSheet;				    
	if (m_currStartIdx+cowsPerSheet >= cowSize)
		m_currEndIdx = cowSize-1;
	else
		m_currEndIdx = m_currStartIdx+(cowsPerSheet-1);
}
void CNotificationSimplex::PrintPage(int nPage)
{
	          				
	FillHeaderOfPage();   
	
	FillEANs(m_currStartIdx,m_currEndIdx,1);	

	FillBottomOfPage(m_pCurrentRecord);	
}




