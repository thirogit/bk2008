#ifndef __COWSTOCKSTATDLG_H__
#define __COWSTOCKSTATDLG_H__

#include "../ctrls/CowStockStatList.h"

class CCowStockStatDlg : public CDialog
{
	DECLARE_DYNAMIC(CCowStockStatDlg)

public:
	CCowStockStatDlg(CCowArray &arrayCows,CWnd* pParent = NULL);   
	virtual ~CCowStockStatDlg();
	enum { IDD = IDD_COWSTOCKSTATDLG };
private:
	CCowStockStatListCtrl m_cowStockStat;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

#endif