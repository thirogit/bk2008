#ifndef __SORTABLELISTCTRL_H__
#define __SORTABLELISTCTRL_H__
#include "stdafx.h"
#pragma warning(disable : 4786)
#include <functional>
#include <set>

/*
#define MAX_ITEM_TEXT 100

struct LVITEM_less : public std::binary_function<LVITEM*, LVITEM*, bool>
{
    bool operator()(const LVITEM* pItem1, const LVITEM* pItem2) const
    {
        CString strItem1(pItem1->pszText);
        CString strItem2(pItem2->pszText);

        return (strItem1 < strItem2);
    }
};
*/
class CSortableListCtrl : public CListCtrl
{
public:
	CSortableListCtrl() : CListCtrl() {};
	virtual ~CSortableListCtrl() {} ;
	//void SortByCol(int Column,BOOL bAsc);
	int GetCurSel();
	int GetColumnCount();
protected:
	//void SortItemsDescending();

protected:
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

};




#endif