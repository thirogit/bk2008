#include "stdafx.h"
#include "SyncDlg.h"
#include "SyncThread.h"
#include "syncdlg.h"
#include "mainfrm_ctrl.h"
#include "RejectedBaseDlg.h"
#include "../log.h"
#include "CattleSQL.h"
#include "../dispacher/NewOutDocDispatcher.h"
#include "../dispacher/NewInDocDispatcher.h"


IMPLEMENT_DYNAMIC(CSyncDlg, CDialog)
CSyncDlg::CSyncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSyncDlg::IDD, pParent),m_syncTrans(&m_syncClient),
	m_bClientConected(false)
{

}

CSyncDlg::~CSyncDlg()
{
}

void CSyncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SYNCLOG,m_syncLog);
	DDX_Control(pDX,IDC_SENDDATA,m_SendDataBtn);
	DDX_Control(pDX,IDC_GETDATA,m_GetDataBtn);
	DDX_Control(pDX,IDC_SYNCPROGRESS,m_syncProgress);
	DDX_Control(pDX,IDC_HOSTNAMELBL,m_hostNameLabel);
	
	
}


BEGIN_MESSAGE_MAP(CSyncDlg, CDialog)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SYNCSTATUS,OnSyncStatus)
	
	ON_BN_CLICKED(IDC_GETDATA, OnGetData)
	ON_BN_CLICKED(IDC_SENDDATA,OnSendData)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void CSyncDlg::OnOK()
{
	
}
void CSyncDlg::OnDestroy()
{
	m_syncThread.AbortSync();
	m_syncThread.SyncKill();
	m_syncSocket.ShutDown(CSyncSocket::both);
	m_syncSocket.Close();	

}




void CSyncDlg::SetStatus(UINT nStatusId)
{
	CString strStatus;
	strStatus.LoadString(nStatusId);
	GetDlgItem(IDC_SYNCHSTATUS)->SetWindowText(strStatus);
}
void CSyncDlg::EnableButtons(BOOL bEnable)
{
	UINT btns[] =
	{
		IDC_GETDATA,
		IDC_SENDDATA
	};
	CWnd *pBtn = NULL;

	for(int i = 0;i < sizeof(btns)/sizeof(UINT);i++)
	{
		pBtn = GetDlgItem(btns[i]);
		pBtn->EnableWindow(bEnable);
		pBtn->UpdateWindow();
	}
}


BOOL CSyncDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_syncSocket.CreateServerSocket(DEFAULT_PORT,this); 
	if(m_syncSocket.Listen() == FALSE)
	{
		AfxMessageBox("Unable to Listen on that port,please try another port");
		m_syncSocket.Close();			
	}
	EnableButtons(FALSE);
	SetStatus(IDS_WAITINGFORCONNECTION);

	//m_syncThread.Start(this,&m_syncTrans);

	m_SendDataBtn.SetButtonImage(IDB_SYNCSENDDATA);
	m_GetDataBtn.SetButtonImage(IDB_SYNCGETDATA);
	m_syncProgress.SetRange(0,100);


	m_hostNameLabel.SetFont((LPCSTR)NULL,-15);

	CString lblCaption = "Host: ";

	char szHostName[100];
	::gethostname(szHostName,sizeof(szHostName)-1);

	lblCaption += szHostName;
	struct hostent *localHost = gethostbyname(szHostName);

	if(localHost)
	{
		struct in_addr addr;
		int i = 0;
        if (localHost->h_addrtype == AF_INET)
        {
            while (localHost->h_addr_list[i] != 0) 
			{
                addr.s_addr = *(u_long *) localHost->h_addr_list[i++];                
            }

			lblCaption += ", IP: ";
			lblCaption +=inet_ntoa(addr);			
        }
	}

	m_hostNameLabel.SetWindowText(lblCaption);
	
	return TRUE;
}

void CSyncDlg::PrintErrors(CPtrArray& syncRecords)
{
	for(int i = 0;i < syncRecords.GetCount();i++)
	{
		CSyncProcessedStatus* syncRecord = (CSyncProcessedStatus*)syncRecords.GetAt(i);
		if(syncRecord->syncStatus == CSyncProcessedStatus::GOT_ERROR)
			m_syncLog.AddText((LPCSTR)syncRecord->syncError);
	}
}

