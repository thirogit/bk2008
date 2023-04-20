#ifndef __NEWINDOCDISPATCHER_H__
#define __NEWINDOCDISPATCHER_H__

#include "../datatypes/InDoc.h"

class NewInDocDispatcher
{
public:
	static void Dispatch(InDoc *pDoc);
};


#endif