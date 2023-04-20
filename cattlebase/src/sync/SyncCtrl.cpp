#include "stdafx.h"
#include "SyncCtrl.h"
#include "globals.h"
#include <set>
#include "missing/StringParser.h"



CCriticalSection CSyncCtrl::m_cs;




const char SyncStringFmtHelper::quotedFmt[] = "\"%s\"";

const char SyncStringFmtHelper::dateFmt[] = "\"%Y-%m-%d\"";


#define DELIMETER ','
#define ESCAPER '\\'
#define QUOTER '\''


CString SyncStringFmtHelper::DecoceEscapeString(const CString & aString)
{
	char escapeChar = '\\';
	CString retStr;
	char oneChar;

	for(int i = 0,l = aString.GetLength();i<l;i++)
	{
		oneChar = aString.GetAt(i);
		if(oneChar == '\\') continue;
		retStr += oneChar;
	}
	return retStr;
}

void SyncStringFmtHelper::FormatDate(char *outBuff,int maxsize,const CDateStr & aDate)
{
	ASSERT(maxsize);

	if(aDate.IsNull())
		outBuff[0] = '\0';
	else
		strncpy(outBuff,aDate.FormatDate(dateFmt),maxsize);
	

}

CString SyncStringFmtHelper::EncodeDoubleQuotes(const CString & aString)
{
	int aLen = aString.GetLength();
	char aChar;
	CString outStr;
	char *outBuff = outStr.GetBuffer(aLen*2 + 1);

	for(int i = 0,j = 0;i < aLen;i++,j++)
	{
		aChar = aString.GetAt(i);
		if(aChar == '"')
		{
			outBuff[j++] = '"';
			outBuff[j] = '"';
		}
		else
			outBuff[j] = aChar;
	}
	outBuff[j] = '\0';
	outStr.ReleaseBuffer();
	return outStr;
}

void SyncStringFmtHelper::FormatString(char *outBuff,int maxsize,const CString & aString)
{
	ASSERT(maxsize);

	if(aString.IsEmpty())
		outBuff[0] = '\0';
	else
		_snprintf(outBuff,maxsize,quotedFmt,(LPCSTR)EncodeDoubleQuotes(aString));
}

void SyncStringFmtHelper::FormatInt(char *outBuff,int maxsize,const NullInt & aInt)
{
	ASSERT(maxsize);

	if(aInt.IsNull())
		outBuff[0] = '\0';
	else
		strncpy(outBuff,(LPCSTR)aInt.GetString(),maxsize);

}

void SyncStringFmtHelper::FormatMoney(char *outBuff,int maxsize,const Money & aMoney)
{

	Money bMoney;
	bMoney = aMoney;
	ASSERT(maxsize);
	if(aMoney < 0.01)
		outBuff[0] = '\0';
	else
		strncpy(outBuff,(LPCSTR)bMoney,maxsize);
}



CSyncCtrl::CSyncCtrl() : m_pThread(NULL),m_bThreadRunning(false),m_bAbort(false),m_pParentWnd(NULL),
						 m_pTransceiver(NULL)
{
}

CSyncCtrl::~CSyncCtrl()
{
}
	

bool CSyncCtrl::Continue()
{
	bool bRet;


	//TRACE("Continue() : begin\n");
	CSingleLock lock(&m_cs);
	
	// Lock
	lock.Lock();
		
	bRet = !m_bAbort;

	// Unlock
	lock.Unlock();
	//TRACE("Continue() : end\n");
	return bRet;
}

void CSyncCtrl::Abort()
{
	
	CSingleLock lock(&m_cs);
	lock.Lock();
	m_bAbort = true;
	lock.Unlock();

}

bool CSyncCtrl::Start(CWnd *pParentWnd,CSyncTransceiver *pTransceiver)
{
	ASSERT(pParentWnd);
	ASSERT(pTransceiver);

	if(m_bThreadRunning)
	    	return false;
	m_pParentWnd = pParentWnd;
	m_pTransceiver = pTransceiver;

	m_bAbort = false;
	
	m_pThread = ::AfxBeginThread( ThreadSync, (LPVOID) this, THREAD_PRIORITY_BELOW_NORMAL);
	if(!m_pThread) return false;
		m_bThreadRunning = true;

	return true;
}

bool CSyncCtrl::IsRunning()
{
	return m_bThreadRunning;
}

