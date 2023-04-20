#ifndef __REJECTEDCOWSLISTCTRL_H__
#define __REJECTEDCOWSLISTCTRL_H__

#include "DataTypes.h"
#include "../ctrls/BaseListCtrl.h"
#include "SyncHelper.h"

class CRejectedCowsListCtrl : public CBaseListCtrl
{
public:
	CRejectedCowsListCtrl(CSyncCowDataArray *rejCowArray,int iConfigId = -1); 
	void UpdateItemCount();
	enum REJCOW_LISTCTRL_COLS
	{
		REJCOW_EAN,
		REJCOW_STOCK,
		REJCOW_WEIGHT,
		REJCOW_SEX,
		REJCOW_MYPRICE,
		REJCOW_DOCIN,
		REJCOW_DOCOUT,
		REJCOW_TERMBUYSTOCK,
		REJCOW_TERMBUYWEIGHT,
		REJCOW_TERMBUYPRICE,
		REJCOW_TERMSELLSTOCK,
		REJCOW_TERMSELLWEIGHT,
		REJCOW_TERMSELLPRICE
	};
protected:
	CSyncCowDataArray *m_rejCowArray;	
	//void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
private:

	

	static const GRIDHEADERS RejectedCowHeader[];
protected:

	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	

	DECLARE_MESSAGE_MAP()

};




#endif