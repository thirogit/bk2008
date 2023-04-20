#ifndef __STOCKLISTCTRL_H__
#define __STOCKLISTCTRL_H__

#include "../BaseListCtrl.h"

class CStockListCtrl : public CBaseListCtrl
{
public:
	CStockListCtrl(); 
	void UpdateItemCount();
protected:
    virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	void SortByCol(int Column,bool bAsc);
private :
	static const GRIDHEADERS szColHeaders[];
   DECLARE_MESSAGE_MAP()
};

#endif