#include "stdafx.h"
#include "HardwareKey.h"

CString	CHardwareKey::GetLastErrorStr() const
{
	return m_sLastError;
}

CString	CHardwareKey::GetLicencedTo() const
{
	return m_sLicencedTo;
}
WORD CHardwareKey::GetClientId() const
{
	return m_wClientId;
}
WORD CHardwareKey::GetFeatures() const
{
	return m_wFeatures;
}
DWORD CHardwareKey::GetKeySerialNo() const
{
	return m_HardwareKeyId;
}
bool CHardwareKey::IsFeatureEnabled(UINT featureNumber) const
{
	ASSERT(featureNumber < sizeof(m_wFeatures)*8); 
	if(featureNumber >= sizeof(m_wFeatures)*8)
		return false;
	WORD feature_mask = 1 << featureNumber;
	return (m_wFeatures & feature_mask) != 0;
}
	
void SetLastError(const CString& sError)
{
	m_sLastError = sError;
}
