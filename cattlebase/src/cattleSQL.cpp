#include "stdafx.h"
#include "cattleSQL.h"
#include "exceptions/SQLException.h"
#include "exceptions/StrException.h"
#include "missing/util.h"
#include "missing/paths.h"
#include "log.h"
#include "global_arrays.h"

#include "datatypes/RRInvoice.h"
#include "datatypes/VATInvoice.h"
#include "datatypes/NullInvoice.h"
#include "query/equal_list.h"

#include "classes/XTokenString.h"

#include <odbcinst.h>
#ifdef _WIN32
#include <afxdb.h>
#else
#include <sql.h>
#include <sqlext.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAJOR_DB_VERSION_REQUIRED (2011)
#define MINOR_DB_VERSION_REQUIRED (23)

/////////////////////////////////////////////////////////////////////////////////////////
CDBVersion::CDBVersion(UINT majorVer,UINT minorVer) : m_majorVersion(majorVer),m_minorVersion(minorVer)
{

}

CDBVersion::CDBVersion(const CDBVersion& copy)
{
	*this = copy;
}

CDBVersion& CDBVersion::operator=(const CDBVersion& right)
{
	m_majorVersion = right.m_majorVersion;
	m_minorVersion = right.m_minorVersion;
	return *this;
}


UINT CDBVersion::GetMajorVersion() const
{
	return m_majorVersion;
}

UINT CDBVersion::GetMinorVersion() const
{
	return m_minorVersion;
}

bool CDBVersion::operator>(const CDBVersion& right) const
{
	if(m_majorVersion > right.m_majorVersion)
	{
		return true;
	}
	else if(m_majorVersion == right.m_majorVersion)
	{
		return m_minorVersion > right.m_minorVersion;
	}
	return false;
}

bool CDBVersion::operator<(const CDBVersion& right) const
{
	return right.operator>(*this);
}

bool CDBVersion::operator>=(const CDBVersion& right) const
{
	if(*this == right)
		return true;

	return *this > right;
}

bool CDBVersion::operator<=(const CDBVersion& right) const
{
	if(*this == right)
		return true;

	return *this < right;
}

bool CDBVersion::operator==(const CDBVersion& right) const
{
	return (m_majorVersion == right.m_majorVersion && m_minorVersion == right.m_minorVersion);
		
}

bool CDBVersion::operator!=(const CDBVersion& right) const
{
	return !(*this == right);
}
/////////////////////////////////////////////////////////////////////////////////////////

const char DB_FILE[] = "CATTLE.FDB";
//const char DB_HOSTNAME[] = "localhost";
const char DB_LOGIN[] = "SYSDBA";
const char DB_PASSWORD[]  = "masterkey";
const char DB_CLIENT[] = "fb\\fbembed.dll";


const char *connectStringFmt =
	"DRIVER=Firebird/InterBase(r) driver;"
	"UID=%s;"
	"PWD=%s;"
	"CLIENT=%s\\%s;"
	"DBNAME=%s\\%s";

const char ErrColBounds[] = "Column index out of bounds";
const char ErrUnknCol[]   = "Unknown column '%s' in field list";

CattleSQL CattleSQL::m_DB;
//***************************************************************************************
CattleSQL& CattleSQL::GetDB()
{
	return m_DB;
}

//***************************************************************************************
CattleSQL::CattleSQL() : m_bOppened(false),m_env(NULL)
{	
	SQLAllocHandle (SQL_HANDLE_ENV, NULL, &m_env);
	SQLRETURN result = SQLSetEnvAttr (m_env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_UINTEGER);
	if(!SQL_SUCCEEDED(result))
	{	
		m_env = NULL;
	}
}

//***************************************************************************************
CattleSQL:: ~CattleSQL()
{
	if(m_bOppened) 
		CloseCattleBase();
	if(m_env)
		SQLFreeHandle (SQL_HANDLE_ENV, m_env);		

}
//***************************************************************************************
void CattleSQL::ErrMsg (SQLHANDLE handle, int handleType)
{

UCHAR	sqlState [6], text [SQL_MAX_MESSAGE_LENGTH];
SDWORD	nativeCode;
SWORD	textLength;

	text [0] = 0;
	sqlState [0] = 0;
	sqlState [5] = 0;
	SQLGetDiagRec (handleType, handle, 1, sqlState, &nativeCode, text, sizeof (text) -1, &textLength);
	m_err.Format("%s: %s",sqlState,text);

}

//***************************************************************************************
bool CattleSQL::OpenCattleBase(LPCSTR szBaseDir/* = NULL*/,LPCSTR szBaseFileName/* = NULL*/)
{
	CString connectionString;
	int ret;
	UCHAR buffer [128];
	SWORD bufferLength;
	HDBC connection;
	LPCSTR pszBaseFileName = szBaseFileName ? szBaseFileName : (DB_FILE);

	if(!m_bOppened)
	{
		CString strBaseDir,szAppDir = CUtil::GetAppDir();
		
		if(szBaseDir)
			strBaseDir = szBaseDir;
		else
			strBaseDir = szAppDir;

		m_dbFilePath = paths_ConcatPath(strBaseDir,pszBaseFileName);

		LOG("Loading %s, from %s",pszBaseFileName,(LPCSTR)strBaseDir);


		connectionString.Format(connectStringFmt,DB_LOGIN,DB_PASSWORD,(LPCSTR)szAppDir,DB_CLIENT,(LPCSTR)strBaseDir,pszBaseFileName);
		
	
		ret = SQLAllocHandle (SQL_HANDLE_DBC, m_env, &connection);
		if(!SQL_SUCCEEDED(ret))
		{				
			ErrMsg(m_env,SQL_HANDLE_ENV);
			return false;
		}

		ret = SQLSetConnectAttr (connection, SQL_ATTR_ODBC_CURSORS, (SQLPOINTER) SQL_CUR_USE_ODBC, 0);
		if(!SQL_SUCCEEDED(ret))
		{
			
			ErrMsg(connection, SQL_HANDLE_DBC);
			SQLFreeHandle (SQL_HANDLE_DBC, connection);
			return false;
  
		}

		//TRACE("szBuff = %s\n",szBuff);

		ret = SQLDriverConnect (connection, hWnd, 
                            (UCHAR*) (LPCSTR)connectionString, SQL_NTS,
                            buffer, sizeof (buffer), &bufferLength,
                            SQL_DRIVER_NOPROMPT);
		if(!SQL_SUCCEEDED(ret))
		{

			ErrMsg(connection, SQL_HANDLE_DBC);
			SQLFreeHandle (SQL_HANDLE_DBC, connection);
			return false;
		}
		//SQL_AUTOCOMMIT_ON
		ret = SQLSetConnectAttr (connection, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF , 0);
		if(!SQL_SUCCEEDED(ret))
		{

			ErrMsg(connection, SQL_HANDLE_DBC);
			SQLFreeHandle (SQL_HANDLE_DBC, connection);
			return false;
		}
    
		m_Connection = connection;
		m_bOppened = true;		
	
	}
	return true;
}
//***************************************************************************************
bool CattleSQL::CloseCattleBase()
{
	if(m_bOppened)
	{
		
		int ret = SQLEndTran (SQL_HANDLE_DBC, m_Connection, SQL_COMMIT);
		
		if(!SQL_SUCCEEDED(ret))
		{
			ErrMsg(m_Connection, SQL_HANDLE_DBC);
			return false;
		}

		ret = SQLDisconnect (m_Connection);
		if(!SQL_SUCCEEDED(ret))
		{
			ErrMsg(m_Connection, SQL_HANDLE_DBC);
			return false;
		}
		m_bOppened = false;
		SQLFreeHandle (SQL_HANDLE_DBC, m_Connection);
		
		m_Connection = NULL;
	}
    
	
	return true;
}

CDBVersion CattleSQL::GetDBVersion()
{
	CDBVersion nullVersion(0,0);
	SQLQuery q;
	q << "SELECT majorver,minorver FROM versions WHERE versionof='DATABASE';";
	SQLResult res;

	int colMAJVER,colMINVER;
	
	if(!Select(q.str().c_str(),res)) return nullVersion;

	colMAJVER = res.ColIndex("MAJORVER");
	colMINVER = res.ColIndex("MINORVER");
	
	if(!res.nextRow()) return nullVersion;

	UINT majVer = res.getColInt(colMAJVER);
	UINT minVer = res.getColInt(colMINVER);

	return CDBVersion(majVer,minVer);	
}

const CDBVersion CattleSQL::GetRequiredDBVersion()
{
	return CDBVersion(MAJOR_DB_VERSION_REQUIRED,MINOR_DB_VERSION_REQUIRED);
}

bool CattleSQL::RequiresUpdate()
{
	const CDBVersion requiredDbVersion = GetRequiredDBVersion();
	if(m_bOppened)
	{
		CDBVersion oppenedDbVersion = GetDBVersion();
		if(oppenedDbVersion < requiredDbVersion)
			return true;
		else if(oppenedDbVersion > requiredDbVersion)
			throw new CSQLException("Database is newer then expected, expected version: %d.%d",
									requiredDbVersion.GetMajorVersion(),
									requiredDbVersion.GetMinorVersion());

	}
	return false;
}

bool CattleSQL::UpdateToRequired()
{
	CDBVersion requiredDbVersion = GetRequiredDBVersion();
	CDBVersion currentDbVersion = GetDBVersion();
	DBUPDATE* pDbUpdate = &_dbUpdates[0];
	while(pDbUpdate->pszUpdateStmts)
	{
		CDBVersion dbVersion(pDbUpdate->uMajorVersion,pDbUpdate->uMinorBersion);
		if(dbVersion <= requiredDbVersion && dbVersion > currentDbVersion)
		{
			if(!DoDbUpdate(pDbUpdate))
			{
				LOG("Error while appling version %d,%d - %s",pDbUpdate->uMajorVersion,pDbUpdate->uMinorBersion,(LPCSTR)GetLastError());
				Rollback();
				return false;
			}
			Commit();
		}
		pDbUpdate++;
	}

	
	return true;
}

bool CattleSQL::DoDbUpdate(const DBUPDATE* pDbUpdate)
{
	char versionUpdateStmtTemplate[] = "UPDATE versions SET majorver = %0,minorver = %1 WHERE VERSIONOF = 'DATABASE'";

	
	CStringArray saUpdatesStmts;
	XTokenString(pDbUpdate->pszUpdateStmts,
				 ";",
				 saUpdatesStmts,
				 0,
				 FALSE,
				 TRUE,
				 TRUE,
				 FALSE);

	bool bResult = true;
	for(int iUpdateStmt = 0;iUpdateStmt < saUpdatesStmts.GetCount();iUpdateStmt++)
	{
		LPCSTR szUpdateStmt = saUpdatesStmts.GetAt(iUpdateStmt);
		HSTMT statement = GetStatement(szUpdateStmt);
		if(statement == NULL)
		{		
			bResult = false;
			break;
		}

		
		SQLRETURN ret = ExecuteStatement (statement);
		if(!SQL_SUCCEEDED(ret) && ret != SQL_NO_DATA)
		{	
			ErrMsg (statement);
			FreeStatement(statement);
			bResult =  false;
			break;
		}		
		
	}

	if(bResult)
	{
		SQLQuery updateDbVersionQ;
		updateDbVersionQ << versionUpdateStmtTemplate;
		updateDbVersionQ.parse();

		bResult = bResult && Update(updateDbVersionQ.str(pDbUpdate->uMajorVersion,pDbUpdate->uMinorBersion).c_str(),false);
	}

	return bResult;

}


