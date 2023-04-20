#include "stdafx.h"
#include "Cow.h"
#include "../exceptions/SQLException.h"
#include "log.h"
#include "CattleSQL.h"
#include "consts.h"
#include "RRInvoice.h"
#include "VATInvoice.h"
#include "sqlupdate/sqldatamembers/SQLStockDataMember.h"
#include "sqlupdate/sqldatamembers/SQLStringDataMember.h"
#include "sqlupdate/sqldatamembers/SQLNullIntDataMember.h"
#include "sqlupdate/sqldatamembers/SQLMoneyDataMember.h"
#include "sqlupdate/sqldatamembers/SQLDateTimeDataMember.h"
#include "sqlupdate/sqldatamembers/SQLCowSexDataMember.h"
#include "sqlupdate/sqldatamembers/SQLHentDataMember.h"
#include "sqlupdate/sqldatamembers/SQLDocDataMember.h"
#include "sqlupdate/sqldatamembers/SQLInvoiceDataMember.h"
#include "sqlupdate/sqldatamembers/SQLGeoCoordinateDataMember.h"
#include "sqlupdate/sqldatamembers/SQLWeightDataMember.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Cow::Cow(int Id/* = NULL_COWID*/) : animalid(Id), stock(NULL),fstownr(NULL), 
		        docin(NULL),docout(NULL),weight(NULL_WEIGHT),
				myprice(),outgrp(NULL_INOUT_GRP),ingrp(NULL_INOUT_GRP),
				termbuystock(NULL),termbuyweight(NULL_WEIGHT),
                termsellstock(NULL),termsellweight(NULL_WEIGHT),
				termbuyprice(),termsellprice(),
				rrinv(NULL)
{
	transactedMembers.Add(&passimglabel);	
	transactedMembers.Add(&rrinv);
	transactedMembers.Add(&weight);
	transactedMembers.Add(&stock);	
	transactedMembers.Add(&ean);
	transactedMembers.Add(&birthdate);
	transactedMembers.Add(&birthplace);
	transactedMembers.Add(&sex);
	transactedMembers.Add(&motherean);	
	transactedMembers.Add(&passno);
	transactedMembers.Add(&passdate);
	transactedMembers.Add(&fstownr);
	transactedMembers.Add(&docout);
	transactedMembers.Add(&extras);
	transactedMembers.Add(&myprice);	
	transactedMembers.Add(&outgrp);
	transactedMembers.Add(&ingrp);
	transactedMembers.Add(&termbuystock);
	transactedMembers.Add(&termbuyweight);
	transactedMembers.Add(&termsellstock);
	transactedMembers.Add(&termsellweight);
	transactedMembers.Add(&termbuyprice);
	transactedMembers.Add(&termsellprice);
	transactedMembers.Add(&latitude);
	transactedMembers.Add(&longitude);
	transactedMembers.Add(&healthcertno);

	updatebleMembers.Add(new SQLColumnUpdate("STOCK",			new SQLStockDataMember(&stock))); 
	updatebleMembers.Add(new SQLColumnUpdate("EAN",				new SQLStringDataMember(&ean))); 
	updatebleMembers.Add(new SQLColumnUpdate("BIRTHDATE",		new SQLDateTimeDataMember(&birthdate,true))); 
	updatebleMembers.Add(new SQLColumnUpdate("BIRTHPLACE",		new SQLStringDataMember(&birthplace))); 
	updatebleMembers.Add(new SQLColumnUpdate("SEX",				new SQLCowSexDataMember(&sex))); 
	updatebleMembers.Add(new SQLColumnUpdate("MOTHEREAN",		new SQLStringDataMember(&motherean))); 
	updatebleMembers.Add(new SQLColumnUpdate("WEIGHT",			new SQLWeightDataMember(&weight,5))); 
	updatebleMembers.Add(new SQLColumnUpdate("PASSNO",			new SQLStringDataMember(&passno))); 
	updatebleMembers.Add(new SQLColumnUpdate("PASSDATE",		new SQLDateTimeDataMember(&passdate,true))); 
	updatebleMembers.Add(new SQLColumnUpdate("FSTOWNR",			new SQLHentDataMember(&fstownr))); 
	updatebleMembers.Add(new SQLColumnUpdate("DOCOUT",			new SQLDocDataMember(&docout))); 
	updatebleMembers.Add(new SQLColumnUpdate("EXTRAS",			new SQLStringDataMember(&extras))); 
	updatebleMembers.Add(new SQLColumnUpdate("MYPRICE",			new SQLMoneyDataMember(&myprice))); 
	updatebleMembers.Add(new SQLColumnUpdate("TERMBUYSTOCK",	new SQLStockDataMember(&termbuystock))); 
	updatebleMembers.Add(new SQLColumnUpdate("TERMBUYWEIGHT",	new SQLNullDoubleDataMember<NullDoubleDataMember>(&termbuyweight))); 
	updatebleMembers.Add(new SQLColumnUpdate("TERMSELLSTOCK",	new SQLStockDataMember(&termsellstock))); 
	updatebleMembers.Add(new SQLColumnUpdate("TERMSELLWEIGHT",	new SQLNullDoubleDataMember<NullDoubleDataMember>(&termsellweight))); 
	updatebleMembers.Add(new SQLColumnUpdate("TERMBUYPRICE",	new SQLMoneyDataMember(&termbuyprice))); 
	updatebleMembers.Add(new SQLColumnUpdate("TERMSELLPRICE",	new SQLMoneyDataMember(&termsellprice))); 
	updatebleMembers.Add(new SQLColumnUpdate("OUTGRP",			new SQLRawNullIntDataMember(&outgrp))); 
	updatebleMembers.Add(new SQLColumnUpdate("INGRP",			new SQLRawNullIntDataMember(&ingrp))); 
	updatebleMembers.Add(new SQLColumnUpdate("PASSIMGLABEL",	new SQLStringDataMember(&passimglabel))); 
	updatebleMembers.Add(new SQLColumnUpdate("RRINV",			new SQLRRInvoiceDataMember(&rrinv))); 
	updatebleMembers.Add(new SQLColumnUpdate("LATITUDE",		new SQLLatitudeDataMember(&latitude)));
	updatebleMembers.Add(new SQLColumnUpdate("LONGITUDE",		new SQLLongitudeDataMember(&longitude)));
	updatebleMembers.Add(new SQLColumnUpdate("HEALTHCERTNO",	new SQLStringDataMember(&healthcertno))); 


}

