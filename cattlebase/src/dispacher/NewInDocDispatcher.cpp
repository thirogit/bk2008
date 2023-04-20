#include "stdafx.h"
#include "NewInDocDispatcher.h"
#include "../global_arrays.h"
#include "../mainfrm_ctrl.h"


void NewInDocDispatcher::Dispatch(InDoc *pDoc)
{
	ary_indocs.Add(pDoc);		
	ary_cows.AppendPtr(pDoc->GetCows());
	ary_incows.AppendPtr(pDoc->GetCows());
	UpdateGrid(INCOWSGRID);
	UpdateGrid(ALLCOWSGRID);
	UpdateGrid(INDOCSGRID);
}
