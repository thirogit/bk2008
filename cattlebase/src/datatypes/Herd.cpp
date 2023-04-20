#include "stdafx.h"
#include "Herd.h"

#include "log.h"
#include "CattleSQL.h"
#include "consts.h"

#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "sqlupdate/sqldatamembers/SQLGeoCoordinateDataMember.h"
#include "sqlupdate/sqldatamembers/SQLIntegerDataMember.h"

Herd::Herd(UINT herdId) :  herdid(herdId) ,herdno(0) 
{
	transactedMembers.Add(&name);
	transactedMembers.Add(&code);
	transactedMembers.Add(&herdno);
	transactedMembers.Add(&street);
	transactedMembers.Add(&pobox);
	transactedMembers.Add(&city);
	transactedMembers.Add(&zip);
	transactedMembers.Add(&latitude);
	transactedMembers.Add(&longitude);

	updatebleMembers.Add(new SQLColumnUpdate("HERDNAME",		new SQLStringDataMember(&name)));
	updatebleMembers.Add(new SQLColumnUpdate("HERDNO",			new SQLIntegerDataMember(&herdno)));
	updatebleMembers.Add(new SQLColumnUpdate("STREET",			new SQLStringDataMember(&street)));
	updatebleMembers.Add(new SQLColumnUpdate("POBOX",			new SQLStringDataMember(&pobox)));
	updatebleMembers.Add(new SQLColumnUpdate("CITY",			new SQLStringDataMember(&city)));
	updatebleMembers.Add(new SQLColumnUpdate("ZIP",				new SQLStringDataMember(&zip)));
	updatebleMembers.Add(new SQLColumnUpdate("HERDCODE",		new SQLStringDataMember(&code)));
	updatebleMembers.Add(new SQLColumnUpdate("LATITUDE",		new SQLLatitudeDataMember(&latitude)));
	updatebleMembers.Add(new SQLColumnUpdate("LONGITUDE",		new SQLLongitudeDataMember(&longitude)));
	
}

Herd::~Herd()
{
	updatebleMembers.FreeAndFlush();
}

void Herd::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();

}
void Herd::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}
bool Herd::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}


void Herd::CopyFrom(const Herd& copy)
{
	name.CopyFrom(copy.name);
	code.CopyFrom(copy.code);
	street.CopyFrom(copy.street);
	pobox.CopyFrom(copy.pobox);
	city.CopyFrom(copy.city);
	zip.CopyFrom(copy.zip);
	herdno.CopyFrom(copy.herdno);
	latitude.CopyFrom(copy.latitude);
	longitude.CopyFrom(copy.longitude);
}

