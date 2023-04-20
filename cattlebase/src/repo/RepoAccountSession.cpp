#include "stdafx.h"
#include "RepoAccountSession.h"
#include "RepoException.h"
#include "../datatypes/InDoc.h"
#include "PurchaseDownloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

RepoAccountSession::HentsClusterImpl::HentsClusterImpl(Context* pContext,NewPtrFlushArray<RepoHent>& remoteHents) : m_pContext(pContext)
{
	m_RemoteHents.Append(remoteHents);
}

void RepoAccountSession::HentsClusterImpl::Build()
{

	m_ClusterItems.FreeAndFlush();
	CMapEx<CString,const CString&,RepoHent*,RepoHent*> localHentsPerFarmNo;

	Hent* pLocalHent = NULL;
	for(int i = 0,s = m_pContext->GetHentCount();i < s;i++)
	{
		pLocalHent = m_pContext->GetHent(i);
		localHentsPerFarmNo.SetAt(pLocalHent->GetFarmNo(),new RepoHent(*pLocalHent));
	}
		
	RepoHent* pRemoteHent = NULL,*pLocalRepoHent = NULL;
	HentEquivalenceItem* pHentEquivalenceItem = NULL;
	NewPtrFlushArray<RepoHent>::iterator hentIt = m_RemoteHents.begin();
	while(hentIt != m_RemoteHents.end())
	{
		pRemoteHent = *(hentIt++);
		pLocalRepoHent = NULL;
		localHentsPerFarmNo.Lookup(pRemoteHent->GetFarmNo(),pLocalRepoHent);
		pHentEquivalenceItem = new HentEquivalenceItem(pLocalRepoHent,pRemoteHent);
		m_ClusterItems.Add(pHentEquivalenceItem);

		localHentsPerFarmNo.RemoveKey(pRemoteHent->GetFarmNo());
	}

	pLocalRepoHent = NULL;
	CString sLocalHentFarmNo;
	POSITION pos = localHentsPerFarmNo.GetStartPosition();

	while(pos != NULL)
	{
		localHentsPerFarmNo.GetNextAssoc(pos,sLocalHentFarmNo,pLocalRepoHent);
		m_ClusterItems.Add(new HentEquivalenceItem(pLocalRepoHent,NULL));
	}


}

int RepoAccountSession::HentsClusterImpl::GetCount() const
{
	return m_ClusterItems.GetSize();
}

HentEquivalenceItem* RepoAccountSession::HentsClusterImpl::GetItem(int iIndex)
{
	return m_ClusterItems[iIndex];
}

RepoAccountSession::HentsClusterImpl::~HentsClusterImpl()
{
	m_ClusterItems.FreeAndFlush();
}
//#################################################################################################
RepoAccountSession::StocksClusterImpl::StocksClusterImpl(Context* pContext,NewPtrFlushArray<RepoStock>& remoteStocks) : m_pContext(pContext)
{
	m_RemoteStocks.Append(remoteStocks);
}

void RepoAccountSession::StocksClusterImpl::Build()
{
	m_ClusterItems.FreeAndFlush();
	CMapEx<CString,const CString&,RepoStock*,RepoStock*> localStocksPerCode;

	
	Stock* pLocalStock = NULL;
	for(int i = 0,s = m_pContext->GetStockCount();i < s;i++)
	{
		pLocalStock = m_pContext->GetStock(i);
		localStocksPerCode.SetAt(pLocalStock->GetStockCode(),new RepoStock(*pLocalStock));
	}
	
	RepoStock* pRemoteStock = NULL,*pLocalRepoStock = NULL;
	StockEquivalenceItem* pStockEquivalenceItem = NULL;
	NewPtrFlushArray<RepoStock>::iterator stockIt = m_RemoteStocks.begin();
	while(stockIt != m_RemoteStocks.end())
	{
		pRemoteStock = *(stockIt++);
		pLocalRepoStock = NULL;
		localStocksPerCode.Lookup(pRemoteStock->GetStockCode(),pLocalRepoStock);
		pStockEquivalenceItem = new StockEquivalenceItem(pLocalRepoStock,pRemoteStock);
		m_ClusterItems.Add(pStockEquivalenceItem);

		localStocksPerCode.RemoveKey(pRemoteStock->GetStockCode());
	}

	pLocalRepoStock = NULL;
	CString sLocalStockCode;
	POSITION pos = localStocksPerCode.GetStartPosition();

	while(pos != NULL)
	{
		localStocksPerCode.GetNextAssoc(pos,sLocalStockCode,pLocalRepoStock);
		m_ClusterItems.Add(new StockEquivalenceItem(pLocalRepoStock,NULL));
	}
}		

