#include "stdafx.h"
#include "NullDoc.h"

NullDoc::NullDoc(UINT Id/* = NULL_DOCID*/) : Doc(Id)
{
}

CString NullDoc::GetDocPrefix() const
{
	return CString();
}
CString NullDoc::GetTableName() const
{
	return CString();
}

