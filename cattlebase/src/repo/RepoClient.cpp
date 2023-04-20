#include "stdafx.h"
#include "RepoClient.h"
#include "RepoException.h"
#include "../Utils/StringFormat.h"
//#include "utils/StringEncoder.h"


CursorDescriptor::CursorDescriptor()
{
	m_Count = 0;
	m_MinFetch = 0;
	m_MaxFetch = 0;
}

CursorDescriptor::CursorDescriptor(const CursorDescriptor& src)
{
	CopyFrom(src);
}

CursorDescriptor& CursorDescriptor::operator=(const CursorDescriptor& right)
{
	CopyFrom(right);
	return *this;
}

void CursorDescriptor::CopyFrom(const CursorDescriptor& src)
{
	m_Count = src.m_Count;
	m_MinFetch = src.m_MinFetch;
	m_MaxFetch = src.m_MaxFetch;
}

int CursorDescriptor::GetCount()
{
	return m_Count;
}

int CursorDescriptor::GetMinFetch()
{
	return m_MinFetch;
}

int CursorDescriptor::GetMaxFetch()
{
	return m_MaxFetch;
}

void CursorDescriptor::SetCount(int count)
{
	m_Count = count;
}

void CursorDescriptor::SetMinFetch(int minFetch)
{
	m_MinFetch = minFetch;
}

void CursorDescriptor::SetMaxFetch(int maxFetch)
{
	m_MaxFetch = maxFetch;
}

//#######################################################################################
RepoClient::RepoSessionImpl::RepoSyncTransactionImpl::RepoSyncTransactionImpl(RepoSessionImpl* pParentSession) : m_pParentSession(pParentSession)
{
}

void RepoClient::RepoSessionImpl::RepoSyncTransactionImpl::Close()
{	
	m_pParentSession->CloseTransaction();
}

void RepoClient::RepoSessionImpl::RepoSyncTransactionImpl::Commit()
{
	m_pParentSession->CommitTransaction();
}

void RepoClient::RepoSessionImpl::RepoSyncTransactionImpl::UpdateHent(const RepoHent& hent)
{
	m_pParentSession->UpdateHent(hent);
}

void RepoClient::RepoSessionImpl::RepoSyncTransactionImpl::UpdateStock(const RepoStock& stock)
{
	m_pParentSession->UpdateStock(stock);
}

void RepoClient::RepoSessionImpl::RepoSyncTransactionImpl::UpdateCompany(const RepoCompany& company)
{
	m_pParentSession->UpdateCompany(company);
}
		
//#######################################################################################
RepoClient::RepoSessionImpl::RepoSessionImpl(const std::string& sSessionId,RepoClient* pClient) : m_pClient(pClient),m_sSessionId(sSessionId)
{
	m_pSyncTransaction = NULL;
}

void RepoClient::RepoSessionImpl::Close()
{
	m_pClient->CloseSession(this);
}

void RepoClient::RepoSessionImpl::FetchCompany(RepoCompany& company)
{
	m_pClient->FetchCompany(m_sSessionId,company);
}

void RepoClient::RepoSessionImpl::UpdateCompany(const RepoCompany& company)
{
	m_pClient->UpdateCompany(m_sSessionId,company);
}

int RepoClient::RepoSessionImpl::FetchPurchases(PurchaseCursorImpl* pCursor,int howMany,NewPtrFlushArray<RepoPurchase>& bucket)
{
	return m_pClient->FetchPurchases(m_sSessionId,howMany,bucket);
}

const std::string& RepoClient::RepoSessionImpl::GetSessionId() const
{
	return m_sSessionId;
}


HentCursorPtr RepoClient::RepoSessionImpl::OpenHentCursor()
{
	CursorDescriptor cursorDescriptor = m_pClient->OpenHentCursor(m_sSessionId);

	HentCursorImpl* pCursor = new HentCursorImpl(cursorDescriptor,this);

	return HentCursorPtr(pCursor);
}

void RepoClient::RepoSessionImpl::CloseHentCursor(HentCursorImpl* pCursor)
{
	try
	{
		m_pClient->CloseHentCursor(m_sSessionId);
	}
	catch(RepoException* e)
	{
		e->Delete();
	}
}


int RepoClient::RepoSessionImpl::FetchHents(HentCursorImpl* pCursor,int howMany,NewPtrFlushArray<RepoHent>& bucket)
{
	return m_pClient->FetchHents(m_sSessionId,howMany,bucket);
}


StockCursorPtr RepoClient::RepoSessionImpl::OpenStockCursor()
{
	CursorDescriptor cursorDescriptor = m_pClient->OpenStockCursor(m_sSessionId);

	StockCursorImpl* pCursor = new StockCursorImpl(cursorDescriptor,this);

	return StockCursorPtr(pCursor);
}

void RepoClient::RepoSessionImpl::CloseStockCursor(StockCursorImpl* pCursor)
{
	m_pClient->CloseStockCursor(m_sSessionId);
}

int RepoClient::RepoSessionImpl::FetchStocks(StockCursorImpl* pCursor,int howMany,NewPtrFlushArray<RepoStock>& bucket)
{
	return m_pClient->FetchStocks(m_sSessionId,howMany,bucket);
}

PurchaseCursorPtr RepoClient::RepoSessionImpl::OpenPurchaseCursor()
{
	CursorDescriptor cursorDescriptor = m_pClient->OpenPurchaseCursor(m_sSessionId);

	PurchaseCursorImpl* pCursor = new PurchaseCursorImpl(cursorDescriptor,this);

	return PurchaseCursorPtr(pCursor);

}
void RepoClient::RepoSessionImpl::ClosePurchaseCursor(PurchaseCursorImpl* pCursor)
{
	m_pClient->ClosePurchaseCursor(m_sSessionId);
}

