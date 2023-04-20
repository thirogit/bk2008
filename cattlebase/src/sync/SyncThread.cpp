#include "stdafx.h"
#include "SyncThread.h"
#include "missing/util.h"
#include "missing/StringParser.h"
#include "RejectedBaseDlg.h"
#include "../datalimits.h"
#include "../log.h"
#include "../global_arrays.h"
#include "../exceptions/SyncException.h"
#include "../datatypes/InDoc.h"
#include "../datatypes/OutDoc.h"
#include "../Utils/StringFormat.h"
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char CSyncThread::szSyncPREFIX[] = "SYNC";
const char CSyncThread::szSyncSTART[] = "START";
const char CSyncThread::szOK[] = "OK";
const char CSyncThread::szERR[] = "ERR";
const char CSyncThread::szBAD[] = "BAD";
const char CSyncThread::szHelloMsg[] = "HELLO";
const char CSyncThread::szNextCmd[] = "NEXT";
const char CSyncThread::szInsertCmd[] = "INSERT";
const char CSyncThread::szEOTCmd[] = "EOT";
const char CSyncThread::szEXITCmd[] = "EXIT";
const char CSyncThread::szFLUSHCmd[] = "FLUSH";

#define DELIMETER ','
#define ESCAPER '\\'
#define QUOTER '\''




IMPLEMENT_DYNCREATE(CSyncThread, CWinThread)

CSyncThread::CSyncThread() : m_bAbort(false),m_pParentWnd(NULL),m_pTransceiver(NULL),
										m_bStarted(false),
										m_newInDocArray(NULL),
										m_updInDocArray(NULL),
										m_newOutDocArray(NULL),
										m_newCowArray(NULL),
										m_updCowArray(NULL)
{
}

CSyncThread::~CSyncThread()
{
	LOG("descructor CSyncThread()\n");
}


BOOL CSyncThread::InitInstance()
{
	return TRUE;
}

int CSyncThread::ExitInstance()
{
	//m_bGoSync = false;
	return CWinThread::ExitInstance();
}


BEGIN_MESSAGE_MAP(CSyncThread, CWinThread)
	ON_THREAD_MESSAGE(WM_SYNCSTART,OnStartSync)
	ON_THREAD_MESSAGE(WM_SYNCGET,OnGetSync)
	ON_THREAD_MESSAGE(WM_SYNCFLUSH,OnSyncFlush)
	ON_THREAD_MESSAGE(WM_SYNCSET,OnSetSync)
	ON_THREAD_MESSAGE(WM_SYNCLOCK,OnLock)
END_MESSAGE_MAP()

bool CSyncThread::Start(CWnd *pParentWnd,CSyncTransceiver *pTransceiver)
{
	if(pParentWnd && pTransceiver)
	{
		m_pParentWnd = pParentWnd;
		m_pTransceiver = pTransceiver;
		m_bAbort = false;
		if(!m_bStarted)
		{					
			m_bAutoDelete = false;
			CreateThread();
			m_bStarted = true;
		}

		PostThreadMessage(WM_SYNCSTART,0,0);
		
        return true;
	}
	return false;
}

bool CSyncThread::Continue()
{
	bool bRet;
	CSingleLock lock(&m_abortLock);
	lock.Lock();
	bRet = !m_bAbort;
	lock.Unlock();

	return bRet;
}


void CSyncThread::AbortSync()
{
	
	if(m_bStarted)
	{
		
	CSingleLock lock(&m_abortLock);
	lock.Lock();
	m_bAbort = true;
	lock.Unlock();
		
	}
}


void CSyncThread::SyncLockData()
{
	if(m_bStarted)
		PostThreadMessage(WM_SYNCLOCK,0,0);
}

void CSyncThread::SyncFlush()
{
	if(m_bStarted)
		PostThreadMessage(WM_SYNCFLUSH,0,0);
}

void CSyncThread::SyncGetData()
{
	if(m_bStarted)
		PostThreadMessage(WM_SYNCGET,0,0);
}
void CSyncThread::SyncSetData()
{
	if(m_bStarted)
		PostThreadMessage(WM_SYNCSET,0,0);
}
void CSyncThread::SyncKill()
{
	if(m_bStarted)
	{
		CSingleLock lock(&m_messageLock);
		lock.Lock();
		lock.Unlock();
		PostThreadMessage(WM_QUIT,0,0);
	}
}


void CSyncThread::OnStartSync(WPARAM wp,LPARAM lp) 
{
		
	char szRecvBuffer[MAX_CMD_LEN+1];
	int nLen = 0;
	const int  helloMsgLen = strlen(szHelloMsg);
	//wait for HELLO,message to window
	LOG("Waiting for HELLO.");

	CSingleLock lock(&m_messageLock);
	lock.Lock();

	while(true)
	{
		if(!GetCmd(szRecvBuffer))
			Sleep(WAIT_INTERVAL);
		else
		{
			if(!strncmp(szHelloMsg,szRecvBuffer,helloMsgLen))
				break;
		}
		if(!Continue())
		{
			LOG("Sync start aborted.");
			lock.Unlock();
			return;
		}
	}
	LOG("HELLO received.");

	//send HELLO replay
	
	JustSendCmd(szHelloMsg);

	Sleep(300); //give it some time

	//m_bGoSync = true; 
	SendStatusMsg(STATUS_GOSYNC);
	lock.Unlock();
}

void CSyncThread::SendStatusMsg(SYNC_STATUS_CODES errCode,const char* errMsg/* = NULL*/)
{
		m_pParentWnd->SendMessage(WM_SYNCSTATUS,(WPARAM)errCode,(LPARAM)errMsg);
}

void CSyncThread::SendProgress(int max,int step)
{
	int progress = 100;
	if(max > 0)
			progress = (step*100)/max;
	m_pParentWnd->SendMessage(WM_SYNCSTATUS,STATUS_PROGRESS,(LPARAM)progress);
}




bool CSyncThread::WaitForMsg(char *pMsg)
{
	
	while(true)
	{
		if(!Continue()) return false;
		
		if(GetCmd(pMsg)) break;
		
		Sleep(WAIT_INTERVAL);
		
	}

	return true;
}

bool CSyncThread::GetCmd(char *pCmd)
{
	ASSERT(pCmd);
	return m_pTransceiver->GetCmd(pCmd);
}

bool CSyncThread::PrepareAndSendCmd(const char *pCmd)
{
	char szOutBuffer[MAX_CMD_LEN+1];
	int nLen = _snprintf(szOutBuffer,MAX_CMD_LEN,"%s,%s",szSyncPREFIX,pCmd);
	return JustSendCmd(szOutBuffer,nLen);
}

bool CSyncThread::JustSendCmd(const char *pCmdBuff,int nLen/*=-1*/)
{
	return m_pTransceiver->SendCmd(pCmdBuff,nLen);;
}

