#include "stdafx.h"
#include "BaseCombo.h"
/*
template<class BASECLASS,class BASETYPE>
CBaseCombo<BASECLASS,BASETYPE>::CBaseCombo(int nSize) : BASECLASS(), m_nSize(nSize)
{
	ASSERT(RUNTIME_CLASS(BASECLASS)->IsDeriverFrom(RUNTIME_CLASS(CComboBox)));
}
template<class BASECLASS,class BASETYPE>
void CBaseCombo<BASECLASS,BASETYPE>::InitCombo(BASETYPE *curSel)
{
	ReloadCombo(curSel);
}
template<class BASECLASS,class BASETYPE>
void CBaseCombo<BASECLASS,BASETYPE>::ReloadCombo(BASETYPE *curSel)
{
	int item,defaultItem = -1;
	CString itemText;
	BASETYPE *itemData;

	ResetContent();

	for(int i = 0;i < m_nSize;i++)
	{
		itemData = GetItemText(i,itemText);
		item = AddString(itemText);
		if(item < 0) throw CStrException(GetLastError());
		SetItemData(item,(DWORD_PTR)itemData);
		if(itemData == curSel) 
			defaultItem = item;
		else if(item <= defaultItem)
			defaultItem++;
	}


}
template<class BASECLASS,class BASETYPE>
BASETYPE *CBaseCombo<BASECLASS,BASETYPE>::GetSelItemData()
{
	int nSel = GetCurSel();
	if(nSel < 0) return NULL;
	return (BASETYPE*)GetItemData(nSel);
}
*/
/*
template<class BASECOMBO>
CBaseFindCombo<BASECOMBO>::CBaseFindCombo() : BASECOMBO(), m_nNeutralItem(-1)
{}

template<class BASECOMBO>
CBaseFindCombo<BASECOMBO>::InitFindCombo()
{
	InitCombo(NULL);
	m_neurtalItem  = AddString(NEUTRAL_STR);
	SetCurSel(m_neurtalItem);
}
template<class BASECOMBO>
bool CBaseFindCombo<BASECOMBO>::IsNeutral()
{
	return (GetCurSel() == m_neurtalItem);
}*/

/*
template<class BASECLASS,class BASETYPE>
BASETYPE* CBaseCombo<BASECLASS,BASETYPE>::GetItemText(CString &itemText)
{
}*/
