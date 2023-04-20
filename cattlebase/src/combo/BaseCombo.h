#ifndef __BASECOMBO_H__
#define __BASECOMBO_H__


template<class BASECLASS,class BASETYPE>
class CBaseCombo : public BASECLASS
{
public:
	CBaseCombo(int nSize) : BASECLASS(), m_nSize(nSize)
	{
	}	

	virtual void ReloadCombo(BASETYPE curSel)
	{
		int item,defaultItem = -1;
		CString itemText;
		BASETYPE itemData;

		ResetContent();	

		for(int i = 0;i < m_nSize;i++)
		{
			itemData = GetItemText(i,itemText);
			item = AddString(itemText);
			if(item >= 0)
			{
				SetItemData(item,(DWORD_PTR)itemData);
				if(itemData == curSel) 
					defaultItem = item;
				else if(item <= defaultItem)
					defaultItem++;
			}
		}
		SetCurSel(defaultItem);
	}
	virtual BASETYPE GetSelItemData()
	{
		int nSel = GetCurSel();
		if(nSel < 0) return (BASETYPE)NULL;
		return (BASETYPE)GetItemData(nSel);
	}

	void UpdateItemCount()
	{
		m_nSize = GetNewItemCount();
	}

protected:
	virtual BASETYPE GetItemText(int item,CString &itemText) = 0;
	int m_nSize;
	virtual int GetNewItemCount() { return m_nSize; };
};


template<class BASECLASS,class BASETYPEPTR>
class CBasePtrCombo : public CBaseCombo<BASECLASS,BASETYPEPTR*>
{
public:
	CBasePtrCombo(int nSize) : CBaseCombo<BASECLASS,BASETYPEPTR*>(nSize)
	{
	}	

	virtual void ReloadCombo(BASETYPEPTR *curSel = NULL)
	{
		CBaseCombo<BASECLASS,BASETYPEPTR*>::ReloadCombo(curSel);
	}
};

#define NEUTRAL_STR "*"

template<class BASECOMBO>
class CBaseFindCombo : public BASECOMBO
{
public:
	CBaseFindCombo() : BASECOMBO(), m_nNeutralItem(-1)	{};
	virtual void InitFindCombo()
	{
		ReloadCombo(/*NULL*/);
		m_nNeutralItem  = AddString(NEUTRAL_STR);
		SetCurSel(m_nNeutralItem);
	}
	bool IsNeutral()
	{
		return (GetCurSel() == m_nNeutralItem);
	};

protected:
	int m_nNeutralItem;
};

#endif