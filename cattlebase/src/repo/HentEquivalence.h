#ifndef __HENTEQUIVALENCE_H__
#define __HENTEQUIVALENCE_H__

#include "TypedEquivalence.h"
#include "RepoHent.h"
#include "compare/RepoHentComparator.h"

class HentEquivalence : public TypedEquivalence<RepoHent>
{
public:
	HentEquivalence(RepoHent* pLocalHent,RepoHent* pRemoteHent);
	virtual bool IsEqual();
private:
	RepoHentComparator m_HentComparator;
};

#endif