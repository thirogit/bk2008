#include "stdafx.h"
#include "Expense.h"

#include "log.h"
#include "CattleSQL.h"
#include "consts.h"

#include "../classes/TempStringLoader.h"

#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "sqlupdate/sqldatamembers/SQLMoneyDataMember.h"
#include "sqlupdate/sqldatamembers/SQLDateTimeDataMember.h"
#include "sqlupdate/sqldatamembers/SQLFloatDataMember.h"
#include "sqlupdate/sqldatamembers/SQLExpenseCategoryDataMember.h"
#include "sqlupdate/sqldatamembers/SQLExpenseTypeDataMember.h"

Expense::Expense(UINT Id) : expenseid(Id), grosvalue(ZEROMONEY) , vatrate(0.0), 
							 expensetype(EXPENSE_TYPE_WITH_INVOICE) 

{
	transactedMembers.Add(&grosvalue);	
	transactedMembers.Add(&vatrate);
	transactedMembers.Add(&expensetype);
	transactedMembers.Add(&expensecategory);	
	transactedMembers.Add(&invoiceno);
	transactedMembers.Add(&expensedate);
	transactedMembers.Add(&extras);
	transactedMembers.Add(&expensetitle);
	
	updatebleMembers.Add(new SQLColumnUpdate("GROSVALUE",		new SQLMoneyDataMember(&grosvalue))); 
	updatebleMembers.Add(new SQLColumnUpdate("VATRATE",			new SQLFloatDataMember(&vatrate,6))); 
	updatebleMembers.Add(new SQLColumnUpdate("EXPENSETYPE",		new SQLExpenseTypeDataMember(&expensetype))); 
	updatebleMembers.Add(new SQLColumnUpdate("EXPENSECATEGORY",	new SQLExpenseCategoryDataMember(&expensecategory))); 
	updatebleMembers.Add(new SQLColumnUpdate("INVOICENO",		new SQLStringDataMember(&invoiceno))); 
	updatebleMembers.Add(new SQLColumnUpdate("EXPENSEDATE",		new SQLDateTimeDataMember(&expensedate,true))); 
	updatebleMembers.Add(new SQLColumnUpdate("EXTRAS",			new SQLStringDataMember(&extras))); 
	updatebleMembers.Add(new SQLColumnUpdate("EXPENSETITLE",	new SQLStringDataMember(&expensetitle))); 
}

Expense::~Expense()
{
	updatebleMembers.FreeAndFlush();
}


void Expense::CopyFrom(const Expense& copy)
{
	expenseid = copy.expenseid;
	grosvalue.CopyFrom(copy.grosvalue);
	vatrate.CopyFrom(copy.vatrate);
	expensetype.CopyFrom(copy.expensetype);
	expensecategory.CopyFrom(copy.expensecategory);
	invoiceno.CopyFrom(copy.invoiceno);
	expensedate.CopyFrom(copy.expensedate);
	extras.CopyFrom(copy.extras);
	expensetitle.CopyFrom(copy.expensetitle);
	RawData::Copy(copy);
}


UINT Expense::GetId() const
{
	return expenseid;
}
Money Expense::GetGrosValue() const
{
	return grosvalue.GetValue();
}
double Expense::GetVATRate() const
{
	return vatrate.GetValue();
}
Expense::EXPENSE_TYPE Expense::GetExpenseType() const
{
	return expensetype.GetValue();
}
ExpenseCategoryPtr Expense::GetExpenseCategory() const
{
	return expensecategory.GetValue();
}
CString Expense::GetInvoiceNo() const
{
	return invoiceno.GetValue();
}
CDateTime Expense::GetExpenseDate() const
{
	return expensedate.GetValue();
}
CString Expense::GetExtras() const
{
	return extras.GetValue();
}
CString Expense::GetExpenseTitle() const
{
	return expensetitle.GetValue();
}

void Expense::SetGrosValue(const Money& dGrosValue)
{
	grosvalue.SetTo(dGrosValue);
}
void Expense::SetVATRate(double dVATRate)
{
	vatrate.SetTo(dVATRate);
}
void Expense::SetExpenseType(Expense::EXPENSE_TYPE tExpenseType)
{
	expensetype.SetTo(tExpenseType);
}
void Expense::SetExpenseCategory(ExpenseCategoryPtr ptrExpenseCategory)
{
	expensecategory.SetTo(ptrExpenseCategory);
}
void Expense::SetInvoiceNo(const CString& sInvoiceNo)
{
	invoiceno.SetTo(sInvoiceNo);
}
void Expense::SetExpenseDate(const CDateTime& dtExpenseDate)
{
	expensedate.SetTo(dtExpenseDate);
}
void Expense::SetExtras(const CString& sExtras)
{
	extras.SetTo(sExtras);
}
void Expense::SetExpenseTitle(const CString& sExpenseTitle)
{
	expensetitle.SetTo(sExpenseTitle);
}