LRESULT CSyncDlg::OnSyncStatus(WPARAM wParam,LPARAM lParam)
{

	
	UINT errCodeMap[][3] =
	{
		{ CSyncThread::STATUS_ERR, IDS_SYNCERR , 0},
		{ CSyncThread::STATUS_PROTOCOL_ERR, IDS_SYNCPROTERR , 0},
		{ CSyncThread::STATUS_SYNC_SET_STOCKS , IDS_SYNCSETSTOCKS , 0},
		{ CSyncThread::STATUS_SYNC_SET_HENTS , IDS_SYNCSETHENTS , 0},
		{ CSyncThread::STATUS_SYNC_SET_INDOCS , IDS_SYNCSETINDOCS , 0},
		{ CSyncThread::STATUS_SYNC_SET_CATTLE , IDS_SYNCSETCATTLE , 0},
		{ CSyncThread::STATUS_SYNC_FLUSH , IDS_SYNCFLUSH , 0},
		{ CSyncThread::STATUS_SYNCING_END , IDS_SYNCEND , 0},

		/*{ CSyncThread::STATUS_PROGRESS ,0 , 0},
		{ CSyncThread::STATUS_SYNC_ABORT , IDS_SYNCABORT , 1},*/		
		{ CSyncThread::STATUS_SYNC_GET_OUTDOCS, IDS_SYNCGETOUTDOCS , 0},
		{ CSyncThread::STATUS_SYNC_GET_INDOCS, IDS_SYNCGETINDOCS, 0},
		{ CSyncThread::STATUS_SYNC_GET_CATTLE, IDS_SYNCGETCATTLE, 0 },
		{ CSyncThread::STATUS_SYNC_DATA_LOCKED, IDS_SYNCDATALOCKED, 0}
	};

	UINT nStrID = 0;
	CString errStr;
	int i;
	
	switch(wParam)
	{
	case CSyncThread::STATUS_PROGRESS:
		m_syncProgress.SetPos((int)lParam);
	break;
	case CSyncThread::STATUS_GOSYNC:
		EnableButtons(TRUE);
		SetStatus(IDS_SYNCSTATUS_CONNECTED);
	break;
	case CSyncThread::STATUS_SYNC_DATA_ARRIVED:

		if(AfxMessageBox(IDS_ASKCOMMITSYNC,MB_YESNO,0) == IDYES)
		{
			
			if(!UpdateCows(m_updCowArray))
			{
				AfxMessageBox(IDS_GOTERRORUPDATINGCOWSCANTCONTINUE);
				PrintErrors(m_updCowArray);			
				SyncCleanup(true);
				break;
			}

			if(!InsertLonlyCows(m_newCowArray))
			{
				AfxMessageBox(IDS_GOTERRORINSERTINGLONLYCOWSCANTCONTINUE);
				PrintErrors(m_newCowArray);				
				SyncCleanup(true);
				break;
			}

			if(!UpdateInDocs(m_updInDocArray))
			{
				AfxMessageBox(IDS_GOTERRORUPDATINGINDOCSCANTCONTINUE);
				PrintErrors(m_updInDocArray);				
				SyncCleanup(true);
				break;
			}			
			
			if(!InsertInDocs(m_newInDocArray))
			{
				AfxMessageBox(IDS_GOTERRORSYNCINGINDOCSCANTCONTINUE);
				PrintErrors(m_newInDocArray);
				SyncCleanup(true);
				break;
			}				
				
			if(!InsertOutDocs(m_newOutDocArray))
			{				
				AfxMessageBox(IDS_GOTERRORSYNCINGOUTDOCSCANTCONTINUE);
				PrintErrors(m_newOutDocArray);
				SyncCleanup(true);
				break;
			}
		
			CSyncDocDataArray::ArrayIterator docIt = m_newInDocArray.iterator();
			while(docIt.hasNext())
				NewInDocDispatcher::Dispatch((InDoc*)(*(docIt++))->srcDoc);

			docIt = m_newOutDocArray.iterator();
			while(docIt.hasNext())
				NewOutDocDispatcher::Dispatch((OutDoc*)(*(docIt++))->srcDoc);

			UpdateGrid(INCOWSGRID);
			UpdateGrid(ALLCOWSGRID);

			SyncCleanup(false);


			m_syncThread.SyncLockData();		
		}
		else
		{
			SyncCleanup(true);
		}
	break;
	case CSyncThread::STATUS_SYNCING_END:
		EnableButtons(TRUE);
	default:
		for(i = 0; i < sizeof(errCodeMap)/(sizeof(UINT)*2);i++)
		{
			if(errCodeMap[i][0] == (UINT)wParam)
			{
				break;
			}
		}
		nStrID = errCodeMap[i][1];
	break;

	}	
	

	if(nStrID)
	{
		errStr.LoadString(nStrID);
		if(lParam) 
		{
			errStr += ": ";
			errStr += (char*)lParam;
		}
		errStr += "\r\n";
		m_syncLog.AddText((LPCSTR)errStr);
	}
		
	return (LRESULT)0;
}

