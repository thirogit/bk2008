#ifndef __REJECTEDDOCSLISTCTRL_H__
#define __REJECTEDDOCSLISTCTRL_H__

#include "DataTypes.h"
#include "../ctrls/BaseListCtrl.h"
#include "SyncHelper.h"


class CRejectedDocsListCtrl : public CBaseListCtrl
{
public:
	CRejectedDocsListCtrl(CSyncDocDataArray *rejectedDocArray,const GRIDHEADERS *ppHeaders,int iConfigId = -1); 
	void UpdateItemCount();
	enum REJDOC_LISTCTRL_COLS
	{
		REJDOC_DOCNO,
		REJDOC_DOCDATE,
		REJDOC_LOADDATE,
		REJDOC_HENT,
		REJDOC_PLATENO,
		REJDOC_REASON,
		REJDOC_NOCOWS
	};
protected:
	CSyncDocDataArray *m_rejDocArray;
	
	//void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
protected:

	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	
	
	DECLARE_MESSAGE_MAP()

};





#endif