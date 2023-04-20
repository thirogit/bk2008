#ifndef __EDITSTOCKDLG_H__
#define __EDITSTOCKDLG_H__

#include "BaseStockDlg.h"

class CEditStockDlg : public CBaseStockDlg
{
public:
	CEditStockDlg(Stock* pStock,CWnd* pParent);
protected:
	virtual BOOL OnInitDialog();
	virtual bool CommitStock();
	virtual void CancelStock();
	void UpdateStock(Stock* pStock);
	virtual CString GetDlgCaption();

	Stock* m_pStock;
};

#endif