bool CSyncThread::ExtractResponse(const char *szMsg,char *pBuff)
{
	const char *comaOccur = NULL;
	comaOccur = strchr(szMsg,','); 
	if(comaOccur)
	{		
		strncpy(pBuff,comaOccur+1,MAX_CMD_LEN); //despite MAX_CMD_LEN param, strncpy will finish when null character occurs
	}
	else
	{
		pBuff[0] = '\0';
	}
	return true;
}

CSyncThread::RET_VALS CSyncThread::WaitForOKorError(char *pErrBuff)
{
	char szRecvBuffer[MAX_CMD_LEN+1];
	
	if(WaitForMsg(szRecvBuffer))
	{
		if(!strncmp(szOK,szRecvBuffer,strlen(szOK)))
		{	
			if(ExtractResponse(szRecvBuffer,pErrBuff))
			{
				return OK;
			}
			else
			{
				strncpy(pErrBuff,szRecvBuffer,MAX_CMD_LEN);
				return PROT_ERR;
			}
		}
		else
		{
			if(!strncmp(szERR,szRecvBuffer,strlen(szERR)) || !strncmp(szBAD,szRecvBuffer,strlen(szBAD)) )
			{
				if(ExtractResponse(szRecvBuffer,pErrBuff))
				{
					return ERR;
				}
				else
				{
					strncpy(pErrBuff,szRecvBuffer,MAX_CMD_LEN);
					return PROT_ERR;
				}
			}
			strncpy(pErrBuff,szRecvBuffer,MAX_CMD_LEN);
			return PROT_ERR;
		}
	}

	return ABORT;
}


bool CSyncThread::SendExitCmd()
{
	return PrepareAndSendCmd(szEXITCmd);
}

bool CSyncThread::SendNextCmd()
{
	return PrepareAndSendCmd(szNextCmd);
}

bool CSyncThread::SendEOTCmd()
{
	return PrepareAndSendCmd(szEOTCmd);
}

void CSyncThread::OnSyncFlush(WPARAM wParam,LPARAM lParam)
{
	DoSyncFlush();
}



bool CSyncThread::DoSyncFlush()
{
	char szBuffer[MAX_CMD_LEN+1];
	bool bRet;
	CSingleLock lock(&m_messageLock);
	lock.Lock();

	SendStatusMsg(STATUS_SYNC_FLUSH);
	PrepareAndSendCmd(szFLUSHCmd);
	bRet = HandleError(WaitForOKorError(szBuffer),szBuffer);
	lock.Unlock();
	return bRet;
}




bool CSyncThread::SyncSetHents(CHentArray &hentArray)
{
	int i,s;
	Hent *pHent = NULL;
	char szBuffer[MAX_CMD_LEN+1];

	SendStatusMsg(STATUS_SYNC_SET_HENTS);
	
	LOG("Sending HENTS.");
	PrepareAndSendCmd("HENTS");
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;

	CString sAlias,sName,sZip,sCity,sStreet,sPobox,sPlate;		
	
	for(i = 0,s = hentArray.GetSize();i < s;i ++)
	{
		pHent = hentArray[i];
		
		sAlias = SyncStringFmtHelper::FormatString(pHent->GetAlias());
		sName = SyncStringFmtHelper::FormatString(pHent->GetName());
		sZip = SyncStringFmtHelper::FormatString(pHent->GetZip());
		sCity = SyncStringFmtHelper::FormatString(pHent->GetCity());
		sStreet = SyncStringFmtHelper::FormatString(pHent->GetStreet());
		sPobox = SyncStringFmtHelper::FormatString(pHent->GetPOBox());
		sPlate = SyncStringFmtHelper::FormatString(pHent->GetPlate());

		_snprintf(szBuffer,sizeof(szBuffer),
			"%s," //cmd
			"%d," //hent id
			"\"%s\"," //alias
			"\"%s\"," //name
			"\"%s\"," //zip
			"\"%s\"," //city
			"\"%s\"," //street
			"\"%s\"," //pobox
			"\"%s\"," //farmno
			"\"%s\"," //plateno
			"%d", //henttype
			szInsertCmd,
			pHent->GetId(),
			(LPCSTR)sAlias,
			(LPCSTR)sName,
			(LPCSTR)sZip,
			(LPCSTR)sCity,
			(LPCSTR)sStreet,
			(LPCSTR)sPobox,
			(LPCSTR)pHent->GetFarmNo(),
			(LPCSTR)sPlate,
			pHent->GetHentType()->henttypeid);


		PrepareAndSendCmd(szBuffer);
		if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
			return false;

		SendProgress(s,i);
	}

	SendProgress(s,i);
	
	SendEOTCmd();
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;
	Sleep(300); //little time to change state
	LOG("Sending HENTS successful.");
	return true;	
}

bool CSyncThread::SyncSetInDocs(CDocDataArray &indocArray)
{
	int i,s;
	Doc *pDoc = NULL;
	char szBuffer[MAX_CMD_LEN+1];

	SendStatusMsg(STATUS_SYNC_SET_INDOCS);
	LOG("Sending INDOCS.");
	PrepareAndSendCmd("INDOCS");
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;

	char docDateBuff[13];
	char loadDateBuff[13];

	CString sPlateNo;
	
	for(i = 0,s = indocArray.GetSize();i < s;i ++)
	{
		pDoc = indocArray[i];

		SyncStringFmtHelper::FormatDate(docDateBuff,sizeof(docDateBuff),pDoc->GetDocDate());
		SyncStringFmtHelper::FormatDate(loadDateBuff,sizeof(loadDateBuff),pDoc->GetLoadDate());
		sPlateNo = SyncStringFmtHelper::FormatString(pDoc->GetPlateNo());

		_snprintf(szBuffer,sizeof(szBuffer),
			"%s," //cmd
			"%d," //doc id
			"%d," //doc hent id
			"%d," //reason id
			"%s," //doc date
			"%s," //load date
			"\"%s\"," //plate no
			"%d", //cow count
			szInsertCmd,
			pDoc->GetId(),
			pDoc->GetHent()->GetId(),
			pDoc->GetReason()->GetId(),
			docDateBuff,
			loadDateBuff,
			(LPCSTR)sPlateNo,
			pDoc->GetCowCount()
			);


		PrepareAndSendCmd(szBuffer);
		if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
			return false;

		SendProgress(s,i);
	}
	SendProgress(s,i);
	
	SendEOTCmd();
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;
	Sleep(300); //little time to change state

	LOG("Sending INDOCS successful.");
	return true;

}

