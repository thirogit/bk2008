#ifndef __REPOINVOICEBUILDER_H__
#define __REPOINVOICEBUILDER_H__

#include "RepoInvoice.h"
#include "RepoCowBuilder.h"
#include "BuilderSession.h"

class RepoInvoiceBuilder
{
	class RepoCowBuilderForInvoice : public BuilderSession<RepoCowBuilder>
	{
	public:
		RepoCowBuilderForInvoice(HentResolver* pHentResolver,RepoInvoiceBuilder* pOwner);
		~RepoCowBuilderForInvoice();
		virtual void Commit();
		virtual void Begin();
		virtual void Rollback();
	private:
		RepoCowBuilder m_Builder;
		RepoInvoiceBuilder* m_pOwner;
		RepoCow* m_pCowUnderConstruction;
	};
	
public:
	RepoInvoiceBuilder(HentResolver* pHentResolver);

	void SetInvoiceDate(const CDateTime& invoiceDt);
	void SetTransactionDate(const CDateTime& transactionDt);	
	void SetTransactionPlace(const CString& sTransactionPlace);	
	void SetVatRate(const VATRate& vatRate);
	void SetPayWay(Invoice::PAYWAYS payWay);
	void SetCustomNumber(const CString& sCustomNumber);	
	void SetPayDueDays(int payDueDays);	
	void SetInvoiceHent(RepoHent* pInvoiceHent);
	void SetInvoiceType(RepoInvoiceType invoiceType);
		
	BuilderSession<RepoCowBuilder>* GetCowsBuilder();
	RepoInvoice* Build();
	void Discard();

	void SetInvoiceHent(const CString& sInvoiceHentFarmNo);
protected:
	void EnsureConstruction();
	void AddCow(RepoCow* pCow);
private:
	RepoCowBuilderForInvoice m_CowsBuilder;
	HentResolver* m_pHentResolver;
	RepoInvoice* m_pConstructedInvoice;

};
	

#endif