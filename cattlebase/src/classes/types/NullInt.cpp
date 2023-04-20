#include "stdafx.h"
#include "NullInt.h"

NullInt::NullInt(int NullIndicator/* = DEFAULT_NULL_VALUE*/) : 	m_iNullInd(NullIndicator), m_iValue(NullIndicator) 
{
}

NullInt::NullInt(int Value,int NullIndicator) : m_iNullInd(NullIndicator), m_iValue(Value)
{
}

void NullInt::CopyFrom(const NullInt& copy)
{
	m_iNullInd = copy.m_iNullInd;
	m_iValue = copy.m_iValue;
}

NullInt::NullInt(const NullInt& copy)
{
	CopyFrom(copy);
}

NullInt& NullInt::operator=(const int right)
{
	m_iValue = right;
	return *this;
}

NullInt& NullInt::operator=(const NullInt& right)
{
	CopyFrom(right);
	return *this;
}

bool NullInt::IsNull() const
{
	return (m_iNullInd == m_iValue);
}

void NullInt::MakeNull()
{
	m_iValue = m_iNullInd;
}


int NullInt::ToInt(int whatIfNull/* = 0*/) const
{
	return IsNull() ? whatIfNull : m_iValue;
}

void NullInt::ToString(char *pszDest,UINT size,const char* whatIfNull/* = NULL*/) const
{
	ASSERT(size > 0 && pszDest);

	if(IsNull())
	{
		if(whatIfNull)
			strncpy(pszDest,whatIfNull,size);
		else
			pszDest[0] = '\0';
	}
	else
	{
		_snprintf(pszDest,size,"%d",m_iValue);
	}
}


CString NullInt::ToString(const char *fmt/* = NULL*/,const char * whatIfNull/* = NULL*/) const
{
	CString ret;
	if(IsNull())
	{
		if(whatIfNull)
			ret = whatIfNull;
	}
	else 
	{
		if(fmt)
			ret.Format(fmt,m_iValue);
		else
			ret.Format("%d",m_iValue);
	}
	return ret;
}

	
	
bool operator>=(const NullInt& left,const NullInt& right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();

	if(l && r) return true;
    if(r) return true;
	if(l) return false;

	return left.m_iValue >= right.m_iValue;
}

bool operator>(const NullInt& left,const NullInt& right)
{
	if(!left.IsNull()) 
	{
		if(right.IsNull()) return true; //nie-null zawsze wiekszy od null
		else return left.m_iValue > right.m_iValue; 
	}
	return false; //nawet jezeli right nie jest null to left=null > right=nie-null
			     // a gdy oba rowne null to takze zwracamy false bo nie pytamy sie o rownosc
}

bool operator<(const NullInt& left,const NullInt& right)
{
	return right > left;
}


bool operator<=(const NullInt& left,const NullInt& right)
{
	return right >= left;
}

bool operator<=(const int left,const NullInt& right)
{
	if(right.IsNull()) return false;
    return right.m_iValue >= left;
}

bool operator>=( int left, const NullInt& right)
{
	return operator<=(right,left);
}

bool operator>=(const NullInt& left,const int right)
{
	if(left.IsNull()) return false;
	return left.m_iValue >= right;
}

bool operator<=(const NullInt& left,const int right)
{
	if(left.IsNull()) return true;
	return  left.m_iValue <= right ;
}

bool operator!=(const NullInt &left ,const NullInt &right)
{	
	return !(left == right);
}


bool operator==(const NullInt &left ,const NullInt &right)
{
	bool l = left.IsNull();
	bool r = right.IsNull();
	
	if(l && r) return true;
	else if(r || l) return false;

	return (left.m_iValue == right.m_iValue);
}

NullInt& operator +=(NullInt &left ,NullInt &right)
{
	if(!right.IsNull()) 
	{
		if(!left.IsNull())
			left.m_iValue += right.m_iValue;
		else left.m_iValue = right.m_iValue;
	}
	return left;
    
}

bool operator==(const int left ,const NullInt &right)
{
	if(right.IsNull()) return false;

	return (left == right.m_iValue);
}

bool operator==(const NullInt &left ,const int right)
{
	return operator==(right,left);
}

bool operator>(const NullInt& left,const int right)
{
	if(left.IsNull())
		return false;

	return left.m_iValue > right;
}
bool operator!=(const int left ,const NullInt &right)
{
	return !operator==(left,right);
}

bool operator!=(const NullInt &left,const int right)
{
	return !operator==(left,right);
}