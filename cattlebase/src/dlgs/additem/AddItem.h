#pragma once

#include "AddItemBase.h"

template<class ARRAY,class LISTCTRL>
class CAddItemDlg : public CAddItemBaseDlg
{
public:
	CAddItemDlg(ARRAY &inputArray,CWnd* pParent = NULL) : CAddItemBaseDlg(pParent)
	{
		m_itemList = new LISTCTRL(&inputArray);
	};
};