Cow::~Cow()
{
	updatebleMembers.FreeAndFlush();
}

void Cow::CommitChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Commit();

}
void Cow::RollbackChanges()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
		(*(it++))->Rollback();
}
bool Cow::IsUncommited()
{
	TransactedDataMemberArray::ArrayIterator it = transactedMembers.iterator();
	while(it.hasNext())
	{
		if((*(it++))->IsUncommited())
			return true;
	}
	return false;
}

void Cow::CopyFrom(const Cow &copy)
{
	RawData::Copy((RawData&)copy);

	animalid = copy.animalid;
	ean.CopyFrom(copy.ean);
	birthdate.CopyFrom(copy.birthdate);
	birthplace.CopyFrom(copy.birthplace);
	sex.CopyFrom(copy.sex);
	stock.CopyFrom(copy.stock);
	motherean.CopyFrom(copy.motherean);
	weight.CopyFrom(copy.weight);
	passno.CopyFrom(copy.passno);
	passdate.CopyFrom(copy.passdate);
	fstownr.CopyFrom(copy.fstownr);
	docin = copy.docin;
	docout.CopyFrom(copy.docout);
	extras.CopyFrom(copy.extras);
	myprice.CopyFrom(copy.myprice);

	termbuystock.CopyFrom(copy.termbuystock);
	termbuyweight.CopyFrom(copy.termbuyweight);
	termsellstock.CopyFrom(copy.termsellstock);
	termsellweight.CopyFrom(copy.termsellweight);
	termbuyprice.CopyFrom(copy.termbuyprice);
	termsellprice.CopyFrom(copy.termsellprice);
	outgrp.CopyFrom(copy.outgrp);
	ingrp.CopyFrom(copy.ingrp);
	passimglabel.CopyFrom(copy.passimglabel);
	rrinv.CopyFrom(copy.rrinv);
	latitude.CopyFrom(copy.latitude);
	longitude.CopyFrom(copy.longitude);
	healthcertno.CopyFrom(copy.healthcertno);
}

