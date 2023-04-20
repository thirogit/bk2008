#ifndef __REPOACCOUNTDAO_H__
#define __REPOACCOUNTDAO_H__

#include "../taskprogress/SimpleProgress.h"
#include "db/Transaction.h"
#include "BaseDAO.h"
#include "../PtrFlushArray.h"
#include "../repo/RepoAccount.h"

class RepoAccountDAO : public BaseDAO
{
public:
	bool LoadRepoAccounts(PtrFlushArray<RepoAccount>& result,SimpleProgress *progress = NULL);
	void UpdateRepoAccount(Transaction transaction,const RepoAccount* pAccount);
	void DeleteRepoAccount(Transaction transaction,const RepoAccount* pAccount);
	RepoAccount* InsertRepoAccount(Transaction transaction,const RepoAccount* pAccount);

};

#endif