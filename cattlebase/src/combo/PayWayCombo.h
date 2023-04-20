#ifndef __PAYWAYCOMBO_H__
#define __PAYWAYCOMBO_H__

#include "../DataTypes.h"
#include "BaseCombo.h"


class CPaywayCombo : public CComboBox
{
public:
	void ReloadCombo(Invoice::PAYWAYS curSel = Invoice::PAY_TRANS); //default parameter just for CFindBaseCombo template
};

typedef CBaseFindCombo<CPaywayCombo> CFindPaywayCombo;

#endif