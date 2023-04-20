#include "stdafx.h"
#include "PrinterTypeChoice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	THUMBNAIL_WIDTH		140
#define	THUMBNAIL_HEIGHT	80

const CPrinterChoiceDlg::PRINTER_TYPE_INFO CPrinterChoiceDlg::_printerTypes[] =
{
	{IDB_DYMOPRINTER,IDS_DYMO,PRINT_DYMO},
	{IDB_SEIKOPRINTER,IDS_SEIKO,PRINT_SEIKO},
	{IDB_WINPRINTER,IDS_WINPRINT,PRINT_WINPRINT},
	{0,0,0}
};


UINT CPrinterChoiceDlg::ChoosePrinterType(UINT uPrintTypeMask,CWnd *pParent/* = NULL*/)
{
	CPrinterChoiceDlg printerChoiceDlg(uPrintTypeMask,pParent);
	if(printerChoiceDlg.DoModal() == IDOK)
	{
		return printerChoiceDlg.GetSelectedPrinterType();
	}
	return PRINT_NONE;
}


CPrinterChoiceDlg::CPrinterChoiceDlg(UINT uPrintTypeMask,CWnd* pParent /*=NULL*/)
	: CDialog(CPrinterChoiceDlg::IDD, pParent),m_uPrintTypeMask(uPrintTypeMask),m_selectedPrinterType(PRINT_NONE)
{
}

void CPrinterChoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRINTDOCTYPESLIST, m_PrinterChoiceList);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
}

BEGIN_MESSAGE_MAP(CPrinterChoiceDlg, CDialog)
	
	ON_NOTIFY(NM_CLICK, IDC_PRINTDOCTYPESLIST, OnClickListThumb)
	ON_NOTIFY(LVN_KEYDOWN, IDC_PRINTDOCTYPESLIST, OnKeydownListThumb)
END_MESSAGE_MAP()


BOOL CPrinterChoiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CancelBtn.SetButtonImage(IDB_REDX);

	if (m_PrinterChoiceList.GetItemCount() != 0)
		m_PrinterChoiceList.DeleteAllItems();

	m_PrinterChoiceList.SetRedraw(FALSE);

	m_ImageListThumb.DeleteImageList();
	m_ImageListThumb.Create(THUMBNAIL_WIDTH,THUMBNAIL_HEIGHT,ILC_COLOR8|ILC_MASK,0,1);
	m_ImageListThumb.SetBkColor(RGB(255,0,255));
	
	int iImgIndex = 0;
	const PRINTER_TYPE_INFO* pPrinterType = _printerTypes;
	while(pPrinterType->uPrinterType != 0)
	{
		if(m_uPrintTypeMask & pPrinterType->uPrinterType)
		{
			CBitmap printerBmp;
			printerBmp.LoadBitmap(pPrinterType->uResourceBmp);
			CString printerText;
			printerText.LoadString(pPrinterType->uResourceStr);
			m_ImageListThumb.Add(&printerBmp,RGB(255,0,255));

			int iItem = m_PrinterChoiceList.InsertItem(0,printerText, iImgIndex);
			m_PrinterChoiceList.SetItemData(iItem,(DWORD)pPrinterType);
			iImgIndex++;
		}
		pPrinterType++;
	}
	
	m_PrinterChoiceList.SetImageList(&m_ImageListThumb, LVSIL_NORMAL);
	m_nSelectedItem = -1;
	m_selectedPrinterType = PRINT_NONE;
	
	m_ImageListThumb.SetImageCount(iImgIndex);

	m_PrinterChoiceList.SetRedraw(TRUE);
	
	return TRUE;  
}


void CPrinterChoiceDlg::OnClickListThumb(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	// retrieve message info.
	LPNMITEMACTIVATE pItemAct = (LPNMITEMACTIVATE)pNMHDR;

	// determine which item receives the click
	LVHITTESTINFO  hitTest;
	ZeroMemory(&hitTest, sizeof(LVHITTESTINFO));
	hitTest.pt = pItemAct->ptAction;
	m_PrinterChoiceList.SendMessage(LVM_SUBITEMHITTEST, 0, (LPARAM)&hitTest);

	// draw the selected image
	if (hitTest.iItem != m_nSelectedItem && hitTest.iItem >= 0)
	{
		m_nSelectedItem = hitTest.iItem;
		Selected();
	}

	// select the item clicked
	*pResult = 0;
}

void CPrinterChoiceDlg::OnKeydownListThumb(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	if (pLVKeyDow->wVKey == VK_LEFT || pLVKeyDow->wVKey == VK_RIGHT)
	{	
		// get total number of items in the list
		int nTotal = m_PrinterChoiceList.GetItemCount();

		// rule out the situation of an empty list
		if (nTotal == 0)
		{
			*pResult = 1; // current record stays selected
			return;
		}
    
		// reset selected item index
		int  nNewItem = -1;

		// set the selection
		if (pLVKeyDow->wVKey == VK_LEFT)	// left arrow
		{
			if (m_nSelectedItem > 0)
				nNewItem = m_nSelectedItem - 1;
		}
		else	// right arrow
		{
			if (m_nSelectedItem < nTotal-1)
				nNewItem = m_nSelectedItem + 1;
		}

		// update the selection
		if (nNewItem != -1)
		{
			// update the selected item index
			m_nSelectedItem = nNewItem;
			
		}	 
	}
	
	// high-light the selected item
	*pResult = 0;
}

void  CPrinterChoiceDlg::Selected()
{
	PRINTER_TYPE_INFO* pPrinterType = (PRINTER_TYPE_INFO*)m_PrinterChoiceList.GetItemData(m_nSelectedItem);
	m_selectedPrinterType = pPrinterType->uPrinterType;
	EndDialog(IDOK);
}

void CPrinterChoiceDlg::OnOK()
{
	if(m_nSelectedItem != -1)
		Selected();
}

UINT CPrinterChoiceDlg::GetSelectedPrinterType()
{
	return m_selectedPrinterType;
}