bool CSyncThread::SyncSetCattle(CCowArray &cowArray)
{
	int i,s;
	Cow *pCow = NULL;
	char szBuffer[MAX_CMD_LEN+1];
	const char szIntFmt[] = "%d";
	
	LOG("Sending CATTLE.");

	PrepareAndSendCmd("CATTLE");
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;

	SendStatusMsg(STATUS_SYNC_SET_CATTLE);
	
	char birthDateBuff[13], //v
		 weightBuff[20],//v
		 buypriceBuff[20],//v
		 mypriceBuff[20],//v
		 buystockBuff[10],//v
		 buyweightBuff[20],//v
		 termbuystockBuff[10],
		 termbuyweightBuff[20],
		 termsellstockBuff[10],
		 termsellweightBuff[20],
		 termbuypriceBuff[20],	
		 termsellpriceBuff[20];	
	
	CString sPassNo,sFstOwnr;

	for(i = 0,s = cowArray.GetSize();i < s;i ++)
	{
		pCow = cowArray[i];

		
		buystockBuff[0] = '\0';
		termbuystockBuff[0] = '\0';
		termbuyweightBuff[0] = '\0';
		termsellstockBuff[0] = '\0';
		termsellweightBuff[0] = '\0';
		termbuypriceBuff[0] = '\0';
		termsellpriceBuff[0] = '\0';

		SyncStringFmtHelper::FormatDate(birthDateBuff,sizeof(birthDateBuff),pCow->GetBirthDate());
		sPassNo = SyncStringFmtHelper::FormatString(pCow->GetPassNo());
		SyncStringFmtHelper::FormatDouble(weightBuff,sizeof(weightBuff),NullDouble(pCow->GetWeight().GetDouble()));
		
		SyncStringFmtHelper::FormatDouble(buyweightBuff,sizeof(buyweightBuff),pCow->GetBuyWeight());

		if(pCow->HasFirstOwner())
		{
			sFstOwnr = SyncStringFmtHelper::FormatString(pCow->GetFirstOwner()->GetAlias());
		}
		SyncStringFmtHelper::FormatDouble(buypriceBuff,sizeof(buypriceBuff),pCow->GetBuyPrice());
		SyncStringFmtHelper::FormatDouble(mypriceBuff,sizeof(mypriceBuff),pCow->GetMyPrice());
		
		
		if(pCow->GetBuyStock()) _snprintf(buystockBuff,sizeof(buystockBuff),szIntFmt,pCow->GetBuyStock()->GetId());

		if(pCow->GetTermBuyStock()) _snprintf(termbuystockBuff,sizeof(termbuystockBuff),szIntFmt,pCow->GetTermBuyStock()->GetId());
		if(pCow->GetTermSellStock()) _snprintf(termsellstockBuff,sizeof(termsellstockBuff),szIntFmt,pCow->GetTermSellStock()->GetId());

		SyncStringFmtHelper::FormatDouble(termsellweightBuff,sizeof(termsellweightBuff),pCow->GetTermSellWeight());
		SyncStringFmtHelper::FormatDouble(termbuyweightBuff,sizeof(termbuyweightBuff),pCow->GetTermBuyWeight());

		SyncStringFmtHelper::FormatDouble(termbuypriceBuff,sizeof(termbuypriceBuff),pCow->GetTermBuyPrice());
		SyncStringFmtHelper::FormatDouble(termsellpriceBuff,sizeof(termsellpriceBuff),pCow->GetTermSellPrice());

		_snprintf(szBuffer,sizeof(szBuffer),
			"%s," //cmd
			"\"%s\"," //ean
			"%s,"  //birth date
			"%d," //sex
			"%d," // stock id
			"%s," // weight
			"\"%s\"," //passno
			"\"%s\"," //fstownr
			"%d," //in doc id
			"," 
			"%s," //buyprice
			"%s," //myprice
			"%s," //buystock
			"%s," //buyweight
			"%s," //termbuystock
			"%s," //termbuyweight
			"%s," //termbuyprice
			"%s," //termsellstock
			"%s," //termsellweight
			"%s," //termsellprice
			"%d," // has any buy invoice
			"%d", // ingroup
			szInsertCmd,
			(LPCSTR)pCow->GetEAN(),
			birthDateBuff,
			pCow->GetSex().getCOWSEX(),
			pCow->GetStock()->GetId(),
			weightBuff,
			sPassNo,
			sFstOwnr,
			pCow->GetInDoc()->GetId(),
            buypriceBuff,
			mypriceBuff,
			buystockBuff,
			buyweightBuff, 
			termbuystockBuff,
			termbuyweightBuff,
			termbuypriceBuff,
			termsellstockBuff,
			termsellweightBuff,
			termsellpriceBuff,
			pCow->HasAnyBuyInvoice() ? 1 : 0,
			pCow->GetInGroup().ToInt(NULL_INOUT_GRP));	


		PrepareAndSendCmd(szBuffer);
		if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
			return false;

		SendProgress(s,i);
	}
	SendProgress(s,i);
	
	SendEOTCmd();
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;
	Sleep(300); //little time to change state
	LOG("Sending CATTLE successful.");
	return true;
}

bool CSyncThread::IsEndRecord(const char *endToken)
{
	return strcmp(endToken,"END") == 0;
}

bool CSyncThread::HandleError(RET_VALS retVal,const char *pErrBuffer)
{
	bool bRet = false;

	switch(retVal)
	{

		case PROT_ERR:
			LOG("PROT_ERR: %s",pErrBuffer);
			SendStatusMsg(STATUS_PROTOCOL_ERR,pErrBuffer);
			break;
		case ERR:
			LOG("ERR: %s",pErrBuffer);
			SendStatusMsg(STATUS_ERR,pErrBuffer);
			break;
		case ABORT:
			LOG("ABORTED");
			//SendStatusMsg(STATUS_SYNC_ABORT);
			break;
		case OK:
			LOG("OK: %s",pErrBuffer);
			bRet = true;
			break;
	}

	return bRet;
}

