#ifndef __SYNCCTRL_H__
#define __SYNCCTRL_H__

#include "SyncSocket.h"
#include "DataTypes.h"

#define WM_SYNCSTATUS (WM_APP+1001)

enum EXCEPTION_CODES
{
	INVALIDRECORDFORMAT = 1,
	INVALIDRECORDPREFIX,
	INVALIDFIELD
};

class CSyncException : public CException
{
public:
	CSyncException(EXCEPTION_CODES errorCode,CString &recordStr) : m_errorCode(errorCode),m_recordStr(recordStr) {};
	EXCEPTION_CODES GetErrorCode() { return m_errorCode; };
	LPCSTR GetErrorStr() { return (LPCSTR)m_recordStr; };
private:
	EXCEPTION_CODES m_errorCode;
	CString m_recordStr;
};


class CSyncDocData
{
public:

	CSyncDocData() : docno(0),hent(NULL),
		reason(NULL),rejectReason(NOT_REJECTED),hentid(0),reasonid(0),srcDoc(NULL)	{}
	int docno;
	DocData *srcDoc;
	CDateStr docdate;
	CDateStr loaddate;
	int hentid;
	HentData *hent;
	CString plateno;
	int reasonid;
	ReasonData *reason;

	enum REJECT_REASON
	{
		NOT_REJECTED = 0,
		DOCNO_NOT_FOUND = 1,
		HENT_NOT_FOUND = 2,
		REASON_NOT_FOUND = 4
	};
	
	REJECT_REASON rejectReason;
};


class CSyncCowData
{
public:

	CSyncCowData() : stock(NULL),weight(-1),
		myprice(ZEROPRICE),docin(NULL),docout(NULL),
		rejectReason(NOT_REJECTED),stockid(0),docinno(0),docoutno(0),
		sync_docin(NULL),sync_docout(NULL),srcCow(NULL) {}
	CowData *srcCow;
	CString ean;
	CowSex sex;
	int stockid;
	StockData *stock;
	NullInt weight;
	Money myprice;
	int docinno;
	DocData *docin;
	CSyncDocData *sync_docin;
	int docoutno;
	DocData *docout;
	CSyncDocData *sync_docout;

	enum REJECT_REASON
	{
		NOT_REJECTED = 0,
		EAN_NOT_FOUND = 1,
		STOCK_NOT_FOUND = 2,
		DOCIN_NOT_FOUND = 4,
		DOCOUT_NOT_FOUND = 8
	};
	
	REJECT_REASON rejectReason;
};



typedef CTypedPtrArray<CPtrArray,CSyncDocData*> CSyncDocDataArray;
typedef CTypedPtrArray<CPtrArray,CSyncCowData*> CSyncCowDataArray;

class SyncStringFmtHelper
{
public:
	static void FormatDate(char *outBuff,int maxsize,const CDateStr & aDate);
	static void FormatString(char *outBuff,int maxsize,const CString & aString);
	static void FormatInt(char *outBuff,int maxsize,const NullInt & aInt);
	static void FormatMoney(char *outBuff,int maxsize,const Money & aMoney);
	static CString EncodeDoubleQuotes(const CString & aString);
	static CString DecoceEscapeString(const CString & aString);
private:
	static const char quotedFmt[];
	static const char dateFmt[];
};



class  
{
public:

	

	CSyncCtrl();
	virtual ~CSyncCtrl();
	
	void Abort();	
	bool IsRunning();

	void Wait4Thread();

protected:

	enum RET_VALS
	{
		BAD = -1,
		ERR,
		OK,
		ABORT
	};

	bool Continue();
	
	static UINT ThreadSync(LPVOID pVoid);
	//static bool SyncInDocs(CSyncCtrl *pSyncCtrl);
	static bool SyncSetStocks(CSyncCtrl *pSyncCtrl,CStockDataArray & stockArray);
	static bool SyncSetHents(CSyncCtrl *pSyncCtrl,CHentDataArray &hentArray);
	static bool SyncSetInDocs(CSyncCtrl *pSyncCtrl,CDocDataArray &indocArray);
	static bool SyncSetCattle(CSyncCtrl *pSyncCtrl,CCowDataArray &cowArray);
	static bool HandleError(CSyncCtrl::RET_VALS retVal,CSyncCtrl *pSyncCtrl,const char *pErrBuffer);
	static bool SyncGetDocs(CSyncCtrl *pSyncCtrl,const char* strTableName,CSyncDocDataArray &newDocArray,
							CSyncDocDataArray &updDocArray);
	static bool ParseDocRecord(const char *szBuffer,CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray);
	static bool SyncGetCows(CSyncCtrl *pSyncCtrl,CSyncCowDataArray &newCowArray,
							CSyncCowDataArray &updCowArray,CSyncCowDataArray &rejArray);
	static bool ParseCowRecord(const char *szBuffer,CSyncCowDataArray &newCowArray,CSyncCowDataArray &updCowArray);
	static bool IsEndRecord(const char *endToken);


	static CCriticalSection m_cs;
	


	bool WaitForMsg(char *pMsg);
	bool GetCmd(char *pCmd);
	bool JustSendCmd(const char *pCmdBuff,int nLen = -1);
	RET_VALS WaitForOKorError(char *pErrBuff);
	int PrepareCmd(const char *pCmd,char *szOutBuffer);
	bool ExtractResponse(const char *szMsg,char *pBuff);
	bool SendNextCmd();
	bool SendEOTCmd();
	bool SendExitCmd();
	bool PrepareAndSendCmd(const char *pCmd);
	void SendStatusMsg(WPARAM wParam,LPARAM lParam = 0);
private:
	CWinThread* m_pThread;
	bool m_bThreadRunning;
	
	CWnd * m_pParentWnd;
	CSyncTransceiver *m_pTransceiver;

	bool m_bAbort;

	

};


#endif