#ifndef __GRIDVIEW_H__
#define __GRIDVIEW_H__

#include "popmenunode.h"
#include "myruntimeclass.h"
#include "../DataArray.h"
#include "GridHeaders.h"
#include "GridHeaderCtrl.h"

#define ODDCOLOR RGB(213,255,213)
#define EVENCOLOR RGB(0xFF,0xFF,0xFF) 
#define MAX_TOOLTIP_WIDTH 500

class CGrid : public CListView
{
public:
	
    CGrid(const GRIDHEADERS *ppHeaders, int pConfigItemId); 
	virtual ~CGrid();
	virtual void OnInitialUpdate(); 
	
	
	int GetCurSel(void);
	void SelectAll();
	
	
	void InvertSelection();	
	
	void SelectRow(int nRow);
	
	void SetCurPos(int nRow);

	int GetItemCount() const;

	
	int GetMaxFontSize() const;
	int GetMinFontSize() const;
	int GetFontHeight() const;
	void SetFontHeight(int iHeight);

	
protected:
	virtual void GetData(int iItem,int nCol,LPSTR pszText,LPVOID pData) = 0;
	virtual bool GetCellColor(int iItem,int nCol,COLORREF &cr) { return false; };
	void SortByCol(int nCol,bool bAsc);
	virtual BOOL PreCreateWindow(CREATESTRUCT &cs);	
	virtual BOOL GetToolTipText(int iItem,int iSubItem,CString &tooltipText) { return FALSE; };
	int OnToolHitTest(CPoint point, TOOLINFO * pTI) const ;
	UINT GetColID(int nCol);
	void RepaintSortArrows();
	virtual void GridSortBy(DataMemberSortParamsArray &sortList) = 0;
	virtual void OnRightClick(int iItem,int iSubItem) = 0;
	virtual void PostInit() {};
	virtual void OnDoubleClick(int iItem,int iSubItem) {};
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL);

	
private:   

	const GRIDHEADERS *pHeaders;
	int iHeaderCount;
	int ConfigItemId;
	bool *ColSortAsc;

	BOOL m_bToolTipCtrlCustomizeDone;
protected:
	
	DataMemberSortParamsArray m_sortCols;
	bool m_shiftPressLastSort;
	CImageList m_sortImageList;
	CGridHeaderCtrl m_headerCtrl;

	CFont* m_pGridFont;
	int m_FontHeight;
private:
	afx_msg void GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult );
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDbClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg void OnLvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);
	
	DECLARE_MESSAGE_MAP()
};

#endif