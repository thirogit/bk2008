#ifndef __IHENT_H__
#define __IHENT_H__

#include "../../datatypes/HentType.h"
#include "../../datatypes/Country.h"
#include "../../classes/types/DateTime.h"
#include "../../classes/types/Longitude.h"
#include "../../classes/types/Latitude.h"

class IHent
{	
public:
	virtual UINT GetId() const = 0;
	virtual CString GetName() const = 0;
	virtual CString GetAlias() const = 0;
	virtual CString GetStreet() const = 0;
	virtual CString GetPOBox() const = 0;
	virtual CString GetCity() const = 0;
	virtual CString GetZip() const = 0;
	virtual CString GetPhone() const = 0;
	virtual CString GetNIP() const = 0;
	virtual CString GetFarmNo() const = 0;
	virtual CString GetWetNo() const = 0;
	virtual CString GetExtras() const = 0;
	virtual Country* GetCountry() const = 0;
	virtual CString GetPlate() const = 0; 
	virtual HentType* GetHentType() const = 0;
	virtual bool GetSyncIt() const = 0;
	virtual CString GetPESEL() const = 0;
	virtual CString GetREGON() const = 0;
	virtual CString GetIdNo() const = 0;
	virtual CDateTime GetIssueDate() const = 0;
	virtual CString GetIssuePost() const = 0;
	virtual CString GetAccountNo() const = 0;
	virtual CString GetBankName() const = 0;
	virtual CString GetWetLicenceNo() const = 0;
	virtual CString GetCellPhoneNo() const = 0;
	virtual CString GetEmailAddress() const = 0;
	virtual Latitude GetLatitude() const = 0;
	virtual Longitude GetLongitude() const = 0;
};
#endif