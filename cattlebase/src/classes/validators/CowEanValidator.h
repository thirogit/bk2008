#ifndef __COWEANVALIDATOR_H__
#define __COWEANVALIDATOR_H__

#include "../types/Validator.h"
#include "Code39Validator.h"


class CCowEanValidator : public CCode39Validator
{
public:
	virtual bool IsValid(const CString& anArg);
	virtual CString TellMeWhy(const CString& anArg);
};

#endif