void CSyncDlg::OnAccept()
{
	if(m_bClientConected) 
	{
		CAsyncSocket asyncSock;
		m_syncSocket.Accept(asyncSock);
		asyncSock.ShutDown(CAsyncSocket::both);
		return;
	}
	m_syncClient.CreateClientSocket(this);
	m_syncSocket.Accept(m_syncClient);
	m_syncThread.Start(this,&m_syncTrans);
	m_bClientConected = true;

}
void CSyncDlg::OnDisconnect()
{
	m_syncThread.AbortSync();
	if(m_bClientConected)
		SetStatus(IDS_SYNCSTATUS_DISCONNECTED);

	EnableButtons(FALSE);
	m_syncClient.Close();
	m_syncClient.ShutDown(CAsyncSocket::both);
	m_bClientConected = false;
	
}
void CSyncDlg::OnConnect()
{
}
void CSyncDlg::OnOutOfBandData()
{
}
void CSyncDlg::OnReceive()
{
	m_syncTrans.ReceiveData();
}
void CSyncDlg::OnSend()
{

}
void CSyncDlg::OnGetData()
{

	
	EnableButtons(FALSE);
	m_syncThread.SetOutputArrays(m_newInDocArray,m_updInDocArray,m_newOutDocArray,
								 m_newCowArray,m_updCowArray);
	m_syncThread.SyncGetData();
}

void CSyncDlg::OnSendData()
{
	if(AfxMessageBox(IDS_WARNSYNCSETDATA,MB_YESNO,0) == IDYES)
	{
		EnableButtons(FALSE);
		m_syncThread.SyncSetData();
	}

}

bool CSyncDlg::UpdateCows(CSyncCowDataArray &cows2update)
{
	CString errorStr;
	bool bGotError = false;
	CSyncCowData *pSyncCow = NULL;
	Cow * pCow = NULL;
	CSyncCowDataArray::ArrayIterator it = cows2update.iterator();
	while(it.hasNext())
	{
		pSyncCow = *(it++);

		pCow = pSyncCow->srcCow;
			
		if(!pCow->UpdateThisCow(false))
		{
			pSyncCow->syncStatus = CSyncProcessedStatus::GOT_ERROR;
			errorStr.Format(IDS_ERRUPDATINGSYNCCOW,(LPCSTR)pCow->GetEAN());
			pSyncCow->syncError = errorStr + ": " + SQLDB.GetLastError();
			LOG("UpdateCows(): %s",(LPCSTR)pSyncCow->syncError);
			bGotError = true;			
		}
		else
			pSyncCow->syncStatus = CSyncProcessedStatus::PROCESSED_NOERROR;
	}
	return !bGotError;
}


bool CSyncDlg::UpdateInDocs(CSyncDocDataArray & indocs2update)
{
	CSyncDocData *pSyncDoc = NULL;
	Doc *updDoc = NULL;
	bool bGotError = false;

	CSyncDocDataArray::ArrayIterator it = indocs2update.iterator();
	while(it.hasNext())
	{
		pSyncDoc = *(it++);
		
		updDoc = pSyncDoc->srcDoc;

		updDoc->ChangeHent(pSyncDoc->hent);
		updDoc->ChangePlateNo(pSyncDoc->plateno);
		updDoc->ChangeReason(pSyncDoc->reason);

		if(updDoc->UpdateThisDoc(false))
		{
			pSyncDoc->syncStatus = CSyncProcessedStatus::PROCESSED_NOERROR;
		}
		else
		{
			bGotError = true;
			pSyncDoc->syncStatus = CSyncProcessedStatus::GOT_ERROR;
			pSyncDoc->syncError.LoadString(IDS_ERROTUPDATINGSYNCINDOC);
			pSyncDoc->syncError += ": ";
			pSyncDoc->syncError += SQLDB.GetLastError();
			LOG("UpdateInDocs(): %s",(LPCSTR)pSyncDoc->syncError);
			
		}
	}
	return !bGotError;
}

