#ifndef __SYNCHELPER_H__
#define __SYNCHELPER_H__

#include "DataTypes.h"


#define REJ_CLR_REPAIRABLE RGB(0,255,64)
#define REJ_CLR_INREPAIRABLE RGB(223,0,0)
#define REJ_CLR_REPAIRED RGB(81,81,255)

class CSyncCowData;

typedef PtrFlushArray<CSyncCowData> CSyncCowDataArray;

class CSyncProcessedStatus
{
public:
	enum SYNC_PROCESSES_STATUS
	{		
		PROCESSED_NOERROR = 1000,
		GOT_ERROR,
		NOT_PROCESSED
	};
public:
	CSyncProcessedStatus() : syncStatus(NOT_PROCESSED) {};


	CString syncError;
	SYNC_PROCESSES_STATUS syncStatus;
};


class CSyncDocData : public CSyncProcessedStatus
{
public:

	CSyncDocData() : docno(0),hent(NULL),
		reason(NULL),rejectReason(NOT_REJECTED),hentid(0),reasonid(0),srcDoc(NULL)
	{};
	int docno;
	Doc *srcDoc;
	CDateTime docdate;
	CDateTime loaddate;
	int hentid;
	Hent *hent;
	CString plateno;
	int reasonid;
	Reason *reason;
	

	CSyncCowDataArray cowArray;

	enum REJECT_REASON
	{
		NOT_REJECTED = 0,
		DOCNO_NOT_FOUND = 1,
		HENT_NOT_FOUND = 2,
		REASON_NOT_FOUND = 4,
		NO_COWS = 8,
		HENT_NOT_FOUND_REPAIRED = 16,
		REASON_NOT_FOUND_REPAIRED = 32,		
	};
	
	UINT rejectReason;
};


class CSyncCowData : public CSyncProcessedStatus
{
public:

	CSyncCowData() : stock(NULL),weight(-1),
		myprice(),docin(NULL),//docout(NULL),
		rejectReason(NOT_REJECTED),stockid(0),docinno(0),docoutno(0),
		sync_docin(NULL),sync_docout(NULL),srcCow(NULL),termbuystock(NULL),
		termbuyweight(NULL_WEIGHT),	termsellstock(NULL),termsellweight(NULL_WEIGHT),
		termbuyprice(),termsellprice(),termsellstockid(0),
		termbuystockid(0) {};


	Cow *srcCow;
	CString ean;
	CowSex sex;
	int stockid;
	Stock *stock;
	Weight weight;
	Money myprice;
	int docinno;
	Doc *docin;
	CSyncDocData *sync_docin;
	int docoutno;
	//Doc *docout;
	CSyncDocData *sync_docout;
	int outgrp;
	int ingrp;

	int termbuystockid;
	Stock* termbuystock;
	NullDouble termbuyweight;
	int termsellstockid;
	Stock* termsellstock;
	NullDouble termsellweight;
	Money termbuyprice;
	Money termsellprice;

	enum REJECT_REASON
	{
		NOT_REJECTED = 0,
		EAN_NOT_FOUND = 1,
		STOCK_NOT_FOUND = 2,
		DOCIN_NOT_FOUND = 4,
		DOCOUT_NOT_FOUND = 8,
		DUPLICATE_NOTSOLD_FOUND = 16,
		STOCK_NOT_FOUND_REPAIRED = 32,
		CHNGS_VIOLATE_INVOICE = 64,
		CHNGS_DROPPED = 128,
		TERMBUYSTOCK_NOT_FOUND = 256,
		TERMBUYSTOCK_NOT_FOUND_REPAIRED = 512,
		TERMSELLSTOCK_NOT_FOUND = 1024,
		TERMSELLSTOCK_NOT_FOUND_REPAIRED = 2048,
	};
	
	UINT rejectReason;
};


class CSyncDocDataArray : public PtrFlushArray<CSyncDocData>
{
public:
		CSyncDocData* GetDoc(int docNo)
		{
			int i,s = GetSize();
			CSyncDocData *pSyncDoc = NULL;
			for(i = 0;i < s;i ++)
			{
					pSyncDoc = GetAt(i);
					if(pSyncDoc->docno == docNo)
						return pSyncDoc;
			}
			return NULL;
		}
};



class SyncStringFmtHelper
{
public:
	static void FormatDate(char *outBuff,int maxsize,const CDateTime & aDate);
	static CString FormatString(const CString & aString,bool unPolish = true);
	static void FormatInt(char *outBuff,int maxsize,const NullInt & aInt);
	static void FormatDouble(char *outBuff,int maxsize,const NullDouble& aDouble);
	static CString EncodeDoubleQuotes(const CString & aString);
	static CString DecoceEscapeString(const CString & aString);	
	static CString EncodeSpecialCharacters(const CString& inString);
	static bool IsSpecialCharacter(char c);
private:
	static const char quotedFmt[];	
};

#endif