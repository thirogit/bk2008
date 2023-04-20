#ifndef __PREDICATES_H__
#define __PREDICATES_H__

#include "../Discrepancy.h"
#include "../compare/Predicate.h"

template<class EQUIV_TYPE>
class AlwaysTrue : public Predicate<EQUIV_TYPE>
{
public:
	virtual bool Evaluate(EQUIV_TYPE candidate)
	{
		return true;
	}
};

template<class EQUIV_TYPE>
class NotPredicate : public Predicate<EQUIV_TYPE>
{
public:
	NotPredicate(Predicate<EQUIV_TYPE>& predicate) : m_Predicate(predicate)
	{
	}
	virtual bool Evaluate(EQUIV_TYPE candidate)
	{
		return !m_Predicate.Evaluate(candidate);
	}
private:
	Predicate<EQUIV_TYPE>& m_Predicate;
};


template<class EQUIV_TYPE>
class SelectMissingRemote : public Predicate<EQUIV_TYPE>
{
public:
	virtual bool Evaluate(EQUIV_TYPE candidate)
	{
		return candidate->GetDiscrepancy().GetType() == MISSING_REMOTE;
	}
};


template<class EQUIV_TYPE>
class SelectMissingLocal : public Predicate<EQUIV_TYPE>
{
public:
	virtual bool Evaluate(EQUIV_TYPE candidate)
	{
		return candidate->GetDiscrepancy().GetType() == MISSING_LOCAL;
	}
};


template<class EQUIV_TYPE>
class SelectNotEqual : public Predicate<EQUIV_TYPE>
{
public:
	virtual bool Evaluate(EQUIV_TYPE candidate)
	{		
		return candidate->GetDiscrepancy().GetType() == HAS_BOTH && !candidate->IsEqual();
	}
};


#endif