//bool RepoClient::RepoSessionImpl::FetchNextPurchase(PurchaseCursorImpl* pCursor,RepoPurchase& purchase)
//{
//	return m_pClient->FetchNextPurchase(m_sSessionId,purchase);
//}

void RepoClient::RepoSessionImpl::DownloadPurchase(UINT purchaseId)
{
	m_pClient->DownloadPurchase(m_sSessionId,purchaseId);
}

RepoSyncTransaction* RepoClient::RepoSessionImpl::OpenTransaction()
{
	m_pClient->OpenSyncTransaction(m_sSessionId);
	m_pSyncTransaction = new RepoSyncTransactionImpl(this);
	return m_pSyncTransaction;
}

void RepoClient::RepoSessionImpl::UpdateHent(const RepoHent& hent)
{
	m_pClient->UpdateHent(m_sSessionId,hent);
}

void RepoClient::RepoSessionImpl::UpdateStock(const RepoStock& stock)
{
	m_pClient->UpdateStock(m_sSessionId,stock);
}

void RepoClient::RepoSessionImpl::CommitTransaction()
{
	m_pClient->CommitSyncTransaction(m_sSessionId);
}

void RepoClient::RepoSessionImpl::CloseTransaction()
{
	m_pClient->CloseSyncTransaction(m_sSessionId);
	delete m_pSyncTransaction;
	m_pSyncTransaction = NULL;
}
//#######################################################################################
RepoClient::HentCursorImpl::HentCursorImpl(CursorDescriptor& descriptor,RepoClient::RepoSessionImpl* pSession) : 
		HentCursor(descriptor.GetCount(),CursorFetchLimit(descriptor.GetMinFetch(),descriptor.GetMaxFetch())),
		m_pSession(pSession)
{
}

RepoClient::HentCursorImpl::~HentCursorImpl()
{
	m_pSession->CloseHentCursor(this);
}

int RepoClient::HentCursorImpl::FetchMany(int howMany,NewPtrFlushArray<RepoHent>& bucket)
{
	return m_pSession->FetchHents(this,howMany,bucket);	
}
//#######################################################################################
RepoClient::StockCursorImpl::StockCursorImpl(CursorDescriptor& descriptor,RepoSessionImpl* pSession) : 
		m_pSession(pSession),
		StockCursor(descriptor.GetCount(),CursorFetchLimit(descriptor.GetMinFetch(),descriptor.GetMaxFetch()))
{
}

RepoClient::StockCursorImpl::~StockCursorImpl()
{
	m_pSession->CloseStockCursor(this);
}

int RepoClient::StockCursorImpl::FetchMany(int howMany,NewPtrFlushArray<RepoStock>& bucket)
{
	return m_pSession->FetchStocks(this,howMany,bucket);
}
//#######################################################################################
RepoClient::PurchaseCursorImpl::PurchaseCursorImpl(CursorDescriptor& descriptor,RepoSessionImpl* pSession) : 
		m_pSession(pSession),
		PurchaseCursor(descriptor.GetCount(),CursorFetchLimit(descriptor.GetMinFetch(),descriptor.GetMaxFetch()))
{
}


RepoClient::PurchaseCursorImpl::~PurchaseCursorImpl()
{
	m_pSession->ClosePurchaseCursor(this);
}

int RepoClient::PurchaseCursorImpl::FetchMany(int howMany, NewPtrFlushArray<RepoPurchase>& bucket)
{
	return 	m_pSession->FetchPurchases(this,howMany,bucket);
}
//#######################################################################################
RepoClient::RepoClient(const CString& sAddress) : m_pBKRepo(NULL),m_sRepoAddress(sAddress)
{

}

void RepoClient::Begin()
{
	if(!m_pBKRepo)
	{
		m_pBKRepo = new BKRepo();
		m_pBKRepo->endpoint = (LPCSTR)m_sRepoAddress;
	}
}

void RepoClient::Finish()
{
	if(m_pBKRepo)
	{
		RepoSessionImpl* pSession = NULL;
		std::set<RepoSessionImpl*>::iterator sessionIt = m_Sessions.begin();
		while(sessionIt != m_Sessions.end())
		{
			pSession = *sessionIt;
			try
			{
				CallCloseSession(pSession->GetSessionId());
			}
			catch(RepoException* e)
			{
				e->Delete();
			}
			delete pSession;
			sessionIt++;
		}
		m_Sessions.clear();

		delete m_pBKRepo;
		m_pBKRepo = NULL;
	}
}

RepoSession* RepoClient::OpenSession(const CString& sLogin,const CString& sPassword)
{
	Begin();

	std::string sSessionId = CallOpenSession((LPCSTR)sLogin,(LPCSTR)sPassword);

	RepoSessionImpl* pSession = new RepoSessionImpl(sSessionId,this);
	m_Sessions.insert(pSession);

	return pSession;
}

void RepoClient::CallCloseSession(const std::string& sSessionId)
{
	ns1__sessionRequest closeSessionRq;
	ns1__closeSessionResponse closeSessionRs;

	closeSessionRq.sessionId = sSessionId;

	m_pBKRepo->__ns1__closeSession(&closeSessionRq,&closeSessionRs);
	throwIfSoapError();
	throwIfRepoError(closeSessionRs.result);
}

std::string RepoClient::CallOpenSession(const std::string& sLogin,const std::string& sPassword)
{
	ns1__openOfficeSessionRequest openSessionRq;
	ns1__openSessionResponse openSessionRs;
	

	openSessionRq.login = sLogin;
	openSessionRq.password = sPassword;

	m_pBKRepo->__ns1__openOfficeSession(&openSessionRq,&openSessionRs);
	throwIfSoapError();
	throwIfRepoError(openSessionRs.result);
	return openSessionRs.sessionId;
}

