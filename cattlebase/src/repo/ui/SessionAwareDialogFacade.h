#ifndef __SESSIONVIEWFACADE_H__
#define __SESSIONVIEWFACADE_H__

#include "SessionAware.h"
#include "ctrls/RepoToolBar.h"

class SessionElementViewFacade : public SessionAware
{
public:
	virtual void Show() = 0;
	virtual void Hide() = 0;	
};


#endif