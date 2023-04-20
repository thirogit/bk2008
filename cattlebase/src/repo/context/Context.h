#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "../../datatypes/HentArray.h"
#include "../../missing/MapEx.h"
#include "../../datatypes/StockDataArray.h"
#include "../../datatypes/CompanyInfo.h"
#include "../../datatypes/Agent.h"
#include "../NewPtrFlushArray.h"
#include "ContextTransaction.h"
#include "ContextTransactionObj.h"
#include "IContext.h"

class Context //: public IContext
{
public:	
	Context();
	
	virtual int GetHentCount() const;
	virtual Hent* GetHent(int iIndex);
	virtual Hent* GetHent(const CString& sFarmNo);
	virtual void GetHents(CHentArray& hents);
	

	int GetStockCount() const;
	Stock* GetStock(int iIndex);
	Stock* GetStock(const CString& stockCode);
	void GetStocks(CStockDataArray& stocks);
	
	
	Stock* InsertStock(const Stock* pStockToInsert);
	Agent* InsertAgent(const Agent* pAgentToInsert);
	Hent*  InsertHent(const Hent* pHentToInsert);


	const Company* GetCompany() const;


	int GetAgentCount() const;
	Agent* GetAgent(int iIndex);
	Agent* GetAgent(const CString& sAgentCode);


	ContextTransactionObj OpenTransaction();
protected:

	
	
private:
	NewPtrFlushArray<ContextTransaction> m_Transactions;

	CMapEx<CString,const CString&,Hent*,Hent*> m_HentsPerFarmNo;
	CMapEx<CString,const CString&,Stock*,Stock*> m_StocksPerFarmNo;
	CMapEx<CString,const CString&,Agent*,Agent*> m_AgentsPerCode;
};

#endif