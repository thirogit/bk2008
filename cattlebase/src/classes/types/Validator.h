#ifndef __VALIDATOR_H__
#define __VALIDATOR_H__


template<class TYPE>
class CValidator 
{
public:
	virtual bool IsValid(TYPE anArg) = 0;
	virtual CString TellMeWhy(TYPE anArg) = 0;
	virtual bool IsSoftViolation(TYPE anArg) = 0;
	virtual bool IsHardViolation(TYPE anArg) = 0;

};

#endif