#include "stdafx.h"
#include "Invoice.h"

#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "sqlupdate/sqldatamembers/SQLMoneyDataMember.h"
#include "sqlupdate/sqldatamembers/SQLFloatDataMember.h"
#include "sqlupdate/sqldatamembers/SQLDateTimeDataMember.h"
#include "sqlupdate/sqldatamembers/SQLIntegerDataMember.h"
#include "sqlupdate/sqldatamembers/SQLPayWayDataMember.h"

#include "log.h"
#include "CattleSQL.h"
//#include "consts.h"

Invoice::Invoice(UINT Id/* = NULL_INVOICEID*/) : invoiceid(Id),invtype(IN_INV_VAT),
					VATrate(0.0),payway(PAY_CASH),correct(NULL),correctfor(NULL),invoiceno(0),payduedays(0)
{

	transactedMembers.Add(&extras);	
	transactedMembers.Add(&invoicedate);
	transactedMembers.Add(&paiddate);
	transactedMembers.Add(&invoicevalue);	
	transactedMembers.Add(&customnumber);
	transactedMembers.Add(&VATrate);

	transactedMembers.Add(&payduedays);
	transactedMembers.Add(&payway);
	transactedMembers.Add(&transactiondate);
	transactedMembers.Add(&transactionplace);
	
	updatebleMembers.Add(new SQLColumnUpdate("EXTRAS",			new SQLStringDataMember(&extras))); 
	updatebleMembers.Add(new SQLColumnUpdate("INVOICEDATE",		new SQLDateTimeDataMember(&invoicedate,true)));
	updatebleMembers.Add(new SQLColumnUpdate("PAID",			new SQLDateTimeDataMember(&paiddate,true)));
	updatebleMembers.Add(new SQLColumnUpdate("INVOICEVALUE",	new SQLMoneyDataMember(&invoicevalue))); 
	updatebleMembers.Add(new SQLColumnUpdate("CUSTOMNUMBER",	new SQLStringDataMember(&customnumber)));
	updatebleMembers.Add(new SQLColumnUpdate("VATRATE",			new SQLFloatDataMember(&VATrate,5)));
	updatebleMembers.Add(new SQLColumnUpdate("PAYDUEDAYS",		new SQLIntegerDataMember(&payduedays)));
	updatebleMembers.Add(new SQLColumnUpdate("PAYWAY",			new SQLPayWayDataMember(&payway)));
	updatebleMembers.Add(new SQLColumnUpdate("TRANSACTIONPLACE",new SQLStringDataMember(&transactionplace)));
	updatebleMembers.Add(new SQLColumnUpdate("TRANSACTIONDATE",	new SQLDateTimeDataMember(&transactiondate,true)));

	
}

Invoice::~Invoice()
{
	invEntries.FreeAndFlush();
	updatebleMembers.FreeAndFlush();
}

void Invoice::CopyFrom(const Invoice& src)
{
	RawData::Copy(src);

	invoiceno = src.invoiceno;
	invoiceid = src.invoiceid;
	invtype = src.invtype;
	correct = src.correct; 
	correctfor = src.correctfor;
	extras.CopyFrom(src.extras);
	invoicedate.CopyFrom(src.invoicedate);
	paiddate.CopyFrom(src.paiddate);
	invoicevalue.CopyFrom(src.invoicevalue);
	VATrate.CopyFrom(src.VATrate);
	payway.CopyFrom(src.payway);
	customnumber.CopyFrom(src.customnumber);
	payduedays.CopyFrom(src.payduedays);
	transactionplace.CopyFrom(src.transactionplace);
	transactiondate.CopyFrom(src.transactiondate);
}

	
bool Invoice::IsPaid() const
{
	return !GetPaidDate().IsNull();
}

bool Invoice::CanEditThisInvoice() const
{
	return correct == NULL && !IsPaid();
}
bool Invoice::CanDeleteThisInvoice() const
{
	return  correct == NULL && CAN_DO(CANDELETE,this) && !IsPaid();
}

