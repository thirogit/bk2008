#include "stdafx.h"
#include "HentType.h"


HentType::HentType() : henttypeid(0) 
{
	
}

bool HentType::IsIndividual()
{
	return henttypeid == HTYPE_INDIVIDUAL;
}
bool HentType::IsCompany()
{
	return henttypeid == HTYPE_COMPANY;
}
