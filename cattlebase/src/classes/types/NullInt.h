#ifndef __NULLINT_H__
#define __NULLINT_H__

#include "Nullable.h"

#define DEFAULT_NULL_VALUE (-1)

class NullInt : public Nullable
{
public:
	explicit NullInt(int NullIndicator = DEFAULT_NULL_VALUE);
	explicit NullInt(int Value,int NullIndicator);
	NullInt(const NullInt& copy);
	NullInt& operator=(const int right);
	NullInt& operator=(const NullInt& right);

	void CopyFrom(const NullInt& copy);
	bool IsNull() const;
	void MakeNull();
	int ToInt(int whatIfNull = 0) const;
	void ToString(char *pszDest,UINT size,const char* whatIfNull = NULL) const;
	CString ToString(const char *fmt = NULL,const char * whatIfNull = NULL) const;

	friend bool operator>=(const NullInt& left,const NullInt& right);
	friend bool operator>(const NullInt& left,const NullInt& right);
	friend bool operator<(const NullInt& left,const NullInt& right);
	friend bool operator<=(const NullInt& left,const NullInt& right);
	friend bool operator!=(const NullInt &left ,const NullInt &right);
	friend bool operator==(const NullInt &left ,const NullInt &right);

	friend bool operator<=(const int left,const NullInt& right);
	friend bool operator>=(const int left, const NullInt& right);
	friend bool operator==(const int left ,const NullInt &right);
	friend bool operator!=(const int left ,const NullInt &right);
	friend bool operator<(const int left,const NullInt& right);
	friend bool operator>(const int left, const NullInt& right);
	

	friend bool operator>=(const NullInt& left,const int right);
	friend bool operator<=(const NullInt& left,const int right);
	friend bool operator==(const NullInt &left ,const int right);
	friend bool operator!=(const NullInt &left ,const int right);
	friend bool operator>(const NullInt& left,const int right);
	friend bool operator<(const NullInt& left,const int right);
	
	friend NullInt& operator +=(NullInt &left ,NullInt &right);
	
	
protected:
	int m_iNullInd;
	int m_iValue;
	
};


#endif