bool Invoice::InsertThisInvoice(bool bCommit/*=true*/)
{

	SQLQuery q;

	const char insStmtStr[] = "INSERT INTO invoices(invoicedate,paid,VATrate,invoicevalue,"
							  "correct,invtype,payway,extras,creator,creattime,lastmodifier,"
							  "lmodiftime,customnumber,invoiceno,payduedays,transactionplace,transactiondate) "
							  "VALUES(%0n,%1n,%2,%3,%4,%5,%6q,%7n,%8,%9q,%10,%11q,%12n,%13,%14,%15n,%16n);";

	const char nextInvoiceNoQuery[] = "SELECT MAX(invoiceno) FROM invoices WHERE invtype = %0 OR invtype = %1";

	int nextinvno = 0;
	
	q << nextInvoiceNoQuery;
	q.parse();
	if((nextinvno = SQLDB.SelectScalar(q.str(GetRealInvoiceType(),CORRCT_INV).c_str(),0)) < 0)
	{
		return false;
	}
	nextinvno++;


	q.reset();

	q << insStmtStr;
	q.parse();
		
	SQLQueryParms p;
	char payWayStr[2] = {(char)payway.GetValue(),0};



	SQLDB.SetCurrentRawData(*this);


	p  << SQLString(invoicedate.GetValue(),true)
		<< SQLString(paiddate.GetValue(),true)
		<< SQLString(VATrate.GetValue())
		<< SQLString(invoicevalue.GetValue());

		if(invtype == CORRCT_INV)
			p << SQLString(correctfor->GetId());
		else
			p << SQLStringNull();

		 p  << SQLString(int(invtype))
		  << SQLString(payWayStr)
		  << SQLString(extras.GetValue())
		  << SQLString(creation.user->userid)
		  << SQLString(creation.eventDt)
		  << SQLString(modification.user->userid)
		  << SQLString(modification.eventDt)
		  << SQLString(customnumber.GetValue())
		  << SQLString(nextinvno)
		  << SQLString(payduedays.GetValue())
		  << SQLString(transactionplace.GetValue())
		  << SQLString(transactiondate.GetValue(),true);
	  
		LOG("InsertThisInvoice() query: %s",q.str(p).c_str());

		if(!SQLDB.Insert(q.str(p).c_str()))
		{
			LOG("InsertThisInvoice() error: %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

		if((invoiceid = SQLDB.SelectScalar("SELECT MAX(invoiceid) FROM invoices",1)) < 0)
		{
			SQLDB.Rollback();
			return false;
		}	

		invoiceno = nextinvno;

		SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::ADDED);

		if(bCommit) SQLDB.Commit();

		return true;
}
bool Invoice::DeleteThisInvoice(bool bCommit/*=true*/)
{

	SQLQuery q;
    q << "DELETE FROM invoices WHERE invoiceid = " << GetId();
	
	LOG("DeleteThisInvoice() stmt: %s",q.str().c_str());
	
	SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::DELETED);

	if(!SQLDB.Delete(q.str().c_str(),bCommit))
	{
		LOG("DeleteThisInvoice() error: %s",(LPCSTR)SQLDB.GetLastError());		
		return false;
	}
	ResetId();		
	return true;
}


Money Invoice::GetBruttoValueAfterCorrect() const
{
	Invoice* pNonConstInvoice  = const_cast<Invoice*>(this);
	return pNonConstInvoice->GetLastGoodInvoice()->GetInvoiceGrossValue();	
}

Invoice* Invoice::GetFirstInvoice()
{
	Invoice *pInv = this;
	while(pInv->invtype == CORRCT_INV)
		pInv = pInv->correctfor;
	return pInv;
}
InvoiceTypes Invoice::GetRealInvoiceType() const
{
	Invoice* pNonConstInvoice = const_cast<Invoice*>(this);
	return pNonConstInvoice->GetFirstInvoice()->GetThisInvoiceType();
}