bool Cow::CanEditThisCow()
{
	if(animalid == NULL_COWID) return true; //new cow always allow edit
	return  CAN_DO(CANEDIT,this);
}
//***************************************************************************************
bool Cow::CanDeleteThisCow()
{
	return !IsOut() && !HasAnyBuyInvoice() && CAN_DO(CANDELETE,this);
}
//***************************************************************************************
bool Cow::DeleteThisCow(bool bCommit/*=true*/)
{
	if(!CanDeleteThisCow())
		return false;
	
	SQLQuery q;
    q << "DELETE FROM cattle WHERE animalid = " << GetId();
	
	LOG("DeleteThisCow() stmt: %s",q.str().c_str());
	SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::DELETED);
	if(SQLDB.Delete(q.str().c_str(),bCommit))
	{
		ResetId();		
		return true;
	}
	LOG("DeleteThisCow() error: %s",(LPCSTR)SQLDB.GetLastError());
	return false;

}
//***************************************************************************************
bool Cow::InsertThisCow(bool bCommit/*=true*/)
{

	if(animalid)  
	{
		SQLDB.SetLastError("This cow has not zero animalid, possibly it had beed already inserted.");
        return false;
	}

	char QueryStr[] = "INSERT INTO cattle(ean,birthdate,birthplace,sex,stock,motherean,"
					  "weight,passno,passdate,fstownr,docin,docout,extras,creator,"
					  "creattime,lastmodifier,lmodiftime,myprice,passimglabel,latitude,longitude,healthcertno)"
					  " VALUES(%0n,%1n,%2n,%3,%4,%5n,%6,%7n,%8n,%9,%10,NULL,%11n,%12,%13q,%14,%15q,%16,%17n,%18,%19,%20n);";


	SQLQuery q;
	q << QueryStr;
	q.parse();

	
	SQLQueryParms p;

	SQLDB.SetCurrentRawData(*this);


	p << SQLString(ean.GetValue()) 
		  << SQLString(birthdate.GetValue(),true)
		  << SQLString(birthplace.GetValue())
		  << SQLString(sex.GetValue())
		  << SQLString(stock.GetValue()->GetId())
		  << SQLString(motherean.GetValue())
		  << SQLString(weight.GetValue().GetDouble())
		  << SQLString(passno.GetValue())
		  << SQLString(passdate.GetValue(),true);

		if(HasFirstOwner())
			p  << SQLString(fstownr.GetValue()->GetId());
		else
			p << SQLStringNull();

		p << SQLString(docin->GetId())
          << SQLString(extras.GetValue())
		  << SQLString(creation.user->userid)
		  << SQLString(creation.eventDt)
		  << SQLString(modification.user->userid)
		  << SQLString(modification.eventDt);

		if(myprice.GetValue().IsNull())
			p << SQLStringNull();
		else
			p << SQLString(myprice.GetValue());

		p << SQLString(passimglabel.GetValue())
		  << SQLString(latitude.GetValue())
		  << SQLString(longitude.GetValue());

		p << SQLString(healthcertno.GetValue());

	try
	{
		LOG("InsertThisCow() query: %s",q.str(p).c_str());
		if(!SQLDB.Insert(q.str(p).c_str()))
		{
			  LOG("InsertThisCow() error: %s",(LPCSTR)SQLDB.GetLastError());
			  return false;
		}
		
		
		if((animalid = SQLDB.SelectScalar("SELECT MAX(animalid) FROM cattle",1)) < 0)
		{		
			SQLDB.Rollback();
			return false;
		}
	}
	catch(CSQLException *er)
	{		
		SQLDB.SetLastError(er->GetError());
		er->Delete();
		return false;
	}
	SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::ADDED);

	if(bCommit) SQLDB.Commit();
	return true;

}
bool Cow::UpdateThisCow(bool bCommit/*=true*/)
{
	if(IsUncommited())
	{
		SQLQuery q;
		q << "UPDATE cattle SET "; 

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

		q << " WHERE animalid = " << GetId();

		LOG("UpdateThisCow() stmt %s",q.str().c_str());

		if(!SQLDB.Update(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisCow() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

	}
	return true;
}
//*****************************************************************************
Weight Cow::GetWeight() const
{
	return weight.GetValue();		
}
void Cow::SetWeight(const Weight& aWeight)
{
	weight.SetTo(aWeight);
}
void  Cow::ChangeWeight(const Weight& aWeight)
{
	weight.ChangeTo(aWeight);
}
//*****************************************************************************
Stock* Cow::GetStock()  const
{
	return stock.GetValue();
}
void Cow::SetStock(Stock* pStock)
{
	stock.SetTo(pStock);
}
void Cow::ChangeStock(Stock* pStock)
{
	stock.ChangeTo(pStock);
}
//*****************************************************************************
CString Cow::GetEAN() const
{
	return ean.GetValue();
}
void Cow::SetEAN(const CString& EAN)
{
	ean.SetTo(EAN);
}
void Cow::ChangeEAN(const CString& EAN)
{
	ean.ChangeTo(EAN);
}
//*****************************************************************************
CString Cow::GetMotherEAN() const
{
	return motherean.GetValue();
}
void Cow::SetMotherEAN(const CString& motherEAN)
{
	motherean.SetTo(motherEAN);
}
void Cow::ChangeMotherEAN(const CString& motherEAN)
{
	motherean.ChangeTo(motherEAN);
}
//*****************************************************************************
CDateTime Cow::GetBirthDate() const
{
	return birthdate.GetValue();
}
void Cow::SetBirthDate(const CDateTime& birthDt)
{
	birthdate.SetTo(birthDt);
}
void Cow::ChangeBirthDate(const CDateTime& birthDt)
{
	birthdate.ChangeTo(birthDt);
}
//*****************************************************************************
CString Cow::GetPassNo() const
{
	return passno.GetValue();
}
void Cow::SetPassNo(const CString& passNo)
{
	passno.SetTo(passNo);
}
void Cow::ChangePassNo(const CString& passNo)
{
	passno.ChangeTo(passNo);
}
//*****************************************************************************
CDateTime Cow::GetPassDate() const
{
	return passdate.GetValue();
}
void Cow::SetPassDate(const CDateTime& passDt)
{
	passdate.SetTo(passDt);
}
void Cow::ChangePassDate(const CDateTime& passDt)
{
	passdate.ChangeTo(passDt);
}
//*****************************************************************************
CString Cow::GetBirthplace() const
{
	return birthplace.GetValue();
}
void Cow::SetBirthPlace(const CString& birthPlace)
{
	birthplace.SetTo(birthPlace);
}
void Cow::ChangeBirthPlace(const CString& birthPlace)
{
	birthplace.ChangeTo(birthPlace);
}
//*****************************************************************************
CString Cow::GetExtras() const
{
	return extras.GetValue();
}
void Cow::SetExtras(const CString& sExtras)
{
	extras.SetTo(sExtras);
}
void Cow::ChangeExtras(const CString& sExtras)
{
	extras.ChangeTo(sExtras);
}
//*****************************************************************************
Money Cow::GetMyPrice() const
{
	return myprice.GetValue();
}
void Cow::SetMyPrice(const Money& dMyPrice)
{
	myprice.SetTo(dMyPrice);
}
void Cow::ChangeMyPrice(const Money& dMyPrice)
{
	myprice.ChangeTo(dMyPrice);
}
//*****************************************************************************
Hent* Cow::GetFirstOwner() const
{
	return fstownr.GetValue();
}
void Cow::SetFirstOwner(Hent* pHent)
{
	fstownr.SetTo(pHent);
}
void Cow::ChangeFirstOwner(Hent* pHent)
{
	fstownr.ChangeTo(pHent);
}
//*****************************************************************************
CowSex Cow::GetSex() const
{
	return sex.GetValue();
}
void Cow::SetSex(const CowSex& cowsex)
{
	sex.SetTo(cowsex);
}
void Cow::ChangeSex(const CowSex& cowsex)
{
	sex.ChangeTo(cowsex);
}
//*****************************************************************************
RRInvoice*	Cow::GetRRInvoice() const
{
	RRInvoice *pRRInvoice = rrinv.GetValue(); 
	if(pRRInvoice) 
		return pRRInvoice->GetLastGoodRRInvoice();

	return NULL;
}

RRInvoice* Cow::GetFirstRRInvoice() const
{
	return rrinv.GetValue();
}

void Cow::SetRRInvoice(RRInvoice *pRRInvoice)
{
	rrinv.SetTo(pRRInvoice);
}
void Cow::DeleteRRInvoice()
{
	rrinv.ChangeTo(NULL);
}
void Cow::AssignToRRInvoice(RRInvoice *pRRInvoice)
{
	rrinv.ChangeTo(pRRInvoice);
}
//*****************************************************************************
NullInt Cow::GetOutGroup() const
{
	return outgrp.GetValue();
}
void Cow::SetOutGrp(int iOutGrp)
{
	outgrp.SetTo(NullInt(iOutGrp,NULL_INOUT_GRP));
}
void Cow::ChangeOutGrp(int iOutGrp)
{
	outgrp.ChangeTo(NullInt(iOutGrp,NULL_INOUT_GRP));
}
//*****************************************************************************
NullInt Cow::GetInGroup() const
{
	return ingrp.GetValue();
}
void Cow::SetInGrp(int iInGrp)
{
	ingrp.SetTo(NullInt(iInGrp,NULL_INOUT_GRP));
}
void Cow::ChangeInGrp(int iInGrp)
{
	ingrp.ChangeTo(NullInt(iInGrp,NULL_INOUT_GRP));
}
//*****************************************************************************
Doc* Cow::GetOutDoc() const
{
	return docout.GetValue();
}
void Cow::SetOutDoc(Doc* pOutDoc)
{
	docout.SetTo(pOutDoc);
}
void Cow::DeleteOutDoc()
{
	docout.ChangeTo(NULL);
}
void Cow::AssignOutDoc(Doc* pOutDoc)
{
	docout.ChangeTo(pOutDoc);
}
//*****************************************************************************
Money Cow::GetTermBuyPrice() const
{
	return termbuyprice.GetValue();
}
void Cow::SetTermBuyPrice(const Money& dTermBuyPrice) 
{
	termbuyprice.SetTo(dTermBuyPrice);
}
void Cow::ChangeTermBuyPrice(const Money& dTermBuyPrice) 
{
	termbuyprice.ChangeTo(dTermBuyPrice);
}
//*****************************************************************************
Money Cow::GetTermSellPrice() const
{
	return termsellprice.GetValue();
}
void Cow::SetTermSellPrice(const Money& dTermSellPrice) 
{
	termsellprice.SetTo(dTermSellPrice);
}
void Cow::ChangeTermSellPrice(const Money& dTermSellPrice) 
{
	termsellprice.ChangeTo(dTermSellPrice);
}
//*****************************************************************************
NullDouble Cow::GetTermBuyWeight() const
{
	return termbuyweight.GetValue();
}
void Cow::SetTermBuyWeight(const NullDouble& buyWeight)
{
	termbuyweight.SetTo(buyWeight);
}
void Cow::ChangeTermBuyWeight(const NullDouble& buyWeight)
{
	termbuyweight.ChangeTo(buyWeight);
}
//*****************************************************************************
NullDouble Cow::GetTermSellWeight()const
{
	return termsellweight.GetValue();
}
void Cow::SetTermSellWeight(const NullDouble& sellWeight)
{
	termsellweight.SetTo(sellWeight);
}
void Cow::ChangeTermSellWeight(const NullDouble& sellWeight)
{
	termsellweight.ChangeTo(sellWeight);
}
//*****************************************************************************
Stock* Cow::GetTermBuyStock() const
{
	return termbuystock.GetValue();
}
void Cow::SetTermBuyStock(Stock* pBuyStock)
{
	termbuystock.SetTo(pBuyStock);
}
void Cow::ChangeTermBuyStock(Stock* pBuyStock)
{
	termbuystock.ChangeTo(pBuyStock);
}
//*****************************************************************************
Stock* Cow::GetTermSellStock() const
{
	return termsellstock.GetValue();
}
void Cow::SetTermSellStock(Stock* pSellStock)
{
	termsellstock.SetTo(pSellStock);
}
void Cow::ChangeTermSellStock(Stock* pSellStock)
{
	termsellstock.ChangeTo(pSellStock);
}
//*****************************************************************************
CString	Cow::GetPassImgLabel() const
{
	return passimglabel.GetValue();
}
void Cow::SetPassImgLabel(const CString& sPassImgLabel)
{
	passimglabel.SetTo(sPassImgLabel);
}
void Cow::ChangePassImgLabel(const CString& sPassImgLabel)
{
	passimglabel.ChangeTo(sPassImgLabel);
}
//*****************************************************************************
Latitude Cow::GetLatitude() const
{
	return latitude.GetValue();
}

void Cow::SetLatitude(const Latitude& lati)
{
	latitude.SetTo(lati);
}

void Cow::ChangeLatitude(const Latitude& lati)
{
	latitude.ChangeTo(lati);
}

Longitude Cow::GetLongitude() const
{
	return longitude.GetValue();
}

void Cow::SetLongitude(const Longitude& longi)
{
	longitude.SetTo(longi);
}

void Cow::ChangeLongitude(const Longitude& longi)
{
	longitude.ChangeTo(longi);
}


CString	Cow::GetHealthCertNo() const
{
	return healthcertno.GetValue();
}

void Cow::SetHealthCertNo(const CString& sHealthCertNo)
{
	healthcertno.SetTo(sHealthCertNo);
}

void Cow::ChangeHealthCertNo(const CString& sHealthCertNo)
{
	healthcertno.ChangeTo(sHealthCertNo);
}


bool Cow::HasPassImg() const
{
	return !GetPassImgLabel().IsEmpty();
}
Money Cow::GetActBuyPrice() const
{
	Money buyprice = GetBuyPrice();
	RRInvoice* pRRInv = GetRRInvoice();
	if(	(pRRInv && pRRInv->GetPayWay() == Invoice::PAY_CASH))
	{
		return (buyprice + buyprice*pRRInv->GetVATRate());
	}
	return buyprice;
}

Money Cow::GetActSellPrice() const
{
	return GetSellPrice();
}
Invoice* Cow::GetBuyInvoice() const
{
	if(HasAnyBuyInvoice())
	{
		Invoice* pInvoice = GetRRInvoice();
		if(pInvoice)
			return pInvoice;
		else
			return GetBuyVATInvoice();
	}
	return NULL;
}

Money Cow::GetBuyPrice()  const
{
	Invoice* pInvoice = GetBuyInvoice();
	if(pInvoice != NULL)
	{
		CowInvoiceEntry* pEntry = pInvoice->GetEntryForCow(this);
		return pEntry->GetPrice();
	}
	return Money();
}
Money		Cow::GetSellPrice()  const
{
	Invoice* sellInvoice = GetSellVATInvoice();
	if(sellInvoice)
		return GetSellVATInvoice()->GetEntryForCow(this)->GetPrice();

	return Money();
}
NullDouble		Cow::GetBuyWeight()  const
{
	Invoice* pInvoice = GetBuyInvoice();
	if(pInvoice != NULL)
	{
		CowInvoiceEntry* pEntry = pInvoice->GetEntryForCow(this);
		return NullDouble(pEntry->GetWeight().GetDouble());
	}
	return NullDouble();
}
NullDouble	Cow::GetSellWeight()  const
{

	Invoice* pInvoice = GetSellVATInvoice();
	if(pInvoice != NULL)
	{
		CowInvoiceEntry* pEntry = pInvoice->GetEntryForCow(this);
		return NullDouble(pEntry->GetWeight().GetDouble());
	}
	return NullDouble();
	
}
Stock*	Cow::GetBuyStock()  const
{
	Invoice* pInvoice = GetBuyInvoice();
	if(pInvoice != NULL)
	{
		CowInvoiceEntry* pEntry = pInvoice->GetEntryForCow(this);
		return pEntry->GetStock();
	}
	return NULL;
}
Stock*	Cow::GetSellStock()  const
{
	return GetSellVATInvoice()->GetEntryForCow(this)->GetStock();
}

bool Cow::HasAnyBuyInvoice() const
{
	return GetRRInvoice() != NULL || GetInDoc()->HasInvoice();
}


bool Cow::IsOut() const
{
	return docout.GetValue() != NULL;
}
bool Cow::IsNew() const
{
	return animalid == NULL_COWID;
}

int Cow::GetId() const
{
	return animalid;
}

void Cow::ResetId()
{
	animalid = 0;
}

Doc* Cow::GetInDoc() const
{
	return docin;
}
void Cow::SetInDoc(Doc* pInDoc)
{
	docin = pInDoc;
}

bool Cow::HasFirstOwner() const
{
	return GetFirstOwner() != NULL;
}

Hent* Cow::GetDeliver() const
{
	return docin->GetHent();
}
Hent* Cow::GetBuyer() const
{
	if(IsOut())
		return GetOutDoc()->GetHent();
	return NULL;
}
bool Cow::HasRRBuyInvoice() const
{
	return GetRRInvoice() != NULL;
}

bool Cow::HasVATBuyInvoice()const
{
	return docin->HasInvoice();
}
bool Cow::HasVATSellInvoice() const
{
	if(IsOut())
	{
		return GetOutDoc()->HasInvoice();
	}
	return false;
}

bool Cow::HasBirthDate() const
{
	return !GetBirthDate().IsNull();
}

VATInvoice* Cow::GetSellVATInvoice() const
{
	if(IsOut())
	{
		VATInvoice* pVATInvoice = GetOutDoc()->GetInvoice();
		if(pVATInvoice != NULL)
			return pVATInvoice->GetLastGoodVATInvoice();
	}
	return NULL;
}
VATInvoice* Cow::GetBuyVATInvoice() const
{
	VATInvoice* pVATInvoice = docin->GetInvoice();
	if(pVATInvoice)
		return pVATInvoice->GetLastGoodVATInvoice();
	return NULL;
}

