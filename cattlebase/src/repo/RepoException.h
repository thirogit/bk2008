#ifndef __REPOEXCEPTION_H__
#define __REPOEXCEPTION_H__

#include "../exceptions/BaseException.h" 

class RepoException : public CBaseException
{
	DECLARE_DYNAMIC(RepoException)
public:
	RepoException(const CString &errorMsg);
	RepoException(const RepoException &src);
};

#endif