bool CSyncThread::SyncGetCows(CSyncDocDataArray &newInDocArray,CSyncDocDataArray &newOutDocArray,
	CSyncCowDataArray &newCowArray,CSyncCowDataArray &updCowArray,CSyncCowDataArray &rejNewArray,
	CSyncCowDataArray &rejUpdArray)
{
	bool bRun;
	char szBuffer[MAX_CMD_LEN+1];


	SendStatusMsg(STATUS_SYNC_GET_CATTLE);
	LOG("Request CATTLE");

	szBuffer[0] = 0;
	PrepareAndSendCmd("CATTLE");
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;

	int cc = 0,cowCount = 2*atoi(szBuffer);

	LOG("Syncing CATTLE");
	try
	{
		bRun = true;
		do
		{
			SendNextCmd();
			if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
				return false;
						
			bRun = ParseCowRecord(szBuffer,newCowArray,updCowArray);		

			TRACE("%s\n",szBuffer);
			cc++;
			SendProgress(cowCount,cc);
								
		} while(bRun);

	}
	catch(CSyncException *ex)
	{
		//send message
		SendExitCmd();
		HandleError(WaitForOKorError(szBuffer),szBuffer);
		LOG("Error when syncing CATTLE:Code %d, %s",ex->GetSyncErrorCode(),ex->GetRecordsStr());

		SendStatusMsg(STATUS_PROTOCOL_ERR,CStringFormat("CATTLE STATUS_PROTOCOL_ERR %d",ex->GetSyncErrorCode()));
		ex->Delete();
		return false;
	}

	SendExitCmd();
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;
	LOG("Syncing CATTLE successful");


	int i,s;
	Cow *pCow = NULL;
	Hent *pHent = NULL;
	Stock *pStock = NULL;
	Doc * pDoc = NULL;
	bool alreadyRejected = false;
	CSyncDocData *pSyncDoc = NULL;
	
	CSyncCowData *pSyncCow = NULL;
	for(i=0,s=updCowArray.GetSize();i<s;i++)
	{
		alreadyRejected = false;
		pSyncCow = updCowArray[i];
		pCow = ary_incows.GetCowByEAN(pSyncCow->ean);
		if(!pCow)
		{
			pSyncCow->rejectReason |= CSyncCowData::EAN_NOT_FOUND;
			rejUpdArray.Add(pSyncCow);
			updCowArray.RemoveAt(i);
			i--;s--;
			alreadyRejected = true;
		}
		
		if(pCow)
		{
			pSyncCow->srcCow = pCow;
		}

		pStock = ary_stocks.GetStockDataByStockId(pSyncCow->stockid);
		if(!pStock)
		{
			pSyncCow->rejectReason |= CSyncCowData::STOCK_NOT_FOUND;

			if(!alreadyRejected)
			{
				rejUpdArray.Add(pSyncCow);
				updCowArray.RemoveAt(i);
				i--;s--;
				alreadyRejected = true;
			}
		}
		else
		{
			pSyncCow->stock = pStock;
			if(pCow)
			{
				if(pCow->GetStock() != pStock)
				{
					if(pCow->HasAnyBuyInvoice())
					{
						pSyncCow->rejectReason |= CSyncCowData::CHNGS_VIOLATE_INVOICE;
						if(!alreadyRejected)
						{
							rejUpdArray.Add(pSyncCow);
							updCowArray.RemoveAt(i);
							i--;s--;
							alreadyRejected = true;
						}					
					}
					else
						pCow->ChangeStock(pStock);
				}				
			}
		}
		
		if(pSyncCow->docoutno != 0)
		{
			pSyncDoc = newOutDocArray.GetDoc(pSyncCow->docoutno);
			if(pSyncDoc)
			{
				pSyncCow->sync_docout = pSyncDoc;
				if(!alreadyRejected)
					pSyncDoc->cowArray.Add(pSyncCow);
				
			}
			else
			{
					pSyncCow->rejectReason |= CSyncCowData::DOCOUT_NOT_FOUND;
					if(!alreadyRejected)
					{
						rejUpdArray.Add(pSyncCow);
						updCowArray.RemoveAt(i);
						i--;s--;
						alreadyRejected = true;
					}
			}
		}

		
		if(pCow && (pSyncCow->sex != pCow->GetSex()))
		{
			if(pCow->HasAnyBuyInvoice())
			{
				pSyncCow->rejectReason |= CSyncCowData::CHNGS_VIOLATE_INVOICE;
				if(!alreadyRejected)
				{
					rejUpdArray.Add(pSyncCow);
					updCowArray.RemoveAt(i);
					i--;s--;
					alreadyRejected = true;
				}
			}
			else
				pCow->ChangeSex(pSyncCow->sex);
		}
		
		
		if(pCow)
			pCow->ChangeMyPrice(pSyncCow->myprice);

		
		if(pCow && pSyncCow->weight != pCow->GetWeight())
		{			
			if(pCow->HasAnyBuyInvoice())
			{
				pSyncCow->rejectReason |= CSyncCowData::CHNGS_VIOLATE_INVOICE;
				if(!alreadyRejected)
				{
					rejUpdArray.Add(pSyncCow);
					updCowArray.RemoveAt(i);
					i--;s--;
					alreadyRejected = true;
				}
			}
			else
				pCow->ChangeWeight(pSyncCow->weight);
		}	


		if(pSyncCow->termsellstockid)
		{
			pStock = ary_stocks.GetStockDataByStockId(pSyncCow->termsellstockid);
			if(!pStock)
			{
				pSyncCow->rejectReason |= CSyncCowData::TERMSELLSTOCK_NOT_FOUND;

				if(!alreadyRejected)
				{
					rejUpdArray.Add(pSyncCow);
					updCowArray.RemoveAt(i);
					i--;s--;
					alreadyRejected = true;
				}
			}
			else
			{
				pSyncCow->termsellstock = pStock;
				if(pCow)
					pCow->ChangeTermSellStock(pStock);
				
			}
		}

		if(pSyncCow->termbuystockid)
		{
			pStock = ary_stocks.GetStockDataByStockId(pSyncCow->termbuystockid);
			if(!pStock)
			{
				pSyncCow->rejectReason |= CSyncCowData::TERMBUYSTOCK_NOT_FOUND;

				if(!alreadyRejected)
				{
					rejUpdArray.Add(pSyncCow);
					updCowArray.RemoveAt(i);
					i--;s--;
					alreadyRejected = true;
				}
			}
			else
			{
				pSyncCow->termbuystock = pStock;
				if(pCow)
					pCow->ChangeTermBuyStock(pStock);
			}
		}
		else
		{
			pSyncCow->termbuystock = NULL;
			if(pCow)
				pCow->ChangeTermBuyStock(NULL);
		}
		
		if(pCow)
		{
			pCow->ChangeTermBuyWeight(pSyncCow->termbuyweight);
		}
		
		if(pCow)
		{
			pCow->ChangeTermSellWeight(pSyncCow->termsellweight);
		}

		if(pCow)
		{			
			pCow->ChangeTermBuyPrice(pSyncCow->termbuyprice);
		}

		if(pCow)
		{
			pCow->ChangeTermSellPrice(pSyncCow->termsellprice);
		}

		if(pCow)
		{
			pCow->ChangeInGrp(pSyncCow->ingrp);
		}

		cc++;
		SendProgress(cowCount,cc);

	}
	for(i=0,s=newCowArray.GetSize();i<s;i++)
	{
		pSyncCow = newCowArray[i];
		pCow = new Cow(NULL_COWID);
		alreadyRejected = false;		

		if(ary_incows.GetCowByEAN(pSyncCow->ean))
		{
			pSyncCow->rejectReason |= CSyncCowData::DUPLICATE_NOTSOLD_FOUND;
			rejNewArray.Add(pSyncCow);
			newCowArray.RemoveAt(i);
			i--;s--;
			alreadyRejected = true;

		}

		pCow->SetEAN(pSyncCow->ean);
		pSyncCow->srcCow = pCow;
		
		pStock = ary_stocks.GetStockDataByStockId(pSyncCow->stockid);
		if(!pStock)
		{
			pSyncCow->rejectReason |= CSyncCowData::STOCK_NOT_FOUND;
			if(!alreadyRejected)
			{
				rejNewArray.Add(pSyncCow);
				newCowArray.RemoveAt(i);
				i--;s--;
				alreadyRejected = true;
			}
		}
		else
		{
			pSyncCow->stock = pStock;
			pCow->SetStock(pStock);
		}

		
		pSyncDoc = newInDocArray.GetDoc(pSyncCow->docinno);
		if(pSyncDoc)
		{
			pSyncCow->sync_docin = pSyncDoc;
			if(!alreadyRejected)
				pSyncDoc->cowArray.Add(pSyncCow);
		}
		else
		{
			pDoc = NULL;
			if(pSyncCow->docinno != 0)
				pDoc = ary_indocs.GetDocDataByDocId(pSyncCow->docinno);

			if(pDoc)
			{
				pSyncCow->sync_docin = NULL;
				pSyncCow->docin = pDoc;
				pCow->SetInDoc(pDoc);
			}
			else
			{
				pSyncCow->rejectReason |= CSyncCowData::DOCIN_NOT_FOUND;
				if(!alreadyRejected)
				{
					rejNewArray.Add(pSyncCow);
					newCowArray.RemoveAt(i);
					i--;s--;
					alreadyRejected = true;
				}
			}
		}

		if(pSyncCow->docoutno != 0)
		{
			pSyncDoc = newOutDocArray.GetDoc(pSyncCow->docoutno);
			if(pSyncDoc)
			{
				pSyncCow->sync_docout = pSyncDoc;
				if(!alreadyRejected)
					pSyncDoc->cowArray.Add(pSyncCow);
			}
			else
			{
				pSyncCow->rejectReason |= CSyncCowData::DOCOUT_NOT_FOUND;
				if(!alreadyRejected)
				{
					rejNewArray.Add(pSyncCow);
					newCowArray.RemoveAt(i);
					i--;s--;
					alreadyRejected = true;
				}
			
			}
		}

	
		pCow->SetSex(pSyncCow->sex);
		pCow->SetMyPrice(pSyncCow->myprice);
		pCow->SetWeight(pSyncCow->weight);


		if(pSyncCow->termsellstockid)
		{
			pStock = ary_stocks.GetStockDataByStockId(pSyncCow->termsellstockid);
			if(!pStock)
			{
				pSyncCow->rejectReason |= CSyncCowData::TERMSELLSTOCK_NOT_FOUND;

				if(!alreadyRejected)
				{
					rejUpdArray.Add(pSyncCow);
					newCowArray.RemoveAt(i);
					i--;s--;
					alreadyRejected = true;
				}
			}
			else
			{
				pSyncCow->termsellstock = pStock;
				pCow->SetTermSellStock(pStock);
			}
		}

		if(pSyncCow->termbuystockid)
		{
			pStock = ary_stocks.GetStockDataByStockId(pSyncCow->termbuystockid);
			if(!pStock)
			{
				pSyncCow->rejectReason |= CSyncCowData::TERMBUYSTOCK_NOT_FOUND;

				if(!alreadyRejected)
				{
					rejUpdArray.Add(pSyncCow);
					newCowArray.RemoveAt(i);
					i--;s--;
					alreadyRejected = true;
				}
			}
			else
			{
				pSyncCow->termbuystock = pStock;
				pCow->SetTermBuyStock(pStock);
			}
		}

		pCow->SetTermBuyWeight(pSyncCow->termbuyweight);
		pCow->SetTermSellWeight(pSyncCow->termsellweight);
		pCow->SetTermBuyPrice(pSyncCow->termbuyprice);
		pCow->SetTermSellPrice(pSyncCow->termsellprice);
		pCow->SetInGrp(pSyncCow->ingrp);
		
		cc++;
		SendProgress(cowCount,cc);
	}

	return true;

}

