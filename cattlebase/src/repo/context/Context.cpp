#include "stdafx.h"
#include "Context.h"
#include "../../global_arrays.h"
#include "../../dispacher/NewHentDispatcher.h"

Context::Context()
{
	Hent* pHent = NULL;
	CHentArray::ArrayIterator it = ary_hents.iterator();
	while(it.hasNext())
	{
		pHent = *(it++);
		m_HentsPerFarmNo.SetAt(pHent->GetFarmNo(),pHent);
	}
	Stock* pStock = NULL;
	CStockDataArray::ArrayIterator stockIt = ary_stocks.iterator();
	while(stockIt.hasNext())
	{
		pStock = *(stockIt++);
		m_StocksPerFarmNo.SetAt(pStock->GetStockCode(),pStock);
	}

	Agent* pAgent = NULL;

	CAgentArray::ArrayIterator agentIt = ary_agents.iterator();
	while(agentIt.hasNext())
	{
		pAgent = *(agentIt++);
		m_AgentsPerCode.SetAt(pAgent->GetAgentCode(),pAgent);
	}	
}

int Context::GetHentCount() const
{
	return ary_hents.GetCount();
}

Hent* Context::GetHent(int iIndex)
{
	return ary_hents.GetAt(iIndex);
}

Hent* Context::GetHent(const CString& farmNo)
{
	Hent* pHent = NULL;
	m_HentsPerFarmNo.Lookup(farmNo,pHent);
	return pHent;
}

void Context::GetHents(CHentArray& hents)
{
	hents.Append(ary_hents);
}


int Context::GetStockCount() const
{
	return ary_stocks.GetCount();
}

Stock* Context::GetStock(int iIndex)
{
	return ary_stocks.GetAt(iIndex);
}

Stock* Context::GetStock(const CString& stockCode)
{
	Stock* pStock = NULL;
	m_StocksPerFarmNo.Lookup(stockCode,pStock);
	return pStock;
}

void Context::GetStocks(CStockDataArray& stocks)
{
	stocks.Append(ary_stocks);
}

const Company* Context::GetCompany() const
{
	return Company::GetCompany();
}

ContextTransactionObj Context::OpenTransaction()
{
	return ContextTransactionObj(NULL);
}



Agent* Context::GetAgent(const CString& sAgentCode)
{
	Agent *pAgent = NULL;
	m_AgentsPerCode.Lookup(sAgentCode,pAgent);
	return pAgent;
}


Stock* Context::InsertStock(const Stock* pStockToInsert)
{
	Stock* pNewStock = new Stock(NULL_STOCKID);
	pNewStock->CopyFrom(*pStockToInsert);

	if(!pNewStock->InsertThisStock())
	{
		return NULL;
	}

	ary_stocks.Add(pNewStock);
	m_StocksPerFarmNo.SetAt(pNewStock->GetStockCode(),pNewStock);
	return pNewStock;

}

Agent* Context::InsertAgent(const Agent* pAgentToInsert)
{
	Agent* pNewAgent = new Agent(NULL_AGENTID);
	pNewAgent->CopyFrom(*pAgentToInsert);
	if(!pNewAgent->InsertThisAgent())
	{
		return NULL;
	}
	m_AgentsPerCode.SetAt(pNewAgent->GetAgentCode(),pNewAgent);
	ary_agents.Add(pNewAgent);
	return pNewAgent;
}

Hent*  Context::InsertHent(const Hent* pHentToInsert)
{
	Hent* pNewHent = new Hent(NULL_HENTID);
	pNewHent->CopyFrom(*pHentToInsert);
	
	if(!pNewHent->InsertThisHent())
	{
		return NULL;		
	}

	m_HentsPerFarmNo.SetAt(pNewHent->GetFarmNo(),pNewHent);
	NewHentDispatcher::Dispatch(pNewHent);
	return pNewHent;

}