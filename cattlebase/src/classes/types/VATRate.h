#ifndef __VATRATE_H__
#define __VATRATE_H__

#include "Percent.h"

class VATRate : public Percent
{
public:
	VATRate();
	VATRate(double dVATRate);
	VATRate(const VATRate& copy);
	VATRate& operator=(const VATRate &right);
	VATRate& operator=(double dVATRate);
	
	/*bool operator>=(const VATRate& right);
	bool operator<=(const VATRate& right);
	bool operator>(const VATRate& right);
	bool operator<(const VATRate& right);
	bool operator==(const VATRate& right);
	bool operator!=(const VATRate& right);*/
protected:
	void ValidateNull();

	

};




#endif
