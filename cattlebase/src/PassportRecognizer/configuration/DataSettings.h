#ifndef __DATASETTINGS_H__
#define __DATASETTINGS_H__

#include <map>

class DataSettings 
{
public:
	DataSettings();
	
	enum OPTIONAL_FIELDS
	{  
		PASSPORT_DATE,
		PASSPORT_NO,
		BIRTH,
		SEX,
		STOCK,
		MOTHER_EAN,
		FIRST_OWNER
	};
	
	bool GetDoUppercaseHent() const;
	bool GetDoNotUpdateValueIfPresent() const;
	bool GetUseDefaultStock() const;
	UINT GetDefaultStockId() const;
	bool IsFieldEnabled(OPTIONAL_FIELDS optinalField) const;
	
	void SetDoUppercaseHent(bool bDoUppercaseHent);
	void SetDoNotUpdateValueIfPresent(bool bDoNotUpdateValueIfPresent);
	void SetUseDefaultStock(bool bUseDefaultStock);
	void SetDefaultStockId(UINT uDefaultStockId);
	void SetOptionalFieldEnabled(OPTIONAL_FIELDS optinalField, bool bEnabled);
	
protected:
	bool				m_bDoUppercaseHent;
	bool				m_bDoNotUpdateValueIfPresent;	
	bool				m_bUseDefaultStock;
	UINT				m_uDefaultStockId;
	std::map<OPTIONAL_FIELDS,bool> m_optionalFieldsEnabled;
};

#endif