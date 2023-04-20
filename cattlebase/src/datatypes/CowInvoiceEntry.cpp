#include "stdafx.h"
#include "CowInvoiceEntry.h"
#include "../cattleSQL.h"
#include "../log.h"

CowInvoiceEntry::CowInvoiceEntry(Cow* pCow,Invoice* pInvoice) :
	m_pInvoice(pInvoice), m_pCow(pCow), m_pInvStock(NULL),
	m_InvWeight(NULL_WEIGHT), m_InvPrice(ZEROMONEY)
{
}
Stock* CowInvoiceEntry::GetStock() const
{
	return m_pInvStock;
}
Weight	CowInvoiceEntry::GetWeight() const
{
	return m_InvWeight;
}
Money CowInvoiceEntry::GetPrice() const
{
	return m_InvPrice;
}
Cow* CowInvoiceEntry::GetCow() const
{
	return m_pCow;
}

Invoice* CowInvoiceEntry::GetInvoice() const
{
	return m_pInvoice;
}

void CowInvoiceEntry::SetStock(Stock* invStock)
{
	m_pInvStock = invStock;
}
void CowInvoiceEntry::SetWeight(const Weight& weight)
{
	m_InvWeight = weight;
}
void CowInvoiceEntry::SetPrice(double dPrice)
{
	m_InvPrice = dPrice;
}
bool CowInvoiceEntry::InsertThis(bool bCommit/* = true*/)
{
	SQLQuery q;

	const char insStmtStr[] = "INSERT INTO cowpricing(cow,invoice,price,stock,weight)"
							  "VALUES(%0,%1,%2,%3,%4);";

	q << insStmtStr;
	q.parse();
		
	SQLQueryParms p;
	
	p  << SQLString(GetCow()->GetId())
		<< SQLString(GetInvoice()->GetId())
		  << SQLString(GetPrice())
		  << SQLString(GetStock()->GetId())
		  << SQLString(GetWeight().GetDouble());
		  
		LOG("InsertThisCowInvoiceEntry() query: %s",q.str(p).c_str());

		if(!SQLDB.Insert(q.str(p).c_str()))
		{
			LOG("InsertThisCowInvoiceEntry() error: %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

		if(bCommit) SQLDB.Commit();

		return true;
}

bool CowInvoiceEntry::DeleteThis(bool bCommit/* = true*/)
{
	SQLQuery q;
	q << "DELETE FROM cowpricing WHERE invoice = %0 AND cow = %1";
	q.parse();

	LOG("DeleteThisCowInvoiceEntry() stmt: %s",q.str(GetInvoice()->GetId(),GetCow()->GetId()).c_str());

	if(SQLDB.Delete(q.str(GetInvoice()->GetId(),GetCow()->GetId()).c_str(),bCommit))
	{		
		return true;
	}
	LOG("DeleteThisCowInvoiceEntry() error: %s",(LPCSTR)SQLDB.GetLastError());
	return false;
}