void RepoClient::throwIfRepoError(const ns1__result *result)
{
	if(result->status != ns1__status__SUCCESS)
	{
		throw new RepoException(result->msg.c_str());
	}
}

LPCSTR RepoClient::StringifySoapError(int soapError) const
{
	const struct SOAP_ERRORS
	{
		int errorCode;
		const char* errorText;
	} soapErrors[] = 
	{
		{SOAP_CLI_FAULT,		"SOAP_CLI_FAULT"},
		{SOAP_SVR_FAULT,		"SOAP_SVR_FAULT"},
		{SOAP_TAG_MISMATCH,		"SOAP_TAG_MISMATCH"},
		{SOAP_TYPE,				"SOAP_TYPE"},
		{SOAP_SYNTAX_ERROR,		"SOAP_SYNTAX_ERROR"},
		{SOAP_NO_TAG,			"SOAP_NO_TAG"},
		{SOAP_IOB,				"SOAP_IOB"},
		{SOAP_MUSTUNDERSTAND,	"SOAP_MUSTUNDERSTAND"},
		{SOAP_NAMESPACE,		"SOAP_NAMESPACE"},
		{SOAP_USER_ERROR,		"SOAP_USER_ERROR"},
		{SOAP_FATAL_ERROR,		"SOAP_FATAL_ERROR"},
		{SOAP_FAULT,			"SOAP_FAULT"},
		{SOAP_NO_METHOD,		"SOAP_NO_METHOD"},
		{SOAP_NO_DATA,			"SOAP_NO_DATA"},
		{SOAP_GET_METHOD,		"SOAP_GET_METHOD"},
		{SOAP_PUT_METHOD,		"SOAP_PUT_METHOD"},
		{SOAP_DEL_METHOD,		"SOAP_DEL_METHOD"},
		{SOAP_HEAD_METHOD,		"SOAP_HEAD_METHOD"},
		{SOAP_HTTP_METHOD,		"SOAP_HTTP_METHOD"},
		{SOAP_EOM,				"SOAP_EOM"},
		{SOAP_MOE,				"SOAP_MOE"},
		{SOAP_HDR,				"SOAP_HDR"},
		{SOAP_NULL,				"SOAP_NULL"},
		{SOAP_DUPLICATE_ID,		"SOAP_DUPLICATE_ID"},
		{SOAP_MISSING_ID,		"SOAP_MISSING_ID"},
		{SOAP_HREF,				"SOAP_HREF"},
		{SOAP_UDP_ERROR,		"SOAP_UDP_ERROR"},
		{SOAP_TCP_ERROR,		"SOAP_TCP_ERROR"},
		{SOAP_HTTP_ERROR,		"SOAP_HTTP_ERROR"},
		{SOAP_SSL_ERROR,		"SOAP_SSL_ERROR"},
		{SOAP_ZLIB_ERROR,		"SOAP_ZLIB_ERROR"},
		{SOAP_DIME_ERROR,		"SOAP_DIME_ERROR"},
		{SOAP_DIME_HREF,		"SOAP_DIME_HREF"},
		{SOAP_DIME_MISMATCH,	"SOAP_DIME_MISMATCH"},
		{SOAP_DIME_END,			"SOAP_DIME_END"},
		{SOAP_MIME_ERROR,		"SOAP_MIME_ERROR"},
		{SOAP_MIME_HREF,		"SOAP_MIME_HREF"},
		{SOAP_MIME_END,			"SOAP_MIME_END"},
		{SOAP_VERSIONMISMATCH,	"SOAP_VERSIONMISMATCH"},
		{SOAP_PLUGIN_ERROR,		"SOAP_PLUGIN_ERROR"},
		{SOAP_DATAENCODINGUNKNOWN,	"SOAP_DATAENCODINGUNKNOWN"},
		{SOAP_REQUIRED,			"SOAP_REQUIRED"},
		{SOAP_PROHIBITED,		"SOAP_PROHIBITED"},
		{SOAP_OCCURS,			"SOAP_OCCURS"},
		{SOAP_LENGTH,			"SOAP_LENGTH"},
		{SOAP_FD_EXCEEDED,		"SOAP_FD_EXCEEDED"},
		{0,						NULL}
	};
	
	const SOAP_ERRORS* pSoapErrorTranslation = soapErrors;
	while(pSoapErrorTranslation->errorCode)
	{
		if(soapError == pSoapErrorTranslation->errorCode)
			return pSoapErrorTranslation->errorText;
		pSoapErrorTranslation++;
	}

	return NULL;
}

void RepoClient::throwIfSoapError()
{
	if(m_pBKRepo->soap->error != SOAP_OK)
	{
		const char** pszFaultMsg = soap_faultstring(m_pBKRepo->soap);
		if(*pszFaultMsg != NULL)
		{			
			throw new RepoException(CStringFormat("%s: %s",StringifySoapError(m_pBKRepo->soap->error),*pszFaultMsg));
		}
		else
		{
			throw new RepoException(StringifySoapError(m_pBKRepo->soap->error));
		}
	}
}

void RepoClient::CloseSession(RepoSessionImpl* pSession)
{	
	try
	{
		CallCloseSession(pSession->GetSessionId());
	}
	catch(RepoException* e)
	{
		e->Delete();
	}

	m_Sessions.erase(pSession);
	delete pSession;
}


RepoClient::~RepoClient()
{
	Finish();
}