int CSyncCtrl::PrepareCmd(const char *pCmd,char *szOutBuffer)
{
	return _snprintf(szOutBuffer,MAX_CMD_LEN,"%s,%s\n",szSyncPREFIX,pCmd);
}


bool CSyncCtrl::WaitForMsg(char *pMsg)
{
	
	while(true)
	{
		if(!Continue()) return false;
		
		if(GetCmd(pMsg)) break;
		
		Sleep(WAIT_INTERVAL);
		
	}

	return true;
}

bool CSyncCtrl::GetCmd(char *pCmd)
{
	ASSERT(pCmd);
	return m_pTransceiver->GetCmd(pCmd);
}

bool CSyncCtrl::PrepareAndSendCmd(const char *pCmd)
{
	char szBuffer[MAX_CMD_LEN+1];
	int nLen = PrepareCmd(pCmd,szBuffer);
	return JustSendCmd(szBuffer,nLen);
}

bool CSyncCtrl::JustSendCmd(const char *pCmdBuff,int nLen/*=-1*/)
{
	return m_pTransceiver->SendCmd(pCmdBuff,nLen);;
}

bool CSyncCtrl::ExtractResponse(const char *szMsg,char *pBuff)
{
	char *comaOccur = NULL;
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

CSyncCtrl::RET_VALS CSyncCtrl::WaitForOKorError(char *pErrBuff)
{
	char szRecvBuffer[MAX_CMD_LEN+1];
	
	if(WaitForMsg(szRecvBuffer))
	{
		if(!strncmp(szOK,szRecvBuffer,strlen(szOK)))
		{	
			return (ExtractResponse(szRecvBuffer,pErrBuff) ? OK : BAD);
		}
		else
		{
			if(!strncmp(szERR,szRecvBuffer,strlen(szERR)))
			{
				return (ExtractResponse(szRecvBuffer,pErrBuff) ? ERR : BAD);
			}
			return BAD;
		}
	}

	return ABORT;
}


bool CSyncCtrl::SendExitCmd()
{
	return PrepareAndSendCmd(szEXITCmd);
}

bool CSyncCtrl::SendNextCmd()
{
	return PrepareAndSendCmd(szNextCmd);
}

bool CSyncCtrl::SendEOTCmd()
{
	return PrepareAndSendCmd(szEOTCmd);
}

void CSyncCtrl::SendStatusMsg(WPARAM wParam,LPARAM lParam/* = 0*/)
{
	if(m_pParentWnd)
		m_pParentWnd->SendMessage(WM_SYNCSTATUS,wParam,lParam);
}


bool CSyncCtrl::HandleError(CSyncCtrl::RET_VALS retVal,CSyncCtrl *pSyncCtrl,const char *pErrBuffer)
{
	bool bRet = false;

	switch(retVal)
	{

		case BAD:
			LOG("BAD: %s",pErrBuffer);
			pSyncCtrl->SendStatusMsg(STATUS_PROTOCOL_ERR,(LPARAM)pErrBuffer);
			break;
		case ERR:
			LOG("ERR: %s",pErrBuffer);
			pSyncCtrl->SendStatusMsg(STATUS_ERR,(LPARAM)pErrBuffer);
			break;
		case ABORT:
			LOG("ABORTED");
			pSyncCtrl->SendStatusMsg(STATUS_SYNC_ABORT);
			break;
		case OK:
			LOG("OK: %s",pErrBuffer);
			bRet = true;
			break;
	}

	return bRet;
}

bool CSyncCtrl::SyncSetStocks(CSyncCtrl *pSyncCtrl,CStockDataArray & stockArray)
{
	int i,s;
	StockData *pStock = NULL;
	char szBuffer[MAX_CMD_LEN+1];

	LOG("Sending STOCKS.");
	pSyncCtrl->PrepareAndSendCmd("STOCKS");
	if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
		return false;

	pSyncCtrl->SendStatusMsg(STATUS_SYNC_SET_STOCKS);
	pSyncCtrl->SendStatusMsg(STATUS_PROGRESS,0);
	

	for(i = 0,s = stockArray.GetSize();i < s;i ++)
	{
		pStock = stockArray[i];
		_snprintf(szBuffer,sizeof(szBuffer),"%s,%d,\"%s\"",szInsertCmd,pStock->stockid,(LPCSTR)pStock->stockcode);
		pSyncCtrl->PrepareAndSendCmd(szBuffer);
		if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
			return false;
				
		pSyncCtrl->SendStatusMsg(STATUS_PROGRESS,(i*100)/s);

	}

	pSyncCtrl->SendEOTCmd();
	Sleep(300); //little time to change state
	LOG("Sending STOCKS successful.");
	return true;
}


bool CSyncCtrl::SyncSetHents(CSyncCtrl *pSyncCtrl,CHentDataArray &hentArray)
{
	int i,s;
	HentData *pHent = NULL;
	char szBuffer[MAX_CMD_LEN+1];

	LOG("Sending HENTS.");
	pSyncCtrl->PrepareAndSendCmd("HENTS");
	if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
		return false;

	pSyncCtrl->SendStatusMsg(STATUS_SYNC_SET_HENTS);
	pSyncCtrl->SendStatusMsg(STATUS_PROGRESS,0);

	char aliasBuff[MAXALIAS*2+1+2];
	char nameBuff[MAXNAME*2+1+2];
	char zipBuff[MAXZIP*2+1+2];
	char cityBuff[MAXCITY*2+1+2];
	char streetBuff[MAXSTREET*2+1+2];
	char poboxBuff[MAXPOBOX*2+1+2];
	char plateBuff[MAXPLATE*2+1+2];		
	
	for(i = 0,s = hentArray.GetSize();i < s;i ++)
	{
		pHent = hentArray[i];
		
		SyncStringFmtHelper::FormatString(aliasBuff,sizeof(aliasBuff),pHent->alias);
		SyncStringFmtHelper::FormatString(nameBuff,sizeof(nameBuff),pHent->name);
		SyncStringFmtHelper::FormatString(zipBuff,sizeof(zipBuff),pHent->zip);
		SyncStringFmtHelper::FormatString(cityBuff,sizeof(cityBuff),pHent->city);
		SyncStringFmtHelper::FormatString(streetBuff,sizeof(streetBuff),pHent->street);
		SyncStringFmtHelper::FormatString(poboxBuff,sizeof(poboxBuff),pHent->pobox);
		SyncStringFmtHelper::FormatString(plateBuff,sizeof(plateBuff),pHent->plate);

		_snprintf(szBuffer,sizeof(szBuffer),
			"%s,%d,%s,%s,%s,%s,%s,%s,\"%s\",%s,%d",
			szInsertCmd,
			pHent->hentid,
			aliasBuff,
			nameBuff,
			zipBuff,
			cityBuff,
			streetBuff,
			poboxBuff,
			(LPCSTR)pHent->arimrno,
			plateBuff,
			 pHent->henttype->henttypeid);
		pSyncCtrl->PrepareAndSendCmd(szBuffer);
		if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
			return false;
	}
	
	pSyncCtrl->SendEOTCmd();
	Sleep(300); //little time to change state
	LOG("Sending HENTS successful.");
	return true;	
}

bool CSyncCtrl::SyncSetInDocs(CSyncCtrl *pSyncCtrl,CDocDataArray &indocArray)
{
	int i,s;
	DocData *pDoc = NULL;
	char szBuffer[MAX_CMD_LEN+1];

	LOG("Sending INDOCS.");
	pSyncCtrl->PrepareAndSendCmd("INDOCS");
	if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
		return false;

	pSyncCtrl->SendStatusMsg(STATUS_SYNC_SET_INDOCS);

	pSyncCtrl->SendStatusMsg(STATUS_PROGRESS,0);

	char docDateBuff[13];
	char loadDateBuff[13];

	char plateNoBuff[MAXPLATE*2+3];	
	
	for(i = 0,s = indocArray.GetSize();i < s;i ++)
	{
		pDoc = indocArray[i];

		SyncStringFmtHelper::FormatDate(docDateBuff,sizeof(docDateBuff),pDoc->docdate);
		SyncStringFmtHelper::FormatDate(loadDateBuff,sizeof(loadDateBuff),pDoc->loaddate);
		SyncStringFmtHelper::FormatString(plateNoBuff,sizeof(plateNoBuff),pDoc->plateno);

		_snprintf(szBuffer,sizeof(szBuffer),
			"%s,%d,%d,%d,%s,%s,%s",
			szInsertCmd,
			pDoc->docno,
			pDoc->hent->hentid,
			pDoc->reason->reasonid,
			docDateBuff,
			loadDateBuff,
			plateNoBuff
			);


		pSyncCtrl->PrepareAndSendCmd(szBuffer);
		if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
			return false;
	}
	
	pSyncCtrl->SendEOTCmd();
	Sleep(300); //little time to change state

	LOG("Sending INDOCS successful.");
	return true;

}

bool CSyncCtrl::SyncSetCattle(CSyncCtrl *pSyncCtrl,CCowDataArray &cowArray)
{
	int i,s;
	CowData *pCow = NULL;
	char szBuffer[MAX_CMD_LEN+1];
	const char szIntFmt[] = "%d";
	
	LOG("Sending CATTLE.");

	pSyncCtrl->PrepareAndSendCmd("CATTLE");
	if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
		return false;

	pSyncCtrl->SendStatusMsg(STATUS_SYNC_SET_CATTLE);

	pSyncCtrl->SendStatusMsg(STATUS_PROGRESS,0);

	char birthDateBuff[13], //v
		 weightBuff[10],//v
		 passnoBuff[13],//v
		 fstownrBuff[10],//v
		 buypriceBuff[20],//v
		 mypriceBuff[20],//v
		buystockBuff[10],//v
		 buyweightBuff[10];//v

	for(i = 0,s = cowArray.GetSize();i < s;i ++)
	{
		pCow = cowArray[i];

		fstownrBuff[0] = '\0';
		buystockBuff[0] = '\0';

		SyncStringFmtHelper::FormatDate(birthDateBuff,sizeof(birthDateBuff),pCow->birthdate);
		SyncStringFmtHelper::FormatString(passnoBuff,sizeof(passnoBuff),pCow->passno);
		SyncStringFmtHelper::FormatInt(weightBuff,sizeof(weightBuff),pCow->weight);
		
		SyncStringFmtHelper::FormatInt(buyweightBuff,sizeof(buyweightBuff),pCow->buyweight);

		if(pCow->fstownr) _snprintf(fstownrBuff,sizeof(fstownrBuff),szIntFmt,pCow->fstownr->hentid);
		SyncStringFmtHelper::FormatMoney(buypriceBuff,sizeof(buypriceBuff),pCow->buyprice);
		SyncStringFmtHelper::FormatMoney(mypriceBuff,sizeof(mypriceBuff),pCow->myprice);
		
		
		if(pCow->buystock) _snprintf(buystockBuff,sizeof(buystockBuff),szIntFmt,pCow->buystock->stockid);

		_snprintf(szBuffer,sizeof(szBuffer),
			"%s,\"%s\",%s,"
			"%d,%d,%s,"
			"%s,%s,%d,,"
			"%s,%s,%s,%s",
			szInsertCmd,
			(LPCSTR)pCow->ean,birthDateBuff,
			(COWSEX)pCow->sex,pCow->stock->stockid,weightBuff,
			passnoBuff,fstownrBuff,pCow->docin->docno,
            buypriceBuff,mypriceBuff,buystockBuff,buyweightBuff);	


		pSyncCtrl->PrepareAndSendCmd(szBuffer);
		if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
			return false;
	}
	
	pSyncCtrl->SendEOTCmd();
	Sleep(300); //little time to change state
	LOG("Sending CATTLE successful.");
	return true;
}

bool CSyncCtrl::ParseCowRecord(const char *szBuffer,CSyncCowDataArray &newCowArray,CSyncCowDataArray &updCowArray)
{
	CStringParser recordParser;
	CParseOptions po(DELIMETER,QUOTER,ESCAPER);
	LPCSTR token = NULL;

	recordParser.Parse(szBuffer,po);

	if(recordParser.GetCount() > 1) throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));	

	if(IsEndRecord(recordParser.GetAt(1))) return false;

	if(recordParser.GetCount() != 9) throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));
	if(strcmp(recordParser.GetAt(0),"COW")) throw new CSyncException(INVALIDRECORDPREFIX,CString(szBuffer));


	CSyncCowData * pSyncCowData = new CSyncCowData();
	pSyncCowData->ean = recordParser.GetAt(1);
	pSyncCowData->sex = atoi(recordParser.GetAt(2));

	pSyncCowData->stockid = atoi(recordParser.GetAt(3));
	//pSyncCowData->stock = ary_stocks.GetPtrByStockId(pSyncCowData->stockid);
	pSyncCowData->weight = atoi(recordParser.GetAt(4));
	
	token = recordParser.GetAt(5);
	if(token[0])
		pSyncCowData->myprice = my_atof(token);
	else
		pSyncCowData->myprice = ZEROPRICE;

	pSyncCowData->docinno = atoi(recordParser.GetAt(6));

	token = recordParser.GetAt(7);
	if(token[0])
	{
		pSyncCowData->docoutno = atoi(token);        
	}	

	//pSyncCowData->docout = ary_outdocs.GetPtrByDocNo(pSyncCowData->docoutno);
	//pSyncCowData->docin = ary_indocs.GetPtrByDocNo(pSyncCowData->docinno);

	token = recordParser.GetAt(8);
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

	return true;
}


