#ifndef __AGENTLISTCTRL_H__
#define __AGENTLISTCTRL_H__

#include "BaseListCtrl.h"

class CAgentListCtrl : public CBaseListCtrl
{
public:
	CAgentListCtrl(); 
	void UpdateItemCount();
protected:
    virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	void SortByCol(int Column,bool bAsc);
private :
	static const GRIDHEADERS szColHeaders[];
   DECLARE_MESSAGE_MAP()
};

#endif