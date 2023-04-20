#ifndef __PRINTERTYPECHOICE_H__
#define __PRINTERTYPECHOICE_H__

#include "../imgbutton/ImageButton.h"

#define PRINT_NONE ((UINT)0)

#define PRINT_DYMO ((UINT)1)
#define PRINT_SEIKO ((UINT)2)
#define PRINT_WINPRINT ((UINT)4)

#define PRINT_MASK_ALL ((UINT)-1)

class CPrinterChoiceDlg : public CDialog
{
public:
	static UINT ChoosePrinterType(UINT uPrintTypeMask,CWnd *pParent = NULL); 
protected:
	CPrinterChoiceDlg(UINT uPrintTypeMask,CWnd* pParent = NULL);	
	enum { IDD = IDD_PRINTERTYPECHOICE };

	struct PRINTER_TYPE_INFO
	{
		UINT uResourceBmp;
		UINT uResourceStr;
		UINT uPrinterType;

	};

	UINT GetSelectedPrinterType();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
protected:
	CListCtrl	m_PrinterChoiceList;
	CImageList	m_ImageListThumb;
	CImageButton  m_CancelBtn;
	
	int			m_nSelectedItem;
	UINT		m_selectedPrinterType;
	UINT		m_uPrintTypeMask;

	void		Selected();
	void		AddPrinterItem(PRINTER_TYPE_INFO* pPrinterTypeInfo);

	virtual BOOL OnInitDialog();
	virtual void OnOK();
		
	afx_msg void OnClickListThumb(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListThumb(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()

	static const PRINTER_TYPE_INFO _printerTypes[];
};

#endif 