int RepoAccountSession::StocksClusterImpl::GetCount() const
{
	return m_ClusterItems.GetSize();
}
StockEquivalenceItem* RepoAccountSession::StocksClusterImpl::GetItem(int iIndex)
{
	return m_ClusterItems.GetAt(iIndex);
}

RepoAccountSession::StocksClusterImpl::~StocksClusterImpl()
{
	m_ClusterItems.FreeAndFlush();
}

//#################################################################################################
RepoAccountSession::PurachesClusterImpl::PurachesClusterImpl(NewPtrFlushArray<RepoPurchase>& purchases)
{
	NewPtrFlushArray<RepoPurchase>::iterator purchaseIt = purchases.begin();
	while(purchaseIt != purchases.end())
	{
		m_ClusterItems.Add(new PurchaseItem(*(purchaseIt++)));
	}
}

int RepoAccountSession::PurachesClusterImpl::GetCount() const
{
	return m_ClusterItems.GetSize();
}

PurchaseItem* RepoAccountSession::PurachesClusterImpl::GetItem(int iIndex)
{
	return m_ClusterItems.GetAt(iIndex);
}

RepoAccountSession::PurachesClusterImpl::~PurachesClusterImpl()
{
	m_ClusterItems.FreeAndFlush();
}


//#################################################################################################
RepoAccountSession::RepoAccountSession(RepoAccount* pAccount,Context* pContext) : 
	m_pAccount(pAccount),m_pSession(NULL),m_pCompany(NULL),m_pHentsCluster(NULL),m_pContext(pContext),
		m_pStocksCluster(NULL),m_pPurchasesCluster(NULL)
{
	m_pClient = new RepoClient(pAccount->GetAddress());
}

RepoAccountSession::~RepoAccountSession()
{
	delete m_pClient;
	CleanSession();	
}

const RepoAccount* RepoAccountSession::GetAccount() const
{
	return m_pAccount;
}

void RepoAccountSession::UpdateAccount(const RepoAccount& accountUpdate)
{
	if(!IsOpen())
	{
		m_pAccount->CopyFrom(accountUpdate);
	}
	else
	{
		throw new RepoException("cannot update account with open session");
	}
}

void RepoAccountSession::Open()
{
	if(!m_pSession)
	{
		m_pSession = m_pClient->OpenSession(m_pAccount->GetLogin(),m_pAccount->GetPassword());
		NotifyObserversAboutConnect();
	}
}

void RepoAccountSession::NotifyObserversAboutConnect()
{
	NewPtrFlushArray<RepoSessionObserver>::iterator observerIt = m_Observers.begin();
	int index = 0;
	while(observerIt != m_Observers.end())
	{
		(*observerIt)->OnConnected();			
		observerIt++;
	}
}

void RepoAccountSession::NotifyObserversAboutDisconnect()
{
	NewPtrFlushArray<RepoSessionObserver>::iterator observerIt = m_Observers.begin();
	int index = 0;
	while(observerIt != m_Observers.end())
	{
		(*observerIt)->OnDisconnected();
		observerIt++;
	}
}


void RepoAccountSession::CleanSession()
{
	if(m_pCompany)
	{
		delete m_pCompany;
		m_pCompany = NULL;
	}

	if(m_pHentsCluster)
	{
		delete m_pHentsCluster;
		m_pHentsCluster = NULL;
	}

	if(m_pStocksCluster)
	{
		delete m_pStocksCluster;
		m_pStocksCluster = NULL;
	}

	if(m_pPurchasesCluster)
	{
		delete m_pPurchasesCluster;
		m_pPurchasesCluster = NULL;
	}
}

void RepoAccountSession::RegisterObserver(RepoSessionObserver* pObserver)
{
	if(FindObserver(pObserver) >= 0)
		return;

	m_Observers.Add(pObserver);

}

void RepoAccountSession::UnregisterObserver(RepoSessionObserver* pObserver)
{
	int index = FindObserver(pObserver);
	if(index >= 0)
		m_Observers.RemoveAt(index);
}


int RepoAccountSession::FindObserver(RepoSessionObserver* pObserver)
{
	NewPtrFlushArray<RepoSessionObserver>::iterator observerIt = m_Observers.begin();
	int index = 0;
	while(observerIt != m_Observers.end())
	{
		if(*observerIt == pObserver)
			return index;
		index++;
		observerIt++;
	}

	return -1;
	
}

void RepoAccountSession::Close()
{
	throwIfNoSession();
	m_pSession->Close();
	m_pSession = NULL;
	CleanSession();
	NotifyObserversAboutDisconnect();
}


bool RepoAccountSession::IsOpen() const
{
	return m_pSession != NULL;
}

const RepoCompany* RepoAccountSession::GetCompany() const
{
	return m_pCompany;
}