void RepoClient::FetchCompany(const std::string& sSessionId,RepoCompany& company)
{
	ns1__sessionRequest sessionRequest; 
	ns1__fetchCompanyResponse fetchCompanyResponse;

	sessionRequest.sessionId = sSessionId;
	m_pBKRepo->__ns1__fetchCompany(&sessionRequest,&fetchCompanyResponse);
	throwIfSoapError();
	throwIfRepoError(fetchCompanyResponse.result);

	ns1__company * pSoapCompany = fetchCompanyResponse.company;
	
	company.SetCompanyName(pSoapCompany->name.c_str());
	company.SetCity(pSoapCompany->city.c_str());
	company.SetZipCode(pSoapCompany->zip.c_str());
	company.SetStreet(pSoapCompany->street.c_str());
	company.SetPOBox(pSoapCompany->POBox.c_str());
	company.SetFarmNo(pSoapCompany->farmNo.c_str());	
	company.SetNIP(pSoapCompany->nip.c_str());
	//company.SetREGON(pSoapCompany->);
	//company.SetBank(pSoapCompany->);
	//company.SetAccountNo(pSoapCompany->);
	//company.SetWetIdNo(pSoapCompany->w);
	//company.SetWetLicNo();
	company.SetPhoneNo(pSoapCompany->phoneNo.c_str());
	
	

}


CursorDescriptor RepoClient::OpenHentCursor(const std::string& sSessionId)
{
	ns1__sessionRequest openHentCursorRequest;
	ns1__openHentCursorResponse openHentCursorResponse;

	openHentCursorRequest.sessionId = sSessionId;

	m_pBKRepo->__ns1__openHentCursor(&openHentCursorRequest,&openHentCursorResponse);
	throwIfSoapError();
	throwIfRepoError(openHentCursorResponse.result);

	CursorDescriptor result;
	result.SetCount(openHentCursorResponse.count);
	result.SetMinFetch(openHentCursorResponse.minFetch);
	result.SetMaxFetch(openHentCursorResponse.maxFetch);
	return result;

}

void RepoClient::constructRepoHent(RepoHentBuilder* pBuilder,ns1__hent* pSoapHent)
{
	pBuilder->SetFarmNo(pSoapHent->farmno.c_str());
	pBuilder->SetName(pSoapHent->name.c_str());	
	pBuilder->SetAlias(pSoapHent->alias.c_str());	
	pBuilder->SetStreet(pSoapHent->street.c_str());
	pBuilder->SetPOBox(pSoapHent->pobox.c_str());
	pBuilder->SetCity(pSoapHent->city.c_str());
	pBuilder->SetZip(pSoapHent->zip.c_str());
	pBuilder->SetHentType(getRepoHentTypeForSoapHentType(pSoapHent->henttype));

	CString sPhone;
	CString sFiscalNo;
	CString sWetNo;
	CString sExtras;
	CString sPlate;
	CString sPersonalNo;
	CString sREGON;
	CString sPersonalIdNo;
	CDateTime issueDt;
	CString sIssuePost;
	CString sBankAccountNo;
	CString sBankName;
	CString sWetLicenceNo;
	CString sCellPhoneNo;
	CString sEmailAddress;


	if(pSoapHent->phone)
		sPhone = pSoapHent->phone->c_str();

	if(pSoapHent->nip)
		sFiscalNo = pSoapHent->nip->c_str();

	if(pSoapHent->wetno)
		sWetNo = pSoapHent->wetno->c_str();

	if(pSoapHent->extras)
		sExtras = pSoapHent->extras->c_str();

	if(pSoapHent->plate)
		sPlate = pSoapHent->plate->c_str();

	if(pSoapHent->pesel)
		sPersonalNo = pSoapHent->pesel->c_str();

	if(pSoapHent->regon)
		sREGON = pSoapHent->regon->c_str();

	if(pSoapHent->idno)
		sPersonalIdNo = pSoapHent->idno->c_str();
		
	if(pSoapHent->issuedate)
		issueDt = CDateTime(*pSoapHent->issuedate);
	
	if(pSoapHent->issuepost)
		sIssuePost = pSoapHent->issuepost->c_str();

	if(pSoapHent->bankaccountno)
		sBankAccountNo = pSoapHent->bankaccountno->c_str();		
	
	if(pSoapHent->bankname)
		sBankName = pSoapHent->bankname->c_str();

	if(pSoapHent->wetlicenceno)
		sWetLicenceNo = pSoapHent->wetlicenceno->c_str();

	if(pSoapHent->cellphoneno)
		sCellPhoneNo = pSoapHent->cellphoneno->c_str();

	if(pSoapHent->emailaddress)
		sEmailAddress = pSoapHent->emailaddress->c_str();


	pBuilder->SetPhone(sPhone);
	pBuilder->SetNIP(sFiscalNo);
	pBuilder->SetWetNo(sWetNo);
	pBuilder->SetExtras(sExtras);
	pBuilder->SetPlate(sPlate);
	pBuilder->SetPESEL(sPersonalNo);
	pBuilder->SetREGON(sREGON);
	pBuilder->SetIdNo(sPersonalIdNo);
	pBuilder->SetIssueDate(issueDt);
	pBuilder->SetIssuePost(sIssuePost);
	pBuilder->SetAccountNo(sBankAccountNo);
	pBuilder->SetBankName(sBankName);
	pBuilder->SetWetLicenceNo(sWetLicenceNo);
	pBuilder->SetCellPhoneNo(sCellPhoneNo);
	pBuilder->SetEmailAddress(sEmailAddress);
	/*if(pSoapHent->latitude)
		pBuilder->SetLatitude(Latitude(*pSoapHent->latitude));

	if(pSoapHent->longitude)
		pBuilder->SetLongitude(Longitude(*pSoapHent->longitude));*/
}


