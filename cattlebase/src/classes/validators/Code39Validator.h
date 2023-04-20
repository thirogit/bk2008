#ifndef __CODE39VALIDATOR_H__
#define __CODE39VALIDATOR_H__

#include "../types/Validator.h"

class CCode39Validator : public CValidator<const CString&>
{
public:
	virtual bool IsSoftViolation(const CString& anArg);
	virtual bool IsHardViolation(const CString& anArg);

};

#endif