bool CSyncCtrl::IsEndRecord(const char *endToken)
{
	return strcmp(endToken,"END") == 0;
}

bool CSyncCtrl::SyncGetCows(CSyncCtrl *pSyncCtrl,CSyncCowDataArray &newCowArray,CSyncCowDataArray &updCowArray,CSyncCowDataArray &rejArray)
{
	bool bRun;
	char szBuffer[MAX_CMD_LEN+1];

	LOG("Request CATTLE");
	pSyncCtrl->PrepareAndSendCmd("CATTLE");
	if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
		return false;

	LOG("Syncing CATTLE");
	try
	{
		bRun = true;
		do
		{
			pSyncCtrl->SendNextCmd();
			if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
				return false;
			
			bRun = ParseCowRecord(szBuffer,newCowArray,updCowArray);		

			TRACE("%s\n",szBuffer);
								
		} while(bRun);

	}
	catch(CSyncException *ex)
	{
		//send message
		pSyncCtrl->SendExitCmd();
		HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer);
		LOG("Error when syncing CATTLE:Code %d, %s",ex->GetErrorCode(),ex->GetErrorStr());
		return false;
	}

	if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
		return false;
	LOG("Syncing CATTLE successful");


	int i,s;
	CowData *pCow = NULL;
	HentData *pHent = NULL;
	StockData *pStock = NULL;
	DocData * pDoc = NULL;
	
	CSyncCowData *pSyncCow = NULL;
	for(i=0,s=updCowArray.GetSize();i<s;i++)
	{
		pSyncCow = updCowArray[i];
		pCow = ary_incows.GetCow(pSyncCow->ean);
		if(!pCow)
		{
			pSyncCow->rejectReason += EAN_NOT_FOUND;
			rejectedArray.Add(pSyncCow);
			updCowArray.RemoveAt(i);
			i--;s--;
			continue;
		}
		
		pSyncCow->srcCow = pCow;
		pCow->SQLUpdate.Reset()

		pStock = ary_stock.GetPtrByStockId(pSyncCow->stockid);
		if(!pStock)
		{
			pSyncCow->rejectReason += STOCK_NOT_FOUND;
			rejectedArray.Add(pSyncCow);
			updCowArray.RemoveAt(i);
			i--;s--;
			continue;
		}
		else
		{
			if(pCow->stock != pStock)
			{
				pCow->SQLUpdate.Set(CowData::update_stock);
				pSyncCow->stock = pStock;
			}
		}

		
		pDoc = ary_indocs.GetPtrByDocNo(pSyncCow->docinno);

		if(pDoc->docin != pDoc)
			throw;

		if(pSyncCow->docoutno > 0)
		{
			pDoc = ary_outdocs.GetPtrByDocNo(pSyncCow->docoutno);
			if(!pDoc)
			{
				pSyncCow->rejectReason += DOCOUT_NOT_FOUND;
				rejectedArray.Add(pSyncCow);
				updCowArray.RemoveAt(i);
				i--;s--;
				continue;
			}
			else
			{
				if(pCow->docout != pDoc)
				{
					pCow->SQLUpdate.Set(CowData::update_docout);
					pSyncCow->docout = pDoc;
				}
			}
		}

	
	if(pSyncCow->sex != pCow->sex)
	{
		pCow->SQLUpdate(CowData::update_sex);
	}

	if(pSyncCow->myprice != pCow->myprice)
	{
		pCow->SQLUpdate(CowData::update_myprice);
	}

	if(pSyncCow->weight != pCow->weight)
	{
		pCow->SQLUpdate(CowData::update_weight);
	}


	return true;

}

