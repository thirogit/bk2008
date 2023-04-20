#include "stdafx.h"
#include "SQLColumnUpdate.h"



SQLColumnUpdate::SQLColumnUpdate(const CString& columnName,SQLValue *pValue,bool bDelete/* = true*/) :
	m_ColumnName(columnName),m_pValue(pValue),m_Delete(bDelete)	
{

}
SQLColumnUpdate::~SQLColumnUpdate()
{
	if(m_Delete)
		delete m_pValue;
}

CString SQLColumnUpdate::ToEqualString()
{
	return m_ColumnName + " = " + m_pValue->ToString();
}

bool SQLColumnUpdate::NeedsUpdate()
{
	return m_pValue->IsChanged();
}