void Expense::ChangeGrosValue(const Money& dGrosValue)
{
	grosvalue.ChangeTo(dGrosValue);
}
void Expense::ChangeVATRate(double dVATRate)
{
	vatrate.ChangeTo(dVATRate);
}
void Expense::ChangeExpenseType(Expense::EXPENSE_TYPE tExpenseType)
{
	expensetype.ChangeTo(tExpenseType);
}
void Expense::ChangeExpenseCategory(ExpenseCategoryPtr ptrExpenseCategory)
{
	expensecategory.ChangeTo(ptrExpenseCategory);
}
void Expense::ChangeInvoiceNo(const CString& sInvoiceNo)
{
	invoiceno.ChangeTo(sInvoiceNo);
}
void Expense::ChangeExpenseDate(const CDateTime& dtExpenseDate)
{
	expensedate.ChangeTo(dtExpenseDate);
}
void Expense::ChangeExtras(const CString& sExtras)
{
	extras.ChangeTo(sExtras);
}
void Expense::ChangeExpenseTitle(const CString& sExpenseTitle)
{
	expensetitle.ChangeTo(sExpenseTitle);
}


void Expense::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();

}
void Expense::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}
bool Expense::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}



//***************************************************************************************
bool Expense::InsertThisExpense(bool bCommit/* = true*/)
{
	SQLQuery q;

	const char QueryStr[] =  "INSERT INTO expenses(grosvalue,vatrate,expensetype,expensecategory,invoiceno,expensedate,extras,"
						 "creattime,creator,lmodiftime,lastmodifier,expensetitle)"
						 " VALUES (%0,%1,%2q,%3,%4n,%5n,%6n,%7q,%8,%9q,%10,%11q);";

	char expenseTypeStr[2] = {(char)expensetype.GetValue(),'\0'};

	q << QueryStr;
	q.parse();

	SQLDB.SetCurrentRawData(*this);

	SQLQueryParms p;

	p << SQLString(GetGrosValue()) 
	  << SQLString(GetVATRate())
	  << SQLString(expenseTypeStr)
	  << SQLString(GetExpenseCategory()->GetId())
	  << SQLString(GetInvoiceNo())
	  << SQLString(GetExpenseDate(),true)
	  << SQLString(GetExtras())
	  << SQLString(creation.eventDt)
	  << SQLString(creation.user->userid)
	  << SQLString(modification.eventDt)
	  << SQLString(modification.user->userid)
	  << SQLString(GetExpenseTitle(),true)
	;

	LOG("InsertThisExpense() query: %s",q.str(p).c_str());
	if(!SQLDB.Insert(q.str(p).c_str())) 
	{
		LOG("InsertThisExpense() error: %s",(LPCSTR)SQLDB.GetLastError());
		return false;
	}

	if((expenseid = SQLDB.SelectScalar("SELECT MAX(expenseid) FROM expenses",1)) < 0)
	{		
		SQLDB.Rollback();
		return false;
	}		

	if(bCommit) 
		SQLDB.Commit();
	
	return true;	
}

bool Expense::UpdateThisExpense(bool bCommit/* = true*/)
{
	if(IsUncommited())
	{
		SQLQuery q;
		q << "UPDATE expenses SET ";
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

		q <<  " WHERE expenseid = " << GetId();

		LOG("UpdateThisExpense() stmt %s",q.str().c_str());

		if(!SQLDB.Update(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisExpense() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}
	}
	return true;
	
}
//***************************************************************************************
bool Expense::DeleteThisExpense(bool bCommit/* = true*/)
{
	SQLQuery q;
	q <<  "DELETE FROM expenses WHERE expenseid  = %0";
	q.parse();
		
	LOG("DeleteThisExpense() stmt: %s",q.str(GetId()).c_str());
	if(SQLDB.Delete(q.str(GetId()).c_str(),bCommit))
	{
		expenseid = 0;		
		return true;
	}
	LOG("DeleteThisExpense() error: %s",(LPCSTR)SQLDB.GetLastError());
	return false;
	
}


/////////////////////////////////////////////////////////////////////////////////////////
ExpenseTypeStr::ExpenseTypeStr() : m_bLoaded(false)
{
}
		
CString ExpenseTypeStr::GetExpenseTypeStr(Expense::EXPENSE_TYPE expenseType)
{
	static ExpenseTypeStr m_instance;
	if(!m_instance.m_bLoaded)
	{
		m_instance.m_expenseTypeInvoice = (LPCSTR)(CTempStringLoader(IDS_EXPENSETYPEINVOICE));
		m_instance.m_expenseTypeCash = (LPCSTR)(CTempStringLoader(IDS_EXPENSETYPECASH));		
		m_instance.m_bLoaded = true;
	}
	
	switch(expenseType)
	{
		default:
			return CString();
		case Expense::EXPENSE_TYPE_WITH_INVOICE:
			return m_instance.m_expenseTypeInvoice;
		case Expense::EXPENSE_TYPE_NON_INVOICE:
			return m_instance.m_expenseTypeCash;
	}

}
/////////////////////////////////////////////////////////////////////////////////////////