void RepoAccountSession::throwIfNoSession()
{
	if(!m_pSession)
		throw new RepoException("No session.");
}

void RepoAccountSession::FetchCompany()
{
	throwIfNoSession();

	RepoCompany company;	
	m_pSession->FetchCompany(company);

	if(!m_pCompany)
		m_pCompany = new RepoCompany();

	m_pCompany->CopyFrom(company);

}

bool RepoAccountSession::GotCompany()
{
	return m_pCompany != NULL;
}

bool RepoAccountSession::GotHents() const
{
	return m_pHentsCluster != NULL;
}

void RepoAccountSession::FetchHents(SimpleProgress* pProgress)
{
	throwIfNoSession();

	if(m_pHentsCluster != NULL)
	{
		delete m_pHentsCluster;
		m_pHentsCluster = NULL;
	}

	HentCursorPtr hentCursor = m_pSession->OpenHentCursor();
	
	int hentCount = hentCursor->GetCount();
	int fetchSize = max(1,hentCursor->GetLimits().GetMaxFetch());
	int steps = hentCount/fetchSize + (hentCount%fetchSize > 0 ? 1 : 0);
	
	pProgress->SetSteps(steps);

	NewPtrFlushArray<RepoHent> remoteHents;
	
	try
	{		
		while(hentCursor->FetchMany(fetchSize,remoteHents))
		{
			if(!pProgress->Step())
			{
				remoteHents.FreeAndFlush();
				return;
			}
		}

		m_pHentsCluster = new HentsClusterImpl(m_pContext,remoteHents);
		m_pHentsCluster->Build();
	}
	catch(RepoException* e)
	{
		remoteHents.FreeAndFlush();
		throw e;
	}
}

HentsCluster* RepoAccountSession::GetHents()
{
	return m_pHentsCluster;
}

bool RepoAccountSession::GotStocks() const
{
	return m_pStocksCluster != NULL;
}

void RepoAccountSession::FetchStocks(SimpleProgress* pProgress)
{
	throwIfNoSession();

	if(m_pStocksCluster != NULL)
	{
		delete m_pStocksCluster;
		m_pStocksCluster = NULL;
	}

	StockCursorPtr stockCursor = m_pSession->OpenStockCursor();

	int stockCount = stockCursor->GetCount();
	int fetchSize = max(1,stockCursor->GetLimits().GetMaxFetch());
	int steps = stockCount/fetchSize + (stockCount%fetchSize > 0 ? 1 : 0);
	
	pProgress->SetSteps(steps);

	NewPtrFlushArray<RepoStock> remoteStocks;
	
	try
	{
		while(stockCursor->FetchMany(fetchSize,remoteStocks) > 0)
		{
			if(!pProgress->Step())
			{
				remoteStocks.FreeAndFlush();
				return;
			}
		}

		m_pStocksCluster = new StocksClusterImpl(m_pContext,remoteStocks);
		m_pStocksCluster->Build();
	}
	catch(RepoException* e)
	{
		remoteStocks.FreeAndFlush();
		throw e;
	}
	
}

StocksCluster* RepoAccountSession::GetStocks()
{
	return m_pStocksCluster;
}

bool RepoAccountSession::GotPurchases() const
{
	return m_pPurchasesCluster != NULL;
}

void RepoAccountSession::FetchPurchases(SimpleProgress* pProgress)
{
	throwIfNoSession();

	if(m_pPurchasesCluster != NULL)
	{
		delete m_pPurchasesCluster;
		m_pPurchasesCluster = NULL;
	}

	PurchaseCursorPtr purchaseCursor = m_pSession->OpenPurchaseCursor();
	
	int purchaseCount = purchaseCursor->GetCount();
	int fetchSize = max(1,purchaseCursor->GetLimits().GetMaxFetch());
	int steps = purchaseCount/fetchSize + (purchaseCount%fetchSize > 0 ? 1 : 0);
	
	pProgress->SetSteps(steps);	

	NewPtrFlushArray<RepoPurchase> remotePurchases;
	
	
	while(purchaseCursor->FetchMany(fetchSize,remotePurchases) > 0)
	{		
		if(!pProgress->Step())
		{
			remotePurchases.FreeAndFlush();
			return;
		}
	}

	m_pPurchasesCluster = new PurachesClusterImpl(remotePurchases);
	//m_pPurchasesCluster->Build();
}

PurachesCluster* RepoAccountSession::GetPurchases()
{
	return m_pPurchasesCluster;
}


void RepoAccountSession::UploadCompany()
{
	throwIfNoSession();
	const Company* pCompany = m_pContext->GetCompany();
	RepoSyncTransaction* pTransaction = NULL;
	try
	{
		pTransaction = m_pSession->OpenTransaction();
		pTransaction->UpdateCompany(RepoCompany(*pCompany));
		pTransaction->Commit();
		m_pCompany->CopyFrom(*pCompany);
	}
	catch(RepoException* e)
	{
		if(pTransaction)
			pTransaction->Close();
		throw e;
	}

	pTransaction->Close();
}

