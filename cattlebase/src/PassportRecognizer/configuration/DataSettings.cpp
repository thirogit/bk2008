#include "stdafx.h"
#include "DataSettings.h"
#include "../../datatypes/Stock.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DataSettings::DataSettings()
{
	m_bDoUppercaseHent = true;
	m_bDoNotUpdateValueIfPresent = true;
	m_bUseDefaultStock = false;
	m_uDefaultStockId = NULL_STOCKID;

	m_optionalFieldsEnabled[PASSPORT_DATE] = true;
	m_optionalFieldsEnabled[PASSPORT_NO] = true;
	m_optionalFieldsEnabled[BIRTH] = true;
	m_optionalFieldsEnabled[SEX] = true;
	m_optionalFieldsEnabled[STOCK] = true;
	m_optionalFieldsEnabled[MOTHER_EAN] = true;
	m_optionalFieldsEnabled[FIRST_OWNER] = true;
}
	
	
bool DataSettings::GetDoUppercaseHent() const
{
	return m_bDoUppercaseHent;
}

bool DataSettings::GetDoNotUpdateValueIfPresent() const
{
	return m_bDoNotUpdateValueIfPresent;
}

bool DataSettings::GetUseDefaultStock() const
{
	return m_bUseDefaultStock;
}

UINT DataSettings::GetDefaultStockId() const
{
	if(GetUseDefaultStock())
		return m_uDefaultStockId;

	return NULL_STOCKID;
}

bool DataSettings::IsFieldEnabled(OPTIONAL_FIELDS optinalField) const
{
	std::map<DataSettings::OPTIONAL_FIELDS,bool>::const_iterator it = m_optionalFieldsEnabled.find(optinalField);
	if(it != m_optionalFieldsEnabled.end())
	{
		return it->second;
	}
	return false;
}
	
void DataSettings::SetDoUppercaseHent(bool bDoUppercaseHent)
{
	m_bDoUppercaseHent = bDoUppercaseHent;
}

void DataSettings::SetDoNotUpdateValueIfPresent(bool bDoNotUpdateValueIfPresent)
{
	m_bDoNotUpdateValueIfPresent = bDoNotUpdateValueIfPresent;
}

void DataSettings::SetUseDefaultStock(bool bUseDefaultStock)
{
	m_bUseDefaultStock = bUseDefaultStock;
}

void DataSettings::SetDefaultStockId(UINT uDefaultStockId)
{
	m_uDefaultStockId = uDefaultStockId;
}

void DataSettings::SetOptionalFieldEnabled(OPTIONAL_FIELDS optinalField, bool bEnabled)
{
	m_optionalFieldsEnabled[optinalField] = bEnabled;
}
