#include "stdafx.h"
#include "SQLWeightDataMember.h"
#include "../../../Utils/StringFormat.h"

SQLWeightDataMember::SQLWeightDataMember(WeightDataMember* pWeightMember,int precision) :
		SQLDataMember(pWeightMember),m_precision(precision)
{
}
CString SQLWeightDataMember::GetSQLStringValue(WeightDataMember* pWeightMember)
{
	return CStringFormat("%.*f",max(2,m_precision),pWeightMember->GetValue().GetDouble());
}