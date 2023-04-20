#include "stdafx.h"
#include "InDoc.h"
#include "../CattleSQL.h"

InDoc::InDoc(int docId) : Doc(docId)
{
}

bool InDoc::InsertThisDoc(bool bCommit/* = true*/)
{
	if(Doc::InsertThisDoc(bCommit))
	{
		SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::ADDED);
		return true;
	}
	return false;
}

bool InDoc::DeleteThisDoc(bool bCommit/* = true*/)
{
	if(Doc::DeleteThisDoc(bCommit))
	{
		SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::DELETED);
		return true;
	}
	return false;
}

bool InDoc::CanMakeVATInvoiceOnThisDoc() const
{
	return Doc::CanMakeVATInvoiceOnThisDoc() && HaveNoRRInvs();
}

bool InDoc::HaveNoRRInvs() const
{
	InDoc* thisDoc = const_cast<InDoc*>(this);
	CCowArray::ArrayIterator cowIt = thisDoc->GetCows()->iterator();
	while(cowIt.hasNext())
	{
		if((*(cowIt++))->HasRRBuyInvoice())
			return false;
	}
	return true;
}

CString InDoc::GetDocPrefix() const
{
	return "PZ";
}
CString InDoc::GetTableName() const
{
	return "indocs";
}
