#ifndef __SQLCOLUMNUPDATE_H__
#define __SQLCOLUMNUPDATE_H__

#include "SQLValue.h"

class SQLColumnUpdate
{
public:
	SQLColumnUpdate(const CString& columnName,SQLValue *pValue,bool bDelete = true);
	~SQLColumnUpdate();
	CString ToEqualString();
	bool	NeedsUpdate();
protected:
	CString		m_ColumnName;
	SQLValue*	m_pValue;
	bool		m_Delete;
};



#endif