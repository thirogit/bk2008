#ifndef __HARDWAREKEY_H__
#define __HARDWAREKEY_H__

#include "../classes/types/DateTime.h"

class CHardwareKey 
{
public:
	
	virtual bool			IsKeyPresent() const = 0;	
	virtual bool			WiteKeyExpiryDate(const CDateTime& keyExpiryDt) = 0;
	virtual CDateTime		GetKeyExpiryDate() const = 0;
	virtual CDateTime		GetDateOfLastKeyCheck() const = 0;
	virtual bool			WriteDateOfLastKeyCheck(const CDateTime& lastKeyCheckDt) = 0;
	
	virtual unsigned int	GetCowLimit() const = 0;
	virtual unsigned int	GetCowCount() const = 0;
	virtual void			SetCowLimit(unsigned int cowLimit) = 0;
	virtual bool			ReadKey() = 0;
	virtual void			IncreaseCowCount(unsigned int cowAmount) = 0;


	CString					GetLastErrorStr() const;
	CString					GetLicencedTo() const;
	WORD					GetClientId() const;
	WORD					GetFeatures() const;
	DWORD					GetKeySerialNo() const;
	bool					IsFeatureEnabled(UINT featureNumber) const;
	
protected:
	void SetLastError(const CString& sError);


	DWORD			m_HardwareKeyId;
	WORD			m_wFeatures;
	WORD			m_wClientId;
	CString			m_sLicencedTo;
	CString			m_sLastError;
	
};


#endif 