int RepoClient::FetchHents(const std::string& sSessionId,int howMany,NewPtrFlushArray<RepoHent>& bucket)
{
	ns1__cursorRequest fetchHentsRequest;
	ns1__fetchHentsResponse fetchHentsResponse;

	fetchHentsRequest.sessionId = sSessionId;
	fetchHentsRequest.size = howMany;

	m_pBKRepo->__ns1__fetchHents(&fetchHentsRequest,&fetchHentsResponse);
	throwIfSoapError();
	throwIfRepoError(fetchHentsResponse.result);

	std::vector<ns1__hent * >::iterator hentIt = fetchHentsResponse.hents.begin();
	ns1__hent* pSoapHent = NULL;
	while(hentIt != fetchHentsResponse.hents.end())
	{
		pSoapHent = *hentIt;
		RepoHentBuilder builder;
		constructRepoHent(&builder,pSoapHent);
		bucket.Add(builder.Build());
		hentIt++;
	}

	return  fetchHentsResponse.hents.size();
}

RepoHentType RepoClient::getRepoHentTypeForSoapHentType(ns1__hentType henttype)
{
	switch(henttype)
	{
		case ns1__hentType__INDIVIDUAL:
			return HentType_Individual;
		case ns1__hentType__COMPANY:
			return HentType_Company;
	}

	throw new RepoException("Invalid soap hent type.");
}

ns1__hentType RepoClient::getSoapHentTypeForRepoHentType(RepoHentType henttype)
{
	switch(henttype)
	{
		case HentType_Individual:
			return ns1__hentType__INDIVIDUAL;
		case HentType_Company:
			return ns1__hentType__COMPANY;
	}

	throw new RepoException("Invalid repo hent type.");
}

void RepoClient::CloseHentCursor(const std::string& sSessionId)
{
	ns1__sessionRequest closeHentCursorRequest;
	ns1__closeHentCursorResponse closeHentCursorResponse;

	closeHentCursorRequest.sessionId = sSessionId;

	m_pBKRepo->__ns1__closeHentCursor(&closeHentCursorRequest,&closeHentCursorResponse);
	throwIfSoapError();
	throwIfRepoError(closeHentCursorResponse.result);
	
}

CursorDescriptor RepoClient::OpenStockCursor(const std::string& sSessionId)
{
	ns1__sessionRequest openStockCursorRequest;
	ns1__openStockCursorResponse openStockCursorResponse;

	openStockCursorRequest.sessionId = sSessionId;

	m_pBKRepo->__ns1__openStockCursor(&openStockCursorRequest,&openStockCursorResponse);
	throwIfSoapError();
	throwIfRepoError(openStockCursorResponse.result);

	CursorDescriptor result;
	result.SetCount(openStockCursorResponse.count);
	result.SetMinFetch(openStockCursorResponse.minFetch);
	result.SetMaxFetch(openStockCursorResponse.maxFetch);
	return result;
	
}
int RepoClient::FetchStocks(const std::string& sSessionId,int howMany,NewPtrFlushArray<RepoStock>& bucket)
{
	ns1__cursorRequest fetchStocksRequest;
	ns1__fetchStocksResponse fetchStocksResponse;

	fetchStocksRequest.sessionId = sSessionId;
	fetchStocksRequest.size = howMany;

	m_pBKRepo->__ns1__fetchStocks(&fetchStocksRequest,&fetchStocksResponse);

	throwIfSoapError();
	throwIfRepoError(fetchStocksResponse.result);
	
	CString sStockName;		
	Money myPricePerKg;
	CowSex preDefSex;
	RepoStock* pStock = NULL;

	ns1__stock* pSoapStock = NULL; 
	std::vector<ns1__stock * >::iterator stockIt = fetchStocksResponse.stocks.begin();
	while(stockIt != fetchStocksResponse.stocks.end())
	{
		pSoapStock = *stockIt;
		sStockName.Empty();
		myPricePerKg.MakeNull();
		preDefSex.MakeUNK();	
	
		pStock = new RepoStock();
		pStock->SetStockCode(pSoapStock->stockCode.c_str());

		if(pSoapStock->stockName)
			sStockName = pSoapStock->stockName->c_str();
				

		if(pSoapStock->pricePerKg)
			myPricePerKg = *pSoapStock->pricePerKg;
		
		preDefSex = getCowSexForSoapCowSex(pSoapStock->preDefSex);

		pStock->SetStockName(sStockName);
		pStock->SetMyPerKgPrice(myPricePerKg);
		pStock->SetPredefSex(preDefSex);
		bucket.Add(pStock);
		stockIt++;
	}

	return fetchStocksResponse.stocks.size();
	

}

COWSEX RepoClient::getCowSexForSoapCowSex(ns1__cowSex soapCowSex)
{
	switch(soapCowSex)
	{
		case ns1__cowSex__XX:
			return SEX_XX;
		case ns1__cowSex__XY:
			return SEX_XY;
		case ns1__cowSex__NONE:
			return SEX_UNK;
	}

	throw new RepoException("Invalid cow soap sex.");
}

ns1__cowSex RepoClient::getSoapCowSexForCowSex(COWSEX sex)
{
	switch(sex)
	{
		case SEX_XX:
			return ns1__cowSex__XX;
		case SEX_XY:
			return ns1__cowSex__XY;
		case SEX_UNK:
			return ns1__cowSex__NONE;
	}

	throw new RepoException("Invalid cow sex.");
}


void RepoClient::CloseStockCursor(const std::string& sSessionId)
{
	ns1__sessionRequest closeStockCursorRequest;
	ns1__closeStockCursorResponse closeStockCursorResponse;

	closeStockCursorRequest.sessionId = sSessionId;

	m_pBKRepo->__ns1__closeStockCursor(&closeStockCursorRequest,&closeStockCursorResponse);
	throwIfSoapError();
	throwIfRepoError(closeStockCursorResponse.result);
	
}


