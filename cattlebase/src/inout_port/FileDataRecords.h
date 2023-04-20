#ifndef __FILEDATARECORDS_H__INCLUDED
#define __FILEDATARECORDS_H__INCLUDED

#include "../datalimits.h"

#pragma pack (1)

#define NIP_LENGTH 10
#define EAN_LENGTH 14
#define WETNO_LENGTH 8
#define PESEL_LENGTH 11
#define REGON_LENGTH 14
#define IDNO_LENGTH 12
#define ACCOUNTNO_LENGTH 26
#define PASSNO_LENGTH 10

#define EXPORTIMPORT_FILE_ID "BKB"

struct FLAT_GEO_COORD
{
	unsigned short degrees;
	unsigned short minutes;
	unsigned short seconds;
	unsigned char hemisphere;
	bool null;
};

struct FLAT_DATE
{
	int year;
	int month;
	int day;
	int hour;
	int minutes;
	int seconds;
	bool null;
};

struct HENT_FILERECORD
{
	int hentid;
	int country;
	int henttype;
	FLAT_DATE issuedate;
	char name[MAXNAME+1];
	char alias[MAXALIAS+1];
	char street[MAXSTREET+1];
	char pobox[MAXPOBOX+1];
	char city[MAXCITY+1];
	char zip[MAXZIP+1];
	char phone[MAXPHONE+1];
	char nip[NIP_LENGTH+1];
	char arimrno[EAN_LENGTH+1];
	char wetno[WETNO_LENGTH+1];
	char extras[MAXEXTRAS+1];	
	char plate[MAXPLATE+1];	
	char pesel[PESEL_LENGTH+1];
	char regon[REGON_LENGTH+1];
	char idno[IDNO_LENGTH+1];	
	char issuepost[MAXISSUEPOST+1];
	char accountno[ACCOUNTNO_LENGTH+1];
	char bankname[MAXBANKNAME+1];
	char cellphone[MAXCELLPHONE+1];
	char emailaddress[MAXEMAILADDRESS+1];
	char wetlicenceno[MAXWETLICENCENO+1];
	FLAT_GEO_COORD longitude;
	FLAT_GEO_COORD latitude;
};


struct FILEHEADER
{
	char fileid[3];
	DWORD checksum;
	HENT_FILERECORD company;
	int stockcount;
	int hentcount;
	int docscount;
	int rrinv_count;
	int cowpasscount;	
};

struct PASSIMG_FILERECORDHEADER
{
	int cowid;
	ULONGLONG imgsize;
};

struct STOCK_FILERECORD
{
	int stockid;
	int predefsex;	
	char stockname[MAXSTOCKNAME+1];
	char stockcode[MAXSTOCKCODE+1];	
};


struct COW_FILERECORD
{
	int animalid;
	char ean[EAN_LENGTH+1];
	FLAT_DATE birthdate;
	char birthplace[MAXBIRTHPLACE];
	int sex;
	int stock;
	char motherean[EAN_LENGTH+1];
	double weight;
	char passno[PASSNO_LENGTH+1];
	FLAT_DATE passdate;
	int fstownr;
	char extras[MAXEXTRAS+1];
	double myprice;
	int ingrp;	
	bool haspassimg;
	int rrinvoiceid;
	FLAT_GEO_COORD longitude;
	FLAT_GEO_COORD latitude;
};

struct DOC_FILERECORD_HEADER
{
	int docid;
	int hent;
	FLAT_DATE docdate;
	FLAT_DATE loaddate;
	char extras[MAXEXTRAS+1];
	char plateno[MAXPLATE+1];	
	int reason;
	int invoice;
	int cowcount;
	int expensecount;
	int loadstart;
	int loadend;
};


struct DOC_FILERECORD
{
	DOC_FILERECORD_HEADER header;
	COW_FILERECORD cows[1];
};



struct INVOICE_FILERECORD_HEADER
{
	int invoiceid;	
	char custom_number[MAXCUSTOMNUMBER+1];
	char payway;
	double vat_rate;
	double invoicevalue;
	FLAT_DATE invoice_date;
	int payduedays;
	char extras[MAXEXTRAS+1];
	int entriescount;
};
struct COWINVOICEENTRY_FILERECORD
{
	int cowid;
	double weight;
	int stock;
	double price;
};

struct INVOICE_FILERECORD
{
	INVOICE_FILERECORD_HEADER header;
	COWINVOICEENTRY_FILERECORD entries[1];
};






#endif