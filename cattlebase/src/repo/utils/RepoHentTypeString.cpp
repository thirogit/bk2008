#include "stdafx.h"
#include "RepoHentTypeString.h"

	
RepoHentTypeString::RepoHentTypeString(RepoHentType hentType) : m_hentType(hentType)
{
	switch(hentType)
	{
		case HentType_Individual:
			LoadString(IDS_HENTTYPE_INDIVIDUAL);
			break;
		case HentType_Company:
			LoadString(IDS_HENTTYPE_COMPANY);
			break;
	}
}
	
const CString& RepoHentTypeString::ToString()
{
	return *this;  	
}