bool CSyncDlg::InsertLonlyCows(CSyncCowDataArray & cows2insert)
{
	bool bGotError = false;
	CSyncCowData *pSyncCow = NULL;
	CString errorStr;
	CSyncCowDataArray::ArrayIterator it = cows2insert.iterator();
	while(it.hasNext())
	{
		pSyncCow = *(it++);
		if(!pSyncCow->sync_docin && pSyncCow->docin) // lonly cow has an existing indoc and do not have syncindoc
		{
			if(pSyncCow->srcCow->InsertThisCow(false))
			{
				pSyncCow->docin->GetCows()->Add(pSyncCow->srcCow);
				ary_cows.Add(pSyncCow->srcCow);
				if(!pSyncCow->sync_docout) 
					ary_incows.Add(pSyncCow->srcCow);
				pSyncCow->syncStatus = CSyncProcessedStatus::PROCESSED_NOERROR;
			}
			else
			{
				pSyncCow->syncStatus = CSyncProcessedStatus::GOT_ERROR;
			
				errorStr.Format(IDS_ERRINSLONLYCOW,(LPCSTR)pSyncCow->srcCow->GetEAN());
				pSyncCow->syncError = errorStr + ": " + SQLDB.GetLastError();
				LOG("InsertLonlyCows(): %s",(LPCSTR)pSyncCow->syncError);
				bGotError = true;
			}
		}
	}
	return !bGotError;
}
bool CSyncDlg::InsertInDocs(CSyncDocDataArray & docs2insert)
{
	CSyncDocData *pSyncDoc = NULL;
	Doc * pNewDoc = NULL;
	Cow *pCow = NULL;
	CSyncCowData *pSyncCow = NULL;	
	bool bGotError = false;

	CSyncDocDataArray::ArrayIterator docIt = docs2insert.iterator();
	while(docIt.hasNext())
	{
		pSyncDoc = *(docIt++);
		pNewDoc = pSyncDoc->srcDoc;		
		
		CSyncCowDataArray::ArrayIterator cowIt = pSyncDoc->cowArray.iterator();
		while(cowIt.hasNext())
		{
			pSyncCow = *(cowIt++);
			pCow = pSyncCow->srcCow;
			pCow->SetInDoc(pNewDoc);
			pNewDoc->GetCows()->Add(pCow);
		}

		if(pNewDoc->InsertThisDoc(false))
		{			
			CCowArray::ArrayIterator it = pNewDoc->GetCows()->iterator();
			while(it.hasNext())
			{
				pCow = *(it++); 
				if(!pCow->InsertThisCow(false))
				{
					pSyncDoc->syncStatus = CSyncProcessedStatus::GOT_ERROR;
					pSyncDoc->syncError.LoadString(IDS_ERRINSERTINGSYNCCOW);
					pSyncDoc->syncError += ": ";
					pSyncDoc->syncError += SQLDB.GetLastError();
					bGotError = true;	
					break;
				}
			}

			if(pSyncDoc->syncStatus != CSyncProcessedStatus::GOT_ERROR)
			{				
				pSyncDoc->syncStatus = CSyncProcessedStatus::PROCESSED_NOERROR;
			}
		}
		else
		{				
			pSyncDoc->syncStatus = CSyncProcessedStatus::GOT_ERROR;
			pSyncDoc->syncError.LoadString(IDS_ERRINSERTINGSYNCINDOC);
			pSyncDoc->syncError += ": ";
			pSyncDoc->syncError += SQLDB.GetLastError();
			bGotError = true;
		}		
	}
	return !bGotError;

}
bool CSyncDlg::InsertOutDocs(CSyncDocDataArray & docs2insert)
{
	CSyncDocData *pSyncDoc = NULL;
	Doc * pNewDoc = NULL;
	Cow *pCow = NULL;
	CSyncCowData *pSyncCow = NULL;
	bool bGotError = false;
	
	CSyncDocDataArray::ArrayIterator docIt = docs2insert.iterator();
	while(docIt.hasNext())
	{
		pSyncDoc = *(docIt++);
		pNewDoc = pSyncDoc->srcDoc;
		
		CSyncCowDataArray::ArrayIterator cowIt = pSyncDoc->cowArray.iterator();
		while(cowIt.hasNext())
		{
			pSyncCow = *(cowIt++);
			pCow = pSyncCow->srcCow;
			pNewDoc->GetCows()->Add(pCow);
			pCow->ChangeOutGrp(pSyncCow->outgrp);
		}

		if(pNewDoc->InsertThisDoc(false))
		{				
			CCowArray::ArrayIterator it = pNewDoc->GetCows()->iterator();
			while(it.hasNext())
			{
				pCow = *(it++);				
				pCow->AssignOutDoc(pNewDoc);
				if(!pCow->UpdateThisCow(false))
				{
					pSyncDoc->syncStatus = CSyncProcessedStatus::GOT_ERROR;
					pSyncDoc->syncError.Format(IDS_ERRASSIGNINGSYNCCOWTOSYNCOUTDOC,(LPCSTR)pCow->GetEAN());
					pSyncDoc->syncError += ": ";
					pSyncDoc->syncError += SQLDB.GetLastError();					
					bGotError = true;
					break;					
				}
			}

			if(pSyncDoc->syncStatus != CSyncProcessedStatus::GOT_ERROR)
			{				
				pSyncDoc->syncStatus = CSyncProcessedStatus::PROCESSED_NOERROR;				
			}			
						
		}
		else
		{				
			pSyncDoc->syncStatus = CSyncProcessedStatus::GOT_ERROR;
			pSyncDoc->syncError.LoadString(IDS_ERRINSERTINGSYNCOUTDOC); 
			pSyncDoc->syncError += ": ";
			pSyncDoc->syncError += SQLDB.GetLastError();
			bGotError = true;
		}		
	}
	return !bGotError;
}



