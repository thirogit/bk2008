#ifndef __PREDICATE_H__
#define __PREDICATE_H__

template<class TYPE>
class Predicate
{
public:
	virtual bool Evaluate(TYPE candidate) = 0;
};


#endif