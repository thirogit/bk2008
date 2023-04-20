#ifndef __HENTCONVERTER_H__
#define __HENTCONVERTER_H__

#include "RepoHent.h"

class HentConverter 
{
public:
	static void ConvertHent(const RepoHent& src,Hent& dest);
	static HentType* GetHentTypeForRepoHentType(RepoHentType hentType);
	
	
};

#endif