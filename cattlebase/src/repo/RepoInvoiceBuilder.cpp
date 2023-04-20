#include "stdafx.h"
#include "RepoInvoiceBuilder.h"
#include "RepoException.h"


RepoInvoiceBuilder::RepoCowBuilderForInvoice::RepoCowBuilderForInvoice(HentResolver* pHentResolver,RepoInvoiceBuilder* pOwner) : m_Builder(pHentResolver),BuilderSession<RepoCowBuilder>(&m_Builder)
{
	m_pOwner = pOwner;
	m_pCowUnderConstruction = NULL;
}

RepoInvoiceBuilder::RepoCowBuilderForInvoice::~RepoCowBuilderForInvoice()
{
	Rollback();
}

void RepoInvoiceBuilder::RepoCowBuilderForInvoice::Commit()
{
	if(!m_pCowUnderConstruction)
		throw new RepoException("Begin session first.");

	RepoCow* pNewCow = m_Builder.Build();
	m_pCowUnderConstruction->CopyFrom(*pNewCow);
	delete pNewCow;

	m_pOwner->AddCow(m_pCowUnderConstruction);
	m_pCowUnderConstruction = NULL;
	
}

void RepoInvoiceBuilder::RepoCowBuilderForInvoice::Begin()
{
	Rollback();
	m_pCowUnderConstruction = new RepoCow();
}

void RepoInvoiceBuilder::RepoCowBuilderForInvoice::Rollback()
{
	if(m_pCowUnderConstruction)
	{
		delete m_pCowUnderConstruction;
		m_pCowUnderConstruction = NULL;
	}
}

//#################################################################################################
RepoInvoiceBuilder::RepoInvoiceBuilder(HentResolver* pHentResolver) : 
																		m_pHentResolver(pHentResolver),
																		m_CowsBuilder(pHentResolver,this),
																		m_pConstructedInvoice(NULL)
																		
{
}

RepoInvoice* RepoInvoiceBuilder::Build()
{
	
	if(!m_pConstructedInvoice)
		throw new RepoException("construction not started");

	if(!m_pConstructedInvoice->GetInvoiceHent())
		throw new RepoException("no invoice hent");
		
	if(m_pConstructedInvoice->GetInvoiceDate().IsNull())
		throw new RepoException("no invoice date");
		
	if(m_pConstructedInvoice->GetPayDueDays() < 0)
		throw new RepoException("invalid pay due days");

	RepoInvoice* result = m_pConstructedInvoice;
	m_pConstructedInvoice = NULL;


	return result;
}

void RepoInvoiceBuilder::AddCow(RepoCow* pCow)
{
	EnsureConstruction();
	m_pConstructedInvoice->AddCow(pCow);
}

void RepoInvoiceBuilder::Discard()
{
	if(m_pConstructedInvoice)
	{
		delete m_pConstructedInvoice;
		m_pConstructedInvoice = NULL;
	}
}

void RepoInvoiceBuilder::EnsureConstruction()
{
	if(!m_pConstructedInvoice)
	{
		m_pConstructedInvoice = new RepoInvoice();
	}
}

BuilderSession<RepoCowBuilder>* RepoInvoiceBuilder::GetCowsBuilder()
{
	return &m_CowsBuilder;
}

void RepoInvoiceBuilder::SetInvoiceHent(const CString& sInvoiceHentFarmNo)
{
	EnsureConstruction();
	m_pConstructedInvoice->SetInvoiceHent(m_pHentResolver->Resolve(sInvoiceHentFarmNo));
}

void RepoInvoiceBuilder::SetInvoiceDate(const CDateTime& invoiceDt)
{
	EnsureConstruction();
	m_pConstructedInvoice->SetInvoiceDate(invoiceDt);
}

void RepoInvoiceBuilder::SetTransactionDate(const CDateTime& transactionDt)
{
	EnsureConstruction();
	m_pConstructedInvoice->SetTransactionDate(transactionDt);
}

void RepoInvoiceBuilder::SetTransactionPlace(const CString& sTransactionPlace)
{
	EnsureConstruction();	
	m_pConstructedInvoice->SetTransactionPlace(sTransactionPlace);
}

void RepoInvoiceBuilder::SetVatRate(const VATRate& vatRate)
{
	EnsureConstruction();
	m_pConstructedInvoice->SetVatRate(vatRate);
}

void RepoInvoiceBuilder::SetPayWay(Invoice::PAYWAYS payWay)
{
	EnsureConstruction();
	m_pConstructedInvoice->SetPayWay(payWay);
}

void RepoInvoiceBuilder::SetCustomNumber(const CString& sCustomNumber)
{
	EnsureConstruction();
	m_pConstructedInvoice->SetCustomNumber(sCustomNumber);
}

void RepoInvoiceBuilder::SetPayDueDays(int payDueDays)
{
	EnsureConstruction();
	m_pConstructedInvoice->SetPayDueDays(payDueDays);
}

void RepoInvoiceBuilder::SetInvoiceHent(RepoHent* pInvoiceHent)
{
	EnsureConstruction();
	m_pConstructedInvoice->SetInvoiceHent(pInvoiceHent);
}

void RepoInvoiceBuilder::SetInvoiceType(RepoInvoiceType invoiceType)
{
	EnsureConstruction();
	m_pConstructedInvoice->SetInvoiceType(invoiceType);
}