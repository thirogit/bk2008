#include "stdafx.h"
#include "NewHentDispatcher.h"



void NewHentDispatcher::Dispatch(Hent *pHent)
{
	ary_hents.Add(pHent);
	UpdateGrid(HENTSGRID);
	switch(pHent->GetHentType()->henttypeid)
	{
		case HTYPE_COMPANY:
			ary_companies.Add(pHent);
			UpdateGrid(COMPANYHENTGRID);
		break;
		case HTYPE_INDIVIDUAL:
			ary_farmers.Add(pHent);
			UpdateGrid(FARMERHENTGRID);		
		break;
	}	
}

void NewHentDispatcher::ReDispatch(Hent *pHent)
{
	LockGrid(COMPANYHENTGRID);
	LockGrid(FARMERHENTGRID);
		
	ary_companies.RemovePtr(pHent);				
	ary_farmers.RemovePtr(pHent);
		
	switch(pHent->GetHentType()->henttypeid)
	{
		case HTYPE_COMPANY:
			ary_companies.Add(pHent);			
		break;
		case HTYPE_INDIVIDUAL:
			ary_farmers.Add(pHent);			
		break;
	}

	UpdateGrid(COMPANYHENTGRID);
	UpdateGrid(FARMERHENTGRID);		
}
