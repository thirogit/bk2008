#include "stdafx.h"
#include "Hent.h"

#include "log.h"
#include "CattleSQL.h"
#include "consts.h"

#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "sqlupdate/sqldatamembers/SQLDateTimeDataMember.h"
#include "sqlupdate/sqldatamembers/SQLBooleanDataMember.h"
#include "sqlupdate/sqldatamembers/SQLHentTypeDataMember.h"
#include "sqlupdate/sqldatamembers/SQLCountryDataMember.h"
#include "sqlupdate/sqldatamembers/SQLGeoCoordinateDataMember.h"

Hent::Hent(UINT hentId) : country(NULL), hentid(hentId) , henttype(NULL) 
{

	transactedMembers.Add(&name);
	transactedMembers.Add(&alias);
	transactedMembers.Add(&street);
	transactedMembers.Add(&pobox);
	transactedMembers.Add(&city);
	transactedMembers.Add(&zip);
	transactedMembers.Add(&phone);
	transactedMembers.Add(&nip);
	transactedMembers.Add(&arimrno);
	transactedMembers.Add(&wetno);
	transactedMembers.Add(&extras);
	transactedMembers.Add(&country);
	transactedMembers.Add(&plate);	
	transactedMembers.Add(&henttype);
	transactedMembers.Add(&syncit);
	transactedMembers.Add(&pesel);
	transactedMembers.Add(&regon);
	transactedMembers.Add(&idno);
	transactedMembers.Add(&issuedate);
	transactedMembers.Add(&issuepost);
	transactedMembers.Add(&accountno);
	transactedMembers.Add(&bankname);
	transactedMembers.Add(&wetlicenceno);
	transactedMembers.Add(&cellphoneno);
	transactedMembers.Add(&emailaddress);
	transactedMembers.Add(&latitude);
	transactedMembers.Add(&longitude);

	updatebleMembers.Add(new SQLColumnUpdate("NAME",			new SQLStringDataMember(&name)));
	updatebleMembers.Add(new SQLColumnUpdate("ALIAS",			new SQLStringDataMember(&alias)));
	updatebleMembers.Add(new SQLColumnUpdate("STREET",			new SQLStringDataMember(&street)));
	updatebleMembers.Add(new SQLColumnUpdate("POBOX",			new SQLStringDataMember(&pobox)));
	updatebleMembers.Add(new SQLColumnUpdate("CITY",			new SQLStringDataMember(&city)));
	updatebleMembers.Add(new SQLColumnUpdate("ZIP",				new SQLStringDataMember(&zip)));
	updatebleMembers.Add(new SQLColumnUpdate("PHONE",			new SQLStringDataMember(&phone)));
	updatebleMembers.Add(new SQLColumnUpdate("NIP",				new SQLStringDataMember(&nip)));
	updatebleMembers.Add(new SQLColumnUpdate("ARIMRNO",			new SQLStringDataMember(&arimrno)));
	updatebleMembers.Add(new SQLColumnUpdate("WETNO",			new SQLStringDataMember(&wetno)));
	updatebleMembers.Add(new SQLColumnUpdate("EXTRAS",			new SQLStringDataMember(&extras)));
	updatebleMembers.Add(new SQLColumnUpdate("COUNTRY",			new SQLCountryDataMember(&country)));
	updatebleMembers.Add(new SQLColumnUpdate("PLATE",			new SQLStringDataMember(&plate)));	
	updatebleMembers.Add(new SQLColumnUpdate("HENTTYPE",		new SQLHentTypeDataMember(&henttype)));
	updatebleMembers.Add(new SQLColumnUpdate("SYNCIT",			new SQLBooleanDataMember(&syncit)));
	updatebleMembers.Add(new SQLColumnUpdate("PESEL",			new SQLStringDataMember(&pesel)));
	updatebleMembers.Add(new SQLColumnUpdate("REGON",			new SQLStringDataMember(&regon)));
	updatebleMembers.Add(new SQLColumnUpdate("IDNO",			new SQLStringDataMember(&idno)));
	updatebleMembers.Add(new SQLColumnUpdate("ISSUEDATE",		new SQLDateTimeDataMember(&issuedate,true)));
	updatebleMembers.Add(new SQLColumnUpdate("ISSUEPOST",		new SQLStringDataMember(&issuepost)));
	updatebleMembers.Add(new SQLColumnUpdate("ACCOUNTNO",		new SQLStringDataMember(&accountno)));
	updatebleMembers.Add(new SQLColumnUpdate("BANKNAME",		new SQLStringDataMember(&bankname)));
	updatebleMembers.Add(new SQLColumnUpdate("CELLPHONE",		new SQLStringDataMember(&cellphoneno)));
	updatebleMembers.Add(new SQLColumnUpdate("EMAIL",			new SQLStringDataMember(&emailaddress)));
	updatebleMembers.Add(new SQLColumnUpdate("LATITUDE",		new SQLLatitudeDataMember(&latitude)));
	updatebleMembers.Add(new SQLColumnUpdate("LONGITUDE",		new SQLLongitudeDataMember(&longitude)));
	updatebleMembers.Add(new SQLColumnUpdate("WETLICENCENO",	new SQLStringDataMember(&wetlicenceno)));

}