void RepoAccountSession::DownloadPurchases(DownloadController* pController)
{
	throwIfNoSession();
	if(m_pPurchasesCluster != NULL)
	{
		PurchaseItem *pItem = NULL;
		RepoPurchase* pPurchase = NULL;
		for(int i = 0,s = m_pPurchasesCluster->GetCount();i < s;i++)
		{
			 pItem = m_pPurchasesCluster->GetItem(i);
			 pPurchase = pItem->GetPurchase();
			 if(pItem->IsDoUpdate())
			 {
#ifndef _DEBUG
				if(pPurchase->GetWasDownloaded())
					continue;
#endif
				DownloadPurchase(*pPurchase,pController);

#ifndef _DEBUG
				m_pSession->DownloadPurchase(pPurchase->GetId());
				pPurchase->SetWasDownloaded(true);
#endif
				
				
			 }
		}
	}
}


void RepoAccountSession::DownloadPurchase(const RepoPurchase& purchase,DownloadController* pControler)
{
	PurchaseDownloader downloader(m_pContext);
	downloader.DownloadPurchase(purchase,pControler);	
}

void RepoAccountSession::UpdateStocks(SimpleProgress* pProgress)
{
	throwIfNoSession();
	
	NewPtrFlushArray<StockEquivalenceItem> itemsForReconcilation;
	RepoSyncTransaction* pTransaction = NULL;
	try
	{
		pTransaction = m_pSession->OpenTransaction();
		
		StockEquivalenceItem* pClusterItem = NULL;
		RepoStock* pStock = NULL;
		int nStockCount = m_pStocksCluster->GetCount();
		pProgress->SetSteps(nStockCount);
		bool bCommit = true;
		for(int i = 0;i < nStockCount;i++)
		{
			pClusterItem = m_pStocksCluster->GetItem(i);
			if(pClusterItem->IsDoUpdate() && !pClusterItem->IsEqual())
			{
				pStock = pClusterItem->GetLocal();
				if(pStock)
				{
					pTransaction->UpdateStock(*pStock);
					itemsForReconcilation.Add(pClusterItem);
				}
			}
			else
			{
				
			}

			if(!pProgress->Step())
			{
				bCommit = false;
				break;
			}
		}

		if(bCommit)
			pTransaction->Commit();

		
		NewPtrFlushArray<StockEquivalenceItem>::iterator itemIt = itemsForReconcilation.begin();
		while(itemIt != itemsForReconcilation.end())
		{
			pClusterItem = *itemIt;
			pClusterItem->ReconcileLocal();
			itemIt++;
		}


		
	}
	catch(RepoException* e)
	{
		if(pTransaction)
			pTransaction->Close();
		throw e;
	}

	pTransaction->Close();
}

void RepoAccountSession::DownloadStocks()
{
	throwIfNoSession();
}

void RepoAccountSession::UploadHents(SimpleProgress* pProgress)
{
	throwIfNoSession();
	

	NewPtrFlushArray<HentEquivalenceItem> itemsForReconcilation;
	RepoSyncTransaction* pTransaction = NULL;
	try
	{
		pTransaction = m_pSession->OpenTransaction();
		
		HentEquivalenceItem* pClusterItem = NULL;
		RepoHent* pHent = NULL;
		int nHentCount = m_pHentsCluster->GetCount();
		pProgress->SetSteps(nHentCount);
		bool bCommit = true;
		for(int i = 0;i < nHentCount;i++)
		{
			pClusterItem = m_pHentsCluster->GetItem(i);
			if(pClusterItem->IsDoUpdate() && !pClusterItem->IsEqual())
			{
				pHent = pClusterItem->GetLocal();
				if(pHent)
				{
					pTransaction->UpdateHent(*pHent);
					itemsForReconcilation.Add(pClusterItem);
				}
			}

			if(!pProgress->Step())
			{
				bCommit = false;
				break;
			}
		}

		if(bCommit)
			pTransaction->Commit();


		NewPtrFlushArray<HentEquivalenceItem>::iterator itemIt = itemsForReconcilation.begin();
		while(itemIt != itemsForReconcilation.end())
		{
			pClusterItem = *itemIt;
			pClusterItem->ReconcileLocal();
			itemIt++;
		}
		
	}
	catch(RepoException* e)
	{
		if(pTransaction)
			pTransaction->Close();
		throw e;
	}

	pTransaction->Close();
}

void RepoAccountSession::DownloadHents()
{
	throwIfNoSession();
}
