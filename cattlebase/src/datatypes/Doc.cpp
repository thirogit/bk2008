#include "stdafx.h"
#include "Doc.h"
#include "VATInvoice.h"
#include "../exceptions/SQLException.h"
#include "log.h"
#include "CattleSQL.h"
#include "consts.h"
#include  "../query/equal_list.h"
#include "../Utils/StringFormat.h"
#include "sqlupdate/sqldatamembers/SQLHentDataMember.h"
#include "sqlupdate/sqldatamembers/SQLDateTimeDataMember.h"
#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "sqlupdate/sqldatamembers/SQLInvoiceDataMember.h"
#include "sqlupdate/sqldatamembers/SQLReasonDataMember.h"
#include "sqlupdate/sqldatamembers/SQLTimeDataMember.h"
#include "sqlupdate/sqldatamembers/SQLAgentDataMember.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Doc::Doc(int docId) : docid(docId),hent(NULL),reason(NULL),invoice(NULL),agent(NULL)
{
	transactedMembers.Add(&hent);	
	transactedMembers.Add(&docdate);
	transactedMembers.Add(&loaddate);
	transactedMembers.Add(&extras);	
	transactedMembers.Add(&plateno);
	transactedMembers.Add(&reason);
	transactedMembers.Add(&invoice);
	transactedMembers.Add(&loadstart);
	transactedMembers.Add(&loadend);
	transactedMembers.Add(&agent);

	
	updatebleMembers.Add(new SQLColumnUpdate("HENT",			new SQLHentDataMember(&hent)));
	updatebleMembers.Add(new SQLColumnUpdate("DOCDATE",			new SQLDateTimeDataMember(&docdate,true))); 
	updatebleMembers.Add(new SQLColumnUpdate("LOADDATE",		new SQLDateTimeDataMember(&loaddate,true)));
	updatebleMembers.Add(new SQLColumnUpdate("EXTRAS",			new SQLStringDataMember(&extras))); 
	updatebleMembers.Add(new SQLColumnUpdate("PLATENO",			new SQLStringDataMember(&plateno))); 
	updatebleMembers.Add(new SQLColumnUpdate("REASON",			new SQLReasonDataMember(&reason))); 
	updatebleMembers.Add(new SQLColumnUpdate("INVOICE",			new SQLInvoiceDataMember<VATInvoiceDataMember>(&invoice))); 
	updatebleMembers.Add(new SQLColumnUpdate("LOADSTRTM",		new SQLTimeDataMember(&loadstart))); 
	updatebleMembers.Add(new SQLColumnUpdate("LOADENDTM",		new SQLTimeDataMember(&loadend))); 
	updatebleMembers.Add(new SQLColumnUpdate("AGENT",			new SQLAgentDataMember(&agent))); 

}

Doc::~Doc()
{
	updatebleMembers.FreeAndFlush();
}

void Doc::CopyFrom(const Doc &copy)
{
	RawData::Copy((RawData&)copy);
	docid = copy.docid;
	hent.CopyFrom(copy.hent);
	docdate.CopyFrom(copy.docdate);
	loaddate.CopyFrom(copy.loaddate);
	extras.CopyFrom(copy.extras);
	plateno.CopyFrom(copy.plateno);
	reason.CopyFrom(copy.reason);
	invoice.CopyFrom(copy.invoice);
	loadstart.CopyFrom(copy.loadstart);
	loadend.CopyFrom(copy.loadend);
	agent.CopyFrom(copy.agent);
}

int	Doc::GetId() const
{
	return docid;
}

void Doc::ResetId()
{
	docid = NULL_DOCID;
}

CString Doc::GetDocNoStr() const
{
	return CStringFormat("%s%04d",(LPCSTR)GetDocPrefix(),GetId());
}

bool Doc::IsNew() const
{
	return GetId() == NULL_DOCID;
}

Hent* Doc::GetHent() const
{
	return hent.GetValue();
}

void Doc::SetHent(Hent* pHent)
{
	hent.SetTo(pHent);
}

void Doc::ChangeHent(Hent* pHent)
{
	hent.ChangeTo(pHent);
}

CDateTime Doc::GetDocDate() const
{
	return docdate.GetValue();
}

void Doc::SetDocDate(const CDateTime& docDt)
{
	docdate.SetTo(docDt);
}

void Doc::ChangeDocDate(const CDateTime& docDt)
{
	docdate.ChangeTo(docDt);
}

CDateTime Doc::GetLoadDate() const
{
	return loaddate.GetValue();
}

void Doc::SetLoadDate(const CDateTime& loadDt)
{
	loaddate.SetTo(loadDt);
}

void Doc::ChangeLoadDate(const CDateTime& loadDt)
{
	loaddate.ChangeTo(loadDt);
}
	
CString Doc::GetExtras() const
{
	return extras.GetValue();
}

void Doc::SetExtras(const CString& sExtras)
{
	extras.SetTo(sExtras);
}

void Doc::ChangeExtras(const CString& sExtras)
{
	extras.ChangeTo(sExtras);
}

CString Doc::GetPlateNo() const
{
	return plateno.GetValue();
}

void Doc::SetPlateNo(const CString& plateNo)
{
	plateno.SetTo(plateNo);
}

void Doc::ChangePlateNo(const CString& plateNo)
{
	plateno.ChangeTo(plateNo);
}

Reason* Doc::GetReason() const
{
	return reason.GetValue();
}

void Doc::SetReason(Reason* pReason)
{
	reason.SetTo(pReason);
}

void Doc::ChangeReason(Reason* pReason)
{
	reason.ChangeTo(pReason);
}


