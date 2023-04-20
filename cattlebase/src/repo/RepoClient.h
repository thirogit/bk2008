#ifndef __REPOCLIENT_H__
#define __REPOCLIENT_H__

#include "RepoSession.h"
#include "generated/BKRepoServiceBKRepoProxy.h"
#include <set>
#include "RepoHent.h"
#include "RepoStock.h"
#include "RepoPurchase.h"
#include "RepoPurchaseBuilder.h"
#include "RepoSyncTransaction.h"

class CursorDescriptor
{
public:
	CursorDescriptor();
	CursorDescriptor(const CursorDescriptor& src);
	CursorDescriptor& operator=(const CursorDescriptor& right);
	void CopyFrom(const CursorDescriptor& src);

	int GetCount();
	int GetMinFetch();
	int GetMaxFetch();

	void SetCount(int count);
	void SetMinFetch(int minFetch);
	void SetMaxFetch(int maxFetch);
private:

	int m_Count;
	int m_MinFetch;
	int m_MaxFetch;
};


class RepoClient
{
	class HentCursorImpl;
	class StockCursorImpl;
	class PurchaseCursorImpl;

	class RepoSessionImpl : public RepoSession
	{
		class RepoSyncTransactionImpl : public RepoSyncTransaction
		{
		public:
			RepoSyncTransactionImpl(RepoSessionImpl* pParentSession);
			void Close();
			void Commit();
			void UpdateHent(const RepoHent& hent);
			void UpdateStock(const RepoStock& stock);
			void UpdateCompany(const RepoCompany& company);
		private:
			RepoSessionImpl* m_pParentSession;
		};

		friend class HentCursorImpl;
		friend class StockCursorImpl;
		friend class PurchaseCursorImpl;
		friend class RepoSyncTransactionImpl;
	public:
		RepoSessionImpl(const std::string& sSessionId,RepoClient* pClient);
		void Close();
		void FetchCompany(RepoCompany& company);
			
		void FetchPuraches();
		const std::string& GetSessionId() const;
		HentCursorPtr OpenHentCursor();
		StockCursorPtr OpenStockCursor();
		PurchaseCursorPtr OpenPurchaseCursor();
		void DownloadPurchase(UINT purchaseId);
		RepoSyncTransaction* OpenTransaction();
	private:
		void CloseHentCursor(HentCursorImpl* pCursor);
		int FetchHents(HentCursorImpl* pCursor,int howMany,NewPtrFlushArray<RepoHent>& bucket);
		
		void CloseStockCursor(StockCursorImpl* pCursor);		
		int FetchStocks(StockCursorImpl* pCursor,int howMany,NewPtrFlushArray<RepoStock>& bucket);

		void ClosePurchaseCursor(PurchaseCursorImpl* pCursor);
		int FetchPurchases(PurchaseCursorImpl* pCursor,int howMany,NewPtrFlushArray<RepoPurchase>& bucket);

		void UpdateCompany(const RepoCompany& company);
		void UpdateHent(const RepoHent& hent);
		void UpdateStock(const RepoStock& stock);
		void CommitTransaction();
		void CloseTransaction();
		
		
	private:
		RepoClient* m_pClient;
		std::string m_sSessionId;
		RepoSyncTransaction* m_pSyncTransaction;
	};

	class HentCursorImpl : public HentCursor
	{
	public:
		HentCursorImpl(CursorDescriptor& descriptor,RepoSessionImpl* pSession);
		virtual int FetchMany(int howMany,NewPtrFlushArray<RepoHent>& bucket);
		virtual ~HentCursorImpl();
	private:
		RepoSessionImpl* m_pSession;
	};

	class StockCursorImpl : public StockCursor
	{
	public:
		StockCursorImpl(CursorDescriptor& descriptor,RepoSessionImpl* pSession);
		virtual int FetchMany(int howMany,NewPtrFlushArray<RepoStock>& bucket);
		virtual ~StockCursorImpl();
	private:
		RepoSessionImpl* m_pSession;
	};

	class PurchaseCursorImpl : public PurchaseCursor
	{
	public:
		PurchaseCursorImpl(CursorDescriptor& descriptor,RepoSessionImpl* pSession);
		virtual int FetchMany(int howMany,NewPtrFlushArray<RepoPurchase>& bucket);
		virtual ~PurchaseCursorImpl();
	private:
		RepoSessionImpl* m_pSession;
	};

	friend class RepoSessionImpl;
public:
	RepoClient(const CString& sAddress);
	~RepoClient();
	RepoSession* OpenSession(const CString& sLogin,const CString& sPassword); 
	void throwIfSoapError();
	void throwIfRepoError(const ns1__result *result);
private:
	void Begin();
	void Finish();
	void CloseSession(RepoSessionImpl* pSession);
	void FetchCompany(const std::string& sSessionId,RepoCompany& company);
		
	CursorDescriptor OpenHentCursor(const std::string& sSessionId);
	int FetchHents(const std::string& sSessionId,int howMany,NewPtrFlushArray<RepoHent>& bucket);
	void CloseHentCursor(const std::string& sSessionId);

	CursorDescriptor OpenStockCursor(const std::string& sSessionId);
	int FetchStocks(const std::string& sSessionId,int howMany,NewPtrFlushArray<RepoStock>& bucket);
	void CloseStockCursor(const std::string& sSessionId);

	CursorDescriptor OpenPurchaseCursor(const std::string& sSessionId);
	int FetchPurchases(const std::string& sSessionId,int howMany,NewPtrFlushArray<RepoPurchase>& bucket);
	void ClosePurchaseCursor(const std::string& sSessionId);

	void DownloadPurchase(const std::string& sSessionId,UINT purchaseId);

	void OpenSyncTransaction(const std::string& sSessionId);
	void CloseSyncTransaction(const std::string& sSessionId);
	void CommitSyncTransaction(const std::string& sSessionId);
	void UpdateHent(const std::string& sSessionId,const RepoHent& hent);
	void UpdateStock(const std::string& sSessionId,const RepoStock& stock);
	void UpdateCompany(const std::string& sSessionId,const RepoCompany& company);

	LPCSTR StringifySoapError(int soapError) const;
	RepoHentType getRepoHentTypeForSoapHentType(ns1__hentType henttype);
	ns1__hentType getSoapHentTypeForRepoHentType(RepoHentType henttype);
	RepoInvoiceType getInvoiceTypeForSoapInvoiceType(ns1__invoiceType soapInvoiceType);

	Invoice::PAYWAYS getPayWayForSoapPayWay(ns1__payWay soapPayWay);
	COWSEX getCowSexForSoapCowSex(ns1__cowSex soapCowSex);
	ns1__cowSex getSoapCowSexForCowSex(COWSEX sex);
	void constructRepoHent(RepoHentBuilder* pBuilder,ns1__hent* pSoapHent);
	void constructRepoInvoice(RepoInvoiceBuilder* pInvoiceBuilder,ns1__invoice* pSoapInvoice);
	void constructRepoCow(RepoCowBuilder* pCowBuilder,ns1__cow* pSoapCow);

	void CallCloseSession(const std::string& sSessionId);
	std::string CallOpenSession(const std::string& sLogin,const std::string& sPassword);
		
private:
	BKRepo* m_pBKRepo;
	CString m_sRepoAddress;
	std::set<RepoSessionImpl*> m_Sessions;
};


#endif