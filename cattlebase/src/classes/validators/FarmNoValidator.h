#ifndef __FARMNOVALIDATOR_H__
#define __FARMNOVALIDATOR_H__

#include "../types/Validator.h"
#include "Code39Validator.h"

class CFarmNoValidator : public CCode39Validator
{
public:
	virtual bool IsValid(const CString& anArg);
	virtual CString TellMeWhy(const CString& anArg);
};

#endif