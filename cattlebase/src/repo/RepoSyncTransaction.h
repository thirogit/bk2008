#ifndef __REPOSYNCTRANSACTION_H__
#define __REPOSYNCTRANSACTION_H__

#include "RepoHent.h"
#include "RepoStock.h"
#include "RepoCompany.h"

class RepoSyncTransaction
{
public:
	virtual void Close() = 0;
	virtual void Commit() = 0;
	virtual void UpdateHent(const RepoHent& hent) = 0;
	virtual void UpdateStock(const RepoStock& stock) = 0;
	virtual void UpdateCompany(const RepoCompany& company) = 0;
	
};


#endif