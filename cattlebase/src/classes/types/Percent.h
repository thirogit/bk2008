#ifndef __PERCENT_H__
#define __PERCENT_H__

#include "NullDouble.h"

#define MAX_PRECISION 10

class Percent : public NullDouble
{
public:
	Percent();
	explicit Percent(double dPercent);
	Percent(const Percent& copy);
	Percent& operator=(const Percent &right);
	Percent& operator=(double dPercent);
	
	CString To100String(LPCSTR szIfNull = NULL, int prec = 2) const;

	bool operator>=(const Percent& right);
	bool operator<=(const Percent& right);
	bool operator>(const Percent& right);
	bool operator<(const Percent& right);
	bool operator==(const Percent& right);
	bool operator!=(const Percent& right);

	
};

#endif
