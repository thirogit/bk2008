#include "stdafx.h"
#include "Rule.h"

#include "log.h"
#include "CattleSQL.h"
#include "consts.h"

#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "sqlupdate/sqldatamembers/SQLMoneyDataMember.h"
#include "sqlupdate/sqldatamembers/SQLCowSexDataMember.h"
#include "sqlupdate/sqldatamembers/SQLIntegerDataMember.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Rule::Rule(UINT Id) : ruleid(Id),nosexvalue(SEX_XX)
{
	transactedMembers.Add(&rulename);	
	transactedMembers.Add(&nosexvalue);
	transactedMembers.Add(&buyrule.limweight);
	transactedMembers.Add(&buyrule.priceoverweightxy);	
	transactedMembers.Add(&buyrule.priceunderweightxy);
	transactedMembers.Add(&buyrule.priceoverweightxx);
	transactedMembers.Add(&buyrule.priceunderweightxx);
	transactedMembers.Add(&sellrule.limweight);
	transactedMembers.Add(&sellrule.priceoverweightxy);	
	transactedMembers.Add(&sellrule.priceunderweightxy);
	transactedMembers.Add(&sellrule.priceoverweightxx);
	transactedMembers.Add(&sellrule.priceunderweightxx);

	updatebleMembers.Add(new SQLColumnUpdate("BUYLIMWEIGHT",			new SQLIntegerDataMember(&buyrule.limweight))); 
	updatebleMembers.Add(new SQLColumnUpdate("BUYPRICEOVERWEIGHTXY",	new SQLMoneyDataMember(&buyrule.priceoverweightxy))); 
	updatebleMembers.Add(new SQLColumnUpdate("BUYPRICEUNDERWEIGHTXY",	new SQLMoneyDataMember(&buyrule.priceunderweightxy))); 
	updatebleMembers.Add(new SQLColumnUpdate("BUYPRICEOVERWEIGHTXX",	new SQLMoneyDataMember(&buyrule.priceoverweightxx))); 
	updatebleMembers.Add(new SQLColumnUpdate("BUYPRICEUNDERWEIGHTXX",	new SQLMoneyDataMember(&buyrule.priceunderweightxx))); 
	updatebleMembers.Add(new SQLColumnUpdate("SELLLIMWEIGHT",			new SQLIntegerDataMember(&sellrule.limweight))); 
	updatebleMembers.Add(new SQLColumnUpdate("SELLPRICEOVERWEIGHTXY",	new SQLMoneyDataMember(&sellrule.priceoverweightxy))); 
	updatebleMembers.Add(new SQLColumnUpdate("SELLPRICEUNDERWEIGHTXY",	new SQLMoneyDataMember(&sellrule.priceunderweightxy))); 
	updatebleMembers.Add(new SQLColumnUpdate("SELLPRICEOVERWEIGHTXX",	new SQLMoneyDataMember(&sellrule.priceoverweightxx))); 
	updatebleMembers.Add(new SQLColumnUpdate("SELLPRICEUNDERWEIGHTXX",	new SQLMoneyDataMember(&sellrule.priceunderweightxx))); 
	updatebleMembers.Add(new SQLColumnUpdate("NOSEXVALUE",				new SQLCowSexDataMember(&nosexvalue))); 
	updatebleMembers.Add(new SQLColumnUpdate("RULENAME",				new SQLStringDataMember(&rulename))); 
}
Rule::~Rule()
{
	updatebleMembers.FreeAndFlush();
}

void Rule::CopyFrom(const Rule& copy)
{
	ruleid = copy.ruleid;
	rulename.CopyFrom(copy.rulename);	
	buyrule.CopyFrom(copy.buyrule);
	sellrule.CopyFrom(copy.sellrule);
	nosexvalue.CopyFrom(copy.nosexvalue);
}

	
RulePart *Rule::GetRulePart(BuyOrSell buy_sell)
{
	switch(buy_sell)
	{
	case BUY:
		return &buyrule;
	case SELL:
		return &sellrule;
	}
	return NULL;
}
	

