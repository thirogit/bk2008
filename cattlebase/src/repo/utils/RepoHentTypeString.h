#ifndef __REPOHENTTYPESTRING_H__
#define __REPOHENTTYPESTRING_H__

#include "../RepoHentType.h"

class RepoHentTypeString : private CString
{
public:	
	RepoHentTypeString(RepoHentType hentType);
	const CString& ToString();
private:
	RepoHentType m_hentType;
};

#endif