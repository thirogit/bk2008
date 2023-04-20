#include "stdafx.h"
#include "PrintPreview.h"
//#include "../classes/TempStringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
//CPrintersCache CBasePrinterDialog::m_printersCache;
//
//CBasePrinterDialog::CBasePrinterDialog(UINT nNumOfCopies,CWnd* pParent /*=NULL*/) : 
//	CDialog(CBasePrinterDialog::IDD, pParent),m_nNumberOfCopies(nNumOfCopies),
//		m_selectedPrinter(NULL)
//{	
//}
//
//void CBasePrinterDialog::DoDataExchange(CDataExchange* pDX)
//{	
//	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_PRINTERLIST, m_PrintersList);
//	DDX_Control(pDX, IDC_COPIESEDIT,m_numberOfCopiesEdit);
//
//}
//
//BEGIN_MESSAGE_MAP(CBasePrinterDialog, CDialog)
//	ON_BN_CLICKED(IDC_BTNPLUS,OnPlus)
//	ON_BN_CLICKED(IDC_BTNMINUS,OnMinus)
//	ON_BN_CLICKED(IDC_REFRESHPRINTERS,OnPrinterListRefresh)
//END_MESSAGE_MAP()
//
//
//void CBasePrinterDialog::SetNumberOfCopiesEdit(UINT nNumberOfCopies)
//{
//	char numberBuffer[10];
//	_snprintf_s(numberBuffer,sizeof(numberBuffer),"%d",nNumberOfCopies);
//	m_numberOfCopiesEdit.SetWindowText(numberBuffer);
//}
//UINT CBasePrinterDialog::GetNumberOfCopiesEdit()
//{
//	CString sNumOfCopies;
//	m_numberOfCopiesEdit.GetWindowText(sNumOfCopies);
//	return atoi(sNumOfCopies);
//}
//
//BOOL CBasePrinterDialog::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	LOGFONT logFont;
//	GetFont()->GetLogFont(&logFont);
//	logFont.lfHeight = -35;
//	m_bigFont.CreateFontIndirect(&logFont);
//	GetDlgItem(IDC_NUMBEROFCOPIESLABEL)->SetFont(&m_bigFont);
//	m_numberOfCopiesEdit.SetFont(&m_bigFont);
//	SetNumberOfCopiesEdit(m_nNumberOfCopies);
//
//	if (m_PrintersList.GetItemCount() != 0)
//		m_PrintersList.DeleteAllItems();
//
//	m_PrintersList.SetRedraw(FALSE);
//
//	m_PrintersThumb.DeleteImageList();
//	m_PrintersThumb.Create(48,48,ILC_COLOR8|ILC_MASK,0,1);
//
//	CBitmap printerConnectionTypeUSB;
//	printerConnectionTypeUSB.LoadBitmap(IDB_USB);
//	CBitmap printerConnectionTypeBT;
//	printerConnectionTypeBT.LoadBitmap(IDB_BLUETOOTH);
//
//	m_PrintersThumb.Add(&printerConnectionTypeUSB,RGB(255,0,255));
//	m_PrintersThumb.Add(&printerConnectionTypeBT,RGB(255,0,255));
//	
//	EnumPrinters();
//
//	if(m_PrintersList.GetItemCount() == 1)
//	{
//		m_PrintersList.SetItemState(0,LVIS_SELECTED | LVIS_FOCUSED,LVIS_SELECTED | LVIS_FOCUSED);
//	}
//
//	m_PrintersList.SetImageList(&m_PrintersThumb, LVSIL_NORMAL);
//	m_PrintersThumb.SetImageCount(2);
//
//	m_PrintersList.SetRedraw(TRUE);
//
//	return TRUE; 
//}
//
//void CBasePrinterDialog::OnOK() 
//{	
//	m_nNumberOfCopies = GetNumberOfCopiesEdit();
//	
//	POSITION pos = m_PrintersList.GetFirstSelectedItemPosition();
//	if (pos != NULL)
//	{
//		int iSelectedItem = m_PrintersList.GetNextSelectedItem(pos);
//		m_selectedPrinter = (CPrinter*)m_PrintersList.GetItemData(iSelectedItem);
//		CDialog::OnOK();
//	}
//	else
//	{
//		MessageBox(CTempStringLoader(IDS_PRINTERNOTSELECTED));
//	}
//	
//	
//}
//
//void CBasePrinterDialog::OnCancel() 
//{
//	CDialog::OnCancel();
//}
//
//void CBasePrinterDialog::OnPlus()
//{
//	SetNumberOfCopiesEdit(GetNumberOfCopiesEdit()+1);
//}
//void CBasePrinterDialog::OnMinus()
//{
//	UINT numOfCopies = GetNumberOfCopiesEdit();
//	if(numOfCopies > 1)
//		SetNumberOfCopiesEdit(numOfCopies-1);
//}
//
//bool CBasePrinterDialog::PreparePrinting()
//{
//	return DoModal() == IDOK;
//}
//
//UINT CBasePrinterDialog::GetNumberOfCopies() const
//{
//	return m_nNumberOfCopies;
//}
//
//CPrinter* CBasePrinterDialog::GetPrinter()
//{
//	return m_selectedPrinter;
//}
//
//void CBasePrinterDialog::EnumPrinters()
//{
//
//	m_PrintersList.DeleteAllItems();
//
//	if(m_printersCache.PrintersCount() == 0)
//	{
//		m_printersCache.Refresh();
//	}
//
//	PtrFlushArray<CPrinter>::ArrayIterator printerIt = m_printersCache.GetIterator();
//	while(printerIt.hasNext())
//	{
//		CPrinter* pPrinter = *(printerIt++);
//		if(GoodPrinter(pPrinter))
//		{
//			AddPrinter(pPrinter);
//		}
//	}
//}
//
//void CBasePrinterDialog::OnPrinterListRefresh()
//{
//	m_printersCache.Refresh();
//	EnumPrinters();
//}
//
//void CBasePrinterDialog::AddPrinter(CPrinter* pPrinter)
//{
//	int iItem = m_PrintersList.InsertItem(0,
//		pPrinter->GetPrinterName(),
//		pPrinter->GetConnectionType() == CPrinter::PRINTER_USB ? 0 : 1);
//	m_PrintersList.SetItemData(iItem,(DWORD)pPrinter);
//}
