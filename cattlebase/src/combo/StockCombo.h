#ifndef __STOCKCOMBO_H__
#define __STOCKCOMBO_H__

#include "../DataTypes.h"
#include "BaseCombo.h"

//template CBaseCombo<CComboBox,Stock>;

class CStockCombo : public CBasePtrCombo<CComboBox,Stock>
{
public:
	CStockCombo();
protected:
	Stock *GetItemText(int item,CString &itemText);
	int GetNewItemCount();
};

//template CBaseFindCombo<CStockCombo>;
typedef CBaseFindCombo<CStockCombo> CFindStockCombo;

/*
class CFindStockCombo : public CStockCombo
{
public:
	CFindStockCombo();
	void InitFindStockCombo();
	bool IsNeutral();
private:
	int m_neurtalItem;
};
*/
#endif