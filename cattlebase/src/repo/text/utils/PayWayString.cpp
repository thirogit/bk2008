#include "stdafx.h"
#include "PayWayString.h"
#include "../../../consts.h"

PayWayString::PayWayString(Invoice::PAYWAYS payWay)
{
	switch(payWay)
	{
		case Invoice::PAY_TRANS:
			m_sPayWay = PayWayNames[0];
			break;
		case Invoice::PAY_CASH:
			m_sPayWay = PayWayNames[1];
			break;
		
	}

}
	
PayWayString::operator CString()  const
{
	return m_sPayWay;
}
PayWayString::operator LPCSTR() const
{
	return (LPCSTR)m_sPayWay;
}

const CString& PayWayString::ToString()
{
	return m_sPayWay;
}