bool Invoice::UpdateThisInvoice(bool bCommit/*=true*/)
{

	if(IsUncommited())
	{
		SQLQuery q;
		q << "UPDATE invoices SET "; 

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

		q << " WHERE invoiceid = " << GetId();

		LOG("UpdateThisInvoice() stmt %s",q.str().c_str());

		if(!SQLDB.Update(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisInvoice() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

	}
	return true;
}

Invoice* Invoice::GetLastGoodInvoice()
{
	Invoice* InvoiceChain = this;
	while(InvoiceChain->correct) InvoiceChain = InvoiceChain->correct;
	return InvoiceChain;
}

CDateTime Invoice::GetPayDueDate() const
{
	int payDueDays = payduedays.GetValue();

	CDateTime paydue;
	paydue.MakeNull();	
	
	paydue.SetDateTime(invoicedate.GetValue().SecondsSinceEpoh() + 
		(payDueDays > 0 ? (payDueDays-1) : payDueDays)*SECONDS_IN_DAY);
	
	return paydue;
}
int Invoice::GetCowQty() const
{
	return invEntries.GetCount();
}

void Invoice::GetInvoiceCows(CCowArray &cows) const
{
	cows.RemoveAll();
	for(int i = 0;i < invEntries.GetSize();i++)
		cows.Add(invEntries[i]->GetCow());
}

CowInvoiceEntry* Invoice::GetEntryForCow(const Cow* pCow)
{
	CowInvoiceEntry* pEntry = NULL;
	CCowInvoiceEntryArray::ArrayIterator it = invEntries.iterator();
	while(it.hasNext())
	{
		pEntry = *(it++);
		if(pEntry->GetCow() == pCow)
			return pEntry;
	}
	return NULL;
}



UINT Invoice::GetId() const
{
	return invoiceid;
}

void Invoice::ResetId()
{
	invoiceid = NULL_INVOICEID;
}

UINT Invoice::GetInvoiceNo() const
{
	return invoiceno;
}

void Invoice::SetInvoiceNo(UINT uiInvoiceNo)
{
	invoiceno = uiInvoiceNo;
}

void Invoice::SetCorrect(Invoice* pCorrect)
{
	correct = pCorrect;
}

Invoice* Invoice::GetCorrect() const
{
	return correct;
}

bool Invoice::HasCorrect() const
{
	return correct != NULL;
}

void Invoice::SetCorrectFor(Invoice* pCorrectFor)
{
	correctfor = pCorrectFor;
}

Invoice* Invoice::GetCorrectFor() const
{
	return correctfor;
}

void Invoice::SetInvoiceType(InvoiceTypes invoiceType)
{
	invtype = invoiceType;
}
InvoiceTypes Invoice::GetThisInvoiceType() const
{
	return invtype;
}


CString Invoice::GetExtras() const
{
	return extras.GetValue();
}

void Invoice::SetExtras(const CString& sExtras)
{
	extras.SetTo(sExtras);
}

void Invoice::ChangeExtras(const CString& sExtras)
{
	extras.ChangeTo(sExtras);
}

CDateTime Invoice::GetInvoiceDate() const
{
	return invoicedate.GetValue();
}

void Invoice::SetInvoiceDate(const CDateTime& invoiceDt)
{
	invoicedate.SetTo(invoiceDt);
}

void Invoice::ChangeInvoiceDate(const CDateTime& invoiceDt)
{
	invoicedate.ChangeTo(invoiceDt);
}

CDateTime Invoice::GetPaidDate() const
{
	return paiddate.GetValue();
}

void Invoice::SetPaidDate(const CDateTime& paidDt)
{
	paiddate.SetTo(paidDt);
}

void Invoice::ChangePaidDate(const CDateTime& paidDt)
{
	paiddate.ChangeTo(paidDt);
}

Money Invoice::GetInvoiceGrossValue() const
{
	Money netValue = GetInvoiceNetValue();
	return netValue + netValue*GetVATRate();
}

Money Invoice::GetInvoiceNetValue() const
{
	return invoicevalue.GetValue();
}

void Invoice::SetInvoiceNetValue(const Money& netValue)
{
	invoicevalue.SetTo(netValue);
}

void Invoice::ChangeInvoiceNetValue(const Money& netValue)
{
	invoicevalue.ChangeTo(netValue);
}

double Invoice::GetVATRate() const
{
	return VATrate.GetValue();
}

void Invoice::SetVATRate(double dVatRate)
{
	VATrate.SetTo(dVatRate);
}

void Invoice::ChangeVATRate(double dVatRate)
{
	VATrate.ChangeTo(dVatRate);
}

Invoice::PAYWAYS Invoice::GetPayWay() const
{
	return payway.GetValue();
}

void Invoice::SetPayWay(Invoice::PAYWAYS aPayWay)
{
	payway.SetTo(aPayWay);
}

void Invoice::ChangePayWay(Invoice::PAYWAYS aPayWay)
{
	payway.ChangeTo(aPayWay);
}

CString Invoice::GetCustomNumber() const
{
	return customnumber.GetValue();
}

void Invoice::SetCustomNumber(const CString& sCustomNumber)
{
	customnumber.SetTo(sCustomNumber);
}

void Invoice::ChangeCustomNumber(const CString& sCustomNumber)
{
	customnumber.ChangeTo(sCustomNumber);
}

int Invoice::GetPayDueDays() const
{
	return payduedays.GetValue();
}

void Invoice::SetPayDueDays(int iPayDueDays)
{
	payduedays.SetTo(iPayDueDays);
}

void Invoice::ChangePayDueDays(int iPayDueDays)
{
	payduedays.ChangeTo(iPayDueDays);
}


CString Invoice::GetTransactionPlace() const
{
	return transactionplace.GetValue();
}

void Invoice::SetTransactionPlace(const CString& sTransactionPlace)
{
	transactionplace.SetTo(sTransactionPlace);
}

void Invoice::ChangeTransactionPlace(const CString& sTransactionPlace)
{
	transactionplace.ChangeTo(sTransactionPlace);
}

CDateTime Invoice::GetTransactionDate() const
{
	return transactiondate.GetValue();
}

void Invoice::SetTransactionDate(const CDateTime& transactionDt)
{
	transactiondate.SetTo(transactionDt);
}

void Invoice::ChangeTransactionDate(const CDateTime& transactionDt)
{
	transactiondate.ChangeTo(transactionDt);
}

void Invoice::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();

}
void Invoice::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}
bool Invoice::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}