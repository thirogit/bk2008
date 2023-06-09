/* BKRepoServiceBKRepoProxy.h
   Generated by gSOAP 2.8.6 from BKRepoService.h

Copyright(C) 2000-2011, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
1) GPL or 2) Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef BKRepoServiceBKRepoProxy_H
#define BKRepoServiceBKRepoProxy_H
#include "BKRepoServiceH.h"
class BKRepo
{   public:
	/// Runtime engine context allocated in constructor
	struct soap *soap;
	/// Endpoint URL of service 'BKRepo' (change as needed)
	const char *endpoint;
	/// Constructor allocates soap engine context, sets default endpoint URL, and sets namespace mapping table
	BKRepo()
	{ soap = soap_new(); endpoint = "http://localhost:8080/services/bk/repo"; if (soap && !soap->namespaces) { static const struct Namespace namespaces[] = 
{
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"ns1", "http://com.bk.repo", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	soap->namespaces = namespaces; } };
	/// Destructor frees deserialized data and soap engine context
	virtual ~BKRepo() { if (soap) { soap_destroy(soap); soap_end(soap); soap_free(soap); } };
	/// Invoke 'openMobileSession' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__openMobileSession(ns1__openMobileSessionRequest *ns1__openMobileSessionRequest_, ns1__openSessionResponse *ns1__openSessionResponse_) { return soap ? soap_call___ns1__openMobileSession(soap, endpoint, NULL, ns1__openMobileSessionRequest_, ns1__openSessionResponse_) : SOAP_EOM; };
	/// Invoke 'openOfficeSession' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__openOfficeSession(ns1__openOfficeSessionRequest *ns1__openOfficeSessionRequest_, ns1__openSessionResponse *ns1__openSessionResponse_) { return soap ? soap_call___ns1__openOfficeSession(soap, endpoint, NULL, ns1__openOfficeSessionRequest_, ns1__openSessionResponse_) : SOAP_EOM; };
	/// Invoke 'closeSession' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__closeSession(ns1__sessionRequest *ns1__closeSessionRequest, ns1__closeSessionResponse *ns1__closeSessionResponse_) { return soap ? soap_call___ns1__closeSession(soap, endpoint, NULL, ns1__closeSessionRequest, ns1__closeSessionResponse_) : SOAP_EOM; };
	/// Invoke 'openHentCursor' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__openHentCursor(ns1__sessionRequest *ns1__openHentCursorRequest, ns1__openHentCursorResponse *ns1__openHentCursorResponse_) { return soap ? soap_call___ns1__openHentCursor(soap, endpoint, NULL, ns1__openHentCursorRequest, ns1__openHentCursorResponse_) : SOAP_EOM; };
	/// Invoke 'fetchHents' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__fetchHents(ns1__cursorRequest *ns1__fetchHentsRequest, ns1__fetchHentsResponse *ns1__fetchHentsResponse_) { return soap ? soap_call___ns1__fetchHents(soap, endpoint, NULL, ns1__fetchHentsRequest, ns1__fetchHentsResponse_) : SOAP_EOM; };
	/// Invoke 'closeHentCursor' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__closeHentCursor(ns1__sessionRequest *ns1__closeHentCursorRequest, ns1__closeHentCursorResponse *ns1__closeHentCursorResponse_) { return soap ? soap_call___ns1__closeHentCursor(soap, endpoint, NULL, ns1__closeHentCursorRequest, ns1__closeHentCursorResponse_) : SOAP_EOM; };
	/// Invoke 'openSyncTransaction' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__openSyncTransaction(ns1__sessionRequest *ns1__openSyncTransactionRequest, ns1__openSyncTransactionResponse *ns1__openSyncTransactionResponse_) { return soap ? soap_call___ns1__openSyncTransaction(soap, endpoint, NULL, ns1__openSyncTransactionRequest, ns1__openSyncTransactionResponse_) : SOAP_EOM; };
	/// Invoke 'updateHent' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__updateHent(ns1__updateHentRequest *ns1__updateHentRequest_, ns1__updateHentResponse *ns1__updateHentResponse_) { return soap ? soap_call___ns1__updateHent(soap, endpoint, NULL, ns1__updateHentRequest_, ns1__updateHentResponse_) : SOAP_EOM; };
	/// Invoke 'updateStock' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__updateStock(ns1__updateStockRequest *ns1__updateStockRequest_, ns1__updateStockResponse *ns1__updateStockResponse_) { return soap ? soap_call___ns1__updateStock(soap, endpoint, NULL, ns1__updateStockRequest_, ns1__updateStockResponse_) : SOAP_EOM; };
	/// Invoke 'closeSyncTransaction' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__closeSyncTransaction(ns1__sessionRequest *ns1__closeSyncTransactionRequest_, ns1__closeSyncTransactionResponse *ns1__closeSyncTransactionResponse_) { return soap ? soap_call___ns1__closeSyncTransaction(soap, endpoint, NULL, ns1__closeSyncTransactionRequest_, ns1__closeSyncTransactionResponse_) : SOAP_EOM; };
	/// Invoke 'commitSyncTransaction' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__commitSyncTransaction(ns1__sessionRequest *ns1__commitSyncTransactionRequest_, ns1__commitSyncTransactionResponse *ns1__commitSyncTransactionResponse_) { return soap ? soap_call___ns1__commitSyncTransaction(soap, endpoint, NULL, ns1__commitSyncTransactionRequest_, ns1__commitSyncTransactionResponse_) : SOAP_EOM; };
	/// Invoke 'openStockCursor' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__openStockCursor(ns1__sessionRequest *ns1__openStockCursorRequest, ns1__openStockCursorResponse *ns1__openStockCursorResponse_) { return soap ? soap_call___ns1__openStockCursor(soap, endpoint, NULL, ns1__openStockCursorRequest, ns1__openStockCursorResponse_) : SOAP_EOM; };
	/// Invoke 'fetchStocks' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__fetchStocks(ns1__cursorRequest *ns1__fetchStocksRequest, ns1__fetchStocksResponse *ns1__fetchStocksResponse_) { return soap ? soap_call___ns1__fetchStocks(soap, endpoint, NULL, ns1__fetchStocksRequest, ns1__fetchStocksResponse_) : SOAP_EOM; };
	/// Invoke 'closeStockCursor' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__closeStockCursor(ns1__sessionRequest *ns1__closeStockCursorRequest, ns1__closeStockCursorResponse *ns1__closeStockCursorResponse_) { return soap ? soap_call___ns1__closeStockCursor(soap, endpoint, NULL, ns1__closeStockCursorRequest, ns1__closeStockCursorResponse_) : SOAP_EOM; };
	/// Invoke 'updateCompany' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__updateCompany(ns1__updateCompanyRequest *ns1__updateCompanyRequest_, ns1__updateCompanyResponse *ns1__updateCompanyResponse_) { return soap ? soap_call___ns1__updateCompany(soap, endpoint, NULL, ns1__updateCompanyRequest_, ns1__updateCompanyResponse_) : SOAP_EOM; };
	/// Invoke 'fetchCompany' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__fetchCompany(ns1__sessionRequest *ns1__fetchCompanyRequest, ns1__fetchCompanyResponse *ns1__fetchCompanyResponse_) { return soap ? soap_call___ns1__fetchCompany(soap, endpoint, NULL, ns1__fetchCompanyRequest, ns1__fetchCompanyResponse_) : SOAP_EOM; };
	/// Invoke 'uploadPurchase' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__uploadPurchase(ns1__uploadPurchaseRequest *ns1__uploadPurchaseRequest_, ns1__uploadPurchaseResponse *ns1__uploadPurchaseResponse_) { return soap ? soap_call___ns1__uploadPurchase(soap, endpoint, NULL, ns1__uploadPurchaseRequest_, ns1__uploadPurchaseResponse_) : SOAP_EOM; };
	/// Invoke 'openPurchaseCursor' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__openPurchaseCursor(ns1__sessionRequest *ns1__openPurchaseCursorRequest, ns1__openPurchaseCursorResponse *ns1__openPurchaseCursorResponse_) { return soap ? soap_call___ns1__openPurchaseCursor(soap, endpoint, NULL, ns1__openPurchaseCursorRequest, ns1__openPurchaseCursorResponse_) : SOAP_EOM; };
	/// Invoke 'closePurchaseCursor' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__closePurchaseCursor(ns1__sessionRequest *ns1__closePurchaseCursorRequest, ns1__closePurchaseCursorResponse *ns1__closePurchaseCursorResponse_) { return soap ? soap_call___ns1__closePurchaseCursor(soap, endpoint, NULL, ns1__closePurchaseCursorRequest, ns1__closePurchaseCursorResponse_) : SOAP_EOM; };
	/// Invoke 'fetchPurchases' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__fetchPurchases(ns1__cursorRequest *ns1__fetchPurchasesRequest, ns1__fetchPurchasesResponse *ns1__fetchPurchasesResponse_) { return soap ? soap_call___ns1__fetchPurchases(soap, endpoint, NULL, ns1__fetchPurchasesRequest, ns1__fetchPurchasesResponse_) : SOAP_EOM; };
	/// Invoke 'downloadPurchase' of service 'BKRepo' and return error code (or SOAP_OK)
	virtual int __ns1__downloadPurchase(ns1__downloadPurchaseRequest *ns1__downloadPurchaseRequest_, ns1__downloadPurchaseResponse *ns1__downloadPurchaseResponse_) { return soap ? soap_call___ns1__downloadPurchase(soap, endpoint, NULL, ns1__downloadPurchaseRequest_, ns1__downloadPurchaseResponse_) : SOAP_EOM; };
};
#endif
