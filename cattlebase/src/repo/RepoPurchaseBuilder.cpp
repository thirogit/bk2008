#include "stdafx.h"
#include "RepoPurchaseBuilder.h"
#include "RepoException.h"

//#################################################################################################
RepoPurchaseBuilder::RepoPurchaseHentBuilder::RepoPurchaseHentBuilder(RepoPurchaseBuilder* pOwner) : BuilderSession<RepoHentBuilder>(&m_Builder)
{
	m_pOwner = pOwner;
	m_HentUnderConstruction = NULL;
}

RepoPurchaseBuilder::RepoPurchaseHentBuilder::~RepoPurchaseHentBuilder()
{
	Rollback();
}

void RepoPurchaseBuilder::RepoPurchaseHentBuilder::Begin()
{
	Rollback();

	m_HentUnderConstruction = new RepoHent();
}

void RepoPurchaseBuilder::RepoPurchaseHentBuilder::Rollback()
{
	if(m_HentUnderConstruction)
	{
		delete m_HentUnderConstruction;
		m_HentUnderConstruction = NULL;
	}
}

void RepoPurchaseBuilder::RepoPurchaseHentBuilder::Commit()
{
	if(!m_HentUnderConstruction)
		throw new RepoException("Begin session first.");

	RepoHent* pNewHent = m_Builder.Build();
	m_HentUnderConstruction->CopyFrom(*pNewHent);
	delete pNewHent;

	m_pOwner->m_pTemplate->hents.Add(m_HentUnderConstruction);
	m_HentUnderConstruction = NULL;
}

//#################################################################################################
RepoPurchaseBuilder::RepoPuracheInvoiceBuilder::RepoPuracheInvoiceBuilder(HentResolver* pHentResolver,RepoPurchaseBuilder* pOwner) : m_Builder(pHentResolver),BuilderSession<RepoInvoiceBuilder>(&m_Builder)
{
	m_pOwner = pOwner;
	m_InvoiceUnderConstruction = NULL;
}

RepoPurchaseBuilder::RepoPuracheInvoiceBuilder::~RepoPuracheInvoiceBuilder()
{
	Rollback();
}

void RepoPurchaseBuilder::RepoPuracheInvoiceBuilder::Begin()
{
	Rollback();

	m_InvoiceUnderConstruction = new RepoInvoice();
}

void RepoPurchaseBuilder::RepoPuracheInvoiceBuilder::Commit()
{
	if(!m_InvoiceUnderConstruction)
		throw new RepoException("Begin session first.");

	RepoInvoice* pNewInvoice = m_Builder.Build();
	m_InvoiceUnderConstruction->CopyFrom(*pNewInvoice);
	delete pNewInvoice;

	m_pOwner->m_pTemplate->invoices.Add(m_InvoiceUnderConstruction);
	m_InvoiceUnderConstruction = NULL;
}

void RepoPurchaseBuilder::RepoPuracheInvoiceBuilder::Rollback()
{
	if(m_InvoiceUnderConstruction)
	{
		delete m_InvoiceUnderConstruction;
		m_InvoiceUnderConstruction = NULL;
	}
	m_Builder.Discard();
}
	
//#################################################################################################

RepoPurchaseBuilder::RepoPurchaseBuilder() : 						
						m_HentsBuilder(this),
						m_InvoiceBuilder(this,this),
						m_pTemplate(NULL)
						
{

}


RepoPurchaseBuilder::~RepoPurchaseBuilder()
{
	if(m_pTemplate)
		delete m_pTemplate;
}

BuilderSession<RepoHentBuilder>* RepoPurchaseBuilder::GetHentsBuilder()
{
	EnsureConstruction();
	return &m_HentsBuilder;
}

BuilderSession<RepoInvoiceBuilder>* RepoPurchaseBuilder::GetInvoicesBuilder()
{
	EnsureConstruction();
	return &m_InvoiceBuilder;
}

void RepoPurchaseBuilder::EnsureConstruction()
{
	if(!m_pTemplate)
	{
		m_pTemplate = new RepoPurchaseTemplate();
		m_pTemplate->purchaseId = 0;
	}
}

RepoPurchase* RepoPurchaseBuilder::Build()
{
	if(!m_pTemplate)
		throw new RepoException("construction not started");

	if(m_pTemplate->purchaseId == 0)
		throw new RepoException("no id (must be != 0)");

	if(m_pTemplate->agentCode.IsEmpty())
		throw new RepoException("missing agent code");
	
	if(m_pTemplate->plateNo.IsEmpty())
		throw new RepoException("missing plate no");

	if(m_pTemplate->startDate.IsNull())
		throw new RepoException("null start date");

	if(m_pTemplate->endDate.IsNull())
		throw new RepoException("null end date");
	
	if(m_pTemplate->hents.GetSize() == 0)
		throw new RepoException("no purache hents");
	
	if(m_pTemplate->invoices.GetSize() == 0)
		throw new RepoException("no invoices");


	RepoPurchase* pNewPurchase = new RepoPurchase(m_pTemplate->purchaseId);

	pNewPurchase->SetAgentCode(m_pTemplate->agentCode);
	pNewPurchase->SetWasDownloaded(m_pTemplate->wasDownloaded);
	pNewPurchase->SetEndDate(m_pTemplate->endDate);
	pNewPurchase->SetStartDate(m_pTemplate->startDate);
	pNewPurchase->SetPlateNo(m_pTemplate->plateNo);
			
	NewPtrFlushArray<RepoHent>::iterator hentIt = m_pTemplate->hents.begin();
	while(hentIt != m_pTemplate->hents.end())
	{
		pNewPurchase->AddHent(*(hentIt++));		
	}
	
	NewPtrFlushArray<RepoInvoice>::iterator invoiceIt = m_pTemplate->invoices.begin();
	while(invoiceIt != m_pTemplate->invoices.end())
	{
		pNewPurchase->AddInvoice(*(invoiceIt++));
	}


	delete m_pTemplate;
	m_pTemplate = NULL;

	return pNewPurchase;
}

RepoHent* RepoPurchaseBuilder::Resolve(const CString& sFarmNo)
{
	RepoHent* pRepoHent = NULL;
	for(int i = 0,count = m_pTemplate->hents.GetSize(); i < count;i++)
	{
		pRepoHent = m_pTemplate->hents.GetAt(i);
		if(pRepoHent->GetFarmNo() == sFarmNo)
			return pRepoHent;
	}

	return NULL;
}

void RepoPurchaseBuilder::SetAgentCode(const CString& sAgentCode)
{
	EnsureConstruction();
	m_pTemplate->agentCode = sAgentCode;
}

void RepoPurchaseBuilder::SetPlateNo(const CString& sPlateNo)
{
	EnsureConstruction();
	m_pTemplate->plateNo = sPlateNo;
}

void RepoPurchaseBuilder::SetStartDate(const CDateTime& startDt)
{
	EnsureConstruction();
	m_pTemplate->startDate = startDt;
}

void RepoPurchaseBuilder::SetEndDate(const CDateTime& endDt)
{
	EnsureConstruction();
	m_pTemplate->endDate = endDt;
}

void RepoPurchaseBuilder::SetWasDownloaded(bool bDownloaded)
{
	EnsureConstruction();
	m_pTemplate->wasDownloaded = bDownloaded;
}

void RepoPurchaseBuilder::SetPurchaseId(UINT purchaseId)
{
	EnsureConstruction();
	m_pTemplate->purchaseId = purchaseId;
}