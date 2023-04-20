#ifndef __HENTDIFFLIST_H__
#define  __HENTDIFFLIST_H__

#include "../BaseListCtrl.h"
#include "../../DataTypes.h"

template<class VALUETYPE>
class CDataValueDiff
{
public:
	virtual VALUETYPE GetValue1() = 0;
	virtual VALUETYPE GetValue2() = 0;
	virtual CString GetTextValue1() = 0;
	virtual CString GetTextValue2() = 0;
	bool IsValue1Selected()
	{
		return bValue1Selected;
	};

	bool IsValue2Selected()
	{
		return !bValue1Selected;
	};
protected:
	bool bValue1Selected;

};


class CHentDiffList : public CBaseListCtrl
{
public:
	CHentDiffList(InvoiceData &invoice);
	void UpdateItemCount();
	enum HENTDIFFLISTCOLS
	{
		COL_HENTMEMBER_NAME = 1000,
		COL_HENTMEMBER_VALUE1
		COL_HENTMEMBER_VALUE2		
	};
private:
    static const GRIDHEADERS HentDiffListCols[]; 
	CCowDataArray m_cowArray;
	
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual void SortByCol(int Column,bool bAsc);
	
protected:
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP();
};

#endif