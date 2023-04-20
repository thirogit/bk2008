#include "stdafx.h"
#include "OutDoc.h"
#include "../CattleSQL.h"

OutDoc::OutDoc(int docId) : Doc(docId)
{
}

bool OutDoc::InsertThisDoc(bool bCommit/* = true*/)
{
	if(Doc::InsertThisDoc(bCommit))
	{
		SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::ADDED);
		return true;
	}
	return false;
}

bool OutDoc::DeleteThisDoc(bool bCommit/* = true*/)
{
	if(Doc::DeleteThisDoc(bCommit))
	{
		SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::DELETED);
		return true;
	}
	return false;
}

CString OutDoc::GetDocPrefix() const
{
	return "WZ";
}
CString OutDoc::GetTableName() const
{
	return "outdocs";
}
