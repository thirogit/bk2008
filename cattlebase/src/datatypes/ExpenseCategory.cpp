#include "stdafx.h"
#include "ExpenseCategory.h"
#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "sqlupdate/sqldatamembers/SQLFloatDataMember.h"
#include "sqlupdate/sqldatamembers/SQLVATRateDataMember.h"

#include "log.h"
#include "CattleSQL.h"
#include "consts.h"


ExpenseCategory::ExpenseCategory(UINT Id) : expensecategoryid(Id)
{

	transactedMembers.Add(&expensecategoryname);	
	transactedMembers.Add(&expensecategorydesc);
	transactedMembers.Add(&defaultvatrate);
	
	updatebleMembers.Add(new SQLColumnUpdate("EXPENSECATEGORYNAME",	new SQLStringDataMember(&expensecategoryname))); 
	updatebleMembers.Add(new SQLColumnUpdate("EXPENSECATEGORYDESC", new SQLStringDataMember(&expensecategorydesc))); 
	updatebleMembers.Add(new SQLColumnUpdate("DEFAULTVATRATE",		new SQLVATRateDataMember(&defaultvatrate,6))); 
	
	
}

void ExpenseCategory::CopyFrom(const ExpenseCategory &copy)
{
	expensecategoryname.CopyFrom(copy.expensecategoryname);
	expensecategoryid = copy.expensecategoryid;
	expensecategorydesc.CopyFrom(copy.expensecategorydesc);
	defaultvatrate.CopyFrom(copy.defaultvatrate);
}

bool ExpenseCategory::InsertThisExpenseCategory(bool bCommit/* = true*/)
{
	SQLQuery q;
		
	const char QueryStr[] =  "INSERT INTO expensecategory(expensecategoryname,expensecategorydesc,defaultvatrate) VALUES (%0q,%1n,%2);";
	q << QueryStr;
	q.parse();
	
  
	LOG("InsertThisExpenseCategory() insert: %s",q.str((LPCSTR)expensecategoryname.GetValue(),
													   (LPCSTR)expensecategorydesc.GetValue(),
													   defaultvatrate.GetValue()).c_str());
	if(!SQLDB.Insert(q.str((LPCSTR)expensecategoryname.GetValue(),
							(LPCSTR)expensecategorydesc.GetValue(),
							defaultvatrate.GetValue()).c_str())) 
		{
			LOG("InsertThisExpenseCategory() error: %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}


		if((expensecategoryid = SQLDB.SelectScalar("SELECT MAX(expensecategoryid) FROM expensecategory",1)) < 0)
		{		
			SQLDB.Rollback();
			expensecategoryid = 0;
			return false;
		}		

		if(bCommit) SQLDB.Commit();
		return true;
}
bool ExpenseCategory::DeleteThisExpenseCategory(bool bCommit/* = true*/)
{
	SQLQuery q;
	q <<  "DELETE FROM expensecategory WHERE expensecategoryid  = %0";
	q.parse();
		
	LOG("DeleteThisExpenseCategory() stmt: %s",q.str(expensecategoryid).c_str());

	if(SQLDB.Delete(q.str(expensecategoryid).c_str(),bCommit))
	{
		expensecategoryid = 0;		
		return true;
	}
	LOG("DeleteThisExpenseCategory() error: %s",(LPCSTR)SQLDB.GetLastError());
	return false;
}
bool ExpenseCategory::UpdateThisExpenseCategory(bool bCommit/* = true*/)
{
	if(IsUncommited())
	{
		SQLQuery q;
		q << "UPDATE expensecategory SET "; 

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
		
		q << " WHERE expensecategoryid = " << GetId();

		LOG("UpdateThisExpenseCategory() stmt %s",q.str().c_str());

		if(!SQLDB.Update(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisExpenseCategory() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

	}
	return true;	
}

UINT ExpenseCategory::GetId() const
{
	return expensecategoryid;
}


CString ExpenseCategory::GetExpenseCategoryName() const
{
	return expensecategoryname.GetValue();
}

CString ExpenseCategory::GetExpenseCategoryDesc() const
{
	return expensecategorydesc.GetValue();
}
	
void ExpenseCategory::SetExpenseCategoryName(const CString& sCategoryName)
{
	expensecategoryname.SetTo(sCategoryName);
}

void ExpenseCategory::SetExpenseCategoryDesc(const CString& sCategoryDesc)
{
	expensecategorydesc.SetTo(sCategoryDesc);
}

void ExpenseCategory::ChangeExpenseCategoryName(const CString& sCategoryName)
{
	expensecategoryname.ChangeTo(sCategoryName);
}

void ExpenseCategory::ChangeExpenseCategoryDesc(const CString& sCategoryDesc)
{
	expensecategorydesc.ChangeTo(sCategoryDesc);
}
VATRate  ExpenseCategory::GetDefaultVATRate() const
{
	return defaultvatrate.GetValue();
}
void ExpenseCategory::SetDefaultVATRate(const VATRate& defaultVATRate)
{
	defaultvatrate.SetTo(defaultVATRate);
}
void ExpenseCategory::ChangeDefaultVATRate(const VATRate& defaultVATRate)
{
	defaultvatrate.ChangeTo(defaultVATRate);
}

bool ExpenseCategory::HasDefaultVATRate() const
{
	return !GetDefaultVATRate().IsNull();
}

ExpenseCategory::~ExpenseCategory()
{
	updatebleMembers.FreeAndFlush();
}

void ExpenseCategory::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();

}
void ExpenseCategory::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}
bool ExpenseCategory::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}