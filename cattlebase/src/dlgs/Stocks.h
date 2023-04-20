#ifndef __STOCKS_H__
#define __STOCKS_H__

#include "..\imgbutton\ImageButton.h"
#include "..\ctrls\stocklistctrl\StockListCtrl.h"
#include "../datatypes/Stock.h"

class CStocksDlg : public  CDialog 
{
public:
	CStocksDlg (CWnd* pParent = NULL);
	static Stock* AddNewStock(CWnd* pParent);
	enum { IDD = IDD_LISTEDITDLG };
protected:
	CImageButton  m_StockOK;
	CImageButton  m_StockEdit;
	CImageButton  m_StockAdd;
	CImageButton  m_StockDelete;
	CStockListCtrl  m_StockList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	static bool IsDuplicate(const CString &sStockCode);
protected:
	afx_msg void OnAddNewStock();
	afx_msg void OnDelStock();
	afx_msg void OnEditStock();
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
};
#endif