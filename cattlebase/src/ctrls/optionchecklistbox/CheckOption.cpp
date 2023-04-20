#include "stdafx.h"
#include "CheckOption.h"

CCheckOption::CCheckOption(const CString& optionDesc,bool *bOptionValue) :
	m_sOptionDesc(optionDesc),m_bOptionValue(bOptionValue)
{
}

const CString& CCheckOption::GetOptionDesc() const
{
	return m_sOptionDesc;
}

bool CCheckOption::GetOptionValue() const
{
	return *m_bOptionValue;
}

void CCheckOption::SetOptionDesc(const CString& sOptDesc)
{
	m_sOptionDesc = sOptDesc;
}

void CCheckOption::SetOptionValue(bool bValue)
{
	*m_bOptionValue = bValue;
}

