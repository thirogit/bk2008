#ifndef __INVLISTCTRL_H__
#define __INVLISTCTRL_H__

#include "DataTypes.h"
#include "BaseListCtrl.h"
#include "../textproviders/InvoiceDataTextProvider.h"
#include "../textproviders/BaseArrayTextProvider.h"

class CInvListCtrl : public CBaseListCtrl
{
public:
	CInvListCtrl(CInvoiceDataArray *invArray,const GRIDHEADERS *ppHeaders,int iConfigId = -1); 
	void UpdateItemCount();
protected:
	CInvoiceDataArray *m_invArray;
	BaseArrayTextProvider<CInvoiceDataArray,InvoiceDataTextProvider> m_textProvider;

	
	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
protected:
	DECLARE_MESSAGE_MAP()

};




#endif