bool CattleSQL::LoadBasics(SQLResult &res,RawData &base,CUserDataArray &users)
{
	int user;

	char szErr1[] = "Creator user ID(%d) not found";
	char szErr2[] = "Last modifier user ID(%d) not found";
			

	user = res.getColInt("creator");
	if((base.creation.user = users.GetPtrByUserId(user)) == NULL)
	{
		m_err.Format(szErr1,user);
		return false;
	}
	base.creation.eventDt  = res.getColDateTime("creattime");

	user = res.getColInt("lastmodifier");

	if((base.modification.user = users.GetPtrByUserId(user)) == NULL)
	{
		m_err.Format(szErr2,user);
		return false;
	}
	base.modification.eventDt  = res.getColDateTime("lmodiftime");

	return true;
}
//***************************************************************************************
bool CattleSQL::LoadReasons(CReasonDataArray &reasons,ITaskProgres *progress/* = NULL*/)
{
	SQLResult res;
	Reason *one = NULL;
	SQLQuery q;


	int colREASONID,colREASONCODE,colREASONNAME,colINOUTREASON;

	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM inoutreasons",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_REASONS,steps);

	reasons.RemoveAll();
	q << "SELECT * FROM inoutreasons;";


	try
	{
		
		if(!Select(q.str().c_str(),res)) return false;

		colREASONID = res.ColIndex("REASONID");
		colREASONCODE = res.ColIndex("REASONCODE");
		colREASONNAME = res.ColIndex("REASONNAME");
		colINOUTREASON = res.ColIndex("INOUTREASON");
	
		while(res.nextRow())
		{	
		    one =  new Reason(res.getColInt(colREASONID));
			
			one->SetReasonCode(res.getColStr(colREASONCODE));
			one->SetReasonName(res.getColStr(colREASONNAME));
			one->SetReasonType((Reason::REASON_INOUT_TYPE)res.getColInt(colINOUTREASON));			
			
			reasons.Add(one);
			one = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}
	}
	catch (CSQLException &er)
	{
		m_err = er.GetError();
		if(one) delete one;
		if(progress) progress->OnTaskError(m_err);
		return false;
	}

return true;
}
//***************************************************************************************
bool CattleSQL::LoadHentTypes(CHentTypeArray &htypes,ITaskProgres *progress/* = NULL*/)
{
HentType *one = NULL;
SQLQuery q;
SQLResult res;

int step = 0;
int steps = SelectScalar("SELECT COUNT(*) FROM henttypes",0,-1);
if(steps < 0) return false;
if(progress) progress->SetTask(LOADING_HENTTYPES,steps);

htypes.RemoveAll();
q << "SELECT * FROM henttypes;";


try
{
	
	if(!Select(q.str().c_str(),res)) return false;
	
		while(res.nextRow())
		{	
		    one =  new HentType;
			res.getColStr("henttypename",one->henttypename);
			one->henttypeid = res.getColInt("henttypeid");
			htypes.Add(one);
			one = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

}
catch (CSQLException &er)
{
	m_err = er.GetError();
	if(one) delete one;
	if(progress) progress->OnTaskError(m_err);
	return false;
}

return true;
}
//***************************************************************************************
bool CattleSQL::LoadHents(CHentArray &hents,CHentTypeArray &htypes,CCountryCodeArray &countries,
						  CUserDataArray &users,ITaskProgres *progress/* = NULL*/)
{

	Hent *oneHent = NULL;
	int countryISO,hentTypeId;
	int colARIMRNO,colALIAS,colCITY,colHENTID,colCOUNTRY,colEXTRAS,colNAME,colNIP,colPHONE,
		colPOBOX,colSTREET,colWETNO,colZIP,colPLATE,colHENTTYPE,colPESEL,colREGON,colIDNO,
		colISSUEDATE,colISSUEPOST,colACCOUNTNO,colBANKNAME,colSYNCIT,
		colWETLICENCENO,colCELLPHONENO,colEMAILADDRESS,colLATITUDE,colLONGITUDE;


	Country* hentCountry = NULL;
	HentType* hentHentType = NULL;
	SQLQuery q;
	SQLResult res;

	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM hents",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_HENTS,steps);

	q << "SELECT * FROM hents;";
	hents.RemoveAll();
	ASSERT(htypes.GetSize() && countries.GetSize() && "HentTypes or Countries array is empty, call load function first");


	try
	{	
		if(!Select(q.str().c_str(),res)) return false;

		colARIMRNO = res.ColIndex("arimrno");
	    colALIAS = res.ColIndex("alias");
		colCITY = res.ColIndex("city");
		colHENTID = res.ColIndex("hentid");
		colCOUNTRY = res.ColIndex("country");
		colEXTRAS = res.ColIndex("extras");
		colNAME = res.ColIndex("name");
		colNIP = res.ColIndex("nip");
		colPHONE = res.ColIndex("phone");
		colPOBOX = res.ColIndex("pobox");
		colSTREET = res.ColIndex("street");
		colWETNO = res.ColIndex("wetno");
		colZIP = res.ColIndex("zip");
		colPLATE = res.ColIndex("plate");
		colHENTTYPE = res.ColIndex("henttype");
		colPESEL = res.ColIndex("pesel");
		colREGON = res.ColIndex("regon");
		colIDNO = res.ColIndex("idno");
		colISSUEDATE = res.ColIndex("issuedate");
		colISSUEPOST = res.ColIndex("issuepost");
		colACCOUNTNO = res.ColIndex("accountno");
		colBANKNAME = res.ColIndex("bankname");
		colSYNCIT = res.ColIndex("syncit");

		colWETLICENCENO  = res.ColIndex("wetlicenceno");
		colCELLPHONENO = res.ColIndex("cellphone");
		colEMAILADDRESS = res.ColIndex("email");
		colLATITUDE = res.ColIndex("latitude");
		colLONGITUDE = res.ColIndex("longitude");
        
		while(res.nextRow())
		{
			
			oneHent = new Hent(res.getColInt(colHENTID));		
					
			countryISO = res.getColInt(colCOUNTRY);
			hentCountry = countries.GetCountryByCountryNum(countryISO);
			if(!hentCountry)
			{
				m_err.Format("Country number(%d) not found for hentid %d.",countryISO,oneHent->GetId());
				delete oneHent;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}
			oneHent->SetCountry(hentCountry);

			hentTypeId = res.getColInt(colHENTTYPE);
			hentHentType = htypes.GetHentTypeByHentTypeId(hentTypeId);
			if(!hentHentType)
			{
				m_err.Format("HentType with Id = %d not found for hent %d.",hentTypeId,oneHent->GetId());
				delete oneHent;
				if(progress) progress->OnTaskError(m_err);
				return false; 
			}
			oneHent->SetHentType(hentHentType);


			oneHent->SetFarmNo(res.getColStr(colARIMRNO));
			oneHent->SetAlias(res.getColStr(colALIAS));
			oneHent->SetCity(res.getColStr(colCITY));
			oneHent->SetExtras(res.getColStr(colEXTRAS));
			oneHent->SetName(res.getColStr(colNAME));
			oneHent->SetNIP(res.getColStr(colNIP));
			oneHent->SetPhone(res.getColStr(colPHONE));
			oneHent->SetPOBox(res.getColStr(colPOBOX));
			oneHent->SetStreet(res.getColStr(colSTREET));
			oneHent->SetWetNo(res.getColStr(colWETNO));
			oneHent->SetZip(res.getColStr(colZIP));
			oneHent->SetPlate(res.getColStr(colPLATE));			
			oneHent->SetPESEL(res.getColStr(colPESEL));
			oneHent->SetREGON(res.getColStr(colREGON));
			oneHent->SetIdNo(res.getColStr(colIDNO));
			oneHent->SetIssueDate(res.getColDateTime(colISSUEDATE));
			oneHent->SetIssuePost(res.getColStr(colISSUEPOST));
			oneHent->SetAccountNo(res.getColStr(colACCOUNTNO));
			oneHent->SetBankName(res.getColStr(colBANKNAME));
			oneHent->SetSyncIt(res.getColInt(colSYNCIT) > 0);
			oneHent->SetWetLicenceNo(res.getColStr(colWETLICENCENO));
			oneHent->SetCellPhoneNo(res.getColStr(colCELLPHONENO));
			oneHent->SetEmailAddress(res.getColStr(colEMAILADDRESS));
			oneHent->SetLatitude(Latitude(res.getColNullDouble(colLATITUDE)));
			oneHent->SetLongitude(Longitude(res.getColNullDouble(colLONGITUDE)));

			if(!LoadBasics(res,*(RawData*)oneHent,users))
			{
				delete oneHent;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}

			hents.Add(oneHent);
			m_stats.AdjustStat(oneHent,DataDayStats::ADDED);
			oneHent = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;

	    }	
	}
	catch (CSQLException &er)
	{
		m_err = er.GetError();
		if(oneHent) delete oneHent;
		if(progress) progress->OnTaskError(m_err);
		return false;
	}

	return true;

}
//***************************************************************************************
bool CattleSQL::LoadCountries(CCountryCodeArray &countries,ITaskProgres *progress/* = NULL*/)
{
	SQLResult res;
	Country *one = NULL;
	SQLQuery q;
	int colCOUNTRY,colCODE2A,colCODE3A,colNUMBER,colINUE,colCURRENCY;
	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM countries",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_COUNTRIES,steps);

	q << "SELECT * FROM countries;";
	countries.RemoveAll();

	try
	{
		if(!Select(q.str().c_str(),res)) return false;
		
		colCOUNTRY = res.ColIndex("COUNTRY");
		colCODE2A = res.ColIndex("CODE2A");
		colCODE3A = res.ColIndex("CODE3A");
		colNUMBER = res.ColIndex("NUMBER");
		colINUE = res.ColIndex("IN_UE");
		colCURRENCY = res.ColIndex("CURRENCY");

		while(res.nextRow())
		{
			one = new Country(res.getColInt(colNUMBER));
			one->SetCountryName(res.getColStr(colCOUNTRY));
			one->SetCountryCode2A(res.getColStr(colCODE2A));
			one->SetCountryCode3A(res.getColStr(colCODE3A));
			one->SetCountryInEU(res.getColInt(colINUE) > 0);
			one->SetCurrencyCode(res.getColStr(colCURRENCY));
			countries.Add(one);
			one = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

	}
	catch (CSQLException *er)
	{
		m_err = er->GetError();
		er->Delete();
		if(one) delete one;
		if(progress) progress->OnTaskError(m_err);
		return false;
	}
	return true;
}
//***************************************************************************************
bool CattleSQL::LoadInDocs(CDocDataArray &docs,CHentArray &hents,CUserDataArray &users,
	CReasonDataArray &reasons,CInvoiceDataArray &invoices,CAgentArray& agents,ITaskProgres *progress/* = NULL*/)
{
	InDoc *onedoc = NULL;
	int hentId,reasonId,invoiceId,agentId;
	Hent* pHent = NULL;
	Reason* pReason = NULL;
	VATInvoice* pInvoice = NULL;
	Agent* pAgent = NULL;
	SQLResult res;
	int colDOCID,colDOCDATE,colPLATENO,colLOADDATE,colEXTRAS,colHENT,colREASON,colINVOICE,colLOADSTRTM,colLOADENDTM,colAGENT;
	SQLQuery q;
	int step = 0,steps = 0;
	
	docs.RemoveAll();	
	
	q << "SELECT COUNT(*) FROM indocs";
	steps = SelectScalar(q.str().c_str(),0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_INDOCS,steps);

	q.reset();
	q << "SELECT * FROM indocs"; 

	TRACE("LoadInDocs(): query %s",q.str().c_str());

	try
	{
		if(!Select(q.str().c_str(),res)) return false;

		colDOCID = res.ColIndex("DOCID");
		colDOCDATE = res.ColIndex("DOCDATE");
		colPLATENO = res.ColIndex("PLATENO");
		colLOADDATE = res.ColIndex("LOADDATE");
		colEXTRAS = res.ColIndex("EXTRAS");
		colHENT = res.ColIndex("HENT");
		colREASON = res.ColIndex("REASON");
		colINVOICE = res.ColIndex("INVOICE");
		colLOADSTRTM = res.ColIndex("LOADSTRTM");
		colLOADENDTM = res.ColIndex("LOADENDTM");
		colAGENT = res.ColIndex("AGENT");
		while(res.nextRow())
		{
		
			onedoc = new InDoc(res.getColInt(colDOCID));
			onedoc->SetDocDate(res.getColDateTime(colDOCDATE));
			onedoc->SetPlateNo(res.getColStr(colPLATENO));
			onedoc->SetLoadDate(res.getColDateTime(colLOADDATE));
			onedoc->SetExtras(res.getColStr(colEXTRAS));
			onedoc->SetLoadStartTime(Time(res.getColInt(colLOADSTRTM,NULL_TIME_VALUE)));
			onedoc->SetLoadEndTime(Time(res.getColInt(colLOADENDTM,NULL_TIME_VALUE)));

			if(!LoadBasics(res,*(RawData*)onedoc,users))
			{
				delete onedoc;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}

			hentId = res.getColInt(colHENT);
			pHent = hents.GetHentByHentId(hentId);


			if(pHent == NULL)
			{
				m_err.Format("hent with Id %d for indoc with Id %d not found.",hentId,onedoc->GetId());
				delete onedoc;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}
			onedoc->SetHent(pHent);

			reasonId = res.getColInt(colREASON);
			pReason = reasons.GetReasonDataByReasonId(reasonId);
			if(pReason == NULL)
			{
				m_err.Format("reason with Id %d for indoc with Id %d not found.",reasonId,onedoc->GetId());
				delete onedoc;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}

			onedoc->SetReason(pReason);
			
			invoiceId = res.getColInt(colINVOICE);
			if(invoiceId != 0)
			{
				pInvoice = (VATInvoice*)invoices.GetPtrByInvId(invoiceId);
				if(pInvoice == NULL)
				{					
						m_err.Format("buy invoice with Id %d for indoc with Id %d not found.",invoiceId,onedoc->GetId());
						delete onedoc;
						if(progress) progress->OnTaskError(m_err);
						return false;					
				}
				pInvoice->invoiceDocs.Add(onedoc);
				onedoc->SetInvoice(pInvoice);
			}

			agentId = res.getColInt(colAGENT);
			if(agentId != 0)
			{
				pAgent = agents.GetAgentByAgentId(agentId);
				if(pAgent == NULL)
				{
					m_err.Format("agent with Id %d for indoc with Id %d not found.",agentId,onedoc->GetId());
					delete onedoc;
					if(progress) progress->OnTaskError(m_err);
					return false;
				}
				onedoc->SetAgent(pAgent);
			}
					
			docs.Add(onedoc);
			m_stats.AdjustStat(onedoc,DataDayStats::ADDED);
			onedoc = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

	}
	catch (CSQLException &er)
	{
		m_err = er.GetError();
		if(onedoc) delete onedoc;		
		if(progress) progress->OnTaskError(m_err);
		return false;
	}
	
	return true;
}

bool CattleSQL::LoadOutDocs(CDocDataArray &docs,CHentArray &hents,CUserDataArray &users,
	CReasonDataArray &reasons,CInvoiceDataArray &invoices,CAgentArray& agents,ITaskProgres *progress/* = NULL*/)
{
	OutDoc *onedoc = NULL;
	int hentId,reasonId,invoiceId,agentId;
	Hent* pHent = NULL;
	Reason* pReason = NULL;
	VATInvoice* pInvoice = NULL;
	Agent* pAgent = NULL;
	SQLResult res;
	int colDOCID,colDOCDATE,colPLATENO,colLOADDATE,colEXTRAS,colHENT,colREASON,colINVOICE,colLOADSTRTM,colLOADENDTM,colAGENT;
	SQLQuery q;
	int step = 0,steps = 0;
	
	docs.RemoveAll();	
	
	q << "SELECT COUNT(*) FROM outdocs";
	steps = SelectScalar(q.str().c_str(),0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_OUTDOCS,steps);

	q.reset();
	q << "SELECT * FROM outdocs"; 

	TRACE("LoadOutDocs(): query %s",q.str().c_str());

	try
	{
		if(!Select(q.str().c_str(),res)) return false;

		colDOCID = res.ColIndex("DOCID");
		colDOCDATE = res.ColIndex("DOCDATE");
		colPLATENO = res.ColIndex("PLATENO");
		colLOADDATE = res.ColIndex("LOADDATE");
		colEXTRAS = res.ColIndex("EXTRAS");
		colHENT = res.ColIndex("HENT");
		colREASON = res.ColIndex("REASON");
		colINVOICE = res.ColIndex("INVOICE");
		colLOADSTRTM = res.ColIndex("LOADSTRTM");
		colLOADENDTM = res.ColIndex("LOADENDTM");
		colAGENT = res.ColIndex("AGENT");
		
		while(res.nextRow())
		{
		
			onedoc = new OutDoc(res.getColInt(colDOCID));
			onedoc->SetDocDate(res.getColDateTime(colDOCDATE));
			onedoc->SetPlateNo(res.getColStr(colPLATENO));
			onedoc->SetLoadDate(res.getColDateTime(colLOADDATE));
			onedoc->SetExtras(res.getColStr(colEXTRAS));
			onedoc->SetLoadStartTime(Time(res.getColInt(colLOADSTRTM,NULL_TIME_VALUE)));
			onedoc->SetLoadEndTime(Time(res.getColInt(colLOADENDTM,NULL_TIME_VALUE)));


			if(!LoadBasics(res,*(RawData*)onedoc,users))
			{
				delete onedoc;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}

			hentId = res.getColInt(colHENT);
			pHent = hents.GetHentByHentId(hentId);


			if(pHent == NULL)
			{
				m_err.Format("hent with Id %d for outdoc with Id %d not found.",hentId,onedoc->GetId());
				delete onedoc;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}
			onedoc->SetHent(pHent);

			reasonId = res.getColInt(colREASON);
			pReason = reasons.GetReasonDataByReasonId(reasonId);
			if(pReason == NULL)
			{
				m_err.Format("reason with Id %d for outdoc with Id %d not found.",reasonId,onedoc->GetId());
				delete onedoc;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}

			onedoc->SetReason(pReason);
			
			invoiceId = res.getColInt(colINVOICE);
			if(invoiceId != 0)
			{
				pInvoice = (VATInvoice*)invoices.GetPtrByInvId(invoiceId);
				if(pInvoice == NULL)
				{					
						m_err.Format("sell invoice with Id %d for outdoc with Id %d not found.",invoiceId,onedoc->GetId());
						delete onedoc;
						if(progress) progress->OnTaskError(m_err);
						return false;					
				}
				pInvoice->invoiceDocs.Add(onedoc);
				onedoc->SetInvoice(pInvoice);
			}

			agentId = res.getColInt(colAGENT);
			if(agentId != 0)
			{
				pAgent = agents.GetAgentByAgentId(agentId);
				if(pAgent == NULL)
				{
					m_err.Format("agent with Id %d for indoc with Id %d not found.",agentId,onedoc->GetId());
					delete onedoc;
					if(progress) progress->OnTaskError(m_err);
					return false;
				}
				onedoc->SetAgent(pAgent);
			}
					
			docs.Add(onedoc);
			m_stats.AdjustStat(onedoc,DataDayStats::ADDED);
			onedoc = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

	}
	catch (CSQLException &er)
	{
		m_err = er.GetError();
		if(onedoc) delete onedoc;		
		if(progress) progress->OnTaskError(m_err);
		return false;
	}
	
	return true;
}

//***************************************************************************************
bool CattleSQL::LoadCows(CCowArray &cows,CCowArray &incows,
						 CDocDataArray &indocs,CDocDataArray &outdocs,
						 CInvoiceDataArray& rr_invoices,
						 CHentArray &hents,CStockDataArray &stocks,
						 CUserDataArray &users,
						 ITaskProgres *progress/* = NULL*/)
{
	Cow *onecow = NULL;
	
	int colANIMALID,colEAN,colBIRTHDATE,colBIRTHPLACE,				
		colSEX,colSTOCK, colMOTHEREAN, colWEIGHT, colPASSNO, colPASSDATE,
		colINGRP,colFSTOWNR, colEXTRAS, colDOCIN, colDOCOUT,
		colMYPRICE,colOUTGRP,colTERMBUYSTOCK,colTERMBUYWEIGHT,colTERMSELLSTOCK,colTERMSELLWEIGHT,colTERMBUYPRICE,
		colTERMSELLPRICE,colPASSIMGLABEL,colRRINV,colHEALTHCERTNO;


	SQLQuery q;
	SQLResult res;


	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM cattle",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_COWS,steps);

	cows.RemoveAll();
	q << "SELECT * FROM cattle";


	try
	{
		if(!Select(q.str().c_str(),res)) return false;

		colANIMALID =  res.ColIndex("ANIMALID");
		colEAN = res.ColIndex("EAN");
		colBIRTHDATE = res.ColIndex("BIRTHDATE");
		colBIRTHPLACE =	res.ColIndex("BIRTHPLACE");				
		colSEX = res.ColIndex("SEX");
		colSTOCK = res.ColIndex("STOCK");
		colMOTHEREAN = res.ColIndex("MOTHEREAN");
		colWEIGHT = res.ColIndex("WEIGHT");
		colPASSNO =	res.ColIndex("PASSNO");
		colPASSDATE = res.ColIndex("passdate");
		colFSTOWNR = res.ColIndex("fstownr");
		colEXTRAS = res.ColIndex("extras");
		colDOCIN = res.ColIndex("docin");
		colDOCOUT = res.ColIndex("docout");
		colMYPRICE = res.ColIndex("myprice");
		colOUTGRP = res.ColIndex("outgrp");
		colINGRP = res.ColIndex("ingrp");

		colTERMBUYSTOCK = res.ColIndex("TERMBUYSTOCK");
		colTERMBUYWEIGHT = res.ColIndex("TERMBUYWEIGHT");
		colTERMSELLSTOCK = res.ColIndex("TERMSELLSTOCK");
		colTERMSELLWEIGHT = res.ColIndex("TERMSELLWEIGHT");
		colTERMBUYPRICE = res.ColIndex("TERMBUYPRICE");
		colTERMSELLPRICE = res.ColIndex("TERMSELLPRICE");
		colPASSIMGLABEL = res.ColIndex("PASSIMGLABEL");
		colRRINV = res.ColIndex("RRINV");
		colHEALTHCERTNO  = res.ColIndex("HEALTHCERTNO");
		
		while(res.nextRow())
		{
			int firstownerid,stockid,docinid,docoutid,rrinvid;
			Hent* pFirstOwner;
			Stock *pCowStock;
			RRInvoice *pRRInvoice;
			Doc *pInDoc;
			Doc *pOutDoc;

			onecow = new Cow(res.getColInt(colANIMALID));
			
			onecow->SetEAN(res.getColStr(colEAN));
			onecow->SetBirthDate(res.getColDateTime(colBIRTHDATE));
			onecow->SetBirthPlace(res.getColStr(colBIRTHPLACE));				
			onecow->SetSex(CowSex((COWSEX)res.getColInt(colSEX)));
			            				
			stockid = res.getColInt(colSTOCK);
			pCowStock = stocks.GetStockDataByStockId(stockid); 
			if(pCowStock == NULL)
			{
				m_err.Format("Stock with stockid = %d for cow(animalid = %d) not found.",stockid,onecow->GetId());
				delete onecow;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}
			onecow->SetStock(pCowStock);
				
			onecow->SetMotherEAN(res.getColStr(colMOTHEREAN));
			onecow->SetWeight(Weight(res.getColFloat(colWEIGHT)));
			onecow->SetPassNo(res.getColStr(colPASSNO));
			onecow->SetPassDate(res.getColDateTime(colPASSDATE));

			firstownerid = res.getColInt(colFSTOWNR);	
			if(firstownerid != 0)
			{
				pFirstOwner = hents.GetHentByHentId(firstownerid);
				if(pFirstOwner == NULL)
				{
					m_err.Format("Hent with id = %d for cow(animalid = %d) not found.",firstownerid,onecow->GetId());
					delete onecow;
					if(progress) progress->OnTaskError(m_err);
					return false;
				}
				onecow->SetFirstOwner(pFirstOwner);
			}			

			onecow->SetExtras(res.getColStr(colEXTRAS));
		
			docinid = res.getColInt(colDOCIN,0);
			if(docinid == 0)
			{
				m_err.Format("Fatal error ,NULL docin id for cow(animalid = %d).",onecow->GetId());
				delete onecow;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}

			pInDoc = indocs.GetDocDataByDocId(docinid);
			if(pInDoc == NULL)
			{
				m_err.Format("InDoc winth id = %d for cow(animalid = %d) not found.",docinid,onecow->GetId());
				delete onecow;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}
			onecow->SetInDoc(pInDoc);
			pInDoc->GetCows()->Add(onecow); 
		
			docoutid = res.getColInt(colDOCOUT,NULL);
			if(docoutid) 
			{
				pOutDoc = outdocs.GetDocDataByDocId(docoutid);
				if(pOutDoc == NULL)
				{
					m_err.Format("OutDoc with id = %d for cow(animalid = %d) not found.",docoutid,onecow->GetId());
					delete onecow;
					if(progress) progress->OnTaskError(m_err);
					return false;
				}
				onecow->SetOutDoc(pOutDoc);
				pOutDoc->GetCows()->Add(onecow);
			}

			onecow->SetMyPrice(Money(res.getColNullDouble(colMYPRICE)));
			onecow->SetOutGrp(res.getColInt(colOUTGRP,NULL_INOUT_GRP));		
			onecow->SetInGrp(res.getColInt(colINGRP,NULL_INOUT_GRP));
			onecow->SetTermBuyStock(stocks.GetStockDataByStockId(res.getColInt(colTERMBUYSTOCK)));
			onecow->SetTermSellStock(stocks.GetStockDataByStockId(res.getColInt(colTERMSELLSTOCK)));
			onecow->SetTermBuyWeight(res.getColNullDouble(colTERMBUYWEIGHT));
			onecow->SetTermSellWeight(res.getColNullDouble(colTERMSELLWEIGHT));
			onecow->SetTermBuyPrice(res.getColNullDouble(colTERMBUYPRICE));
			onecow->SetTermSellPrice(res.getColNullDouble(colTERMSELLPRICE));
			onecow->SetPassImgLabel(res.getColStr(colPASSIMGLABEL));
			onecow->SetHealthCertNo(res.getColStr(colHEALTHCERTNO));

			rrinvid = res.getColInt(colRRINV);
			if(rrinvid)
			{
				pRRInvoice = (RRInvoice*)rr_invoices.GetPtrByInvId(rrinvid);		
				if(!pRRInvoice)
				{
					m_err.Format("RRInvoice with id = %d for cow(animalid = %d) not found.",rrinvid,onecow->GetId());
					if(progress) progress->OnTaskError(m_err);
					delete onecow;
					return false;
				}
				onecow->SetRRInvoice(pRRInvoice);
			}
			
		 			
			if(!LoadBasics(res,*(RawData*)onecow,users))
			{
				delete onecow;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}
			
			cows.Add(onecow);
			if(!onecow->IsOut()) incows.Add(onecow);

			m_stats.AdjustStat(onecow,DataDayStats::ADDED);
			onecow = NULL;

			step++;
			if(progress && !progress->OnStep(step)) return false;
		}
	}
	catch (CSQLException &er)
	{
		m_err = er.GetError();
		if(onecow) delete onecow;
		if(progress) progress->OnTaskError(m_err);
		return false;
	}
	return true;
}
//***************************************************************************************
bool CattleSQL::LoadStocks(CStockDataArray &stocks,CRuleDataArray &rules,ITaskProgres *progress/* = NULL*/)
{
	char stocksQ[] = "SELECT * FROM stocks;";
	Stock *one = NULL;
	SQLResult res;
	int ruleid;
	int colSTOCKNAME,colSTOCKCODE,colSTOCKID,colMYPERKGPRICE,colRULE,colPREDEFSEX,colOFFICIALCODE;

	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM stocks",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_STOCKS,steps);	
	stocks.RemoveAll();

	try
	{
		if(!Select(stocksQ,res)) return false;	

		colSTOCKNAME = res.ColIndex("stockname");
		colSTOCKCODE  = res.ColIndex("stockcode");
		colSTOCKID = res.ColIndex("stockid");
		colMYPERKGPRICE = res.ColIndex("myperkgprice");
		colRULE = res.ColIndex("rule");
		colPREDEFSEX = res.ColIndex("predefsex");
		colOFFICIALCODE = res.ColIndex("officialcode");

		while(res.nextRow())
		{			
			one = new Stock(res.getColInt(colSTOCKID));
			one->SetStockName(res.getColStr(colSTOCKNAME));
			one->SetStockCode(res.getColStr(colSTOCKCODE));		
			one->SetMyPerKgPrice(res.getColNullDouble(colMYPERKGPRICE));
			one->SetOfficialCode(res.getColStr(colOFFICIALCODE));

			ruleid = res.getColInt(colRULE,0);
			if(ruleid)
			{
				Rule* pRule = rules.GetPtrByRuleId(ruleid);
				if(pRule == NULL)
				{
					m_err.Format("Ruleid (%d) for stock(stockid %d) not found.",ruleid,one->GetId());
					delete one;
					if(progress) progress->OnTaskError(m_err);
					return false;
				}
				one->SetRule(pRule);
			}
			
			one->SetPredefSex(CowSex((COWSEX)res.getColInt(colPREDEFSEX,SEX_UNK)));		
			stocks.Add(one);
			one = NULL;       
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

	}
	catch ( CSQLException &er)
	{
		m_err = er.GetError();
		if(one) delete one;
		if(progress) progress->OnTaskError(m_err);
		return false;
	}


return true;
}
//***************************************************************************************
bool CattleSQL::LoadUsers(CUserDataArray &users,ITaskProgres *progress/* = NULL*/)
{
SQLResult res;
UserData *one = NULL;
SQLQuery q;


int step = 0;
int steps = SelectScalar("SELECT COUNT(*) FROM users",0,-1);
if(steps < 0) return false;
if(progress) progress->SetTask(LOADING_USERS,steps);	


users.RemoveAll();
q << "SELECT * FROM users;";

try
{
	if(!Select(q.str().c_str(),res)) return false;
	
	while(res.nextRow())
	{
		one = new UserData;
		res.getColStr("username",one->username);
		one->userid = res.getColInt("userid");

		one->lastlogin = res.getColDateTime("lastlogin");
		
		res.getColStr("fname",one->fname);
		res.getColStr("lname",one->lname);
		res.getColStr("pass",one->pass);
		one->privledges = res.getColInt("privledges");
	
		users.Add(one);
		one = NULL;
		step++;
		if(progress && !progress->OnStep(step)) return false;
       
	}

	if(users.GetSize() == 0) 
	{
		m_err = "User table is empty.";
		if(progress) progress->OnTaskError(m_err);
		return false;
	}


}
catch (CSQLException &er)
{
	if(one != NULL) delete one;
	m_err = er.GetError();
	if(progress) progress->OnTaskError(m_err);
	return false;
}

return true;
}
//***************************************************************************************

bool CattleSQL::LoadRules(CRuleDataArray &rules,ITaskProgres *progress/* = NULL*/)
{
	Rule *one = NULL;
	SQLQuery q;
	SQLResult res;

	int colBUYLIMWEIGHT,colSELLLIMWEIGHT;
	int colBUYPRICEOVERWEIGHTXY,colSELLPRICEOVERWEIGHTXY;
	int colBUYPRICEUNDERWEIGHTXY,colSELLPRICEUNDERWEIGHTXY;
	int colBUYPRICEOVERWEIGHTXX,colSELLPRICEOVERWEIGHTXX;
	int	colBUYPRICEUNDERWEIGHTXX,colSELLPRICEUNDERWEIGHTXX;
	int colRULENAME,colNOSEXVALUE,colRULEID;


	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM rules",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_RULES,steps);	


	rules.RemoveAll();
	q << "SELECT * FROM rules;";
	
	try
	{
		if(!Select(q.str().c_str(),res)) return false;

		colBUYLIMWEIGHT = res.ColIndex("BUYLIMWEIGHT");
		colSELLLIMWEIGHT = res.ColIndex("SELLLIMWEIGHT");
		colBUYPRICEOVERWEIGHTXY = res.ColIndex("BUYPRICEOVERWEIGHTXY");
		colSELLPRICEOVERWEIGHTXY = res.ColIndex("SELLPRICEOVERWEIGHTXY");
		colBUYPRICEUNDERWEIGHTXY = res.ColIndex("BUYPRICEUNDERWEIGHTXY");
		colSELLPRICEUNDERWEIGHTXY = res.ColIndex("SELLPRICEUNDERWEIGHTXY");
		colBUYPRICEOVERWEIGHTXX = res.ColIndex("BUYPRICEOVERWEIGHTXX");
		colSELLPRICEOVERWEIGHTXX = res.ColIndex("SELLPRICEOVERWEIGHTXX");
		colBUYPRICEUNDERWEIGHTXX = res.ColIndex("BUYPRICEUNDERWEIGHTXX");
		colSELLPRICEUNDERWEIGHTXX = res.ColIndex("SELLPRICEUNDERWEIGHTXX");
		colRULENAME = res.ColIndex("RULENAME");
		colNOSEXVALUE = res.ColIndex("NOSEXVALUE");
		colRULEID = res.ColIndex("RULEID");

	
		while(res.nextRow())
		{
			one = new Rule(res.getColInt(colRULEID));
			one->SetRuleName(res.getColStr(colRULENAME));
			one->SetNoSexValue(CowSex((COWSEX)res.getColInt(colNOSEXVALUE)));
			
			RulePart* pBuyRulePart = one->GetRulePart(BUY);
			RulePart* pSellRulePart = one->GetRulePart(SELL);

			pBuyRulePart->SetLimWeight(res.getColInt(colBUYLIMWEIGHT));
			pBuyRulePart->SetPriceOverWeightXX(Money(res.getColFloat(colBUYPRICEOVERWEIGHTXX)));
			pBuyRulePart->SetPriceOverWeightXY(Money(res.getColFloat(colBUYPRICEOVERWEIGHTXY)));
			pBuyRulePart->SetPriceUnderWeightXX(Money(res.getColFloat(colBUYPRICEUNDERWEIGHTXX)));
			pBuyRulePart->SetPriceUnderWeightXY(Money(res.getColFloat(colBUYPRICEUNDERWEIGHTXY)));

			pSellRulePart->SetLimWeight(res.getColInt(colSELLLIMWEIGHT));
			pSellRulePart->SetPriceOverWeightXX(Money(res.getColFloat(colSELLPRICEOVERWEIGHTXX)));
			pSellRulePart->SetPriceOverWeightXY(Money(res.getColFloat(colSELLPRICEOVERWEIGHTXY)));
			pSellRulePart->SetPriceUnderWeightXX(Money(res.getColFloat(colSELLPRICEUNDERWEIGHTXX)));
			pSellRulePart->SetPriceUnderWeightXY(Money(res.getColFloat(colSELLPRICEUNDERWEIGHTXY)));
			
			rules.Add(one);
			one = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;	       
		}

		
	}
	catch (CSQLException &er)
	{
		if(one != NULL) delete one;
		m_err = er.GetError();
		if(progress) progress->OnTaskError(m_err);
		return false;
	}

return true;
}//**************************************************************************************
void CattleSQL::SQLMessageBox(UINT nResId)
{
	CString msgText;
	msgText.LoadString(nResId);
	msgText += '\n' + m_err;
	AfxMessageBox(msgText,MB_OK,0);
}
//***************************************************************************************
bool CattleSQL::LoadInvoices(CInvoiceDataArray &buy_invoices,CInvoiceDataArray &sell_invoices,
							 CInvoiceDataArray &rr_invoices,CUserDataArray &users,ITaskProgres *progress/* = NULL*/)
{
	Invoice *one = NULL;	
	SQLQuery q;	
	SQLResult res;
	char payWayChar[2];
	int len;
	
	int colINVOICEID,colINVTYPE,colEXTRAS,colINVOICEDATE,colPAID,colVATRATE,
		colPAYWAY,colINVOICEVALUE,colCUSTOMNUMBER,colCORRECT,colINVOICENO,
		colPAYDUEDAYS;
	
	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM invoices",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_INVOICES,steps);	


	CInvoiceDataArray allInvoices;

	buy_invoices.RemoveAll();
	sell_invoices.RemoveAll();
	q << "SELECT * FROM invoices ORDER BY invoiceid ASC;";

	try
	{
		if(!Select(q.str().c_str(),res)) return false;

		colINVOICENO = res.ColIndex("INVOICENO");
		colINVOICEID = res.ColIndex("INVOICEID");
		colINVTYPE = res.ColIndex("INVTYPE");
		colEXTRAS = res.ColIndex("EXTRAS");
		colINVOICEDATE = res.ColIndex("INVOICEDATE");
		colPAID = res.ColIndex("PAID");
		colVATRATE = res.ColIndex("VATRATE");
		colPAYWAY = res.ColIndex("PAYWAY");
		colINVOICEVALUE = res.ColIndex("INVOICEVALUE");
		colCUSTOMNUMBER = res.ColIndex("CUSTOMNUMBER");
		colCORRECT = res.ColIndex("CORRECT");
		colPAYDUEDAYS = res.ColIndex("PAYDUEDAYS");
		
		while(res.nextRow())
		{
			one = new NullInvoice(res.getColInt(colINVOICEID));

			one->SetInvoiceNo(res.getColInt(colINVOICENO));
			one->SetInvoiceType((InvoiceTypes)res.getColInt(colINVTYPE));
			one->SetExtras(res.getColStr(colEXTRAS)); 
			one->SetInvoiceDate(res.getColDateTime(colINVOICEDATE));
			one->SetPaidDate(res.getColDateTime(colPAID));
			one->SetVATRate(res.getColFloat(colVATRATE));
			len = 2; //expecting paywaychar and \0
			res.getColStr(colPAYWAY,payWayChar,len);
			ASSERT(len == 1 && (payWayChar[0] == Invoice::PAY_CASH || payWayChar[0] == Invoice::PAY_TRANS));
			one->SetPayWay((Invoice::PAYWAYS)payWayChar[0]);

			one->SetInvoiceNetValue(Money(res.getColFloat(colINVOICEVALUE)));
			one->SetCustomNumber(res.getColStr(colCUSTOMNUMBER));
			one->SetPayDueDays( res.getColInt(colPAYDUEDAYS,1));

			if(!LoadBasics(res,*(RawData*)one,users))
			{
				delete one;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}

			

			if(one->GetThisInvoiceType() == CORRCT_INV)
			{
				UINT correctId = res.getColInt(colCORRECT);
				Invoice* pCorrect = allInvoices.GetPtrByInvId(correctId);
				if(pCorrect == NULL)
				{
					delete one;
					m_err.Format("Can't find invoice(invoice id %d) for correct invoice with id %d" ,	correctId,one->GetId());
					if(progress) progress->OnTaskError(m_err);
					return false;		
				}
				one->SetCorrectFor(pCorrect);				
			}
	
			
			CInvoiceDataArray *destArray = NULL;
			Invoice *typedInvoice = NULL;
				
			switch(one->GetRealInvoiceType())
			{
			case  IN_INV_RR:
				typedInvoice = new RRInvoice(one);
				destArray = &rr_invoices;
				break;
				
			case IN_INV_VAT:
				typedInvoice = new VATInvoice(one);
				destArray = &buy_invoices;
				break;
			case OUT_INV_VAT:
				typedInvoice = new VATInvoice(one);
				destArray = &sell_invoices;
				break;
			default:
				ASSERT(FALSE);
			}

			if(typedInvoice->GetThisInvoiceType() == CORRCT_INV)
				typedInvoice->GetCorrectFor()->SetCorrect(typedInvoice);

			destArray->Add(typedInvoice);
			allInvoices.Add(typedInvoice);
			m_stats.AdjustStat(one,DataDayStats::ADDED);
			delete one;
			one = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

		
	}
	catch (CSQLException &er)
	{
		if(one != NULL) delete one;
		m_err = er.GetError();
		if(progress) progress->OnTaskError(m_err);
		return false;
	}

	return true;
}

bool CattleSQL::LoadCowInvoiceEntries(CInvoiceDataArray &invoices,
									  CCowArray &cows,
									  CStockDataArray& stocks,
									  ITaskProgres *progress/* = NULL*/)
{

	Invoice *one = NULL;	
	SQLQuery q;	
	SQLResult res;
	
	int colINVOICE,colCOW,colSTOCK,colWEIGHT,colPRICE;
	
	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM cowpricing",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_PRICES,steps);

	q << "SELECT * FROM cowpricing";

	try
	{
		if(!Select(q.str().c_str(),res)) return false;

		colINVOICE = res.ColIndex("INVOICE");
		colCOW = res.ColIndex("COW");
		colPRICE = res.ColIndex("PRICE");
		colSTOCK = res.ColIndex("STOCK");
		colWEIGHT = res.ColIndex("WEIGHT");
		while(res.nextRow())
		{
			int invoiceId = res.getColInt(colINVOICE,0);
			Invoice* pInvoice = invoices.GetPtrByInvId(invoiceId);
			if(!pInvoice)
			{
				m_err.Format("Can't find invoice with id = %d for invoice entry" ,invoiceId);
				if(progress) progress->OnTaskError(m_err);
				return false;		
			}
			int cowId = res.getColInt(colCOW,NULL_COWID);
			Cow* pCow = cows.GetCowByCowId(cowId);
			if(!pCow)
			{
				m_err.Format("Can't find cow with id = %d for invoice entry" ,	cowId);
				if(progress) progress->OnTaskError(m_err);
				return false;		
			}
			int stockId = res.getColInt(colSTOCK,0);
			Stock* pStock = stocks.GetStockDataByStockId(stockId);
			if(!pStock)
			{
				m_err.Format("Can't find stock with id = %d for for invoice entry" ,	stockId);
				if(progress) progress->OnTaskError(m_err);
				return false;		
			}
			NullDouble weight(res.getColNullDouble(colWEIGHT));
			if(weight.IsNull())
			{
				m_err.Format("WEIGHT in COWPRICING for cow with id = %d and invoice with id = %d is invalid" ,	cowId,invoiceId);
				if(progress) progress->OnTaskError(m_err);
				return false;
			}

			CowInvoiceEntry* pEntry = new CowInvoiceEntry(pCow,pInvoice);

			pEntry->SetStock(pStock);
			pEntry->SetPrice(res.getColFloat(colPRICE));
			pEntry->SetWeight(Weight(weight.GetDouble()));
		
			pInvoice->invEntries.Add(pEntry);
			
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

		
	}
	catch (CSQLException &er)
	{
		if(one != NULL) delete one;
		m_err = er.GetError();
		if(progress) progress->OnTaskError(m_err);
		return false;
	}

	return true;

}

void CattleSQL::MergeVATInvoices(CInvoiceDataArray &inv_ary,UINT taskId,ITaskProgres *progress/* = NULL*/)
{
	VATInvoice *pInv = NULL;
	int i,s;

	if(progress)
		progress->SetTask(taskId,inv_ary.GetSize());

	for(i = 0,s = inv_ary.GetSize();i<s;i++)
	{
		pInv = (VATInvoice*)inv_ary[i];

		if(pInv->GetThisInvoiceType() == CORRCT_INV)
		{						
			pInv->invoiceDocs.Append(((VATInvoice*)pInv->GetFirstInvoice())->invoiceDocs);
		}
		if(progress && !progress->OnStep(i)) break;
	}
}
//***************************************************************************************
bool CattleSQL::LoadCompany(CompanyForInitialisation *pCompany)
{
#ifndef TRIAL_HENT
	
	int colPHONE,		
		colWETIDNO,		
		colREGON,
		colACCOUNTNO,
		colBANKNAME,
		colWETLICENCENO,
		colNAME,
		colZIP,
		colCITY,
		colSTREET,
		colPOBOX,
		colNIP,
		colFARMNO
//		colLONGITUDE,
//		colLATITUDE
		;


	CString sPhone;
	CString sWetIdNo;	
	CString sREGON;
	CString sAccountNo;
	CString sBankName;
	CString sWetLicenceNo;
	CString m_CompanyName;
	CString m_City;
	CString m_ZipCode;
	CString m_Street;
	CString m_POBox;
	CString m_FarmNo;
	CString m_NIP;

	SQLQuery q;
	SQLResult res;
	q << "SELECT * FROM company;";

	try
	{
		Select(q.str().c_str(),res);

		colNAME = res.ColIndex("name");
		colZIP = res.ColIndex("zip");
		colCITY = res.ColIndex("city");
		colSTREET = res.ColIndex("street");
		colPOBOX = res.ColIndex("pobox");
		colNIP = res.ColIndex("nip");
		colFARMNO = res.ColIndex("farmno");


		colPHONE = res.ColIndex("phone");
		colWETIDNO = res.ColIndex("wetidno");
		colREGON = res.ColIndex("regon");
		colACCOUNTNO = res.ColIndex("accountno");
		colBANKNAME = res.ColIndex("bankname");
		colWETLICENCENO  = res.ColIndex("WETLICENCENO");
		colNAME = res.ColIndex("name");
		colZIP = res.ColIndex("zip");
		colCITY = res.ColIndex("city");
		colSTREET = res.ColIndex("street");
		colPOBOX = res.ColIndex("pobox");
		colNIP = res.ColIndex("nip");
		colFARMNO = res.ColIndex("farmno");

//		colLONGITUDE = res.ColIndex("LONGITUDE");
//		colLATITUDE = res.ColIndex("LATITUDE");
    
//		Company::Lock();
		if(res.nextRow())
		{
			 m_CompanyName = res.getColStr(colNAME);
			 m_City = res.getColStr(colCITY);
			 m_ZipCode = res.getColStr(colZIP);
			 m_Street = res.getColStr(colSTREET);
			 m_POBox = res.getColStr(colPOBOX);
			 m_FarmNo = res.getColStr(colFARMNO);
			 m_NIP = res.getColStr(colNIP);
			sPhone = res.getColStr(colPHONE);
			sWetIdNo = res.getColStr(colWETIDNO);
			sREGON = res.getColStr(colREGON);
			sAccountNo = res.getColStr(colACCOUNTNO);
			sBankName = res.getColStr(colBANKNAME);			
			sWetLicenceNo = res.getColStr(colWETLICENCENO);
//			firminfo->SetLatitude(res.getColNullDouble(colLATITUDE));
			//firminfo->SetLongitude(res.getColNullDouble(colLONGITUDE));
		
	    }
		else
		{
			m_err = "Missing company information.";
			return false;
		}
	}
	catch (CSQLException *e)
	{
		m_err = e->GetError();
		e->Delete();
		return false;
	}

	pCompany->SetFarmNo(m_FarmNo);
	pCompany->SetCity(m_City);
	pCompany->SetZipCode(m_ZipCode);
	pCompany->SetStreet(m_Street);
	pCompany->SetCompanyName(m_CompanyName);
	pCompany->SetNIP(m_NIP);
	pCompany->SetPOBox(m_POBox);
	
	pCompany->SetPhoneNo(sPhone);
	pCompany->SetWetIdNo(sWetIdNo);
	pCompany->SetREGON(sREGON);
	pCompany->SetAccountNo(sAccountNo);
	pCompany->SetBank(sBankName);
	pCompany->SetWetLicNo(sWetLicenceNo);

#else
	firminfo->SetFarmNo("PL063706123002");
	firminfo->SetCity("Kcynia");
	firminfo->SetCountry(countries.GetCountryByCountryNum(616));
	firminfo->SetName("Firma Handlowa BA£DYGA Arkadiusz Ba³dyga");
	firminfo->SetNIP("5581718574");
	firminfo->SetPhone("609729997");
	firminfo->SetPOBox("35/2");
	firminfo->SetStreet("Dworcowa");
	firminfo->SetWetNo("00000000");
	firminfo->SetZip("89-240");
	firminfo->SetPlate("XX 00000");
	firminfo->SetREGON("340730778");
	firminfo->SetAccountNo("28203000451110000002132340");
	firminfo->SetBankName("BG¯ S.A.");			
	firminfo->SetHentType(htypes.GetHentTypeByHentTypeId(HTYPE_COMPANY));
	//firminfo->SetWetLicenceNo();	

#endif
	return true;
}
bool CattleSQL::LoadExpenseCategories(CExpenseCategoryArray& categories,ITaskProgres *progress/* = NULL*/)
{
	ExpenseCategory *one = NULL;	
	SQLQuery q;	
	SQLResult res;

	int colEXPENSECATEGORYNAME,colEXPENSECATEGORYDESC,colEXPENSECATEGORYID,colDEFAULTVATRATE;
	
	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM expensecategory",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_EXPENSECATEGORIES,steps);	


	categories.RemoveAll();
	
	q << "SELECT * FROM expensecategory";

	try
	{
		if(!Select(q.str().c_str(),res)) return false;

		colEXPENSECATEGORYID = res.ColIndex("expensecategoryid");
		colEXPENSECATEGORYNAME = res.ColIndex("expensecategoryname");
		colEXPENSECATEGORYDESC = res.ColIndex("expensecategorydesc");
		colDEFAULTVATRATE	= res.ColIndex("defaultvatrate");
		
		
		while(res.nextRow())
		{
			one = new ExpenseCategory(res.getColInt(colEXPENSECATEGORYID));

			one->SetExpenseCategoryName(res.getColStr(colEXPENSECATEGORYNAME)); 
			one->SetExpenseCategoryDesc(res.getColStr(colEXPENSECATEGORYDESC));
			one->SetDefaultVATRate(res.getColFloat(colDEFAULTVATRATE,-1.0));
		
			categories.Add(one);			
	
			one = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

		
	}
	catch (CSQLException &er)
	{
		if(one != NULL) delete one;
		m_err = er.GetError();
		if(progress) progress->OnTaskError(m_err);
		return false;
	}

return true;	

}
bool CattleSQL::LoadExpenses(CExpenseDataArray& expenses,CExpenseCategoryArray& categories,CUserDataArray &users,ITaskProgres *progress/* = NULL*/)
{
	Expense *one = NULL;	
	SQLQuery q;	
	SQLResult res;

	int expensecategoryid;
	CString expenseTypeStr;
	ExpenseCategoryPtr expensecategory;

	int colGROSVALUE,colVATRATE,colEXPENSETYPE,colEXPENSECATEGORY,
		colINVOICENO,colEXPENSEDATE,colEXTRAS,colEXPENSEID,
		colEXPENSETITLE;

	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM expenses",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_EXPENSES,steps);	


	expenses.RemoveAll();
	
	q << "SELECT * FROM expenses";

	try
	{
		if(!Select(q.str().c_str(),res)) return false;

		colEXPENSEID = res.ColIndex("expenseid");
		colGROSVALUE = res.ColIndex("grosvalue");
		colVATRATE = res.ColIndex("vatrate");
		colEXPENSETYPE = res.ColIndex("expensetype");
		colEXPENSECATEGORY = res.ColIndex("expensecategory");
		colINVOICENO = res.ColIndex("invoiceno");
		colEXPENSEDATE = res.ColIndex("expensedate");
		colEXTRAS = res.ColIndex("extras");
		colEXPENSETITLE = res.ColIndex("expensetitle");
		
		
		while(res.nextRow())
		{
			one = new Expense(res.getColInt(colEXPENSEID));
			expenseTypeStr = res.getColStr(colEXPENSETYPE);
			one->SetExpenseType((Expense::EXPENSE_TYPE)expenseTypeStr[0]);
			one->SetExtras(res.getColStr(colEXTRAS));
			one->SetExpenseDate(res.getColDateTime(colEXPENSEDATE));
			one->SetInvoiceNo(res.getColStr(colINVOICENO));
			one->SetGrosValue(Money(res.getColFloat(colGROSVALUE)));
			one->SetVATRate(res.getColFloat(colVATRATE));
			one->SetExpenseTitle(res.getColStr(colEXPENSETITLE));

			
			expensecategoryid = res.getColInt(colEXPENSECATEGORY);
			expensecategory = categories.GetExpenseCategory(expensecategoryid);
			
			if(expensecategory.IsNull())
			{
				m_err.Format("Cant find expense category with id %d for expense %d.",expensecategoryid,one->GetId());
				delete one;
				return false;
			}
			one->SetExpenseCategory(expensecategory);

			if(!LoadBasics(res,*(RawData*)one,users))
			{
				delete one;
				if(progress) progress->OnTaskError(m_err);
				return false;
			}

		
			expenses.Add(one);			
	
			one = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

		
	}
	catch (CSQLException &er)
	{
		if(one != NULL) delete one;
		m_err = er.GetError();
		if(progress) progress->OnTaskError(m_err);
		return false;
	}

	return true;
}

bool CattleSQL::LoadAgents(CAgentArray& agents,ITaskProgres *progress/* = NULL*/)
{
	Agent *one = NULL;	
	SQLQuery q;	
	SQLResult res;

	int colAGENTID,colAGENTNAME,colAGENTCODE,colAGENTPLATENO;
	
	int step = 0;
	int steps = SelectScalar("SELECT COUNT(*) FROM agents",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetTask(LOADING_AGENTS,steps);	

	agents.RemoveAll();
	
	q << "SELECT * FROM agents";

	try
	{
		if(!Select(q.str().c_str(),res)) return false;

		colAGENTID = res.ColIndex("agentid");
		colAGENTCODE = res.ColIndex("agentcode");
		colAGENTNAME = res.ColIndex("agentname");
		colAGENTPLATENO	= res.ColIndex("plate");
		
		
		while(res.nextRow())
		{
			one = new Agent(res.getColInt(colAGENTID));

			one->SetAgentCode(res.getColStr(colAGENTCODE)); 
			one->SetAgentName(res.getColStr(colAGENTNAME));
			one->SetPlateNo(res.getColStr(colAGENTPLATENO));
		
		
			agents.Add(one);			
	
			one = NULL;
			step++;
			if(progress && !progress->OnStep(step)) return false;
		}

		
	}
	catch (CSQLException &er)
	{
		if(one != NULL) delete one;
		m_err = er.GetError();
		if(progress) progress->OnTaskError(m_err);
		return false;
	}

return true;	

}
//***************************************************************************************

//bool CattleSQL::LoadHerds(CHerdArray& herds,ITaskProgres *progress/* = NULL*/)
//{
//	return false;
//}

//***************************************************************************************
bool CattleSQL::UpdateCompany(const CompanyForChanging* pCompany)
{
	SQLQuery q;
	q << "UPDATE company SET ";
	SQLEqualList eql;


	eql.Add("name",SQLString(pCompany->GetCompanyName(),true));
	eql.Add("city",SQLString(pCompany->GetCity(),true));
	eql.Add("zip",SQLString(pCompany->GetZipCode(),true));
	eql.Add("street",SQLString(pCompany->GetStreet(),true));
	eql.Add("pobox",SQLString(pCompany->GetPOBox(),true));
	eql.Add("farmno",SQLString(pCompany->GetFarmNo(),true));
	eql.Add("nip",SQLString(pCompany->GetNIP(),true));

	 
	eql.Add("phone",SQLString(pCompany->GetPhoneNo(),true));
	eql.Add("wetidno",SQLString(pCompany->GetWetIdNo(),true));		
	eql.Add("accountno",SQLString(pCompany->GetAccountNo(),true)); 
	eql.Add("bankname",SQLString(pCompany->GetBank(),true)); 
	eql.Add("regon",SQLString(pCompany->GetREGON(),true)); 
	eql.Add("wetlicenceno",SQLString(pCompany->GetWetLicNo()));
	//eql.Add("latitude",SQLString(firminfo->GetLatitude()));
	//eql.Add("longitude",SQLString(firminfo->GetLongitude()));

	q << eql.equal_list();	

	LOG("UpdateCompany query %s\n",q.str().c_str());

	return Update(q.str().c_str());
	
}
//***************************************************************************************
CString CattleSQL::GetDBFilePath()
{
	return m_dbFilePath;
}
//***************************************************************************************
bool CattleSQL::DoBackup(const CString &backupPath)
{
	CString normPath = backupPath,newDBFile;
	paths_normalize(normPath);

	newDBFile = paths_ConcatPath(normPath,DB_FILE);
	if(!CopyFile((LPCSTR)m_dbFilePath,(LPCSTR)newDBFile,FALSE))
	{
		m_err = CStrException::ErrorString(::GetLastError());
		return false;
	}
return true;
}
//***************************************************************************************
bool CattleSQL::Commit()
{
	//fail close
	m_stats.Commit();
	bool commitSucceeded = SQL_SUCCEEDED(SQLEndTran( SQL_HANDLE_DBC, m_Connection, SQL_COMMIT ));

	if(commitSucceeded)
	{
        LOG("COMMIT succeeded.");
	}
	else
	{
		ErrMsg (m_Connection, SQL_HANDLE_DBC);
		LOG("COMMIT failed: %s",(LPCSTR)m_err);
	}
	return commitSucceeded;

	
}
//***************************************************************************************
bool CattleSQL::Rollback()
{
	//fail close
	m_stats.Rollback();
	bool rollbackSucceeded = SQL_SUCCEEDED(SQLEndTran( SQL_HANDLE_DBC, m_Connection, SQL_ROLLBACK ));

	if(rollbackSucceeded)
	{
		 LOG("ROLLBACK succeeded.");
	}
	else
	{
		ErrMsg (m_Connection, SQL_HANDLE_DBC);
		LOG("ROLLBACK failed: %s",(LPCSTR)m_err);
	}

	return rollbackSucceeded;
}


bool CattleSQL::Delete(const char *stmt,bool commit/* = false*/)
{	
	HSTMT statement = GetStatement(stmt);
	if(statement == NULL)
	{		
		return false;
	}

	SQLRETURN result = ExecuteStatement(statement);
	if(result == SQL_NO_DATA)
	{
		FreeStatement(statement);
		return true;
	}

	if(!SQL_SUCCEEDED(result))
	{
		ErrMsg(statement);
		FreeStatement(statement);
		return false;
	}

	FreeStatement(statement);

	if(commit) 
		return Commit();

	return true;
}

bool CattleSQL::Insert(const char *stmt,bool commit/* = false*/)
{
	HSTMT statement = GetStatement(stmt);
	if(statement == NULL)
	{		
		return false;
	}

	SQLRETURN ret = ExecuteStatement (statement);
	if(!SQL_SUCCEEDED(ret))
	{	
		ErrMsg (statement);
		FreeStatement(statement);
		return false;
	}

	FreeStatement(statement);

	if(commit) return Commit();

	return true;
}

bool CattleSQL::Update(const char *stmt,bool commit/* = false*/)
{
	HSTMT statement = GetStatement(stmt);
	if(statement == NULL)
	{		
		return false;
	}

	SQLRETURN ret = ExecuteStatement (statement);
	if(!SQL_SUCCEEDED(ret))
	{	
		ErrMsg (statement);
		FreeStatement(statement);
		return false;
	}

	FreeStatement(statement);

	if(commit) return Commit();

	return true;
}

int  CattleSQL::SelectScalar(const char *stmt,int ifNull/* = 0*/,int ifError/* = -1*/)
{
	SQLResult res;
	int iRet = ifNull;
	try
	{
		Select(stmt,res);
		if(!res.nextRow()) return ifNull;
		iRet = res.getColInt(0,ifNull);
	} 
	catch(CSQLException *er)
	{
		Rollback();
		m_err = er->GetError();
		er->Delete();
		return ifError;
	}
	return iRet;
}

HSTMT CattleSQL::GetStatement(const char *stmt)
{
	HSTMT statement = NULL;
	SQLRETURN ret = SQLAllocHandle (SQL_HANDLE_STMT, m_Connection, &statement);
	if(!SQL_SUCCEEDED(ret))
	{	
		ErrMsg (m_Connection, SQL_HANDLE_DBC);
		return NULL;
	}

	ret = SQLPrepare (statement, (UCHAR*) stmt, SQL_NTS);
	if(!SQL_SUCCEEDED(ret))
	{	
		ErrMsg (statement);
		SQLFreeHandle (SQL_HANDLE_STMT, statement);
		return NULL;
	}
	return statement;
}

void CattleSQL::FreeStatement(HSTMT hStmt)
{
	SQLFreeHandle (SQL_HANDLE_STMT, hStmt);
}
SQLRETURN CattleSQL::ExecuteStatement(HSTMT hStmt)
{
	return SQLExecute (hStmt);
}
//***************************************************************************************
bool CattleSQL::Select(const char *stmt, SQLResult &res)
{
	HSTMT statement = GetStatement(stmt);
	if(statement == NULL)
		return false;

	SQLRETURN ret = ExecuteStatement (statement);
	if(!SQL_SUCCEEDED(ret))
	{	
		ErrMsg (statement);
		return false;
	}

	res.SetStmt(statement);

	return true;
}
//***************************************************************************************
void CattleSQL::SetCurrentRawData(RawData &rawData)
{	
	rawData.modification.user = UserData::GetCurrentUser();
	rawData.modification.eventDt = CDateTime::now();
	rawData.creation.user = rawData.modification.user;
	rawData.creation.eventDt = rawData.modification.eventDt;
}
//***************************************************************************************
bool CattleSQL::RestoreBackup(CString &backupPath)
{
	CString appDir,baseFile,backupFile;
	

	ASSERT(!m_bOppened);

	appDir = CUtil::GetAppDir();
	baseFile = paths_ConcatPath(appDir,DB_FILE);
	backupFile= paths_ConcatPath(backupPath,DB_FILE);

	if(!::CopyFile(backupFile,baseFile,FALSE))
	{
		m_err = CStrException::ErrorString(::GetLastError());
		return false;
	}
	return true;
}
//***************************************************************************************
bool CattleSQL::LoadDB(bool bUsersLoaded/* = true*/,ITaskProgres *progress/* = NULL*/)
{
	
	if(!bUsersLoaded)
		if(!LoadUsers(ary_users,progress)) return false;

	bool bRet = LoadReasons(ary_reasons,progress) &&
		   LoadCountries(ary_countries,progress) &&							
		   LoadRules(ary_rules,progress) &&
		   LoadStocks(ary_stocks,ary_rules,progress) &&
		   LoadHentTypes(ary_htypes,progress) &&
		   LoadHents(ary_hents,ary_htypes,ary_countries,ary_users,progress) &&
		   LoadInvoices(ary_buy_invs,ary_sell_invs,ary_rr_invs,ary_users,progress) &&
		   LoadAgents(ary_agents,progress) &&
		   LoadInDocs(ary_indocs,ary_hents,ary_users,ary_reasons,ary_buy_invs,ary_agents,progress) &&
		   LoadOutDocs(ary_outdocs,ary_hents,ary_users,ary_reasons,ary_sell_invs,ary_agents,progress) &&
		   LoadCows(ary_cows,ary_incows,ary_indocs,ary_outdocs,ary_rr_invs,ary_hents,ary_stocks,ary_users,progress) &&
		   LoadExpenseCategories(ary_expensecategories,progress) &&
		   LoadExpenses(ary_expenses,ary_expensecategories,ary_users,progress);
	if(bRet)
	{
		CInvoiceDataArray allInvoices;
		allInvoices.Append(ary_buy_invs);
		allInvoices.Append(ary_sell_invs);
		allInvoices.Append(ary_rr_invs);
		bRet = bRet && LoadCowInvoiceEntries(allInvoices,ary_cows,ary_stocks,progress);

		MergeVATInvoices(ary_buy_invs,MERGING_VATBUYINVOICES,progress);
		MergeVATInvoices(ary_sell_invs,MERGING_VATSELLINVOICES,progress);
	

		Hent *pHent;
		for(int i = 0,s = ary_hents.GetSize();i < s;i++)
		{
			pHent = ary_hents[i];
			switch(pHent->GetHentType()->henttypeid)
			{
				case HTYPE_COMPANY:
					ary_companies.Add(pHent);
				break;
				case HTYPE_INDIVIDUAL:
					ary_farmers.Add(pHent);
				break;
			}
		}

		m_stats.Commit();
		Company::Lock();
		CompanyForInitialisation *pCompany = Company::GetCompanyForInitialisation();

		bRet = bRet && LoadCompany(pCompany);
		
		Company::Unlock();
	}

	return bRet;
		  

}

bool CattleSQL::DropDocs()
{
	char *dropStmts[] =
	{	
		"DELETE FROM cowpricing",
		"DELETE FROM cattle",
		"DELETE FROM indocs",
		"DELETE FROM outdocs",
		"DELETE FROM expenses",
		NULL
	};

	char *resetGen[] = 
	{
		"GEN_ID_OUTDOCS",
		"GEN_ID_INDOCS",
		NULL
	};

	SQLResult res;
	SQLQuery deleteTablesStmt;
	int colINVOICEID;			
	int invoiceId;

	char *(*delStmt) = dropStmts;
	while(*delStmt)
	{
		deleteTablesStmt.reset();
		deleteTablesStmt << *delStmt;
		if(!Delete(deleteTablesStmt.str().c_str(),false))
		{
			Rollback();
			return false;
		}
		delStmt++;
	}

	SQLQuery selectAllInvoicesQuery;
	selectAllInvoicesQuery << "SELECT invoiceid FROM invoices ORDER BY invoiceid DESC;";	
	
	
	SQLQuery deleteInvoiceStmt;
	deleteInvoiceStmt << "DELETE FROM invoices WHERE invoiceid = %0";
	deleteInvoiceStmt.parse();

	
	try
	{
		
		if(!Select(selectAllInvoicesQuery.str().c_str(),res)) 
		{
			Rollback();
			return false;
		}

		colINVOICEID = res.ColIndex("invoiceid");
		
		while(res.nextRow())
		{
			invoiceId = res.getColInt(colINVOICEID);
			if(!Delete(deleteInvoiceStmt.str(invoiceId).c_str(),false))
			{
				Rollback();
				return false;				
			}
		}


		char *(*resetGenStmt) = resetGen;
		SQLQuery resetGeneratorsStmt;
		resetGeneratorsStmt << "SET GENERATOR %0 TO 0;";
		resetGeneratorsStmt.parse();

		while(*resetGenStmt)
		{
			if(!Update(resetGeneratorsStmt.str(*resetGenStmt).c_str(),false))
			{
				Rollback();
				return false;
			}
			resetGenStmt++;
		}
		Commit();		
	}
	catch (CSQLException &er)
	{
		m_err = er.GetError();
		return false;
	}
	return true;
}
	
//#######################################################################################
//#######################################################################################
void SQLResult::SetStmt(HSTMT stmt)
{
	statement = stmt;
	SWORD	count;
	RETCODE ret = SQLNumResultCols (statement, &count);
	if(!SQL_SUCCEEDED(ret))
		throw new CSQLException(statement);
		
	numberColumns = count;
	columns = new Column [numberColumns];
	Column *column = columns;

	SWORD nameLength;
	SQLUINTEGER dataLen;
	SQLSMALLINT decLen;


	for (int n = 1; n <= numberColumns; ++n, ++column)
		{
		
		ret = SQLDescribeCol (statement, n, 
					  (UCHAR*) column->name, sizeof (column->name), &nameLength,
					  &column->sqlType, 
					  &dataLen, 
					  &decLen, 
					  &column->nullable);
		if(!SQL_SUCCEEDED(ret))
			throw new CSQLException(statement);

		//if(column->sqlType == SQL_DECIMAL || column->sqlType == SQL_NUMERIC)
			column->datalength = dataLen+decLen;
		//else column->datalength = dataLen;
		
		}

		//nextRow();
}
//***************************************************************************************
SQLResult::~SQLResult()
{
	if(columns)
		delete[] columns;
	if(statement)
	{
		int ret = SQLFreeHandle (SQL_HANDLE_STMT, statement);
		
		if(!SQL_SUCCEEDED(ret))		
			throw new CSQLException(statement);
	}	
}	
//***************************************************************************************
int SQLResult::numCols()
{
	return numberColumns;
}
//***************************************************************************************
 bool SQLResult::nextRow()
 {
    if(!statement) throw new CSQLException("Object not initialized");
	RETCODE ret = SQLFetch (statement);
	if(ret == SQL_NO_DATA_FOUND) return false;

		if(!SQL_SUCCEEDED(ret))
			throw new CSQLException(statement);
	return true;
 }
 //**************************************************************************************
int SQLResult::ColIndex(const char* szColName)
{
	for(int i = 0;i< numberColumns;i++)
		if(!_strcmpi(szColName,columns[i].name))
			return (i);	

	throw new CSQLException("No column : %s",szColName);
	
}
//***************************************************************************************
const char* SQLResult::ColName(int nCol)
{
	if(nCol >= 0 && nCol < numberColumns)
		return columns[nCol].name;

	return NULL;
}
//***************************************************************************************
int SQLResult::getColInt(int nCol, int iNullValue)
{
	if(nCol  < 0 && nCol >= numberColumns)
		throw new CSQLException(ErrColBounds);
	SQLINTEGER result,Ind;
	
	int ret = SQLGetData (statement, nCol+1, SQL_C_LONG , &result,sizeof(result), &Ind);
	if(!SQL_SUCCEEDED(ret))
			throw new CSQLException(statement);
	if(Ind == SQL_NULL_DATA) return iNullValue;
		
	return result;
}
//***************************************************************************************
int SQLResult::getColInt(const char* szCol, int iNullValue)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) throw new CSQLException(ErrUnknCol,szCol);
	
	return getColInt(colIdx,iNullValue);
}
//***************************************************************************************
CString SQLResult::getColStr(int nCol)
{
	CString colString;
	getColStr(nCol,colString);
	return colString;
}
//***************************************************************************************
bool SQLResult::getColStr(int nCol, char* szStr, int &len)
{
	if(nCol < 0 && nCol >= numberColumns)
		throw new CSQLException(ErrColBounds);

	if(!szStr) throw new CSQLException("Null string pointer");

	SQLINTEGER Ind;
	
	
	int ret = SQLGetData (statement, nCol+1, SQL_C_CHAR , szStr,len, &Ind);
	if(!SQL_SUCCEEDED(ret))
			throw new CSQLException(statement);
	
	if(Ind == SQL_NULL_DATA) 
	{
		len = 0;
		return true;
	}
	else len = Ind;


	return false;
}
//***************************************************************************************
bool SQLResult::getColStr(int nCol, CString &Str)
{	
	if(nCol  < 0 && nCol >= numberColumns)
		throw new CSQLException(ErrColBounds);

	int bufflen = columns[nCol].datalength+1;
	bool ret;
	char * buffer = new char[bufflen];
	ret = getColStr(nCol,buffer,bufflen);
	buffer[bufflen] = 0;
	Str = buffer;
	delete[] buffer;
	return ret;
}
//***************************************************************************************
bool SQLResult::getColStr(const char* szCol, char* szStr, int &len)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throw new CSQLException(ErrUnknCol,szCol);

	return getColStr(colIdx,szStr,len);
}
//***************************************************************************************
bool SQLResult::getColStr(const char* szCol, CString &Str)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throw new CSQLException(ErrUnknCol,szCol);
	return getColStr(colIdx,Str);

}
//***************************************************************************************


CDateTime SQLResult::getColDateTime(int nCol)
{
	SQL_TIMESTAMP_STRUCT StatusTime;
	SQLINTEGER nullindicator;
    SQLGetData(statement, nCol+1, SQL_C_TYPE_TIMESTAMP,&StatusTime, sizeof(StatusTime), &nullindicator);

	if(nullindicator != SQL_NULL_DATA)
	{
		return CDateTime(StatusTime.year,StatusTime.month,StatusTime.day,StatusTime.hour,StatusTime.minute,StatusTime.second);
	}
	else
		return CDateTime();	
	
}
//***************************************************************************************
CDateTime SQLResult::getColDateTime(const char* szCol)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throw new CSQLException(ErrUnknCol,szCol);
	return getColDateTime(colIdx);
}

NullDouble SQLResult::getColNullDouble(const char* szCol)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throw new CSQLException(ErrUnknCol,szCol);
	return getColNullDouble(colIdx);
}

NullDouble SQLResult::getColNullDouble(int nCol)
{
	SQLINTEGER nullindicator;
	 double dRet;
    SQLGetData(statement, nCol+1, SQL_DOUBLE,&dRet, sizeof(dRet), &nullindicator);
	return nullindicator != SQL_NULL_DATA ? NullDouble(dRet) : NullDouble();
}


double SQLResult::getColFloat(int nCol,double fNull/* = 0.0*/)
{
	 SQLINTEGER nullindicator;
	 double dRet;
    SQLGetData(statement, nCol+1, SQL_DOUBLE,&dRet, sizeof(dRet), &nullindicator);
	return nullindicator != SQL_NULL_DATA ? dRet : fNull;


}
double SQLResult::getColFloat(const char* szCol,double fNull/* = 0.0*/)
{
	int colIdx = ColIndex(szCol);
	if(colIdx < 0) 	throw new CSQLException(ErrUnknCol,szCol);
	return getColFloat(colIdx);
}



//#######################################################################################
//#######################################################################################
CattleSQL::DataDayStats::DataDayStats(): total(0),delta(0)
{
	memset(stat_data,0,sizeof(stat_data));
}
//***************************************************************************************
void CattleSQL::DataDayStats::Commit()
{
	total += delta;
	delta = 0;

	for(int i = 0; i < STATISTICS_DAYS;i++)
	{
		stat_data[1][i] += stat_data[0][i];
		stat_data[0][i] = 0;
		ASSERT(stat_data[0][i] >= 0);
	}
}

void CattleSQL::DataDayStats::Rollback()
{
	 delta = 0;

	for(int i = 0; i < STATISTICS_DAYS;i++)
		stat_data[0][i] = 0;
}
void CattleSQL::DataDayStats::AdjustStat(RawData *data,STATOP op)
{
	int day = WhichDay(data->creation.eventDt);
	if(day >= 0)
		stat_data[0][day] += op;
	delta += op;	
}
int CattleSQL::DataDayStats::WhichDay(CDateTime &dt)
{
	CDateTime now = CDateTime::now();
		
	if(now < dt) return (-1); //system time back changed

	int dayidx = dt.GetDiffrenceInDays(now);
	
	if(dayidx >= STATISTICS_DAYS) return (-1);

	return dayidx;
}


int CattleSQL::DataDayStats::GetDayStat(int day)
{
	if(day < 0 || day >= STATISTICS_DAYS) return (-1);
	return stat_data[0][day]+stat_data[1][day];
}
		
void CattleSQL::CattleStats::AdjustStat(Cow *cowd,DataDayStats::STATOP op/*,bool update*/)
{
	//if(!update)
		stat_cows.AdjustStat(cowd,op);
//    if(cowd->docout == NULL) m_CowBalance += op; 
	
}
void CattleSQL::CattleStats::AdjustStat(InDoc *docd,DataDayStats::STATOP op)
{
	stat_indocs.AdjustStat(docd,op);
}
void CattleSQL::CattleStats::AdjustStat(OutDoc *docd,DataDayStats::STATOP op)
{
	stat_outdocs.AdjustStat(docd,op);
}

void CattleSQL::CattleStats::AdjustStat(Hent *hentd,DataDayStats::STATOP op)
{
	stat_hents.AdjustStat(hentd,op);
}

void CattleSQL::CattleStats::AdjustStat(Invoice *inv,DataDayStats::STATOP op)
{
	stat_invoices.AdjustStat(inv,op);
}

void CattleSQL::CattleStats::Commit()
{
	stat_hents.Commit();
	stat_cows.Commit();
	stat_indocs.Commit();
	stat_outdocs.Commit();
	stat_invoices.Commit();
}
void CattleSQL::CattleStats::Rollback()
{
	stat_hents.Rollback();
	stat_cows.Rollback();
	stat_indocs.Rollback();
	stat_outdocs.Rollback();
	stat_invoices.Rollback();
}

int CattleSQL::CattleStats::GetDayStat(STATDATA data,int day)
{
	int day_stat;
	switch(data)
	{
	case HENTS_STAT:
		day_stat = stat_hents.GetDayStat(day);
		break;
	case COWS_STAT:
		day_stat = stat_cows.GetDayStat(day); 
		break;
	case INDOCS_STAT:
		day_stat = stat_indocs.GetDayStat(day);
		break;
	case OUTDOCS_STAT:
		day_stat = stat_outdocs.GetDayStat(day);
		break;
	case INVOICES_STAT:
		day_stat = stat_invoices.GetDayStat(day);
		break;
	}

	return day_stat;
}

int CattleSQL::CattleStats::GetTotal(STATDATA data)
{
	int total;
	switch(data)
	{
	case HENTS_STAT:
		total = stat_hents.GetTotal();
		break;
	case COWS_STAT:
		total = stat_cows.GetTotal();
		break;
	case INDOCS_STAT:
		total = stat_indocs.GetTotal();
		break;
	case OUTDOCS_STAT:
		total = stat_outdocs.GetTotal();
		break;
	case INVOICES_STAT:
		total = stat_invoices.GetTotal();
		break;
	}

	return total;
}