bool CSyncThread::SyncUpdDoc(CDocDataArray& docSrcArray,CSyncDocData *pSyncDoc)
{
	bool bRejected = false;
	Doc *pDoc = docSrcArray.GetDocDataByDocId(pSyncDoc->docno);
	Hent *pHent = NULL;
	Reason *pReason = NULL;
		
	if(!pDoc)
	{
		pSyncDoc->rejectReason |= CSyncDocData::DOCNO_NOT_FOUND;
		bRejected = true;
	}
	else 
	{
		pSyncDoc->srcDoc = pDoc;
	}

		
	pHent = ary_hents.GetHentByHentId(pSyncDoc->hentid);
	if(!pHent)
	{
		pSyncDoc->rejectReason |= CSyncDocData::HENT_NOT_FOUND;
		bRejected = true;
				
	}
	else
	{
		pSyncDoc->hent = pHent;
		if(pDoc)
		{
			pDoc->ChangeHent(pHent);
		}
	}
		
	pReason = ary_reasons.GetReasonDataByReasonId(pSyncDoc->reasonid);
	if(!pReason)
	{
		pSyncDoc->rejectReason |= CSyncDocData::REASON_NOT_FOUND;
		bRejected = true;		
	}
	else
	{
		pSyncDoc->reason = pReason;

		if(pDoc)
		{
			pDoc->ChangeReason(pReason);
		}
	}

	if(pDoc)
	{
		pDoc->ChangePlateNo(pSyncDoc->plateno);
	}
	return !bRejected;	
}


bool CSyncThread::SyncNewDoc(Doc* pNewDoc,CSyncDocData *pSyncDoc)
{
	bool bRejected = false;

	pNewDoc->SetDocDate(pSyncDoc->docdate);
	pNewDoc->SetLoadDate(pSyncDoc->loaddate);
	pNewDoc->SetPlateNo(pSyncDoc->plateno);

	Hent* pHent = ary_hents.GetHentByHentId(pSyncDoc->hentid);
	if(!pHent)
	{
		pSyncDoc->rejectReason |= CSyncDocData::HENT_NOT_FOUND;
		bRejected = true;
	}
	else
	{
		pSyncDoc->hent = pHent;
		pNewDoc->SetHent(pHent);
	}
	Reason* pReason = ary_reasons.GetReasonDataByReasonId(pSyncDoc->reasonid);
	if(!pReason)
	{
		pSyncDoc->rejectReason |= CSyncDocData::REASON_NOT_FOUND;
		bRejected = true;
	}
	else
	{
		pSyncDoc->reason = pReason;
		pNewDoc->SetReason(pReason);
	}	
	return !bRejected;		
}