CursorDescriptor RepoClient::OpenPurchaseCursor(const std::string& sSessionId)
{
	ns1__sessionRequest openPurchaseCursorRq;
	ns1__openPurchaseCursorResponse openPurchaseCursorRs;

	openPurchaseCursorRq.sessionId = sSessionId; 

	m_pBKRepo->__ns1__openPurchaseCursor(&openPurchaseCursorRq,&openPurchaseCursorRs);
	throwIfSoapError();
	throwIfRepoError(openPurchaseCursorRs.result);


	CursorDescriptor result;
	result.SetCount(openPurchaseCursorRs.count);
	result.SetMinFetch(5);//openPurchaseCursorRs.minFetch);
	result.SetMaxFetch(25);//openPurchaseCursorRs.maxFetch);
	return result;
			
}

int RepoClient::FetchPurchases(const std::string& sSessionId,int howMany,NewPtrFlushArray<RepoPurchase>& bucket)
{
	ns1__cursorRequest fetchPurchasesRq;
	ns1__fetchPurchasesResponse fetchPurchasesRs;
	
	fetchPurchasesRq.sessionId = sSessionId;
	fetchPurchasesRq.size = howMany;

	m_pBKRepo->__ns1__fetchPurchases(&fetchPurchasesRq,&fetchPurchasesRs);
	throwIfSoapError();
	throwIfRepoError(fetchPurchasesRs.result);

	std::vector< ns1__qualifiedPurchase * >::iterator purchaseIt = fetchPurchasesRs.purchase.begin();
	 ns1__qualifiedPurchase* pSoapPurchase;
	while(purchaseIt != fetchPurchasesRs.purchase.end())
	{
		pSoapPurchase = *(purchaseIt++);
		RepoPurchaseBuilder builder;
		
		builder.SetPurchaseId(pSoapPurchase->purchaseId);
		builder.SetAgentCode(CString(pSoapPurchase->agent.c_str()));
		builder.SetPlateNo(CString(pSoapPurchase->plateNo.c_str()));
		builder.SetStartDate(CDateTime(pSoapPurchase->startDate));
		builder.SetEndDate(CDateTime(pSoapPurchase->endDate));
		builder.SetWasDownloaded(pSoapPurchase->wasDownloaded);

		BuilderSession<RepoHentBuilder>* pHentsSession =  builder.GetHentsBuilder();

		std::vector<ns1__hent*>::iterator hentIt = pSoapPurchase->hents.begin();
		while(hentIt != pSoapPurchase->hents.end())
		{
			pHentsSession->Begin();
			constructRepoHent(pHentsSession->GetBuilder(),*hentIt);
			pHentsSession->Commit();
			hentIt++;
		}

		BuilderSession<RepoInvoiceBuilder>* pInvoicesSession = builder.GetInvoicesBuilder();

		std::vector<ns1__invoice*>::iterator invoiceIt = pSoapPurchase->invoices.begin();
		while(invoiceIt != pSoapPurchase->invoices.end())
		{
			ns1__invoice* pSoapInvoice = *invoiceIt;
			pInvoicesSession->Begin();
			RepoInvoiceBuilder* pInvoiceBuilder = pInvoicesSession->GetBuilder();
			constructRepoInvoice(pInvoiceBuilder,pSoapInvoice);
			BuilderSession<RepoCowBuilder>* pCowsSession = pInvoiceBuilder->GetCowsBuilder();
			std::vector<ns1__cow*>::iterator cowIt = pSoapInvoice->cows.begin();
			while(cowIt != pSoapInvoice->cows.end())
			{
				pCowsSession->Begin();
				constructRepoCow(pCowsSession->GetBuilder(),*cowIt);
				pCowsSession->Commit();
				cowIt++;
			}
			pInvoicesSession->Commit();
			invoiceIt++;
		}

		bucket.Add(builder.Build());
		
	}

	return  fetchPurchasesRs.purchase.size();
	
}

Invoice::PAYWAYS RepoClient::getPayWayForSoapPayWay(ns1__payWay soapPayWay)
{
	switch(soapPayWay)
	{
	case ns1__payWay__CASH:
		return Invoice::PAY_CASH;
	case ns1__payWay__TRANSFER:
		return Invoice::PAY_TRANS;
	}

	throw new RepoException("Invalid pay way.");
}

void RepoClient::constructRepoInvoice(RepoInvoiceBuilder* pInvoiceBuilder,ns1__invoice* pSoapInvoice)
{
	if(pSoapInvoice->customNumber)
		pInvoiceBuilder->SetCustomNumber(CString(pSoapInvoice->customNumber->c_str()));
	
	pInvoiceBuilder->SetInvoiceDate(CDateTime(pSoapInvoice->invoiceDate));
	
	if(pSoapInvoice->transactionDate)
		pInvoiceBuilder->SetTransactionDate(CDateTime(*pSoapInvoice->transactionDate));
	
	if(pSoapInvoice->transactionPlace)
		pInvoiceBuilder->SetTransactionPlace( CString(pSoapInvoice->transactionPlace->c_str()));	
	
	pInvoiceBuilder->SetVatRate(VATRate(pSoapInvoice->vatrate));
	
	pInvoiceBuilder->SetPayWay(getPayWayForSoapPayWay(pSoapInvoice->payWay));	
	
	pInvoiceBuilder->SetPayDueDays(pSoapInvoice->payDueDays);
	
	pInvoiceBuilder->SetInvoiceHent(CString(pSoapInvoice->invoiceHentNo.c_str()));

	pInvoiceBuilder->SetInvoiceType(getInvoiceTypeForSoapInvoiceType(pSoapInvoice->invoiceType));
	
}

