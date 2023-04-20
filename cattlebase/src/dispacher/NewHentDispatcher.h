#ifndef __NEWHENTDISPATCHER_H__
#define __NEWHENTDISPATCHER_H__

#include "../datatypes/Doc.h"
#include "../global_arrays.h"
#include "../mainfrm_ctrl.h"

class NewHentDispatcher
{
public:
	static void Dispatch(Hent *pHent);
	static void ReDispatch(Hent *pHent);
};


#endif