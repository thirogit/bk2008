#include "stdafx.h"
#include "RepoException.h"

IMPLEMENT_DYNAMIC(RepoException, CBaseException)


RepoException::RepoException(const CString &errorMsg) 
{
	error = errorMsg;
}

RepoException::RepoException(const RepoException &src)
{
	error = src.error;
}