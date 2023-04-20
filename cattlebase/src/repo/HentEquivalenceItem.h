#ifndef __HENTEQUIVALENCEITEM_H__
#define __HENTEQUIVALENCEITEM_H__

#include "HentEquivalence.h"
#include "UpdatableItem.h"

class HentEquivalenceItem : public HentEquivalence,public UpdatableItem
{
public:
	HentEquivalenceItem(RepoHent* pLocalHent,RepoHent* pRemoteHent);
	
};

#endif