bool CSyncCtrl::ParseDocRecord(const char *szBuffer,CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray)
{
	CStringParser recordParser;
	CParseOptions po(DELIMETER,QUOTER,ESCAPER);
	LPCSTR token = NULL;
	const char szDateFormat[] = "%d/%m/%Y";

	recordParser.Parse(szBuffer,po);
	if(recordParser.GetCount() > 1) throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));	

	if(IsEndRecord(recordParser.GetAt(1))) return false;

	if(recordParser.GetCount() != 8) throw new CSyncException(INVALIDRECORDFORMAT,CString(szBuffer));
	if(strcmp(recordParser.GetAt(0),"DOC")) throw new CSyncException(INVALIDRECORDPREFIX,CString(szBuffer));


	CSyncDocData * pSyncDocData = new CSyncDocData();


	pSyncDocData->docno = atoi(recordParser.GetAt(1));
	if(!pSyncDocData->docdate.ScanfDate(szDateFormat,recordParser.GetAt(2),false))
		throw new CSyncException(INVALIDFIELD,CString(recordParser.GetAt(2)));
	if(pSyncDocData->loaddate.ScanfDate(szDateFormat,recordParser.GetAt(3),false))
		throw new CSyncException(INVALIDFIELD,CString(recordParser.GetAt(3)));

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
bool CSyncCtrl::SyncGetDocs(CSyncCtrl *pSyncCtrl,const char* strTableName,CSyncDocDataArray &newDocArray,
							CSyncDocDataArray &updDocArray)
{


	bool bRun;	char szBuffer[MAX_CMD_LEN+1];
	
	LOG("Request %s",strTableName);
	pSyncCtrl->PrepareAndSendCmd(strTableName);
	if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
		return false;
	LOG("Syncing %s",strTableName);
	bRun = true;

	try
	{
		do
		{
			pSyncCtrl->SendNextCmd();
			if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
			{
				pSyncCtrl->SendExitCmd();
				HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer);
				return false;
			}
			
			
			bRun = ParseDocRecord(szBuffer,newDocArray,updDocArray);		

			TRACE("%s\n",szBuffer);
								
		} while(bRun);
	}
	catch(CSyncException *ex)
	{
		//send message
		pSyncCtrl->SendExitCmd();
		HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer);
		LOG("Error when syncing %s:Code %d, %s",strTableName,ex->GetErrorCode(),ex->GetErrorStr());
		return false;
	}


	pSyncCtrl->SendExitCmd();
	if(!HandleError(pSyncCtrl->WaitForOKorError(szBuffer),pSyncCtrl,szBuffer))
		return false;
	LOG("Syncing %s successful",strTableName);
	return true;

}


