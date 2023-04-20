#include "stdafx.h"
#include "VATRate.h"
//#include "../StringFormat.h"

VATRate::VATRate()
{
	
}
VATRate::VATRate(double dVATRate) : Percent(dVATRate)
{
	ValidateNull();
}

void VATRate::ValidateNull()
{
	if(Percent::GetDouble(-1.0) < 0.0)
		MakeNull();
}

VATRate::VATRate(const VATRate& copy)
{
	CopyFrom(copy);
}

VATRate& VATRate::operator=(const VATRate &right)
{
	CopyFrom(right);
	return *this;
}

VATRate& VATRate::operator=(double dVATRate)
{
	Percent::operator=(dVATRate);
	ValidateNull();
	return *this;
}

//bool VATRate::operator>=(const VATRate& right)
//{
//	if(IsNull() && right.IsNull()) return true;
//	if(IsNull() || right.IsNull()) return false;
//
//	return	m_dPercent   >= right.m_dPercent;
//}
//bool VATRate::operator<=(const VATRate& right)
//{
//	if(IsNull() && right.IsNull()) return true;
//	if(IsNull() || right.IsNull()) return false;
//
//	return m_dPercent <= right.m_dPercent;
//}
//bool VATRate::operator>(const VATRate& right)
//{
//	if(IsNull())
//	{
//		return false;
//	}
//	else
//		if(right.IsNull()) return true;
//
//	return m_dPercent > right.m_dPercent;
//		
//}
//bool VATRate::operator<(const VATRate& right)
//{	
//	return !operator>(right);
//}
//bool VATRate::operator==(const VATRate& right)
//{
//	if(IsNull() && right.IsNull()) return true;
//	if(right.IsNull() || IsNull()) return false;
//
//	return m_dPercent == right.m_dPercent;
//}
//bool VATRate::operator!=(const VATRate& right)
//{
//	return !(*this == right);
//}
