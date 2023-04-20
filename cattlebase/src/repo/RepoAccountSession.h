#ifndef __REPOACCOUNTSESSION_H__
#define __REPOACCOUNTSESSION_H__

#include "RepoAccount.h"
#include "RepoCompany.h"
#include "RepoClient.h"
#include "../taskprogress/SimpleProgress.h"
#include "Cluster.h"
#include "context/Context.h"
#include "HentEquivalenceItem.h"
#include "StockEquivalenceItem.h"
#include "PurchaseItem.h"
#include "RepoSessionObserver.h"
#include "DownloadController.h"

typedef Cluster<HentEquivalenceItem> HentsCluster;
typedef Cluster<StockEquivalenceItem> StocksCluster;
typedef Cluster<PurchaseItem> PurachesCluster;

class RepoAccountSession
{	
	class HentsClusterImpl : public HentsCluster
	{
	public:
		HentsClusterImpl(Context* pContext,NewPtrFlushArray<RepoHent>& remoteHents);
		void Build();
		virtual int GetCount() const;
		virtual HentEquivalenceItem* GetItem(int iIndex);
		virtual ~HentsClusterImpl();
	private:
		NewPtrFlushArray< HentEquivalenceItem > m_ClusterItems;
		NewPtrFlushArray<RepoHent> m_RemoteHents;
		Context* m_pContext;

		friend class RepoAccountSession;
	};

	class StocksClusterImpl : public StocksCluster
	{
	public:	
		StocksClusterImpl(Context* pContext,NewPtrFlushArray<RepoStock>& remoteStocks);
		void Build();
		virtual int GetCount() const;
		virtual StockEquivalenceItem* GetItem(int iIndex);
		virtual ~StocksClusterImpl();
	private:
		NewPtrFlushArray< StockEquivalenceItem > m_ClusterItems;
		NewPtrFlushArray<RepoStock> m_RemoteStocks;
		Context* m_pContext;

		friend class RepoAccountSession;
	};

	class PurachesClusterImpl : public PurachesCluster
	{
	public:	
		PurachesClusterImpl(NewPtrFlushArray<RepoPurchase>& purchases);
		virtual int GetCount() const;
		virtual PurchaseItem* GetItem(int iIndex);
		virtual ~PurachesClusterImpl();
	private:
		NewPtrFlushArray< PurchaseItem > m_ClusterItems;		

		friend class RepoAccountSession;
	};
public:
	RepoAccountSession(RepoAccount* pAccount,Context* pContext);
	~RepoAccountSession();
	const RepoAccount* GetAccount() const;
	void UpdateAccount(const RepoAccount& accountUpdate);
	bool IsOpen() const;
	void Open();
	void Close();

	void RegisterObserver(RepoSessionObserver* pObserver);
	void UnregisterObserver(RepoSessionObserver* pObserver);

	bool GotCompany();
	const RepoCompany* GetCompany() const;
	void FetchCompany();
	void UploadCompany();

	bool GotHents() const;
	void FetchHents(SimpleProgress* pProgress);
	HentsCluster* GetHents();
	void UploadHents(SimpleProgress* pProgress);
	void DownloadHents();

	bool GotStocks() const;
	void FetchStocks(SimpleProgress* pProgress);
	StocksCluster* GetStocks();
	void UpdateStocks(SimpleProgress* pProgress);
	void DownloadStocks();

	bool GotPurchases() const;
	void FetchPurchases(SimpleProgress* pProgress);
	PurachesCluster* GetPurchases();
	void DownloadPurchases(DownloadController* pController);

private:
	void throwIfNoSession();
	void CleanSession();
	int FindObserver(RepoSessionObserver* pObserver);
	void NotifyObserversAboutConnect();
	void NotifyObserversAboutDisconnect();

	void DownloadPurchase(const RepoPurchase& purchase,DownloadController* pControler);

private:
	RepoAccount* m_pAccount;
	RepoClient* m_pClient;
	RepoSession* m_pSession;
	RepoCompany* m_pCompany;
	HentsClusterImpl* m_pHentsCluster;
	Context* m_pContext;
	StocksClusterImpl* m_pStocksCluster;

	PurachesClusterImpl* m_pPurchasesCluster;

	NewPtrFlushArray<RepoSessionObserver> m_Observers;

};
	

#endif