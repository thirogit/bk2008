#ifndef __SIMPLEPROGRESS_H__
#define __SIMPLEPROGRESS_H__


class SimpleProgress
{
public:	
	virtual void SetSteps(int maxSteps) = 0;
	virtual bool Step() = 0;
};

#endif