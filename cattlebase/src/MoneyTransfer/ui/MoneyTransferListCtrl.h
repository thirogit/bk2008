#ifndef __MONEYTRANSFERLISTCTRL_H__
#define __MONEYTRANSFERLISTCTRL_H__

#include "../../ctrls/BaseListCtrl.h"
#include "../../PtrFlushArray.h"
#include "../MoneyTransferEntry.h"
#include "MoneyTransferEntryTextProvider.h"

class CMoneyTransferListCtrl : public CBaseListCtrl
{
public:
	CMoneyTransferListCtrl(PtrFlushArray<MoneyTransferEntry>& moneyTransferEntries); 
	void UpdateItemCount();
protected:
    virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
	void SortByCol(int Column,bool bAsc);
private :
	static const GRIDHEADERS szColHeaders[];

	PtrFlushArray<MoneyTransferEntry>& m_moneyTransferEntries;
	MoneyTransferEntryTextProvider m_textProvider;

   DECLARE_MESSAGE_MAP()
};

#endif