//***************************************************************************************
bool Herd::InsertThisHerd(bool bCommit/* = true*/)
{
	/*SQLQuery q;
		
	const char QueryStr[] =  "INSERT INTO hents(name,alias,street,pobox,city,zip,phone,nip,arimrno,wetno,extras,plate,country,"
							 "creattime,creator,lmodiftime,lastmodifier,henttype,accountno,bankname,pesel,regon,idno,issuepost,issuedate,"
							 "cellphone,email,latitude,longitude,wetlicenceno)"
							 " VALUES (%0q,%1q,%2n,%3n,%4n,%5n,%6n,%7n,%8n,%9n,%10n,%11n,%12,%13q,%14,%15q,%16,%17,%18n,%19n,%20n,%21n,"
							 "%22n,%23n,%24n,%25n,%26n,%27,%28,%29n);";
	
	
	q << QueryStr;
	q.parse();
	

	SQLQueryParms p;

	p << SQLString(name.GetValue()) 
		  << SQLString(alias.GetValue())
		  << SQLString(street.GetValue())
		  << SQLString(pobox.GetValue())
		  << SQLString(city.GetValue())
		  << SQLString(zip.GetValue())
		  << SQLString(phone.GetValue())
		  << SQLString(nip.GetValue())
		  << SQLString(arimrno.GetValue())
		  << SQLString(wetno.GetValue())
		  << SQLString(extras.GetValue())
		  << SQLString(plate.GetValue())
		  << SQLString(country.GetValue()->GetId())
		  << SQLString(creation.eventDt)
		  << SQLString(creation.user->userid)
		  << SQLString(modification.eventDt)
		  << SQLString(modification.user->userid)
		  << SQLString(henttype.GetValue()->henttypeid)
		  << SQLString(accountno.GetValue())
		  << SQLString(bankname.GetValue())
		  << SQLString(pesel.GetValue())
		  << SQLString(regon.GetValue())
		  << SQLString(idno.GetValue())
		  << SQLString(issuepost.GetValue())
		  << SQLString(issuedate.GetValue(),true)		 
		  << SQLString(cellphoneno.GetValue())
		  << SQLString(emailaddress.GetValue())
		  << SQLString(latitude.GetValue())
		  << SQLString(longitude.GetValue())
		  << SQLString(wetlicenceno.GetValue());
	
  
		LOG("InsertThisHent() query: %s",q.str(p).c_str());
		if(!SQLDB.Insert(q.str(p).c_str())) 
		{
			LOG("InsertThisHent() error: %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}		

		if((hentid = SQLDB.SelectScalar("SELECT MAX(hentid) FROM hents",1)) < 0)
		{		
			SQLDB.Rollback();
			return false;
		}		
		
		if(bCommit) SQLDB.Commit();
		*/
		return true;
	
	
}
//***************************************************************************************
bool Herd::CanEditThisHerd()
{
	return CAN_DO(CANEDIT,this);	
}
bool Herd::CanDeleteThisHerd()
{
	return  CAN_DO(CANDELETE,this);	
}
bool Herd::UpdateThisHerd(bool bCommit/* = true*/)
{
	if(IsUncommited())
	{
		SQLQuery q;
		q << "UPDATE hents SET ";
		UpatableDataMembers::ArrayIterator it = updatebleMembers.iterator();
		while(it.hasNext())
		{
			SQLColumnUpdate* pColumn = *(it++);
			if(pColumn->NeedsUpdate())
			{
				q << pColumn->ToEqualString();
				q << ", ";
			}
		}

		modification.eventDt = CDateTime::now();
		modification.user = UserData::GetCurrentUser();

		q << "LASTMODIFIER = " << modification.user->userid << ',';
		q << "LMODIFTIME = '" << modification.eventDt.GetDateInFmt(CDateTime::YYYYdotMMdotDD_HHcolonMMcolonSS) << '\'';

		q <<  " WHERE hentid = " << GetId();

		LOG("UpdateThisHent() stmt %s",q.str().c_str());

		if(!SQLDB.Update(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisHent() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}
	}
	return true;
}
//***************************************************************************************
bool Herd::DeleteThisHerd(bool bCommit/* = true*/)
{
	SQLQuery q;
	q <<  "DELETE FROM hents WHERE hentid  = %0";
	q.parse();
		
	LOG("DeleteThisHent() stmt: %s",q.str(GetId()).c_str());
	
	if(SQLDB.Delete(q.str(GetId()).c_str(),bCommit))
	{
		ResetId();		
		return true;
	}

	LOG("DeleteThisHent() error: %s",(LPCSTR)SQLDB.GetLastError());
	return false;
	
}


UINT Herd::GetId() const
{
	return herdid;
}

void Herd::ResetId()
{
	herdid = NULL_HERDID;
}

CString Herd::GetName() const
{
	return name.GetValue();
}

void Herd:: SetName(const CString& herdName)
{
	name.SetTo(herdName);
}

void Herd:: ChangeName(const CString& herdName)
{
	name.ChangeTo(herdName);
}


CString Herd::GetCode() const
{
	return code.GetValue();
}

void Herd:: SetCode(const CString& sHerdCode)
{
	code.SetTo(sHerdCode);
}

void Herd:: ChangeCode(const CString& sHerdCode)
{
	code.ChangeTo(sHerdCode);
}

CString Herd::GetStreet() const
{
	return street.GetValue();
}

void Herd:: SetStreet(const CString& hentStreet)
{
	street.SetTo(hentStreet);
}

void Herd:: ChangeStreet(const CString& hentStreet)
{
	street.ChangeTo(hentStreet);
}

CString Herd::GetPOBox() const
{
	return pobox.GetValue();
}

void Herd:: SetPOBox(const CString& hentPOBox)
{
	pobox.SetTo(hentPOBox);
}

void Herd:: ChangePOBox(const CString& hentPOBox)
{
	pobox.ChangeTo(hentPOBox);
}

CString Herd::GetCity() const
{
	return city.GetValue();
}

void Herd:: SetCity(const CString& hentCity)
{
	city.SetTo(hentCity);
}

void Herd:: ChangeCity(const CString& hentCity)
{
	city.ChangeTo(hentCity);
}

CString Herd::GetZip() const
{
	return zip.GetValue();
}

void Herd:: SetZip(const CString& hentZip)
{
	zip.SetTo(hentZip);
}

void Herd:: ChangeZip(const CString& hentZip)
{
	zip.ChangeTo(hentZip);
}


Latitude Herd::GetLatitude() const
{
	return latitude.GetValue();
}

void Herd::SetLatitude(const Latitude& lati)
{
	latitude.SetTo(lati);
}

void Herd::ChangeLatitude(const Latitude& lati)
{
	latitude.ChangeTo(lati);
}

Longitude Herd::GetLongitude() const
{
	return longitude.GetValue();
}

void Herd::SetLongitude(const Longitude& longi)
{
	longitude.SetTo(longi);
}

void Herd::ChangeLongitude(const Longitude& longi)
{
	longitude.ChangeTo(longi);
}


