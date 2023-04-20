#ifndef __REPOSESSION_H__
#define __REPOSESSION_H__

#include "RepoCompany.h"
#include <boost/shared_ptr.hpp>
#include "Cursor.h"
#include "RepoHent.h"
#include "RepoStock.h"
#include "RepoPurchase.h"
#include "RepoSyncTransaction.h"

typedef Cursor<RepoStock> StockCursor;
typedef boost::shared_ptr<StockCursor> StockCursorPtr; 

typedef Cursor<RepoHent> HentCursor;
typedef boost::shared_ptr<HentCursor> HentCursorPtr; 

typedef Cursor<RepoPurchase> PurchaseCursor;
typedef boost::shared_ptr<PurchaseCursor> PurchaseCursorPtr; 


class RepoSession
{
public:
	virtual void Close() = 0;
	virtual void FetchCompany(RepoCompany& company) = 0;			
	virtual HentCursorPtr OpenHentCursor() = 0;
	virtual StockCursorPtr OpenStockCursor() = 0;
	virtual PurchaseCursorPtr OpenPurchaseCursor() = 0;
	virtual void DownloadPurchase(UINT purchaseId) = 0;
	virtual RepoSyncTransaction* OpenTransaction() = 0;
};


#endif