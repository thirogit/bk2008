#ifndef __SESSIONAWARE_H__
#define __SESSIONAWARE_H__

#include "../RepoAccountSession.h"

class SessionAware
{

public:
    virtual void UpdateFrom(RepoAccountSession* pAccountSession) = 0;
	virtual void BeforeUpdate() = 0;
	virtual void AfterUpdate() = 0;  
};


#endif