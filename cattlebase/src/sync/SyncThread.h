#if !defined(SYNCTHREAD_H__INCLUDED_)
#define SYNCTHREAD_H__INCLUDED_

#include "SyncSocket.h"
#include "DataTypes.h"
#include "SyncHelper.h"

#define WAIT_INTERVAL 100
#define WM_SYNCSTATUS (WM_APP+1001)

#define WM_SYNCSTART			(WM_APP + 101)
#define WM_SYNCFLUSH			(WM_USER + 103)
#define WM_SYNCGET				(WM_USER + 104)
#define WM_SYNCSET				(WM_USER + 105)
#define WM_SYNCLOCK				(WM_USER + 106)




class CSyncThread : public CWinThread
{
	DECLARE_DYNCREATE(CSyncThread)
protected:

// Attributes
public:

	enum SYNC_STATUS_CODES
	{
		STATUS_GOSYNC = 1001,
		STATUS_ERR,
		STATUS_PROTOCOL_ERR,
		STATUS_SYNC_SET_STOCKS,
		STATUS_SYNC_SET_HENTS,
		STATUS_SYNC_SET_INDOCS,
		STATUS_SYNC_SET_CATTLE,
		STATUS_SYNC_FLUSH,
		STATUS_SYNCING_END,
		STATUS_PROGRESS,
		STATUS_SYNC_GET_OUTDOCS,
		STATUS_SYNC_GET_INDOCS,
		STATUS_SYNC_GET_CATTLE,
		STATUS_SYNC_DATA_ARRIVED,
		STATUS_SYNC_DATA_LOCKED
	};

	CSyncThread();      
	virtual ~CSyncThread();
	
// Operations
public:

	bool Start(CWnd *pParentWnd,CSyncTransceiver *pTransceiver);
	void AbortSync();

	void SyncGetData();
	void SyncSetData();
	void SyncSync();
	void SyncKill();
	void SyncFlush();
	void SyncLockData();

	void SetOutputArrays(CSyncDocDataArray &newInDocArray,CSyncDocDataArray &updInDocArray,
					CSyncDocDataArray &newOutDocArray,
					CSyncCowDataArray &newCowArray,CSyncCowDataArray &updCowArray);


private:

	enum RET_VALS
	{
		PROT_ERR = -1,
		ERR,
		OK,
		ABORT
	};

	bool Continue();
	
	
	bool SyncSetStocks(CStockDataArray & stockArray);
	bool SyncSetHents(CHentArray &hentArray);
	bool SyncSetInDocs(CDocDataArray &indocArray);
	bool SyncSetCattle(CCowArray &cowArray);

	bool HandleError(RET_VALS retVal,const char *pErrBuffer);
	bool SyncGetDocs(const char* strTableName,CSyncDocDataArray &newDocArray,
							CSyncDocDataArray &updDocArray);
	bool ParseDocRecord(const char *szBuffer,CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray);
	bool SyncGetCows(CSyncDocDataArray &newInDocArray,CSyncDocDataArray &newOutDocArray,
							CSyncCowDataArray &newCowArray,CSyncCowDataArray &updCowArray,
							CSyncCowDataArray &rejNewArray,CSyncCowDataArray &rejUpdArray);
	bool ParseCowRecord(const char *szBuffer,CSyncCowDataArray &newCowArray,CSyncCowDataArray &updCowArray);
	bool IsEndRecord(const char *endToken);
	bool IsNullToken(const char* szToken);

	bool SyncGetOutDocs(CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray,
							CSyncDocDataArray &rejectedUpdArray,CSyncDocDataArray &rejectedNewArray);
	bool SyncGetInDocs(CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray,
						CSyncDocDataArray &rejectedUpdArray,CSyncDocDataArray &rejectedNewArray);
	bool SyncUpdDoc(CDocDataArray& docSrcArray,CSyncDocData *pSyncDoc);
	bool SyncNewDoc(Doc* pNewDoc,CSyncDocData *pSyncDoc);
	
	CSyncDocDataArray* m_newInDocArray;
	CSyncDocDataArray* m_updInDocArray;
    CSyncDocDataArray* m_newOutDocArray;
	CSyncCowDataArray* m_newCowArray;
	CSyncCowDataArray* m_updCowArray;
	


	bool WaitForMsg(char *pMsg);
	bool GetCmd(char *pCmd);
	bool JustSendCmd(const char *pCmdBuff,int nLen = -1);
	RET_VALS WaitForOKorError(char *pErrBuff);
	
	bool ExtractResponse(const char *szMsg,char *pBuff);
	bool SendNextCmd();
	bool SendEOTCmd();
	bool SendExitCmd();
	bool PrepareAndSendCmd(const char *pCmd);
	bool DoSyncFlush();
	
	bool IsDataLocked();
	
	void SendStatusMsg(SYNC_STATUS_CODES errCode,const char* errMsg = NULL);
	void SendProgress(int max,int step);

	void DeleteNewCows(CSyncCowDataArray& newCows);
	void DeleteNewDocs(CSyncDocDataArray& newDocs);

private:
	
	//virtual BOOL IsIdleMessage(MSG* pMsg);
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	static const char szSyncPREFIX[];
	static const char szSyncSTART[];
	static const char szOK[];
	static const char szERR[];
	static const char szHelloMsg[];
	static const char szNextCmd[];
	static const char szInsertCmd[];
	static const char szEOTCmd[];
	static const char szEXITCmd[];
	static const char szFLUSHCmd[];
	static const char szBAD[];

	CWnd * m_pParentWnd;
	CSyncTransceiver *m_pTransceiver;
	bool m_bAbort;
	bool m_bStarted;
	//bool m_bGoSync;
	CCriticalSection m_messageLock;
	CCriticalSection m_abortLock;


protected:
	afx_msg void OnStartSync(WPARAM wp,LPARAM lp);
	afx_msg void OnAbortSync(WPARAM wp,LPARAM lp);
	afx_msg void OnGetSync(WPARAM wp,LPARAM lp);
	afx_msg void OnSetSync(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSyncFlush(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLock(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


#endif 
