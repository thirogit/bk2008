#ifndef __TRANSACTEDDATAMEMBER_H__
#define __TRANSACTEDDATAMEMBER_H__

class TransactedDataMember
{
public:
	virtual void Rollback() = 0;
	virtual void Commit() = 0;
	virtual bool IsUncommited() = 0;
	
};

#endif