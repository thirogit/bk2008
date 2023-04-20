#ifndef __HERD_H__
#define __HERD_H__

#include "RawData.h"

#include "../classes/types/Latitude.h"
#include "../classes/types/Longitude.h"
#include "DataMember.h"
#include "DataMembersDefs.h"

#define NULL_HERDID (0)

class Herd : public RawData
{
public:

	Herd(UINT herdId);
	~Herd();
	void CopyFrom(const Herd& copy);
	
	UINT GetId() const;
	void ResetId();
	
	CString GetName() const;
	void SetName(const CString& herdName);
	void ChangeName(const CString& herdName);

	CString GetCode() const;
	void SetCode(const CString& sHerdCode);
	void ChangeCode(const CString& sHerdCode);

	int GetHerdNo() const;
	void SetHerdNo(const int nHerdNo);
	void ChangeHerdNo(const int nHerdNo);

	CString GetStreet() const;
	void SetStreet(const CString& hentStreet);
	void ChangeStreet(const CString& hentStreet);
	
	CString GetPOBox() const;
	void SetPOBox(const CString& hentPOBox);
	void ChangePOBox(const CString& hentPOBox);

	CString GetCity() const;
	void SetCity(const CString& hentCity);
	void ChangeCity(const CString& hentCity);

	CString GetZip() const;
	void SetZip(const CString& hentZip);
	void ChangeZip(const CString& hentZip);
		
	Latitude GetLatitude() const;
	void SetLatitude(const Latitude& lati);
	void ChangeLatitude(const Latitude& lati);

	Longitude GetLongitude() const;
	void SetLongitude(const Longitude& longi);
	void ChangeLongitude(const Longitude& longi);

	
	bool InsertThisHerd(bool bCommit = true);
	bool UpdateThisHerd(bool bCommit = true);
	bool DeleteThisHerd(bool bCommit = true);	
	bool CanEditThisHerd();
	bool CanDeleteThisHerd();

	void RollbackChanges();
	void CommitChanges();
	bool IsUncommited();

protected:
	UINT herdid;

	StringDataMember name;
	StringDataMember code;
	IntegerDataMember herdno;
	StringDataMember street;
	StringDataMember pobox;
	StringDataMember city;
	StringDataMember zip;
	LatitudeDataMember latitude;
	LongitudeDataMember longitude;

	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;
	PtrFlushArray<SQLColumnUpdate> insertMembers;

};


#endif