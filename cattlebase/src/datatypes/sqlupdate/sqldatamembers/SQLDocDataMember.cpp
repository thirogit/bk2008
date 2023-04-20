#include "stdafx.h"
#include "SQLDocDataMember.h"
#include "../../Doc.h"

SQLDocDataMember::SQLDocDataMember(DocDataMember* pDocMember) :
		SQLDataMember(pDocMember) 
{
}
 CString SQLDocDataMember::GetSQLStringValue(DocDataMember* pDocMember)
 {
	Doc* pDoc = pDocMember->GetValue();
	if(pDoc)
	{
		return FormatInt(pDoc->GetId());
	}
	else
		return GetSQLNull();
	
}