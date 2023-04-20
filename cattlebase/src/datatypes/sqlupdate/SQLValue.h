#ifndef __SQLVALUE_H__
#define __SQLVALUE_H__

class SQLValue
{
public:
	virtual bool IsChanged() = 0;
	virtual CString ToString() = 0;
};
#endif