bool CSyncCtrl::SyncGetInDocs(CSyncCtrl *pSyncCtrl,CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray,CSyncDocDataArray &rejectedArray)
{
	if(!SyncGetDocs(pSyncCtrl,"INDOCS",newDocArray,updDocArray))
		return false;

	int i,s;
	DocData *pDoc = NULL;
	HentData *pHent = NULL;
	ReasonData *pReason = NULL;
	CSyncDocData *pSyncDoc = NULL;
	for(i=0,s=updDocArray.GetSize();i<s;i++)
	{
		pSyncDoc = updDocArray[i];
		pDoc = ary_indocs.GetPtrByDocNo(pSyncDoc->docno);
		if(!pDoc)
		{
			pSyncDoc->rejectReason += DOCNO_NOT_FOUND;
			rejectedArray.Add(pSyncDoc);
			updDocArray.RemoveAt(i);
			i--;s--;
			continue;
		}
		
		pSyncDoc->srcDoc = pDoc;
		pDoc->SQLUpdate.Reset()

		if(pDoc->hent->hentid != pSyncDoc->hentid)
		{
			pHent = ary_hents.GetPtrByHentId(pSyncDoc->hentid);
			if(!pHent)
			{
				pSyncDoc->rejectReason += HENT_NOT_FOUND;
			}
			else
			{
				pSyncDoc->hent = pHent;
				pDoc->SQLUpdate.Set(DocData::update_hent);
			}
		}

		if(pDoc->reason->reasonid != pSyncDoc->reasonid)
		{
			pReason = ary_reasons.GetPtrByReasonId(pSyncDoc->reasonid);
			if(!pReason)
			{
				pSyncDoc->rejectReason += REASON_NOT_FOUND;
			}
			else
			{
				pSyncDoc->reason = pReason;
				pDoc->SQLUpdate.Set(DocData::update_reason);
			}
		}

		if(pSyncDoc->plateno != pDoc->plateno)
		{
			pDoc->SQLUpdate.Set(DocData::update_plateno);
		}
	}

}