Time Doc::GetLoadStartTime() const
{
	return loadstart.GetValue();
}
void Doc::SetLoadStartTime(const Time& loadStrTm)
{
	loadstart.SetTo(loadStrTm);
}
void Doc::ChangeLoadStartTime(const Time& loadStrTm)
{
	loadstart.ChangeTo(loadStrTm);
}
	
Time Doc::GetLoadEndTime() const
{
	return loadend.GetValue();
}
void Doc::SetLoadEndTime(const Time& loadEndTm)
{
	loadend.SetTo(loadEndTm);
}
void Doc::ChangeLoadEndTime(const Time& loadEndTm)
{
	loadend.ChangeTo(loadEndTm);
}

VATInvoice* Doc::GetInvoice() const
{
	return invoice.GetValue();
}

void Doc::SetInvoice(VATInvoice* pInvoice)
{
	invoice.SetTo(pInvoice);
}

void Doc::AssignToInvoice(VATInvoice* pInvoice)
{
	invoice.ChangeTo(pInvoice);
}

void Doc::DeleteInvoice()
{
	invoice.ChangeTo(NULL);
}

bool Doc::HasInvoice() const
{
	return GetInvoice() != NULL;
}

UINT Doc::GetCowCount() const
{
	return cows.GetSize();
}

CowArrayPtr Doc::GetCows()
{
	return CowArrayPtr(&cows);
}

bool Doc::InsertThisDoc(bool bCommit/* = true*/)
{
	char QueryStr[] = "INSERT INTO %0(hent,docdate,extras,plateno,loaddate,creattime,creator,lmodiftime,lastmodifier,reason,loadstrtm,loadendtm,agent)"
				      " VALUES(%1,%2q,%3q,%4q,%5q,%6q,%7,%8q,%9,%10,%11,%12,%13);";
	

	if(!IsNew()) 
	{
		SQLDB.SetLastError("This has non zero id, maybe it had been already inserted.");
		return false; 
	}
	
	SQLQuery q;
	q << QueryStr;
	q.parse();

	SQLDB.SetCurrentRawData(*this);


	SQLQueryParms p;

	p  << (LPCSTR)GetTableName()
	   << hent.GetValue()->GetId()
	   << SQLString(docdate.GetValue(),true)
	   << (LPCSTR)extras.GetValue()
	   << (LPCSTR)plateno.GetValue()
	   << SQLString(loaddate.GetValue(),true)
	   << creation.eventDt
	   << creation.user->userid
	   << modification.eventDt
	   << modification.user->userid
	   << reason.GetValue()->GetId()
	   << loadstart.GetValue()
	   << loadend.GetValue();


	Agent* pAgent = agent.GetValue();

	if(pAgent != NULL)
	{
		p << pAgent->GetId();
	}
	else
	{
		p << "NULL";
	}
	   

	try
	{
		LOG("InsertThisDoc() query: %s",q.str(p).c_str());

		if(!SQLDB.Insert(q.str(p).c_str()))
		{
			LOG("InsertThisDoc() error: %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}
		
		q.reset();
		q << "SELECT MAX(docid) FROM " << (LPCSTR)GetTableName(); 
		docid = SQLDB.SelectScalar(q.str().c_str(),1,-1);
		if(docid < 0) return false;
		
	}
	catch(CSQLException *er)
	{
		SQLDB.Rollback();
		SQLDB.SetLastError(er->GetError());
		er->Delete();
		return false;
	}
	if(bCommit) return SQLDB.Commit();
	return true;
}

bool Doc::DeleteThisDoc(bool bCommit/* = true*/)
{
	SQLQuery q;
    q << "DELETE FROM %0 WHERE docid = %1";
	q.parse();
	
	LOG("DeleteThisDoc() stmt: %s",q.str((LPCSTR)GetTableName(),GetId()).c_str());

	if(SQLDB.Delete(q.str((LPCSTR)GetTableName(),GetId()).c_str(),bCommit))
	{
		ResetId();		
		return true;
	}
	LOG("DeleteThisDoc() error: %s",(LPCSTR)SQLDB.GetLastError());
	return false;

	
}

bool Doc::CanDeleteThisDoc() const
{
	return !HasInvoice() && CAN_DO(CANDELETE,this);	
}

bool Doc::CanEditThisDoc() const
{
	return CAN_DO(CANEDIT,this);	
}

void Doc::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();
}

void Doc::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}

bool Doc::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}




bool Doc::UpdateThisDoc(bool bCommit/* = true*/)
{

	if(IsUncommited())
	{
		SQLQuery q;
		q << "UPDATE " << GetTableName() << " SET "; 

		UpatableDataMembers::ArrayIterator it = updatebleMembers.iterator();
		while(it.hasNext())
		{
			SQLColumnUpdate* pColumn = *(it++);
			if(pColumn->NeedsUpdate())
			{
				q << pColumn->ToEqualString();
				q << ", ";
			}
		}

		
		modification.eventDt = CDateTime::now();
		modification.user = UserData::GetCurrentUser();

		q << "LASTMODIFIER = " << modification.user->userid << ',';
		q << "LMODIFTIME = '" << modification.eventDt.GetDateInFmt(CDateTime::YYYYdotMMdotDD_HHcolonMMcolonSS) << '\'';

		q << " WHERE docid = " << GetId();

		LOG("UpdateThisDoc() stmt %s",q.str().c_str());

		if(!SQLDB.Update(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisDoc() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

	}
	return true;
}

bool Doc::CanMakeVATInvoiceOnThisDoc() const
{
	return !HasInvoice();
}


Agent* Doc::GetAgent() const
{
	return agent.GetValue();
}
void Doc::SetAgent(Agent* pAgent)
{
	agent.SetTo(pAgent);
}
void Doc::ChangeAgent(Agent* pAgent)
{
	agent.ChangeTo(pAgent);
}