bool Rule::InsertThisRule(bool bCommit/* = true*/)
{
	SQLQuery q;

	const char QueryStr[] =  "INSERT INTO rules(rulename,"
							 "buylimweight,buypriceoverweightxy,buypriceunderweightxy,buypriceoverweightxx,buypriceunderweightxx,"
							 "selllimweight,sellpriceoverweightxy,sellpriceunderweightxy,sellpriceoverweightxx,sellpriceunderweightxx,nosexvalue)"
							 " VALUES (%0q,%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11);";
	q << QueryStr;
	q.parse();
		
	SQLQueryParms p;

	p << SQLString(GetRuleName())
		<< SQLString(buyrule.GetLimWeight())
		<< SQLString(buyrule.GetPriceOverWeightXY())
		<< SQLString(buyrule.GetPriceUnderWeightXY())
		<< SQLString(buyrule.GetPriceOverWeightXX())
		<< SQLString(buyrule.GetPriceUnderWeightXX())
	  << SQLString(sellrule.GetLimWeight())
	  << SQLString(sellrule.GetPriceOverWeightXY())
	  << SQLString(sellrule.GetPriceUnderWeightXY())
	  << SQLString(sellrule.GetPriceOverWeightXX())
	  << SQLString(sellrule.GetPriceUnderWeightXX())
	  << SQLString(CowSex(GetNoSexValue())); 
  
	LOG("InsertThisRule() query: %s",q.str(p).c_str());
	if(!SQLDB.Insert(q.str(p).c_str()))
	{
		LOG("InsertThisRule() error: %s",(LPCSTR)SQLDB.GetLastError());
		return false;
	}

	if((ruleid = SQLDB.SelectScalar("SELECT MAX(ruleid) AS maxruleid FROM rules",1)) < 0)
	{
		SQLDB.Rollback();
		return false;
	}	

	if(bCommit) 
		SQLDB.Commit();
	return true;

	
}
bool Rule::DeleteThisRule(bool bCommit/* = true*/)
{
	SQLQuery q;
	q <<  "DELETE FROM rules WHERE ruleid  = %0";
	q.parse();
	
	LOG("DeleteThisRule() stmt: %s",q.str(GetId()).c_str());
	if(SQLDB.Delete(q.str(GetId()).c_str(),bCommit))
	{
		LOG("DeleteThisRule() error: %s",(LPCSTR)SQLDB.GetLastError());
		ruleid = 0;
		return true;
	}
	return false;

}
bool Rule::UpdateThisRule(bool bCommit/*= true*/)
{
	if(IsUncommited())
	{
		SQLQuery q;
		q << "UPDATE rules SET "; 
		
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
		
		q << " WHERE ruleid = " << GetId();

		LOG("UpdateThisRule() stmt %s",q.str().c_str());

		if(!SQLDB.Update(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisRule() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

	}
	return true;
}

UINT	 Rule::GetId() const
{
	return ruleid;
}

CString	 Rule::GetRuleName() const
{
	return rulename.GetValue();
}

CowSex	 Rule::GetNoSexValue() const
{
	return nosexvalue.GetValue();
}


void Rule::SetRuleName(const CString& sRuleName)
{
	rulename.SetTo(sRuleName);
}

void Rule::SetNoSexValue(const CowSex& cowSex)
{
	nosexvalue.SetTo(cowSex.getCOWSEX());
}


void Rule::ChangeRuleName(const CString& sRuleName)
{
	rulename.ChangeTo(sRuleName);
}

void Rule::ChangeNoSexValue(const CowSex& cowSex)
{
	nosexvalue.ChangeTo(cowSex.getCOWSEX());
}

void Rule::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();

}
void Rule::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}
bool Rule::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}

