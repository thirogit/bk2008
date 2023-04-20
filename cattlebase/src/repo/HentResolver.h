#ifndef __HENTRESOLVER_H__
#define __HENTRESOLVER_H__

#include "RepoHent.h"

class HentResolver 
{
public:
	virtual RepoHent* Resolve(const CString& sFarmNo) = 0;	
};

#endif