#include "stdafx.h"
#include "NewOutDocDispatcher.h"
#include "../global_arrays.h"
#include "../mainfrm_ctrl.h"


void NewOutDocDispatcher::Dispatch(OutDoc *pDoc)
{
	LockGrid(INCOWSGRID);
	ary_incows.RemoveArrayPtr(pDoc->GetCows());
	ary_outdocs.Add(pDoc);
	UpdateGrid(INCOWSGRID);
	UpdateGrid(OUTDOCSGRID);		
}
