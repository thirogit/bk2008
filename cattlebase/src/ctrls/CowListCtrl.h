#ifndef __COWLISTCTRL_H__
#define __COWLISTCTRL_H__

#include "DataTypes.h"
#include "BaseListCtrl.h"
#include "MarkWarnShowOptions.h"

class CCowListCtrl : public CBaseListCtrl
{
public:
	CCowListCtrl(CCowArray *cowArray,const GRIDHEADERS *ppHeaders,int iConfigId = -1); 
	void UpdateItemCount();
protected:
	CMarkWarnShowOptions *m_mws_opts;
	CCowArray *m_cowArray;
	virtual void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
protected:
	DECLARE_MESSAGE_MAP()

};

class CInCowListCtrl : public CCowListCtrl
{
public:
	CInCowListCtrl(CCowArray *cowArray,int iConfigId = -1) :
		CCowListCtrl(cowArray,InListCowHeader,iConfigId)
	{}; 
private:
	static GRIDHEADERS InListCowHeader[];
	
};

class COutCowListCtrl : public CCowListCtrl
{
public:
	COutCowListCtrl(CCowArray *cowArray,int iConfigId = -1);
	
private:
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
	COLORREF m_grpColors[MAX_INOUT_GRP+1];
	static GRIDHEADERS OutListCowHeader[];
};



#endif