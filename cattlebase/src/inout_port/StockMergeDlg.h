#ifndef __STOCKMERGEDLG_H__
#define __STOCKMERGEDLG_H__

#include "MergeDlg.h"
#include "../combo/StockCombo.h"

class CStockMergeDlg : public CMergeDlg
{
	DECLARE_DYNAMIC(CStockMergeDlg)

public:
	CStockMergeDlg(CWnd* pParent = NULL);  
	virtual ~CStockMergeDlg();	

	inline Stock* GetSelectedStock() { return m_selectedStock; };
private:

	CStockCombo m_stocks;
	Stock *m_selectedStock;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	void OnOK();

	DECLARE_MESSAGE_MAP()
};

#endif