bool CSyncThread::SyncGetInDocs(CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray,
								CSyncDocDataArray &rejectedUpdArray,CSyncDocDataArray &rejectedNewArray)
{
	int i,s;
	CSyncDocData *pSyncDoc = NULL;

	SendStatusMsg(STATUS_SYNC_GET_INDOCS);

	if(!SyncGetDocs("INDOCS",newDocArray,updDocArray))
		return false;


	for(i=0,s = updDocArray.GetSize();i<s;i++)
	{
		pSyncDoc = updDocArray[i];
		if(!SyncUpdDoc(ary_indocs,pSyncDoc))
		{
			rejectedUpdArray.Add(pSyncDoc);
			updDocArray.RemoveAt(i);
			i--;
			s--;
		}
	}

	for(i=0,s = newDocArray.GetSize();i<s;i++)
	{
		pSyncDoc = newDocArray[i];
		pSyncDoc->srcDoc = new InDoc(NULL_DOCID);	

		if(!SyncNewDoc(pSyncDoc->srcDoc,pSyncDoc))
		{
			rejectedNewArray.Add(pSyncDoc);
			newDocArray.RemoveAt(i);
			i--;
			s--;
		}
	}	
	return true;
}


bool CSyncThread::SyncGetOutDocs(CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray,
								 CSyncDocDataArray &rejectedUpdArray,CSyncDocDataArray &rejectedNewArray)
{
	CSyncDocData* pSyncDoc = NULL;
	int i,s;
	SendStatusMsg(STATUS_SYNC_GET_OUTDOCS);

	if(!SyncGetDocs("OUTDOCS",newDocArray,updDocArray))
		return false;



	for(i=0,s = updDocArray.GetSize();i<s;i++)
	{
		pSyncDoc = updDocArray[i];
		if(!SyncUpdDoc(ary_outdocs,pSyncDoc))
		{
			rejectedUpdArray.Add(pSyncDoc);
			updDocArray.RemoveAt(i);
			i--;
			s--;
		}
	}

	for(i=0,s = newDocArray.GetSize();i<s;i++)
	{
		pSyncDoc = newDocArray[i];
		pSyncDoc->srcDoc = new OutDoc(NULL_DOCID);	

		if(!SyncNewDoc(pSyncDoc->srcDoc,pSyncDoc))
		{
			rejectedNewArray.Add(pSyncDoc);
			newDocArray.RemoveAt(i);
			i--;
			s--;
		}
	}	
	return true;		
}

bool CSyncThread::SyncGetDocs(const char* strTableName,CSyncDocDataArray &newDocArray,
							CSyncDocDataArray &updDocArray)
{


	bool bRun;	char szBuffer[MAX_CMD_LEN+1];
	
	LOG("Request %s",strTableName);
	PrepareAndSendCmd(strTableName);

	szBuffer[0]= 0;
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;

	int dc = 0,docCount = atoi(szBuffer);

	LOG("Syncing %s",strTableName);
	bRun = true;

	try
	{
		do
		{
			SendNextCmd();
			if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
			{
				SendExitCmd();
				HandleError(WaitForOKorError(szBuffer),szBuffer);
				return false;
			}
			
			
			bRun = ParseDocRecord(szBuffer,newDocArray,updDocArray);		

			TRACE("%s\n",szBuffer);

			dc++;
			SendProgress(docCount,dc);
								
		} while(bRun);
	}
	catch(CSyncException *ex)
	{
		//send message
		SendExitCmd();
		HandleError(WaitForOKorError(szBuffer),szBuffer);
		SendStatusMsg(STATUS_PROTOCOL_ERR,ex->GetRecordsStr());
		LOG("Error when syncing %s:Code %d, %s",strTableName,ex->GetSyncErrorCode(),ex->GetRecordsStr());
		ex->Delete();
		return false;
	}


	SendExitCmd();
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;
	LOG("Syncing %s successful",strTableName);
	return true;

}

bool CSyncThread::IsNullToken(const char* szToken)
{
	if(szToken == NULL)
		return true;

	if(strlen(szToken) == 0)
		return true;

	return false;
}

bool CSyncThread::ParseCowRecord(const char *szBuffer,CSyncCowDataArray &newCowArray,CSyncCowDataArray &updCowArray)
{
	CStringParser recordParser;
	CParseOptions po(DELIMETER,QUOTER,ESCAPER);
	LPCSTR token = NULL;

	recordParser.Parse(szBuffer,po);

	if(recordParser.GetCount() < 2) throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));
	if(strcmp(recordParser.GetAt(0),"COW")) throw new CSyncException(INVALIDRECORDPREFIX,CString(szBuffer));
	if(IsEndRecord(recordParser.GetAt(1))) return false;

//	if(recordParser.GetCount() != 16) throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));
	if(recordParser.GetCount() != 17) throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));

	CSyncCowData * pSyncCowData = new CSyncCowData();
	pSyncCowData->ean = recordParser.GetAt(1);
	pSyncCowData->sex = (COWSEX)atoi(recordParser.GetAt(2));

	pSyncCowData->stockid = atoi(recordParser.GetAt(3));
	//pSyncCowData->stock = ary_stocks.GetStockDataByStockId(pSyncCowData->stockid);
	pSyncCowData->weight = atoi(recordParser.GetAt(4));
	
	token = recordParser.GetAt(5);
	if(!IsNullToken(token))
	{
		pSyncCowData->myprice = CUtil::myatof(token);	
	}

	pSyncCowData->docinno = atoi(recordParser.GetAt(6)); 
	
	token = recordParser.GetAt(7);
	if(!IsNullToken(token))
	{
		pSyncCowData->docoutno = atoi(token);        
	}	

	token = recordParser.GetAt(8);

	pSyncCowData->outgrp = atoi(token);
	if((pSyncCowData->outgrp < 0 || pSyncCowData->outgrp > MAX_INOUT_GRP) && pSyncCowData->docoutno != 0)
		pSyncCowData->outgrp = NULL_INOUT_GRP;


	token = recordParser.GetAt(9);
	if(!IsNullToken(token))
	{
		pSyncCowData->termbuyprice = CUtil::myatof(token);
	}
	else
	{
		pSyncCowData->termbuyprice.MakeNull();
	}

	token = recordParser.GetAt(10);
	if(!IsNullToken(token))
	{
		pSyncCowData->termbuyweight = CUtil::myatof(token);
		if(!pSyncCowData->termbuyweight.IsNull() && pSyncCowData->termbuyweight < 0.001)
			pSyncCowData->termbuyweight.MakeNull();
	}
	else
	{
		pSyncCowData->termbuyweight.MakeNull();
	}


	token = recordParser.GetAt(11);
	if(!IsNullToken(token))
	{
		pSyncCowData->termbuystockid = atoi(token);
	}
	else
	{
		pSyncCowData->termbuystockid = 0;
	}

	token = recordParser.GetAt(12);
	if(!IsNullToken(token))
	{
		pSyncCowData->termsellprice = CUtil::myatof(token);
	}

	token = recordParser.GetAt(13);
	if(!IsNullToken(token))
	{
		pSyncCowData->termsellweight = CUtil::myatof(token);
		if(!pSyncCowData->termsellweight.IsNull() && pSyncCowData->termsellweight < 0.001)
			pSyncCowData->termsellweight.MakeNull();
	}

	token = recordParser.GetAt(14);
	if(!IsNullToken(token))
	{
		pSyncCowData->termsellstockid = atoi(token);
	}

	token = recordParser.GetAt(15);
	switch(token[0])
	{
		case 'N':
			newCowArray.Add(pSyncCowData);
			break;
		case 'U':
			updCowArray.Add(pSyncCowData);
			break;
		default:
			delete pSyncCowData;
			throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));
	}

