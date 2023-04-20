#ifndef __NEWSTOCKDLG_H__
#define __NEWSTOCKDLG_H__

#include "BaseStockDlg.h"

class CNewStockDlg : public CBaseStockDlg
{
public:
	CNewStockDlg(Stock* pStock,CWnd* pParent);
protected:
	virtual bool CommitStock();
	virtual void CancelStock();
	Stock* CreateNewStock();
	virtual CString GetDlgCaption();

	Stock* m_pStock;
};

#endif