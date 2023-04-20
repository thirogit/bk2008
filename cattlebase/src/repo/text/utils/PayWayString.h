#ifndef __PAYWAYSTRING_H__
#define __PAYWAYSTRING_H__

#include "../../../datatypes/Invoice.h"


class PayWayString 
{
public:
	PayWayString(Invoice::PAYWAYS payWay);
	operator CString() const;
	operator LPCSTR() const;
	const CString& ToString();
private:

	CString m_sPayWay;
	

};

#endif