//	pSyncCowData->ingrp = NULL_INOUT_GRP;
//#if 0
	token = recordParser.GetAt(16);
	pSyncCowData->ingrp = atoi(token);
	if(pSyncCowData->ingrp < 0 || pSyncCowData->ingrp > MAX_INOUT_GRP)
		pSyncCowData->ingrp = NULL_INOUT_GRP;
//#endif
	

	return true;
}

bool CSyncThread::ParseDocRecord(const char *szBuffer,CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray)
{
	CStringParser recordParser;
	CParseOptions po(DELIMETER,QUOTER,ESCAPER);
	LPCSTR token = NULL;
	const char szDateFormat[] = "%Y%m%d";

	recordParser.Parse(szBuffer,po);
	if(recordParser.GetCount() < 2) throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));
	if(strcmp(recordParser.GetAt(0),"DOC")) throw new CSyncException(INVALIDRECORDPREFIX,CString(szBuffer));

	if(IsEndRecord(recordParser.GetAt(1))) return false;

	if(recordParser.GetCount() != 8) throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));
	


	CSyncDocData * pSyncDocData = new CSyncDocData();


	pSyncDocData->docno = atoi(recordParser.GetAt(1));
	if(!pSyncDocData->docdate.ParseDate(CDateTime::YYYYMMDD,recordParser.GetAt(2),false))
	{
		delete pSyncDocData;
		throw new CSyncException(INVALIDFIELD,CString(recordParser.GetAt(2)));
	}
	if(!pSyncDocData->loaddate.ParseDate(CDateTime::YYYYMMDD,recordParser.GetAt(3),false))
	{
		delete pSyncDocData;
		throw new CSyncException(INVALIDFIELD,CString(recordParser.GetAt(3)));
	}

	pSyncDocData->hentid = atoi(recordParser.GetAt(4));
	pSyncDocData->plateno = SyncStringFmtHelper::DecoceEscapeString(recordParser.GetAt(5));
	pSyncDocData->reasonid = atoi(recordParser.GetAt(6));	
	token = recordParser.GetAt(7);
	switch(token[0])
	{
		case 'N':
			newDocArray.Add(pSyncDocData);
			break;
		case 'U':
			updDocArray.Add(pSyncDocData);
			break;
		default:
			delete pSyncDocData;
			throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));
	}

	return true;
}


void CSyncThread::OnLock(WPARAM wParam,LPARAM lParam)
{
	char szBuffer[MAX_CMD_LEN+1];
	PrepareAndSendCmd("LOCK");
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		SendStatusMsg(STATUS_ERR,szBuffer);
}

bool CSyncThread::IsDataLocked()
{
	char szBuffer[MAX_CMD_LEN+1];
	PrepareAndSendCmd("GOSYNC");
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return true; //if error , return that data is locked
	return (atoi(szBuffer) >  0);
}


void CSyncThread::OnGetSync(WPARAM wp,LPARAM lp)
{
	CSyncDocDataArray rejUpdInDocs,rejNewInDocs,rejUpdOutDocs,rejNewOutDocs;
	CSyncDocDataArray newInDocs,newOutDocs;
	CSyncDocDataArray updInDocs,updOutDocs/*not used*/;
	char szBuffer[MAX_CMD_LEN+1];
	int i,s;
//	CSyncCowData *pCow = NULL;
	CSyncDocData * pDoc = NULL;
	bool bSyncGetOk = false,bSendExit = false;

	CSyncCowDataArray newCowArray,updCowArray,rejNewCowArray,rejUpdCowArray;


	CSyncDocDataArray::AutoArray auto_rejUpdInDocs(rejUpdInDocs);
	CSyncDocDataArray::AutoArray auto_rejNewInDocs(rejNewInDocs);
	CSyncDocDataArray::AutoArray auto_rejUpdOutDocs(rejUpdOutDocs);
	CSyncDocDataArray::AutoArray auto_rejNewOutDocs(rejNewOutDocs);
	CSyncDocDataArray::AutoArray auto_newInDocs(newInDocs);
	CSyncDocDataArray::AutoArray auto_newOutDocs(newOutDocs);
	CSyncDocDataArray::AutoArray auto_updInDocs(updInDocs);
	CSyncDocDataArray::AutoArray auto_updOutDocs(updOutDocs);	
	CSyncCowDataArray::AutoArray auto_newCowArray(newCowArray);
	CSyncCowDataArray::AutoArray auto_updCowArray(updCowArray);
	CSyncCowDataArray::AutoArray auto_rejNewCowArray(rejNewCowArray);
	CSyncCowDataArray::AutoArray auto_rejUpdCowArray(rejUpdCowArray);


	CSingleLock lock(&m_messageLock);
	lock.Lock();

	do
	{


		if(IsDataLocked())
		{
			SendStatusMsg(STATUS_SYNC_DATA_LOCKED);
			break;
		}		

		PrepareAndSendCmd("GET");
		if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
			break;

		bSendExit = true;
		if(!SyncGetInDocs(newInDocs,updInDocs,rejUpdInDocs,rejNewInDocs))
		{			
			LOG("SyncGetInDocs() faild");
			break;		
		}
		if(!SyncGetOutDocs(newOutDocs,updOutDocs,rejUpdOutDocs,rejNewOutDocs))
		{			
			LOG("SyncGetOutDocs() faild");
			break;
		}
		if(!SyncGetCows(newInDocs,newOutDocs,newCowArray,updCowArray,rejNewCowArray,rejUpdCowArray))
		{
			LOG("SyncGetCows() faild");
			break;
		}

		for(i = 0,s = newInDocs.GetSize();i < s;i ++)
		{
			pDoc = newInDocs[i];
			if(!pDoc->cowArray.GetSize())
			{
				pDoc->rejectReason |= CSyncDocData::NO_COWS;
				newInDocs.RemoveAt(i);
				rejNewInDocs.Add(pDoc);
				i--;
				s--;
			}
			
		}

		for(i = 0,s = rejNewInDocs.GetSize();i < s;i ++)
		{
			pDoc = rejNewInDocs[i];
			if(!pDoc->cowArray.GetSize())
				pDoc->rejectReason |= CSyncDocData::NO_COWS;
		}

		for(i = 0,s = newOutDocs.GetSize();i < s;i ++)
		{
			pDoc = newOutDocs[i];
			if(!pDoc->cowArray.GetSize())
			{
				pDoc->rejectReason |= CSyncDocData::NO_COWS;
				newOutDocs.RemoveAt(i);
				rejNewOutDocs.Add(pDoc);
				i--;
				s--;
			}
			
		}

		for(i = 0,s = rejNewOutDocs.GetSize();i < s;i ++)
		{
			pDoc = rejNewOutDocs[i];
			if(!pDoc->cowArray.GetSize())
				pDoc->rejectReason |= CSyncDocData::NO_COWS;
		}	

		if(rejUpdInDocs.GetSize() > 0 ||
		rejNewInDocs.GetSize() > 0 ||
		rejUpdOutDocs.GetSize() > 0 ||
		rejNewOutDocs.GetSize() > 0 ||
		rejNewCowArray.GetSize() > 0 ||
		rejUpdCowArray.GetSize() > 0)
		{
			CRejectedBaseDlg rejectedDlg(rejUpdInDocs, rejNewInDocs, rejUpdOutDocs, rejNewOutDocs,
										newInDocs,newOutDocs, updInDocs,newCowArray,updCowArray,
										rejNewCowArray,rejUpdCowArray,m_pParentWnd);
			
			bSyncGetOk = rejectedDlg.DoModal() == IDOK;
		}
		else
			bSyncGetOk = true;	

		DeleteNewCows(rejNewCowArray);
		DeleteNewDocs(rejNewOutDocs);
		DeleteNewDocs(rejNewInDocs);

	}
	while(false);

	if(bSendExit)
	{
		SendExitCmd();
		if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		{
			LOG("OnGetSync(): exit cmd send faild");
		}
	}

	lock.Unlock();
	SendStatusMsg(STATUS_SYNCING_END);

	if(bSyncGetOk)
	{
		
		ASSERT(	m_newInDocArray &&
				m_updInDocArray  &&
				m_newOutDocArray &&
				m_newCowArray &&
				m_updCowArray
				);

		m_newInDocArray->Append(newInDocs);
		m_updInDocArray ->Append(updInDocs);
		m_newOutDocArray->Append(newOutDocs);
		m_newCowArray->Append(newCowArray);
		m_updCowArray->Append(updCowArray);

		auto_newInDocs.Release();
		auto_updInDocs.Release();
		auto_newOutDocs.Release();
		auto_newCowArray.Release();
		auto_updCowArray.Release();


		SendStatusMsg(STATUS_SYNC_DATA_ARRIVED);                         

	}
	else
	{
		DeleteNewCows(newCowArray);
		DeleteNewDocs(newOutDocs);
		DeleteNewDocs(newInDocs);
	}
}


