#ifndef __TRANSACTEDBUILDER_H__
#define __TRANSACTEDBUILDER_H__

template<class BUILDER>
class TransactedBuilder 
{
public:
	virtual void Commit() = 0;
	virtual void Begin() = 0;
	virtual void Rollback() = 0;
};

#endif