RepoInvoiceType RepoClient::getInvoiceTypeForSoapInvoiceType(ns1__invoiceType soapInvoiceType)
{
	switch(soapInvoiceType)
	{
	case ns1__invoiceType__LUMP:
		return Invoice_Lump;
	case ns1__invoiceType__REGULAR:
		return Invoice_Regular;
	}

	throw new RepoException("Invalid invoice type.");
}

void RepoClient::constructRepoCow(RepoCowBuilder* pCowBuilder,ns1__cow* pSoapCow)
{
	if(pSoapCow->firstOwnerNo)
		pCowBuilder->SetFirstOwner(pSoapCow->firstOwnerNo->c_str());

	pCowBuilder->SetCowNo(pSoapCow->cowNo.c_str());
	
	pCowBuilder->SetSex(getCowSexForSoapCowSex(pSoapCow->sex));	
	
	pCowBuilder->SetStockCode(pSoapCow->stockCode.c_str());
	
	pCowBuilder->SetWeight(Weight(pSoapCow->weight));	
	
	pCowBuilder->SetPrice(Money(pSoapCow->price));	
	
	if(pSoapCow->latitude)
		pCowBuilder->SetLatitude(Latitude(*pSoapCow->latitude));
	
	if(pSoapCow->longitude)
		pCowBuilder->SetLongitude(Longitude(*pSoapCow->longitude));
	
	if(pSoapCow->passportNo)
		pCowBuilder->SetPassportNo(pSoapCow->passportNo->c_str());		
	
	if(pSoapCow->passportIssueDt)
		pCowBuilder->SetPassportIssueDt(CDateTime(*pSoapCow->passportIssueDt));
	
	if(pSoapCow->healthCertNo)
		pCowBuilder->SetHealthCertNo(pSoapCow->healthCertNo->c_str());
	
	if(pSoapCow->motherNo)
		pCowBuilder->SetMotherNo(pSoapCow->motherNo->c_str());
	
	if(pSoapCow->birthDt)
		pCowBuilder->SetBirthDt(CDateTime(*pSoapCow->birthDt));
	
	if(pSoapCow->birthPlace)
		pCowBuilder->SetBirthPlace(pSoapCow->birthPlace->c_str());
	
}

void RepoClient::ClosePurchaseCursor(const std::string& sSessionId)
{
	ns1__sessionRequest closePurchaseCursorRequest;
	ns1__closePurchaseCursorResponse closePurchaseCursorResponse;

	closePurchaseCursorRequest.sessionId = sSessionId;
	
	m_pBKRepo->__ns1__closePurchaseCursor(&closePurchaseCursorRequest,&closePurchaseCursorResponse); 
	throwIfSoapError();
	throwIfRepoError(closePurchaseCursorResponse.result);
	
	
}

void RepoClient::DownloadPurchase(const std::string& sSessionId,UINT purchaseId)
{
	ns1__downloadPurchaseRequest downloadPurchaseRq;
	ns1__sessionRequest sessionRq;
	ns1__downloadPurchaseResponse downloadPurchaseRs;

	sessionRq.sessionId = sSessionId;
	downloadPurchaseRq.purchaseId = purchaseId;
	downloadPurchaseRq.session = &sessionRq;

	m_pBKRepo->__ns1__downloadPurchase(&downloadPurchaseRq, &downloadPurchaseRs);
	throwIfSoapError();
	throwIfRepoError(downloadPurchaseRs.result);
}

void RepoClient::OpenSyncTransaction(const std::string& sSessionId)
{
	ns1__sessionRequest openSyncTransactionRequest;
	ns1__openSyncTransactionResponse openSyncTransactionResponse;

	openSyncTransactionRequest.sessionId = sSessionId;

	m_pBKRepo->__ns1__openSyncTransaction(&openSyncTransactionRequest,&openSyncTransactionResponse);
	throwIfSoapError();
	throwIfRepoError(openSyncTransactionResponse.result);
}

void RepoClient::CloseSyncTransaction(const std::string& sSessionId)
{
	ns1__sessionRequest closeSyncTransactionRequest;
	ns1__closeSyncTransactionResponse closeSyncTransactionResponse;

	closeSyncTransactionRequest.sessionId = sSessionId;

	m_pBKRepo->__ns1__closeSyncTransaction(&closeSyncTransactionRequest,&closeSyncTransactionResponse);
	throwIfSoapError();
	throwIfRepoError(closeSyncTransactionResponse.result);
}

