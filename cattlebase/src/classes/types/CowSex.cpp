#include "stdafx.h"
#include "CowSex.h"
#include "../../exceptions/StrException.h"

const COWSEXES CowSexes[COWSEXES_SIZE] =
{
	{ "XY", SEX_XY},
	{"BRAK", SEX_UNK},
	{"XX", SEX_XX}
};



CowSex::CowSex(COWSEX sex) 
{
	if(IsCowSex(sex))
		m_sex = sex;
	else
		m_sex = SEX_UNK;
}

void CowSex::MakeXX()
{
	m_sex = SEX_XX;
}

void CowSex::MakeXY()
{
	m_sex = SEX_XY;
}

void CowSex::MakeUNK()
{
	m_sex = SEX_UNK;
}

void CowSex::Make(COWSEX eSex)
{
	if(!IsCowSex(eSex))
		throw new CStrException("CowSex()::Make : bad function argument");
	else m_sex = eSex;
}

CowSex::operator LPCTSTR() const
{
	return CowSexes[((int)m_sex)+1].szCowSexName;
}

CowSex& CowSex::operator =(LPCSTR cowSexStr)
{
	bool bFound = false;
	const char *pszCowSexName = NULL;
	for(int i = 0;i < COWSEXES_SIZE;i++)
	{
		pszCowSexName = CowSexes[i].szCowSexName;
		if(!_strnicmp(pszCowSexName,cowSexStr,strlen(pszCowSexName)))
		{
			m_sex = (COWSEX)(i-1);
			bFound = true;
			break;
		}
	}

	if(!bFound) throw new CStrException("CowSex()::operator= : bad assigment argument");
	return *this;

}

CowSex& CowSex::operator=(const CowSex &right)
{
	m_sex = right.m_sex;
	return *this;
}

CowSex& CowSex::operator=(COWSEX right)
{
	m_sex = right;
	return *this;
}

bool CowSex::IsCowSex(COWSEX eSex) const
{
	return !(eSex != SEX_XY && eSex != SEX_XX && eSex != SEX_UNK);
}
 
COWSEX CowSex::getCOWSEX() const
{
	return m_sex;
}

bool CowSex::IsXX() const
{
	return m_sex == SEX_XX;
}
	
bool CowSex::IsXY() const
{
	return m_sex == SEX_XY;
}
	
bool CowSex::IsUNK() const
{
	return m_sex == SEX_UNK;
}


bool CowSex::operator>=(const CowSex& right)
{
	return m_sex >= right.m_sex;
}
bool CowSex::operator<=(const CowSex& right)
{
	return m_sex <= right.m_sex;
}
bool CowSex::operator>(const CowSex& right)
{
	return m_sex > right.m_sex;
}
bool CowSex::operator<(const CowSex& right)
{
	return m_sex < right.m_sex;
}
bool CowSex::operator==(const CowSex& right)
{
	return m_sex == right.m_sex;
}