void CSyncThread::DeleteNewCows(CSyncCowDataArray& newCows)
{	
	CSyncCowDataArray::ArrayIterator cowIt = newCows.iterator();
	while(cowIt.hasNext())
		delete (*(cowIt++))->srcCow;
}
void CSyncThread::DeleteNewDocs(CSyncDocDataArray& newDocs)
{
	CSyncDocDataArray::ArrayIterator docIt = newDocs.iterator();
	while(docIt.hasNext())
		delete (*(docIt++))->srcDoc;
}


bool CSyncThread::SyncSetStocks(CStockDataArray & stockArray)
{
	int i,s;
	Stock *pStock = NULL;
	char szBuffer[MAX_CMD_LEN+1];

	SendStatusMsg(STATUS_SYNC_SET_STOCKS);
	LOG("Sending STOCKS.");
	PrepareAndSendCmd("STOCKS");
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;
	
	for(i = 0,s = stockArray.GetSize();i < s;i ++)
	{
		pStock = stockArray[i];
		_snprintf(szBuffer,sizeof(szBuffer),"%s,%d,\"%s\",%d",szInsertCmd,pStock->GetId(),
												(LPCSTR)pStock->GetStockCode(),pStock->GetPredefSex().getCOWSEX());
		PrepareAndSendCmd(szBuffer);
		if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
			return false;
				
		SendProgress(s,i);
	}
	SendProgress(s,i); //100%

	SendEOTCmd();
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
		return false;

	Sleep(300); //little time to change state
	LOG("Sending STOCKS successful.");
	return true;
}



void CSyncThread::OnSetSync(WPARAM wParam,LPARAM lParam)
{
	CSingleLock lock(&m_messageLock);
	std::set<Doc*> indocsSet;
	char szBuffer[MAX_CMD_LEN+1];
	int i,s;

	Hent *pHent;
	Doc *pDoc;
	do
	{
		if(!DoSyncFlush()) break;
			
		lock.Lock();
		
		PrepareAndSendCmd("SET");
		if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
			break;
		
		CDocDataArray indocsAry;
		for(i = 0,s = ary_incows.GetSize();i<s;i++)
			indocsSet.insert(ary_incows[i]->GetInDoc());

		CHentArray synchents;
		std::set<Hent*> hentSet;

		for(i = 0,s = ary_hents.GetSize();i < s;i++)
		{
			pHent = ary_hents[i];
			if(pHent->GetSyncIt())
				hentSet.insert(pHent);
		}		

		std::set<Doc*>::iterator setItr = indocsSet.begin();
		while(setItr != indocsSet.end())
		{
			pDoc = *setItr;
			indocsAry.Add(pDoc);
			hentSet.insert(pDoc->GetHent());
			setItr++;
		}

		std::set<Hent*>::iterator hentSetItr = hentSet.begin();
		while(hentSetItr != hentSet.end())
		{		
			synchents.Add(*hentSetItr);			
			hentSetItr++;
		}



		if(SyncSetStocks(ary_stocks) &&
			SyncSetHents(synchents)   &&
			SyncSetInDocs(indocsAry)  &&
			SyncSetCattle(ary_incows))
		{
			break;
		}
	} while(false);

	SendExitCmd();
	if(!HandleError(WaitForOKorError(szBuffer),szBuffer))
	{
		LOG("OnGetSync(): exit cmd send faild");
	}
	lock.Unlock();
	SendStatusMsg(STATUS_SYNCING_END);
		
}


void CSyncThread::SetOutputArrays(CSyncDocDataArray &newInDocArray,CSyncDocDataArray &updInDocArray,
					CSyncDocDataArray &newOutDocArray,
					CSyncCowDataArray &newCowArray,CSyncCowDataArray &updCowArray)
{

	m_newInDocArray = &newInDocArray;
	m_updInDocArray = &updInDocArray;
    m_newOutDocArray = &newOutDocArray;
	m_newCowArray = &newCowArray;
	m_updCowArray = &updCowArray;

}