#ifndef __HENTSESSION_H__
#define __HENTSESSION_H__

#include "IHent.h"
#include "Session.h"

class HentSession : public IHent,public Session
{
public:	
		
	virtual void SetName(const CString& hentName) = 0;

	
	virtual void SetAlias(const CString& hentAlias) = 0;

	
	virtual void SetStreet(const CString& hentStreet) = 0;
	
	
	virtual void SetPOBox(const CString& hentPOBox) = 0;

	
	virtual void SetCity(const CString& hentCity) = 0;

	
	virtual void SetZip(const CString& hentZip) = 0;

	
	virtual void SetPhone(const CString& hentPhone) = 0;

	
	virtual void SetNIP(const CString& hentNIP) = 0;

	
	virtual void SetFarmNo(const CString& hentFarmNo) = 0;

	
	virtual void SetWetNo(const CString& hentWetNo) = 0;

	
	virtual void SetExtras(const CString& hentExtras) = 0;


	virtual void SetCountry(Country* hentCountry) = 0;


	virtual void SetPlate(const CString& hentPlate) = 0;


	virtual void SetHentType(HentType* hentHentType) = 0;


	virtual void SetSyncIt(bool hentSyncIt) = 0;
	

	virtual void SetPESEL(const CString& hentPESEL) = 0;
	

	virtual void SetREGON(const CString& hentREGON) = 0;
	

	virtual void SetIdNo(const CString& hentIdNo) = 0;
	

	virtual void SetIssueDate(const CDateTime& hentIdIssueDate) = 0;
	

	virtual void SetIssuePost(const CString& hentIssuePost) = 0;


	virtual void SetAccountNo(const CString& hentAccountNo) = 0;
	

	virtual void SetBankName(const CString& hentBankName) = 0;


	virtual void SetWetLicenceNo(const CString& sWetLicenceNo) = 0;


	virtual void SetCellPhoneNo(const CString& sCellPhoneNo) = 0;


	virtual void SetEmailAddress(const CString& sEmailAddress) = 0;


	virtual void SetLatitude(const Latitude& lati) = 0;


	virtual void SetLongitude(const Longitude& longi) = 0;

};
	

#endif