bool CSyncCtrl::SyncGetOutDocs(CSyncCtrl *pSyncCtrl,CSyncDocDataArray &newDocArray,CSyncDocDataArray &updDocArray,CSyncDocDataArray &rejectedArray)
{
	if(!SyncGetDocs(pSyncCtrl,"OUTDOCS",newDocArray,updDocArray))
		return false;

}

UINT CSyncCtrl::ThreadSync(LPVOID pVoid)
{
	CSyncCtrl *pPDUCtrl = (CSyncCtrl*)pVoid;
	char szRecvBuffer[MAX_CMD_LEN+1];
	int i,s,nLen = 0;

	
	const int  helloMsgLen = strlen(szHelloMsg);

	bool bAbortThread = false;
	//wait for HELLO,message to window

	LOG("Waiting for HELLO.");
	while(true)
	{
		
		if(!pPDUCtrl->GetCmd(szRecvBuffer))
			Sleep(WAIT_INTERVAL);
		else
		{
			if(!strncmp(szHelloMsg,szRecvBuffer,helloMsgLen))
				break;
		}
		if(!pPDUCtrl->Continue())
		{
			bAbortThread = true;
			break;
		}
	}

	LOG("HELLO received.");

	while(!bAbortThread)
	{

		//send HELLO replay
		nLen = _snprintf(szRecvBuffer,MAX_CMD_LEN,"%s\n",szHelloMsg);
		pPDUCtrl->JustSendCmd(szRecvBuffer,nLen);

		Sleep(300); //give some time

		LOG("Sending START.");
		nLen = pPDUCtrl->PrepareCmd(szSyncSTART,szRecvBuffer);
		pPDUCtrl->JustSendCmd(szRecvBuffer,nLen);		
		if(!HandleError(pPDUCtrl->WaitForOKorError(szRecvBuffer),pPDUCtrl,szRecvBuffer))
			break;

		LOG("START ack.");
		
		
		std::set<DocData*> indocsSet;

		CSyncDocDataArray rejInDocs,rejOutDocs;
		CSyncDocDataArray newInDocs,newOutDocs;
		CSyncDocDataArray updInDocs,updOutDocs;

		CSyncCowDataArray newCowArray,updCowArray,rejCowArray;

		SyncGetDocs(pPDUCtrl,"INDOCS",newInDocs,updInDocs,rejInDocs);
		SyncGetDocs(pPDUCtrl,"OUTDOCS",newOutDocs,updOutDocs,rejOutDocs);
		SyncGetCows(pPDUCtrl,newCowArray,updCowArray,rejCowArray);

		pPDUCtrl->PrepareAndSendCmd(szFLUSHCmd);
		if(!HandleError(pPDUCtrl->WaitForOKorError(szRecvBuffer),pPDUCtrl,szRecvBuffer))
			break;

		CDocDataArray indocsAry;
		for(i = 0,s = ary_incows.GetSize();i<s;i++)
			indocsSet.insert(ary_incows[i]->docin);

		std::set<DocData*>::iterator setItr = indocsSet.begin();
		while(setItr != indocsSet.end())
		{
			indocsAry.Add(*setItr);
			setItr++;
		}

		if(SyncSetStocks(pPDUCtrl,ary_stocks)
			&&
		   SyncSetHents(pPDUCtrl,ary_hents) 
		    &&
		   SyncSetInDocs(pPDUCtrl,indocsAry)
		    &&
		   SyncSetCattle(pPDUCtrl,ary_incows)
		   )
		{

		   //MergeRecords(pPDUCtrl);
		   //SyncFlush(pPDUCtrl);

		   //SyncSetCattle(pPDUCtrl);
		   //SyncSetIndocs(pPDUCtrl);
		   //SyncSetOutDocs(pPDUCtrl);
		}

	break;	
	}
	pPDUCtrl->m_bThreadRunning = false;
	return 0;
}


void CSyncCtrl::Wait4Thread()
{

	if(m_bThreadRunning)
		WaitForSingleObject(m_pThread->m_hThread,INFINITE);

}

//IMPLEMENT_DYNCREATE(PlaySound1, CWinThread)

BEGIN_MESSAGE_MAP(PlaySound1, CWinThread)
    ON_THREAD_MESSAGE(WM_ON_RUN,OnRun)
END_MESSAGE_MAP()
void OnRun(WPARAM wp,LPARAM lp) 
{
 .....
 .....
}
