#ifndef _CATTLESQL_H_
#define _CATTLESQL_H_

#include <odbcinst.h>
#include "query\sql_query.h"
#include "taskprogress/ProgressIFace.h"
#include "datatypes/HentArray.h"
#include "datatypes/ReasonDataArray.h"
#include "datatypes/CountryCodeArray.h"
#include "datatypes/StockDataArray.h"
#include "datatypes/RuleDataArray.h"
#include "datatypes/InvoiceDataArray.h"
#include "datatypes/HentTypeArray.h"
#include "datatypes/UserDataArray.h"
#include "datatypes/ExpenseCategoryArray.h"
#include "datatypes/ExpenseDataArray.h"
#include "datatypes/HerdArray.h"
#include "datatypes/InDoc.h"
#include "datatypes/OutDoc.h"
#include "datatypes/AgentArray.h"
#include "datatypes/DocDataArray.h"
#include "datatypes/CompanyInfo.h"

#include "CattleDBUpdate.h"

using namespace std;

#define MAX_TNAME_LEN    64     // Max size of table names, from afxdb.h
#ifndef MAX
#define MAX(a,b)		((a) >= (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)		((a) <= (b) ? (a) : (b))
#endif

#define STATISTICS_DAYS 30


class CDBVersion
{
public:
	CDBVersion(UINT majorVer,UINT minorVer);
	CDBVersion(const CDBVersion& copy);
	UINT GetMajorVersion() const;
	UINT GetMinorVersion() const;

	CDBVersion& operator=(const CDBVersion& right);
	bool operator>(const CDBVersion& right) const;
	bool operator<(const CDBVersion& right) const;
	bool operator>=(const CDBVersion& right) const;
	bool operator<=(const CDBVersion& right) const;
	bool operator==(const CDBVersion& right) const;
	bool operator!=(const CDBVersion& right) const;
private:
	UINT m_majorVersion;
	UINT m_minorVersion;
};

struct Column
{
	char	name [MAX_TNAME_LEN];
	SWORD	sqlType;
	SQLSMALLINT	nullable;
	int		datalength;					
	
};

class SQLResult
{
public:
	
	SQLResult(): columns(NULL),numberColumns(0),statement(NULL)
	{};

	~SQLResult();
	
	int numCols();

	int ColIndex(const char* szColName);
    const char* ColName(int nCol);
    
    int getColInt(int nCol, int iNullValue=0);
    int getColInt(const char* szCol, int iNullValue=0);

	CString getColStr(int nCol);

    bool getColStr(int nCol, char* szStr, int &len);
	bool getColStr(int nCol, CString &Str);

	bool getColStr(const char* szCol, char* szStr, int &len);
	bool getColStr(const char* szCol, CString &Str);

	CDateTime getColDateTime(int nCol);
	CDateTime getColDateTime(const char* szCol);

	double getColFloat(int nCol,double fNull = 0.0);
	double getColFloat(const char* szColdouble,double fNull = 0.0);
	
	NullDouble getColNullDouble(const char* szCol);
	NullDouble getColNullDouble(int nCol);

	

   /* const unsigned char* getColBlob(int nCol, int& nLen);
    const unsigned char* getColBlob(const char* szCol, int& nLen);*/
   
    bool nextRow();

	void SetStmt(HSTMT stmt);

	
private:
	
    Column *columns;
	int numberColumns;
	HSTMT statement;

};

#define SQLDB (CattleSQL::GetDB())

class CattleSQL 
{

public:
	enum CATTLESQL_LOAD_TASKS
	{
		LOADING_REASONS = 1,
		LOADING_HENTTYPES,
		LOADING_COUNTRIES,
		LOADING_HENTS,
		LOADING_STOCKS,
		LOADING_INDOCS,
		LOADING_OUTDOCS,
		LOADING_RULES,
		LOADING_INVOICES,
		LOADING_PRICES,
		MERGING_VATBUYINVOICES,
		MERGING_VATSELLINVOICES,
		LOADING_COWS,
		LOADING_USERS,
		LOADING_EXPENSECATEGORIES,
		LOADING_EXPENSES,
		LOADING_AGENTS
	};
private:
	CattleSQL();
	static CattleSQL m_DB;
public:
	static CattleSQL& GetDB();

	
	 ~CattleSQL();

	void SQLMessageBox(UINT nResId);
	 
	bool OpenCattleBase(LPCSTR szBaseDir = NULL,LPCSTR szBaseFileName = NULL); 
	bool CloseCattleBase();
	CDBVersion GetDBVersion();
	const CDBVersion GetRequiredDBVersion();
	bool RequiresUpdate();
	bool UpdateToRequired();

	inline CString& GetLastError() { return m_err; };
	void SetLastError(const CString &errStr) { m_err = errStr; };
	void SetLastError(LPCSTR pszErrorStr) { m_err = pszErrorStr; };


	bool Commit();
	bool Rollback();
			
	bool DoBackup(const CString &backupPath);
	bool RestoreBackup(CString &backupPath);
	bool LoadDB(bool bUsersLoaded = true,ITaskProgres *progress = NULL);

