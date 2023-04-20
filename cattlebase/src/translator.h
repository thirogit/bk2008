#ifndef __TRANSLATOR_H__
#define __TRANSLATOR_H__


template<class VALUEIN,class VALUEOUT>
class Translator
{
public:
	virtual VALUEOUT Translate(VALUEIN valueIn) = 0;
};

#endif