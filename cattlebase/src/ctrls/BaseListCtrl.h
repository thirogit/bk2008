#ifndef __BASELISTCTRL_H__
#define __BASELISTCTRL_H__

#include "../grids/GridHeaders.h"

class CBaseListCtrl : public CListCtrl
{
public:
	CBaseListCtrl(const GRIDHEADERS *ppHeaders, int pConfigItemId = -1);
	CBaseListCtrl(const GRIDHEADERS *ppHeaders, int iiHeaderCount,int pConfigItemId);
	virtual void UpdateItemCount() = 0;
	virtual ~CBaseListCtrl();
	int GetCurSel();
	int GetColumnCount();
	UINT GetColID(int nCol);
	void Lock();
	void Unlock();
protected:
	const GRIDHEADERS *pHeaders;
	int iHeaderCount;
	int ConfigItemId;
protected:
	virtual void SetListCols();
	virtual void PostCreate();
	void PreSubclassWindow();
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText) = 0;
	bool GetCellRect(int Item, int Column,CRect &cellRect,bool bScroll = true,int iMIN_COL_WIDTH = MAXLONG);
	virtual void OnColClick(int nCol);
	virtual void SortByCol(int Column,bool bAsc) {};
	virtual bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);
	virtual CString GetColText(UINT nColID) { ASSERT(FALSE); return CString(); };
	virtual int GetItemImage(int iItem,int Column);


protected:
	bool *ColSortAsc;

protected:
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult );

	DECLARE_MESSAGE_MAP()
};

#endif