void RepoClient::UpdateHent(const std::string& sSessionId,const RepoHent& hent)
{
	ns1__updateHentRequest updateHentRequest;
	ns1__updateHentResponse updateHentResponse;
	ns1__sessionRequest sessionRequest;
	sessionRequest.sessionId = sSessionId;
	
	ns1__hent soapHent;
	soapHent.alias = hent.GetAlias();	
	soapHent.farmno = hent.GetFarmNo();

	

	soapHent.city = hent.GetCity();	
	
	soapHent.henttype = getSoapHentTypeForRepoHentType(hent.GetHentType());
	soapHent.name = hent.GetName();
	soapHent.pobox = hent.GetPOBox();
	soapHent.street = hent.GetStreet();
	soapHent.zip = hent.GetZip();

	soapHent.bankaccountno = NULL;
	std::string bankaccountno = hent.GetAccountNo();
	if(bankaccountno.length()) { soapHent.bankaccountno = &bankaccountno; }
	
	soapHent.bankname = NULL;
	std::string bankname = hent.GetBankName();
	if(bankname.length()) { soapHent.bankname = &bankname; }

	soapHent.issuedate = NULL;
	time_t soapIssueDate;
	CDateTime repoIssueDate = hent.GetIssueDate();
	if(!repoIssueDate.IsNull())
	{
		soapIssueDate = (time_t)repoIssueDate.SecondsSinceEpoh();
		soapHent.issuedate = &soapIssueDate;
	}

	soapHent.latitude = NULL;
	Latitude latitude = hent.GetLatitude();
	double dLatitude = latitude.GetDouble();
	if(!latitude.IsNull()) { soapHent.latitude = &dLatitude; }

	soapHent.longitude = NULL;
	Longitude longitude = hent.GetLongitude();
	double dLongitude = longitude.GetDouble();
	if(!longitude.IsNull()) { soapHent.longitude = &dLongitude; }

	soapHent.cellphoneno = NULL;
	std::string cellphoneno =  hent.GetCellPhoneNo();
	if(cellphoneno.length()) { soapHent.cellphoneno = &cellphoneno; }

	soapHent.emailaddress = NULL;
	std::string emailaddress = hent.GetEmailAddress();
	if(emailaddress.length()) { soapHent.emailaddress = &emailaddress; }
	
	soapHent.extras = NULL;
	std::string extras = hent.GetExtras();
	if(extras.length()) { soapHent.extras = &extras; }
	
	soapHent.idno = NULL;
	std::string idno = hent.GetIdNo();		
	if(idno.length()) { soapHent.idno = &idno; }
	
	soapHent.issuepost = NULL;	
	std::string issuepost = hent.GetIssuePost();
	if(issuepost.length()) { soapHent.issuepost = &issuepost; }
	
	soapHent.nip = NULL;
	std::string nip = hent.GetNIP();
	if(nip.length()) { soapHent.nip = &nip; }
	
	soapHent.pesel = NULL;
	std::string pesel = hent.GetPESEL();
	if(pesel.length()) { soapHent.pesel = &pesel; }
	
	soapHent.phone = NULL;
	std::string phone = hent.GetPhone();
	if(phone.length()) { soapHent.phone = &phone; }
	
	soapHent.plate = NULL;	
	std::string plate = hent.GetPlate();
	if(plate.length()) { soapHent.plate = &plate; }
	
	soapHent.regon = NULL;	
	std::string regon = hent.GetREGON();
	if(regon.length()) { soapHent.regon = &regon; }
	
	soapHent.wetlicenceno = NULL;
	std::string wetlicenceno = hent.GetWetLicenceNo();
	if(wetlicenceno.length()) { soapHent.wetlicenceno = &wetlicenceno; }
	
	soapHent.wetno = NULL;
	std::string wetno = hent.GetWetNo();
	if(wetno.length()) { soapHent.wetno = &wetno; }
	
	updateHentRequest.session = &sessionRequest;
	updateHentRequest.hent = &soapHent;

	m_pBKRepo->__ns1__updateHent(&updateHentRequest,&updateHentResponse);
	throwIfSoapError();
	throwIfRepoError(updateHentResponse.result);
}

void RepoClient::UpdateStock(const std::string& sSessionId,const RepoStock& stock)
{
	ns1__updateStockRequest updateStockRequest;
	ns1__updateStockResponse updateStockResponse;
	ns1__sessionRequest sessionRequest;
	ns1__stock soapStock;

	sessionRequest.sessionId = sSessionId;
	updateStockRequest.session = &sessionRequest;

	soapStock.stockCode = stock.GetStockCode();
	soapStock.stockName = NULL;
	std::string stockName = stock.GetStockName();
	if(stockName.length() > 0)
		soapStock.stockName = &stockName;
	
	soapStock.preDefSex = getSoapCowSexForCowSex(stock.GetPredefSex().getCOWSEX());
	 
	soapStock.pricePerKg = NULL;
	Money pricePerKg = stock.GetMyPerKgPrice();
	double dPricePerKg;
	if(!pricePerKg.IsNull())
	{
		dPricePerKg = pricePerKg.GetDouble();
		soapStock.pricePerKg = &dPricePerKg;
	}
	updateStockRequest.stock = &soapStock;

	m_pBKRepo->__ns1__updateStock(&updateStockRequest,&updateStockResponse);
	throwIfSoapError();
	throwIfRepoError(updateStockResponse.result);
}

void RepoClient::UpdateCompany(const std::string& sSessionId,const RepoCompany& company)
{
	ns1__updateCompanyRequest updateCompanyRq;
	ns1__updateCompanyResponse updateCompanyRs;
	ns1__sessionRequest sessionRequest;
	
	updateCompanyRq.session = &sessionRequest;
	sessionRequest.sessionId = sSessionId;
	
	ns1__company * pSoapCompany  = soap_new_ns1__company(m_pBKRepo->soap,-1);

	pSoapCompany->name = company.GetCompanyName();	

	pSoapCompany->street = company.GetStreet();
	pSoapCompany->POBox = company.GetPOBox();
	pSoapCompany->city = company.GetCity();
	pSoapCompany->zip = company.GetZipCode();
	pSoapCompany->farmNo = company.GetFarmNo();
	//pSoapCompany->cellPhoneNo = company.Get;
	pSoapCompany->phoneNo = company.GetPhoneNo();	
	//pSoapCompany->emailAddress = company.Get;
	pSoapCompany->nip = company.GetNIP();
	
	updateCompanyRq.company = pSoapCompany;

	m_pBKRepo->__ns1__updateCompany(&updateCompanyRq,&updateCompanyRs);
	throwIfSoapError();
	throwIfRepoError(updateCompanyRs.result);

}

void RepoClient::CommitSyncTransaction(const std::string& sSessionId)
{
	ns1__sessionRequest commitSyncTransactionRequest;
	ns1__commitSyncTransactionResponse commitSyncTransactionResponse;


	commitSyncTransactionRequest.sessionId = sSessionId;
	m_pBKRepo->__ns1__commitSyncTransaction(&commitSyncTransactionRequest,&commitSyncTransactionResponse);

	throwIfSoapError();
	throwIfRepoError(commitSyncTransactionResponse.result);

}