Hent::~Hent()
{
	updatebleMembers.FreeAndFlush();
}

void Hent::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();

}
void Hent::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}
bool Hent::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}


void Hent::CopyFrom(const Hent& copy)
{
	name.CopyFrom(copy.name);
	alias.CopyFrom(copy.alias);
	street.CopyFrom(copy.street);
	pobox.CopyFrom(copy.pobox);
	city.CopyFrom(copy.city);
	zip.CopyFrom(copy.zip);
	phone.CopyFrom(copy.phone);
	nip.CopyFrom(copy.nip);
	arimrno.CopyFrom(copy.arimrno);
	wetno.CopyFrom(copy.wetno);
	extras.CopyFrom(copy.extras);
	country.CopyFrom(copy.country);
	plate.CopyFrom(copy.plate);
	hentid = copy.hentid;
	henttype.CopyFrom(copy.henttype);
	pesel.CopyFrom(copy.pesel);
	regon.CopyFrom(copy.regon);
	idno.CopyFrom(copy.idno);
	issuedate.CopyFrom(copy.issuedate);
	issuepost.CopyFrom(copy.issuepost);
	accountno.CopyFrom(copy.accountno);
	bankname.CopyFrom(copy.bankname);
	wetlicenceno.CopyFrom(copy.wetlicenceno);
	cellphoneno.CopyFrom(copy.cellphoneno);
	emailaddress.CopyFrom(copy.emailaddress);
	latitude.CopyFrom(copy.latitude);
	longitude.CopyFrom(copy.longitude);

	RawData::Copy(copy);
}

//***************************************************************************************
bool Hent::InsertThisHent(bool bCommit/* = true*/)
{
	SQLQuery q;
		
	const char QueryStr[] =  "INSERT INTO hents(name,alias,street,pobox,city,zip,phone,nip,arimrno,wetno,extras,plate,country,"
							 "creattime,creator,lmodiftime,lastmodifier,henttype,accountno,bankname,pesel,regon,idno,issuepost,issuedate,"
							 "cellphone,email,latitude,longitude,wetlicenceno)"
							 " VALUES (%0q,%1q,%2n,%3n,%4n,%5n,%6n,%7n,%8n,%9n,%10n,%11n,%12,%13q,%14,%15q,%16,%17,%18n,%19n,%20n,%21n,"
							 "%22n,%23n,%24n,%25n,%26n,%27,%28,%29n);";
	
	
	q << QueryStr;
	q.parse();
		
	SQLDB.SetCurrentRawData(*this);

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
		SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::ADDED);
		return true;
	
	
}
//***************************************************************************************
bool Hent::CanEditThisHent()
{
	return CAN_DO(CANEDIT,this);	
}
bool Hent::CanDeleteThisHent()
{
	return  CAN_DO(CANDELETE,this);	
}
bool Hent::UpdateThisHent(bool bCommit/* = true*/)
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
bool Hent::DeleteThisHent(bool bCommit/* = true*/)
{
	SQLQuery q;
	q <<  "DELETE FROM hents WHERE hentid  = %0";
	q.parse();
		
	LOG("DeleteThisHent() stmt: %s",q.str(GetId()).c_str());
	SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::DELETED);
	if(SQLDB.Delete(q.str(GetId()).c_str(),bCommit))
	{
		ResetId();		
		return true;
	}
	LOG("DeleteThisHent() error: %s",(LPCSTR)SQLDB.GetLastError());
	return false;
	
}


UINT Hent::GetId() const
{
	return hentid;
}

void Hent:: ResetId()
{
	hentid = NULL_HENTID;
}

CString Hent::GetName() const
{
	return name.GetValue();
}

void Hent:: SetName(const CString& hentName)
{
	name.SetTo(hentName);
}

void Hent:: ChangeName(const CString& hentName)
{
	name.ChangeTo(hentName);
}


CString Hent::GetAlias() const
{
	return alias.GetValue();
}

