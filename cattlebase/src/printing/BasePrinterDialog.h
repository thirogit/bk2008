#ifndef __BASEPRINTERDIALOG_H__
#define __BASEPRINTERDIALOG_H__

#include "../imgbutton/ImageButton.h"
#include "printers/Printer.h"
#include "PrinterCache.h"

class CBasePrinterDialog : public CDialog
{
public:
	CBasePrinterDialog(UINT nNumOfCopies,CWnd* pParent = NULL);	
	enum { IDD = IDD_PRINTERDIALOG };
	bool PreparePrinting();
	CPrinter* GetPrinter();
	UINT GetNumberOfCopies() const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual bool GoodPrinter(const CPrinter* pPrinter) = 0;
protected:
	CFont m_bigFont;
	CEdit m_numberOfCopiesEdit;
	UINT  m_nNumberOfCopies;
	CPrinter* m_selectedPrinter;
	CListCtrl		m_PrintersList;
	CImageList		m_PrintersThumb;
	CImageButton	m_CancelBtn;
	CImageButton	m_PrintBtn;


private:
	static CPrintersCache m_printersCache;	

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	void SetNumberOfCopiesEdit(UINT nNumberOfCopies);
	UINT GetNumberOfCopiesEdit();
	void EnumPrinters();
	void AddPrinter(CPrinter* pPrinter);

	afx_msg void OnPlus();
	afx_msg void OnMinus();
	afx_msg void OnPrinterListRefresh();
	DECLARE_MESSAGE_MAP()
};

#endif 