	//bool LoadHerds(CHerdArray& herds,ITaskProgres *progress = NULL);
	bool LoadReasons(CReasonDataArray &reasons,ITaskProgres *progress = NULL);
	bool LoadHentTypes(CHentTypeArray &htypes,ITaskProgres *progress = NULL);
	bool LoadCountries(CCountryCodeArray &countries,ITaskProgres *progress = NULL);
	bool LoadHents(CHentArray &hents,CHentTypeArray &htypes,CCountryCodeArray &countries,CUserDataArray &users,ITaskProgres *progress = NULL);
	bool LoadStocks(CStockDataArray &stocks,CRuleDataArray &rules,ITaskProgres *progress = NULL);

	bool LoadInDocs(CDocDataArray &docs,CHentArray &hents,CUserDataArray &users,
		CReasonDataArray &reasons,CInvoiceDataArray &invoices,CAgentArray& agents,ITaskProgres *progress = NULL);

	bool LoadOutDocs(CDocDataArray &docs,CHentArray &hents,CUserDataArray &users,
		CReasonDataArray &reasons,CInvoiceDataArray &invoices,CAgentArray& agents,ITaskProgres *progress = NULL);
	
	bool LoadCows(CCowArray &cows,CCowArray &incows,
				  CDocDataArray &indocs,CDocDataArray &outdocs,
				  CInvoiceDataArray& rr_invoices,CHentArray &hents,
				  CStockDataArray &stocks,CUserDataArray &users,ITaskProgres *progress = NULL);
	bool LoadUsers(CUserDataArray &users,ITaskProgres *progress = NULL);
	bool LoadRules(CRuleDataArray &rules,ITaskProgres *progress = NULL);
	bool LoadInvoices(CInvoiceDataArray &buy_invoices,CInvoiceDataArray &sell_invoices,
					  CInvoiceDataArray &rr_invoices,CUserDataArray &users,ITaskProgres *progress = NULL);
	bool LoadCowInvoiceEntries(CInvoiceDataArray &invoices,CCowArray &cows,CStockDataArray& stocks,ITaskProgres *progress = NULL);
	void MergeVATInvoices(CInvoiceDataArray &inv_ary,UINT taskId,ITaskProgres *progress = NULL);

	bool LoadExpenseCategories(CExpenseCategoryArray& categories,ITaskProgres *progress = NULL);
	bool LoadExpenses(CExpenseDataArray& expenses,CExpenseCategoryArray& categories,CUserDataArray &users,ITaskProgres *progress = NULL);

	bool LoadAgents(CAgentArray& agents,ITaskProgres *progress = NULL);

	bool LoadCompany(CompanyForInitialisation *pCompany);
	bool UpdateCompany(const CompanyForChanging* pCompany);
	bool DropDocs();


	void SetCurrentRawData(RawData &rawData);
	CString GetDBFilePath();

	bool Delete(const char *stmt,bool commit = false);
	bool Insert(const char *stmt,bool commit = false);
	bool Update(const char *stmt,bool commit = false);
	bool Select(const char *stmt, SQLResult &res);
	int  SelectScalar(const char *stmt,int ifNull = 0,int ifError = -1);

	CString m_err; //last error str

	class DataDayStats
	{
	public:

		DataDayStats();
		enum STATOP
		{
			ADDED = 1,
			DELETED = -1
		};

		int GetDayStat(int day);
		void AdjustStat(RawData *data,STATOP op);
		int GetTotal(void) { return total+delta; };
		void Commit();
		void Rollback();
	private:
		int WhichDay(CDateTime &dt);
       	int stat_data[2][STATISTICS_DAYS];
		int total;
		int delta;

    };

	enum STATDATA
	{
		HENTS_STAT,
		COWS_STAT,
		INDOCS_STAT,
		OUTDOCS_STAT,
		INVOICES_STAT
	};
	
	
	class CattleStats
	{
	public:	
		//CattleStats() : m_CowBalance(0) {};
        void AdjustStat(Cow *cowd,DataDayStats::STATOP op/*,bool update = false*/);
		void AdjustStat(InDoc *docd,DataDayStats::STATOP op);
		void AdjustStat(OutDoc *docd,DataDayStats::STATOP op);
		void AdjustStat(Hent *hentd,DataDayStats::STATOP op);
		void AdjustStat(Invoice *inv,DataDayStats::STATOP op);
		int GetDayStat(STATDATA data,int day);
		int GetTotal(STATDATA data);
		void Commit();
		void Rollback();
	private:
		DataDayStats stat_hents;
		DataDayStats stat_cows;
		DataDayStats stat_indocs;
		DataDayStats stat_outdocs;	
		DataDayStats stat_invoices;
	} m_stats;
	

private:
	bool LoadBasics(SQLResult &res,RawData &base,CUserDataArray &users);
	void ErrMsg(SQLHANDLE handle,int handleType = SQL_HANDLE_STMT);
	HSTMT GetStatement(const char *stmt);
	void FreeStatement(HSTMT hStmt);
	SQLRETURN ExecuteStatement(HSTMT hStmt);
	bool DoDbUpdate(const DBUPDATE* pDbUpdate);
	
	
	
	bool m_bOppened;
	HENV m_env;
	HDBC m_Connection;
	SQLHWND hWnd;
	CString m_dbFilePath;

	
	
};





#endif