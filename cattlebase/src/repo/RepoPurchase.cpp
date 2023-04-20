#include "stdafx.h"
#include "RepoPurchase.h"


RepoPurchase::RepoPurchase(UINT puracheId) : 	m_PuracheId(puracheId),m_Downloaded(false)
						
{
}

RepoPurchase::~RepoPurchase()
{
	m_Hents.FreeAndFlush();
	m_Invoices.FreeAndFlush();
}

RepoPurchase::RepoPurchase(const RepoPurchase& src)
{
	m_PuracheId = src.m_PuracheId;
	CopyFrom(src);
}


void RepoPurchase::CopyFrom(const RepoPurchase& src)
{
	m_Hents.FreeAndFlush();
	m_Invoices.FreeAndFlush();

	m_AgentCode = src.m_AgentCode;
	m_PlateNo = src.m_PlateNo;
	m_StartDate = src.m_StartDate;
	m_EndDate = src.m_EndDate;


	NewPtrFlushArray<RepoHent>::const_iterator hentIt = src.m_Hents.begin();
	while(hentIt != src.m_Hents.end())
	{
		m_Hents.Add(new RepoHent(*(*(hentIt++))));
	}

	
	NewPtrFlushArray<RepoInvoice>::const_iterator invoiceIt = src.m_Invoices.begin();
	while(invoiceIt != src.m_Invoices.end())
	{
		m_Invoices.Add(new RepoInvoice(*(*(invoiceIt++))));
	}
	m_Downloaded = src.m_Downloaded;
}

UINT RepoPurchase::GetId() const
{
	return m_PuracheId;
}

int RepoPurchase::GetCowCount() const
{
	int count = 0;
	NewPtrFlushArray<RepoInvoice>::const_iterator invoiceIt = m_Invoices.begin();
	while(invoiceIt != m_Invoices.end())
	{
		count += (*invoiceIt)->GetCowCount();
		invoiceIt++;
	}
	return count;
}

const CString& RepoPurchase::GetAgentCode() const
{
	return m_AgentCode;
}
const CString& RepoPurchase::GetPlateNo() const
{
	return m_PlateNo;
}

const CDateTime& RepoPurchase::GetStartDate() const
{
	return m_StartDate;
}

const CDateTime& RepoPurchase::GetEndDate() const
{
	return m_EndDate;
}

void RepoPurchase::SetAgentCode(const CString& sAgentCode)
{
	m_AgentCode = sAgentCode;
}

void RepoPurchase::SetPlateNo(const CString& sPlateNo)
{
	m_PlateNo = sPlateNo;
}

void RepoPurchase::SetStartDate(const CDateTime& startDt)
{
	m_StartDate = startDt;
}

void RepoPurchase::SetEndDate(const CDateTime& endDt)
{
	m_EndDate = endDt;
}

int RepoPurchase::GetInvoiceCount() const
{
	return m_Invoices.GetSize();
}

RepoInvoice* RepoPurchase::GetInvoice(int iInvoice)
{
	return m_Invoices.GetAt(iInvoice);
}

const RepoInvoice* RepoPurchase::GetInvoice(int iInvoice) const
{
	return const_cast<RepoPurchase*>(this)->GetInvoice(iInvoice);
}

void RepoPurchase::AddInvoice(RepoInvoice* pInvoice)
{
	m_Invoices.Add(pInvoice);
}

int RepoPurchase::GetHentCount() const
{
	return m_Hents.GetSize();
}

RepoHent* RepoPurchase::GetHent(int iHent)
{
	return m_Hents.GetAt(iHent);
}

void RepoPurchase::AddHent(RepoHent* pHent)
{
	m_Hents.Add(pHent);
}

bool RepoPurchase::GetWasDownloaded() const
{
	return m_Downloaded;
}

void RepoPurchase::SetWasDownloaded(bool bDownloaded)
{
	m_Downloaded = bDownloaded;
}