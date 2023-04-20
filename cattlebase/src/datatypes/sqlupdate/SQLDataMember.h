#ifndef __SQLDATAMEMBER_H__
#define __SQLDATAMEMBER_H__

#include "SQLValue.h"

#define MAX_FLOAT_PREC 15

template<class DATAMEMBERTYPE>
class SQLDataMember : public SQLValue
{
public:
	SQLDataMember(DATAMEMBERTYPE* pDataMember) :	m_pDataMember(pDataMember) {};
	virtual bool IsChanged()
	{
		return m_pDataMember->IsChanged();
	}
	virtual ~SQLDataMember() {};

	virtual CString ToString()
	{
		return GetSQLStringValue(m_pDataMember);
	}
	
	CString GetSQLNull() { return "NULL"; }
	CString FormatInt(int iValue) 
	{
		CString intString;
		intString.Format("%d",iValue);
		return intString;
	}

	CString FormatFloat(double dValue,int prec = 15)
	{
		int validPrec = prec;
		if(validPrec > MAX_FLOAT_PREC)
			validPrec = MAX_FLOAT_PREC;

		CString floatString;
		floatString.Format("%.*f",validPrec,dValue);
		return floatString;
	}

	CString QuoteString(const CString& sString)
	{
		return '\'' + sString + '\'';
	}

	CString FormatString(const CString& sString)
	{

	}
	virtual CString GetSQLStringValue(DATAMEMBERTYPE* pDataMember) = 0;

private:
	DATAMEMBERTYPE* m_pDataMember;
};

#endif