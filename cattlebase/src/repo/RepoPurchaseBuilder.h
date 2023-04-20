#ifndef __REPOPURCHASEBUILDER_H__
#define __REPOPURCHASEBUILDER_H__

#include "RepoHentBuilder.h"
#include "RepoPurchase.h"
#include "RepoInvoice.h"
#include "../PtrFlushArray.h"
#include "RepoInvoiceBuilder.h"
#include "HentResolver.h"
#include "BuilderSession.h"

class RepoPurchaseBuilder : private HentResolver
{
	class RepoPurchaseHentBuilder : public BuilderSession<RepoHentBuilder>
	{
	public:
		RepoPurchaseHentBuilder(RepoPurchaseBuilder* pOwner);
		~RepoPurchaseHentBuilder();
		virtual void Begin();
		virtual void Commit();
		virtual void Rollback();
	protected:
		
	private:
		RepoHentBuilder m_Builder;
		RepoPurchaseBuilder* m_pOwner;
		RepoHent* m_HentUnderConstruction;
	};

	class RepoPuracheInvoiceBuilder : public BuilderSession<RepoInvoiceBuilder> 
	{
	public:
		RepoPuracheInvoiceBuilder(HentResolver* pHentResolver,RepoPurchaseBuilder* pOwner);
		~RepoPuracheInvoiceBuilder();
		virtual void Begin();
		virtual void Commit();
		virtual void Rollback();
	private:
		RepoInvoiceBuilder m_Builder;
		RepoPurchaseBuilder* m_pOwner;
		RepoInvoice* m_InvoiceUnderConstruction;
	};

	class RepoPurchaseTemplate
	{
	public:		
		UINT purchaseId;
		CString agentCode;
		CString plateNo;
		CDateTime startDate;
		CDateTime endDate;
		bool wasDownloaded;

		NewPtrFlushArray<RepoHent> hents;
		NewPtrFlushArray<RepoInvoice> invoices;
	};

	friend class RepoPuracheInvoiceBuilder;
	friend class RepoPurchaseHentBuilder;

public:
	RepoPurchaseBuilder();
	~RepoPurchaseBuilder();

	void SetPurchaseId(UINT purchaseId);
	void SetAgentCode(const CString& sAgentCode);	
	void SetPlateNo(const CString& sPlateNo);
	void SetStartDate(const CDateTime& startDt);
	void SetEndDate(const CDateTime& endDt);
	void SetWasDownloaded(bool bDownloaded);
	
	BuilderSession<RepoHentBuilder>* GetHentsBuilder();
	BuilderSession<RepoInvoiceBuilder>* GetInvoicesBuilder();

	RepoPurchase* Build();

private:
	virtual RepoHent* Resolve(const CString& sFarmNo);
	void EnsureConstruction();

	RepoPurchaseHentBuilder m_HentsBuilder;
	RepoPuracheInvoiceBuilder m_InvoiceBuilder;

	RepoPurchaseTemplate* m_pTemplate;
	

	
};
	

#endif