void Hent:: SetAlias(const CString& hentAlias)
{
	alias.SetTo(hentAlias);
}

void Hent:: ChangeAlias(const CString& hentAlias)
{
	alias.ChangeTo(hentAlias);
}

CString Hent::GetStreet() const
{
	return street.GetValue();
}

void Hent:: SetStreet(const CString& hentStreet)
{
	street.SetTo(hentStreet);
}

void Hent:: ChangeStreet(const CString& hentStreet)
{
	street.ChangeTo(hentStreet);
}

CString Hent::GetPOBox() const
{
	return pobox.GetValue();
}

void Hent:: SetPOBox(const CString& hentPOBox)
{
	pobox.SetTo(hentPOBox);
}

void Hent:: ChangePOBox(const CString& hentPOBox)
{
	pobox.ChangeTo(hentPOBox);
}

CString Hent::GetCity() const
{
	return city.GetValue();
}

void Hent:: SetCity(const CString& hentCity)
{
	city.SetTo(hentCity);
}

void Hent:: ChangeCity(const CString& hentCity)
{
	city.ChangeTo(hentCity);
}

CString Hent::GetZip() const
{
	return zip.GetValue();
}

void Hent:: SetZip(const CString& hentZip)
{
	zip.SetTo(hentZip);
}

void Hent:: ChangeZip(const CString& hentZip)
{
	zip.ChangeTo(hentZip);
}

CString Hent::GetPhone() const
{
	return phone.GetValue();
}

void Hent:: SetPhone(const CString& hentPhone)
{
	phone.SetTo(hentPhone);
}

void Hent:: ChangePhone(const CString& hentPhone)
{
	phone.ChangeTo(hentPhone);
}

CString Hent::GetNIP() const
{
	return nip.GetValue();
}

void Hent:: SetNIP(const CString& hentNIP)
{
	nip.SetTo(hentNIP);
}

void Hent:: ChangeNIP(const CString& hentNIP)
{
	nip.ChangeTo(hentNIP);
}

bool Hent::HasNIP() const
{
	return !nip.GetValue().IsEmpty();
}

CString Hent::GetFarmNo() const
{
	return arimrno.GetValue();
}

void Hent:: SetFarmNo(const CString& hentFarmNo)
{
	arimrno.SetTo(hentFarmNo);
}

void Hent:: ChangeFarmNo(const CString& hentFarmNo)
{
	arimrno.ChangeTo(hentFarmNo);
}

CString Hent::GetWetNo() const
{
	return wetno.GetValue();
}

void Hent:: SetWetNo(const CString& hentWetNo)
{
	wetno.SetTo(hentWetNo);
}

void Hent:: ChangeWetNo(const CString& hentWetNo)
{
	wetno.ChangeTo(hentWetNo);
}

bool Hent::HasWetNo() const
{
	return !wetno.GetValue().IsEmpty();
}

CString Hent::GetExtras() const
{
	return extras.GetValue();
}

void Hent:: SetExtras(const CString& hentExtras)
{
	extras.SetTo(hentExtras);
}

void Hent:: ChangeExtras(const CString& hentExtras)
{
	extras.ChangeTo(hentExtras);
}

Country* Hent::GetCountry() const
{
	return country.GetValue();
}

void Hent::SetCountry(Country* hentCountry)
{
	country.SetTo(hentCountry);
}

void Hent::ChangeCountry(Country* hentCountry)
{
	country.ChangeTo(hentCountry);
}

bool Hent::IsFromPoland() const
{
	Country* pCountry = country.GetValue();
	if(pCountry)
		return pCountry->GetId() == ISO_POL_NUMBER;

	return false;
}


CString Hent::GetPlate() const
{
	return plate.GetValue();
}
	
void Hent:: SetPlate(const CString& hentPlate)
{
	plate.SetTo(hentPlate);
}

void Hent:: ChangePlate(const CString& hentPlate)
{
	plate.ChangeTo(hentPlate);
}


HentType* Hent::GetHentType() const
{
	return henttype.GetValue();
}

void Hent:: SetHentType(HentType* hentHentType)
{
	henttype.SetTo(hentHentType);
}

void Hent:: ChangeHentType(HentType* hentHentType)
{
	henttype.ChangeTo(hentHentType);
}


bool Hent::GetSyncIt() const
{
	return syncit.GetValue();
}

void Hent:: SetSyncIt(bool hentSyncIt)
{
	syncit.SetTo(hentSyncIt);
}

void Hent:: ChangeSyncIt(bool hentSyncIt)
{
	syncit.ChangeTo(hentSyncIt);
}


