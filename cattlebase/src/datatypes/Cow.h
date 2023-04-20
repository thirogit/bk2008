#ifndef __COW_H__
#define __COW_H__

#include "RawData.h"
#include "Stock.h"
#include "Hent.h"
#include "DataMember.h"
#include "DataMembersDefs.h"

#define NULL_INOUT_GRP (-1)
#define MAX_INOUT_GRP 9 //0..9
#define NULL_WEIGHT (-1)
#define NULL_COWID (0)

class Doc;
class RRInvoice;
class VATInvoice;
class Invoice;

typedef DataMember<Stock*,Stock*> StockDataMember;
typedef DataMember<Hent*,Hent*> HentDataMember;
typedef DataMember<Doc*,Doc*> DocDataMember;
typedef DataMember<RRInvoice*,RRInvoice*> RRInvoiceDataMember;
typedef DataMember<VATInvoice*,VATInvoice*> VATInvoiceDataMember;

class Cow : public RawData
{
public:
	
	Cow(int Id = NULL_COWID);
	~Cow();
	virtual void CopyFrom(const Cow &copy);
	
	int	GetId() const;
	void ResetId();

	Doc*			GetInDoc() const;
	void			SetInDoc(Doc* pInDoc);
	

	Weight			GetWeight() const;
	void			SetWeight(const Weight& aWeight);
	void			ChangeWeight(const Weight& aWeight);

	Stock*			GetStock() const;
	void			SetStock(Stock* pStock);
	void			ChangeStock(Stock* pStock);

	CString			GetEAN() const;
	void			SetEAN(const CString& EAN);
	void			ChangeEAN(const CString& EAN);

	CString			GetMotherEAN() const;
	void			SetMotherEAN(const CString& motherEAN);
	void			ChangeMotherEAN(const CString& motherEAN);
	
	CDateTime		GetBirthDate() const;
	void			SetBirthDate(const CDateTime& birthDt);
	void			ChangeBirthDate(const CDateTime& birthDt);
	
	CString			GetPassNo() const;
	void			SetPassNo(const CString& passNo);
	void			ChangePassNo(const CString& passNo);
		
	CDateTime		GetPassDate() const;	
	void			SetPassDate(const CDateTime& passDt);
	void			ChangePassDate(const CDateTime& passDt);

	CString			GetBirthplace() const;
	void			SetBirthPlace(const CString& birthPlace);
	void			ChangeBirthPlace(const CString& birthPlace);

	CString			GetExtras() const;
	void			SetExtras(const CString& sExtras);
	void			ChangeExtras(const CString& sExtras);


	Money			GetMyPrice() const;
	void			SetMyPrice(const Money& dMyPrice);
	void			ChangeMyPrice(const Money& dMyPrice);


	Hent*			GetFirstOwner() const;
	void			SetFirstOwner(Hent* pHent);
	void			ChangeFirstOwner(Hent* pHent);

	CowSex			GetSex() const;
	void			SetSex(const CowSex& cowsex);
	void			ChangeSex(const CowSex& cowsex);

	RRInvoice*		GetRRInvoice() const;
	void			SetRRInvoice(RRInvoice *pRRInvoice);
	void			DeleteRRInvoice();
	void			AssignToRRInvoice(RRInvoice *pRRInvoice);
	RRInvoice*		GetFirstRRInvoice() const;

	NullInt			GetOutGroup() const;
	void			SetOutGrp(int iOutGrp);
	void			ChangeOutGrp(int iOutGrp);

	NullInt			GetInGroup() const;
	void			SetInGrp(int iInGrp);
	void			ChangeInGrp(int iInGrp);
	
	Doc*			GetOutDoc() const;
	void			SetOutDoc(Doc* pOutDoc);
	void			DeleteOutDoc();
	void			AssignOutDoc(Doc* pOutDoc);
	
	Money			GetTermBuyPrice() const;
	void			SetTermBuyPrice(const Money& dTermBuyPrice) ;
	void			ChangeTermBuyPrice(const Money& dTermBuyPrice) ;

