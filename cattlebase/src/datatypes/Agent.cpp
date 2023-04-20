#include "stdafx.h"
#include "Agent.h"
#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "log.h"
#include "CattleSQL.h"
Agent::Agent(UINT Id/* = NULL_AGENTID*/) : agentId(Id)
{
	transactedMembers.Add(&agentcode);	
	transactedMembers.Add(&agentname);
	transactedMembers.Add(&plate);
	
	updatebleMembers.Add(new SQLColumnUpdate("AGENTCODE",	new SQLStringDataMember(&agentcode))); 
	updatebleMembers.Add(new SQLColumnUpdate("AGENTNAME",	new SQLStringDataMember(&agentname))); 
	updatebleMembers.Add(new SQLColumnUpdate("PLATE",		new SQLStringDataMember(&plate)));
}

Agent::~Agent()
{
	updatebleMembers.FreeAndFlush();
}

void Agent::CopyFrom(const Agent& src)
{
	agentId = src.agentId;
	agentcode.CopyFrom(src.agentcode);
	agentname.CopyFrom(src.agentname);
	plate.CopyFrom(src.plate);
}

UINT Agent::GetId() const
{
	return agentId;
}

CString Agent::GetAgentCode() const
{
	return agentcode.GetValue();
}

CString Agent::GetAgentName() const
{
	return agentname.GetValue();
}

CString Agent::GetPlateNo() const
{
	return plate.GetValue();
}

void Agent::SetAgentCode(const CString& sAgentCode)
{
	agentcode.SetTo(sAgentCode);
}

void Agent::SetAgentName(const CString& sAgentName)
{
	agentname.SetTo(sAgentName);
}

void Agent::SetPlateNo(const CString& sPlateNo)
{
	plate.SetTo(sPlateNo);
}

void Agent::ChangeAgentCode(const CString& sAgentCode)
{
	agentcode.ChangeTo(sAgentCode);
}

void Agent::ChangeAgentName(const CString& sAgentName)
{
	agentname.ChangeTo(sAgentName);
}

void Agent::ChangePlateNo(const CString& sPlateNo)
{
	plate.ChangeTo(sPlateNo);
}

bool Agent::InsertThisAgent(bool bCommit/* = true*/)
{
	SQLQuery q;
		
	const char QueryStr[] =  "INSERT INTO agents(agentname,agentcode,plate) VALUES (%0q,%1q,%2n);";
	q << QueryStr;
	q.parse();
	
  
	LOG("InsertThisAgent() insert: %s",q.str((LPCSTR)agentname.GetValue(),
													   (LPCSTR)agentcode.GetValue(),
													   (LPCSTR)plate.GetValue()).c_str());

		if(!SQLDB.Insert(q.str((LPCSTR)agentname.GetValue(),
							(LPCSTR)agentcode.GetValue(),
							(LPCSTR)plate.GetValue()).c_str())) 
		{
			LOG("InsertThisAgent() error: %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}


		if((agentId = SQLDB.SelectScalar("SELECT MAX(agentid) FROM agents",1)) < 0)
		{		
			SQLDB.Rollback();
			agentId = 0;
			return false;
		}		

		if(bCommit) SQLDB.Commit();
		return true;
}

bool Agent::DeleteThisAgent(bool bCommit/* = true*/)
{
	SQLQuery q;
	q <<  "DELETE FROM agents WHERE agentid  = %0";
	q.parse();
		
	LOG("DeleteThisAgent() stmt: %s",q.str(agentId).c_str());

	if(SQLDB.Delete(q.str(agentId).c_str(),bCommit))
	{
		agentId = 0;		
		return true;
	}
	LOG("DeleteThisAgent() error: %s",(LPCSTR)SQLDB.GetLastError());
	return false;
}

bool Agent::UpdateThisAgent(bool bCommit/* = true*/)
{
	if(IsUncommited())
	{
		SQLQuery q;
		q << "UPDATE agents SET "; 

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
		
		q << " WHERE agentid = " << GetId();

		LOG("UpdateThisAgent() stmt %s",q.str().c_str());

		if(!SQLDB.Update(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisAgent() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

	}
	return true;
}

void Agent::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();

}
void Agent::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}
bool Agent::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}