CString Hent::GetPESEL() const
{
	return pesel.GetValue();
}

void Hent:: SetPESEL(const CString& hentPESEL)
{
	pesel.SetTo(hentPESEL);
}

void Hent:: ChangePESEL(const CString& hentPESEL)
{
	pesel.ChangeTo(hentPESEL);
}

bool Hent::HasPESEL() const
{
	return !pesel.GetValue().IsEmpty();
}


CString Hent::GetREGON() const
{
	return regon.GetValue();
}

void Hent:: SetREGON(const CString& hentREGON)
{
	regon.SetTo(hentREGON);
}

void Hent:: ChangeREGON(const CString& hentREGON)
{
	regon.ChangeTo(hentREGON);
}

bool Hent::HasREGON() const
{
	return !regon.GetValue().IsEmpty();
}


CString Hent::GetIdNo() const
{
	return idno.GetValue();
}

void Hent:: SetIdNo(const CString& hentIdNo)
{
	idno.SetTo(hentIdNo);
}

void Hent:: ChangeIdNo(const CString& hentIdNo)
{
	idno.ChangeTo(hentIdNo);
}


CDateTime Hent::GetIssueDate() const
{
	return issuedate.GetValue();
}

void Hent:: SetIssueDate(const CDateTime& hentIdIssueDate)
{
	issuedate.SetTo(hentIdIssueDate);
}

void Hent:: ChangeIssueDate(const CDateTime& hentIdIssueDate)
{
	issuedate.ChangeTo(hentIdIssueDate);
}


CString Hent::GetIssuePost() const
{
	return issuepost.GetValue();
}

void Hent:: SetIssuePost(const CString& hentIssuePost)
{
	issuepost.SetTo(hentIssuePost);
}

void Hent:: ChangeIssuePost(const CString& hentIssuePost)
{
	issuepost.ChangeTo(hentIssuePost);
}

CString Hent::GetAccountNo() const
{
	return accountno.GetValue();
}

void Hent:: SetAccountNo(const CString& hentAccountNo)
{
	accountno.SetTo(hentAccountNo);
}

void Hent:: ChangeAccountNo(const CString& hentAccountNo)
{
	accountno.ChangeTo(hentAccountNo);
}


CString Hent::GetBankName() const
{
	return bankname.GetValue();
}

void Hent:: SetBankName(const CString& hentBankName)
{
	bankname.SetTo(hentBankName);
}

void Hent:: ChangeBankName(const CString& hentBankName)
{
	bankname.ChangeTo(hentBankName);
}

CString Hent::GetComboItemText() const
{
	return GetAlias() + " , " + GetName();
}

CString Hent::GetWetLicenceNo() const
{
	return wetlicenceno.GetValue();
}

void Hent::SetWetLicenceNo(const CString& sWetLicenceNo)
{
	wetlicenceno.SetTo(sWetLicenceNo);
}

void Hent::ChangeWetLicenceNo(const CString& sWetLicenceNo)
{
	wetlicenceno.ChangeTo(sWetLicenceNo);
}

bool Hent::HasWetLicenceNo() const
{
	return !wetlicenceno.GetValue().IsEmpty();
}

CString Hent::GetCellPhoneNo() const
{
	return cellphoneno.GetValue();
}

void Hent::SetCellPhoneNo(const CString& sCellPhoneNo)
{
	cellphoneno.SetTo(sCellPhoneNo);
}

void Hent::ChangeCellPhoneNo(const CString& sCellPhoneNo)
{
	cellphoneno.ChangeTo(sCellPhoneNo);
}

CString Hent::GetEmailAddress() const
{
	return emailaddress.GetValue();
}

void Hent::SetEmailAddress(const CString& sEmailAddress)
{
	emailaddress.SetTo(sEmailAddress);
}

void Hent::ChangeEmailAddress(const CString& sEmailAddress)
{
	emailaddress.ChangeTo(sEmailAddress);
}

Latitude Hent::GetLatitude() const
{
	return latitude.GetValue();
}

void Hent::SetLatitude(const Latitude& lati)
{
	latitude.SetTo(lati);
}

void Hent::ChangeLatitude(const Latitude& lati)
{
	latitude.ChangeTo(lati);
}

Longitude Hent::GetLongitude() const
{
	return longitude.GetValue();
}

void Hent::SetLongitude(const Longitude& longi)
{
	longitude.SetTo(longi);
}

void Hent::ChangeLongitude(const Longitude& longi)
{
	longitude.ChangeTo(longi);
}