	Money			GetTermSellPrice() const;
	void			SetTermSellPrice(const Money& dTermSellPrice) ;
	void			ChangeTermSellPrice(const Money& dTermSellPrice) ;

	NullDouble		GetTermBuyWeight() const;
	void			SetTermBuyWeight(const NullDouble& buyWeight) ;
	void			ChangeTermBuyWeight(const NullDouble& buyWeight) ;

	NullDouble		GetTermSellWeight() const;
	void			SetTermSellWeight(const NullDouble& sellWeight);
	void			ChangeTermSellWeight(const NullDouble& sellWeight);

	Stock*			GetTermBuyStock() const;
	void			SetTermBuyStock(Stock* pBuyStock) ;
	void			ChangeTermBuyStock(Stock* pBuyStock) ;

	Stock*			GetTermSellStock() const;
	void			SetTermSellStock(Stock* pSellStock) ;
	void			ChangeTermSellStock(Stock* pSellStock) ;

	CString			GetPassImgLabel() const;
	void			SetPassImgLabel(const CString& sPassImgLabel);
	void			ChangePassImgLabel(const CString& sPassImgLabel);

	Latitude		GetLatitude() const;
	void			SetLatitude(const Latitude& lati);
	void			ChangeLatitude(const Latitude& lati);

	Longitude		GetLongitude() const;
	void			SetLongitude(const Longitude& longi);
	void			ChangeLongitude(const Longitude& longi);

	CString			GetHealthCertNo() const;
	void			SetHealthCertNo(const CString& sHealthCertNo);
	void			ChangeHealthCertNo(const CString& sHealthCertNo);

	VATInvoice*		GetSellVATInvoice() const;
	VATInvoice*		GetBuyVATInvoice() const;
	Hent*			GetDeliver() const;
	Hent*			GetBuyer() const;
	Money			GetActBuyPrice() const;
	Money			GetActSellPrice() const;	

	Money			GetBuyPrice() const;
	Money			GetSellPrice() const;
	NullDouble		GetBuyWeight() const;
	NullDouble		GetSellWeight()const;
	Stock*			GetBuyStock() const;
	Stock*			GetSellStock() const;
	Invoice*		GetBuyInvoice() const;
	
	bool			HasRRBuyInvoice() const ;
	bool			HasVATBuyInvoice()const ;
	bool			HasAnyBuyInvoice() const ;
	bool			HasVATSellInvoice() const ;
	bool			HasPassImg() const ;
	bool			HasFirstOwner() const ;
	bool			HasBirthDate() const ;


	bool			IsOut() const;
	bool			IsNew() const;

	void CommitChanges();
	void RollbackChanges();
	bool IsUncommited();

	bool InsertThisCow(bool bCommit = true);
	bool CanDeleteThisCow();
	bool CanEditThisCow();
	bool DeleteThisCow(bool bCommit = true);
	bool UpdateThisCow(bool bCommit = true);

protected:
	int animalid;
	Doc *docin;

	StringDataMember passimglabel;		
	RRInvoiceDataMember rrinv;
	WeightDataMember weight;
	StockDataMember stock;	
	StringDataMember ean;
	DateTimeDataMember birthdate;
	StringDataMember birthplace;
	CowSexDataMember sex;
	StringDataMember motherean;	
	StringDataMember passno;
	DateTimeDataMember passdate;
	HentDataMember fstownr;
	DocDataMember docout;
	StringDataMember extras;
	MoneyDataMember myprice;	
	NullIntDataMember outgrp;
	NullIntDataMember	ingrp;
	StockDataMember termbuystock;
	NullDoubleDataMember termbuyweight;
	StockDataMember termsellstock;
	NullDoubleDataMember termsellweight;
	MoneyDataMember termbuyprice;
	MoneyDataMember termsellprice;	
	LatitudeDataMember latitude;
	LongitudeDataMember longitude;
	StringDataMember healthcertno;

	TransactedDataMemberArray transactedMembers;
	UpatableDataMembers updatebleMembers;

};



#endif