void CSyncDlg::OnBnClickedOk()
{
	EndDialog(IDOK);
}

void CSyncDlg::SyncCleanup(bool bCleanupAfterError)
{
	if(bCleanupAfterError)
	{
		SQLDB.Rollback();
		RollbackCowSyncChanges(m_updCowArray);		
			
		CSyncCowDataArray::ArrayIterator syncCowIt = m_newCowArray.iterator();
		while(syncCowIt.hasNext())
			delete (*(syncCowIt++))->srcCow;


		CSyncDocDataArray::ArrayIterator syncInDocIt = m_newInDocArray.iterator();
		while(syncInDocIt.hasNext())
			delete (*(syncInDocIt++))->srcDoc;
			
		CSyncDocDataArray::ArrayIterator syncOutDocIt = m_newOutDocArray.iterator();
		while(syncOutDocIt.hasNext())
			delete (*(syncOutDocIt++))->srcDoc;
	}
	else
	{
		SQLDB.Commit();
		CommitCowSyncChanges(m_updCowArray);
		CommitCowSyncChanges(m_newCowArray);
	}

	m_newOutDocArray.FreeAndFlush();
	m_newCowArray.FreeAndFlush();
	m_newInDocArray.FreeAndFlush();
	m_updInDocArray.FreeAndFlush();
	m_updCowArray.FreeAndFlush();
			
}

void CSyncDlg::RollbackCowSyncChanges(CSyncCowDataArray& syncCows)
{
	CSyncCowDataArray::ArrayIterator syncCowIt = syncCows.iterator();
		while(syncCowIt.hasNext())
			(*(syncCowIt++))->srcCow->RollbackChanges();
}

void CSyncDlg::CommitCowSyncChanges(CSyncCowDataArray& syncCows)
{
	CSyncCowDataArray::ArrayIterator syncCowIt = syncCows.iterator();
		while(syncCowIt.hasNext())
			(*(syncCowIt++))->srcCow->CommitChanges();
}