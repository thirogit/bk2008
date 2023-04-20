#ifndef __DOCLISTCTRL_H__
#define __DOCLISTCTRL_H__

#include "DataTypes.h"
#include "BaseListCtrl.h"
#include "../textproviders/DocTextProvider.h"
#include "../textproviders/BaseArrayTextProvider.h"

class CDocListCtrl : public CBaseListCtrl
{
public:
	CDocListCtrl(CDocDataArray *docArray,int iConfigId = -1); 
	CDocListCtrl(CDocDataArray *docArray,const GRIDHEADERS *ppHeaders,int iConfigId = -1); 
	void UpdateItemCount();
protected:
	CDocDataArray *m_docArray;
	BaseArrayTextProvider<CDocDataArray,DocTextProvider> m_textProvider;	
	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);

	static GRIDHEADERS DocHeader[];
protected:
	
	//afx_msg void OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult );
	

	DECLARE_MESSAGE_MAP()

};




#endif