#include "stdafx.h"
#include "Stock.h"

#include "log.h"
#include "CattleSQL.h"
#include "consts.h"

#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "sqlupdate/sqldatamembers/SQLCowSexDataMember.h"
#include "sqlupdate/sqldatamembers/SQLMoneyDataMember.h"
#include "sqlupdate/sqldatamembers/SQLRuleDataMember.h"


Stock::Stock(UINT Id/* = NULL_STOCKID*/) : stockid(Id),rule(NULL),myperkgprice(ZEROMONEY) 
{	

	transactedMembers.Add(&stockname);	
	transactedMembers.Add(&stockcode);
	transactedMembers.Add(&rule);
	transactedMembers.Add(&myperkgprice);	
	transactedMembers.Add(&predefsex);
	transactedMembers.Add(&officialcode);

	updatebleMembers.Add(new SQLColumnUpdate("STOCKNAME",		new SQLStringDataMember(&stockname))); 
	updatebleMembers.Add(new SQLColumnUpdate("STOCKCODE",		new SQLStringDataMember(&stockcode))); 
	updatebleMembers.Add(new SQLColumnUpdate("PREDEFSEX",		new SQLCowSexDataMember(&predefsex))); 
	updatebleMembers.Add(new SQLColumnUpdate("MYPERKGPRICE",	new SQLMoneyDataMember(&myperkgprice))); 
	updatebleMembers.Add(new SQLColumnUpdate("RULE",			new SQLRuleDataMember(&rule)));
	updatebleMembers.Add(new SQLColumnUpdate("OFFICIALCODE",    new SQLStringDataMember(&officialcode))); 
}
Stock::~Stock()
{
	updatebleMembers.FreeAndFlush();
}

int	Stock::GetId() const
{
	return stockid;
}

void Stock::ResetId()
{
	stockid = NULL_STOCKID;
}


CString	Stock::GetStockName() const
{
	return stockname.GetValue();
}

void Stock::SetStockName(const CString& stockName)
{
	stockname.SetTo(stockName);
}

void Stock::ChangeStockName(const CString& stockName)
{
	stockname.ChangeTo(stockName);
}


CString	Stock::GetStockCode() const
{
	return stockcode.GetValue();
}

void Stock::SetStockCode(const CString& stockCode)
{
	stockcode.SetTo(stockCode);
}

void Stock::ChangeStockCode(const CString& stockCode)
{
	stockcode.ChangeTo(stockCode);
}


Money Stock::GetMyPerKgPrice() const
{
	return myperkgprice.GetValue();
}

void Stock::SetMyPerKgPrice(const Money& dMyPerKgPrice)
{
	myperkgprice.SetTo(dMyPerKgPrice);
}

void Stock::ChangeMyPerKgPrice(const Money& dMyPerKgPrice)
{
	myperkgprice.ChangeTo(dMyPerKgPrice);
}


CowSex Stock::GetPredefSex() const
{
	return predefsex.GetValue();
}

void Stock::SetPredefSex(const CowSex& cowsex)
{
	predefsex.SetTo(cowsex);
}

void Stock::ChangePredefSex(const CowSex& cowsex)
{
	predefsex.ChangeTo(cowsex);
}


Rule* Stock::GetRule() const
{
	return rule.GetValue();
}

void Stock::SetRule(Rule* pRule)
{
	rule.SetTo(pRule);
}

void Stock::ChangeRule(Rule* pRule)
{
	rule.ChangeTo(pRule);
}


CString	Stock::GetOfficialCode() const
{
	return officialcode.GetValue();
}

void Stock::SetOfficialCode(const CString& sOfficialCode)
{
	officialcode.SetTo(sOfficialCode);
}

void Stock::ChangeOfficialCode(const CString& sOfficialCode)
{
	officialcode.ChangeTo(sOfficialCode);
}

void Stock::CopyFrom(const Stock &copy)
{
	stockname.CopyFrom(copy.stockname);
	stockcode.CopyFrom(copy.stockcode);
	stockid = copy.stockid;
	rule.CopyFrom(copy.rule);
	myperkgprice.CopyFrom(copy.myperkgprice);
	predefsex.CopyFrom(copy.predefsex);
	officialcode.CopyFrom(copy.officialcode);
}

bool Stock::InsertThisStock(bool bCommit /*=true*/)
{
	if(!IsNew()) 
	{
		SQLDB.SetLastError("This stock has not id = 0, possibly it had been allready inserted.");
		return false; 
	}
	if(GetStockCode().GetLength() == 0)
	{
		SQLDB.SetLastError("Empty stock code.");
		return false;
	}

	SQLResult res;
	SQLQuery q;
	q <<  "INSERT INTO stocks(stockname,stockcode,myperkgprice,predefsex,officialcode) VALUES (%0q,%1q,%2,%3,%4n)";
	q.parse();


	LOG("InsertThisStock() query: %s",q.str((LPCSTR)GetStockName(),(LPCSTR)GetStockCode(),
										GetMyPerKgPrice(),GetPredefSex(),(LPCSTR)GetOfficialCode()).c_str());

	if(!SQLDB.Insert(q.str((LPCSTR)GetStockName(),(LPCSTR)GetStockCode(),
										GetMyPerKgPrice(),GetPredefSex(),(LPCSTR)GetOfficialCode()).c_str())) 
	{
		LOG("InsertThisStock() error: %s",(LPCSTR)SQLDB.GetLastError());
		return false;
	}
    
	if((stockid = SQLDB.SelectScalar("SELECT MAX(stockid) FROM stocks",1)) < 0)
	{
		SQLDB.Rollback();
		return false;
	}
	if(bCommit) SQLDB.Commit();
	return true;

}
//***************************************************************************************
bool Stock::DeleteThisStock(bool bCommit/* = true*/)
{
	SQLQuery q;
	q << "DELETE FROM stocks WHERE stockid = " << GetId();

	LOG("DeleteThisStock() stmt: %s",q.str().c_str());
	if(!SQLDB.Delete(q.str().c_str(),bCommit))
	{
		LOG("DeleteThisStock() error: %s",(LPCSTR)SQLDB.GetLastError());		
		return false;
	}
	ResetId();
	return true;
}
//***************************************************************************************
bool Stock::UpdateThisStock(bool bCommit/* = true*/)
{
	if(IsUncommited())
	{
		SQLQuery q;
		q << "UPDATE stocks SET "; 
		
		UpatableDataMembers membertToUpdate;

		UpatableDataMembers::ArrayIterator it = updatebleMembers.iterator();
		while(it.hasNext())
		{
			SQLColumnUpdate* pColumn = *(it++);
			if(pColumn->NeedsUpdate())
			{
				membertToUpdate.Add(pColumn);
			}
		}	

		it = membertToUpdate.iterator();
		while(it.hasNext())
		{
			SQLColumnUpdate* pColumn = *(it++);
			if(pColumn->NeedsUpdate())
			{
				q << pColumn->ToEqualString();
				if(it.hasNext())
					q << ", ";
			}
		}	
		
		q << " WHERE stockid = " << GetId();

		LOG("UpdateThisStock() stmt %s",q.str().c_str());

		if(!SQLDB.Update(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisStock() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

	}
	return true;
}

void Stock::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();

}
void Stock::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}
bool Stock::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}

bool Stock::IsNew() const
{
	return GetId() == NULL_STOCKID;
}