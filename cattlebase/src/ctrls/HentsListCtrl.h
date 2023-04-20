#ifndef __HENTSLISTCTRL_H__
#define __HENTSLISTCTRL_H__

#include "DataTypes.h"
#include "BaseListCtrl.h"
#include "../datatypes/HentArray.h"
#include "../textproviders/HentTextProvider.h"
#include "../textproviders/BaseArrayTextProvider.h"

class CHentsListCtrl : public CBaseListCtrl
{
public:
	CHentsListCtrl(CHentArray *pHentArray,
						const GRIDHEADERS *ppHeaders,
						int iConfigId = -1); 
	void UpdateItemCount();
protected:
	CHentArray *m_pHentArray;
	BaseArrayTextProvider<CHentArray,HentTextProvider> m_textProvider;	
	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
protected:
	DECLARE_MESSAGE_MAP()

};




#endif