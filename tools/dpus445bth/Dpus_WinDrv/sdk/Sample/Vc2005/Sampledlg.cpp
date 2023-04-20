// SampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sample.h"
#include "SampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleDlg dialog




CSampleDlg::CSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SET_DATA_BTN, OnPrintBtn)
	ON_BN_CLICKED(IDC_GET_DATA_BTN, OnGetInfoBtn)
	ON_BN_CLICKED(IDC_CB_STA_BTN, OnGetCallbackBtn)
	ON_CBN_SELCHANGE(IDC_LISTBOX, OnSelchangeListbox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



// CSampleDlg message handlers

BOOL CSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CreatePrinterList();

	TCHAR	lpTitle[1024] = {0};
	wsprintf( lpTitle, TEXT("Sample [%s]"), DLL_FILE_NAME );
	SetWindowText( lpTitle );
	OnSelchangeListbox(); 

	return TRUE;  // return TRUE  unless you set the focus to a control
}




// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSampleDlg::OnPrintBtn() 
{
	WriteSamp();
}

void CSampleDlg::OnGetInfoBtn() 
{
	ReadSamp();
}

void CSampleDlg::OnGetCallbackBtn() 
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CB_STA_BTN);
	CListBox* pList = (CListBox*)GetDlgItem(IDC_CB_LIST);

	if( pBtn->GetCheck() )
	{
		CallbackSamp( TRUE );
		pList->ResetContent(); 
		pList->EnableWindow( TRUE );
	}
	else
	{
		CallbackSamp( FALSE );
		pList->EnableWindow( FALSE );
	}
}


void CSampleDlg::OnSelchangeListbox() 
{
	TCHAR	lpPrinterName[MAX_PATH];
	CComboBox* pCombBox = (CComboBox*)GetDlgItem( IDC_LISTBOX );
	CButton* pCbBtn		= (CButton*)GetDlgItem( IDC_CB_STA_BTN );
	CButton* pGetdataBtn= (CButton*)GetDlgItem( IDC_GET_DATA_BTN );
	CButton* pSetdataBtn= (CButton*)GetDlgItem( IDC_SET_DATA_BTN );
	CListBox* pList = (CListBox*)GetDlgItem(IDC_CB_LIST);

	pCbBtn->EnableWindow(FALSE);
	pGetdataBtn->EnableWindow(FALSE);
	pSetdataBtn->EnableWindow(FALSE);
	pList->ResetContent(); 
	pCbBtn->SetCheck( FALSE );

	pCombBox->GetLBText( pCombBox->GetCurSel(), lpPrinterName );
	if( OpenSamp( lpPrinterName ) )
	{
		pList->SetWindowText( TEXT("OPEN ERROR") );
		pCbBtn->EnableWindow(TRUE);
		pGetdataBtn->EnableWindow(TRUE);
		pSetdataBtn->EnableWindow(TRUE);
	}
}



VOID CSampleDlg::CreatePrinterList()
{
	BYTE		*pPrinter;
 	DWORD		cbNeedPr, cbReturnPr;
	DWORD		dwIndex;
	CComboBox*	pCombBox = (CComboBox*)GetDlgItem( IDC_LISTBOX );

	//Get size of structure
 	EnumPrinters( PRINTER_ENUM_LOCAL, NULL,	2, NULL, 0, &cbNeedPr, &cbReturnPr );

	//Get printer info
 	pPrinter = (PBYTE) new BYTE[ cbNeedPr ];
	if( pPrinter )
	{
		if( EnumPrinters( PRINTER_ENUM_LOCAL, NULL, 2, pPrinter, cbNeedPr, &cbNeedPr, &cbReturnPr ) )
		{
			PRINTER_INFO_2* pPrinterInfo2 = (PRINTER_INFO_2 *) pPrinter;
			for( dwIndex=0 ; dwIndex < cbReturnPr; dwIndex++, pPrinterInfo2++ ) 
			{
				if( !_tcsnccmp( TEXT("SII"), pPrinterInfo2->pDriverName, lstrlen( TEXT("SII") ) ) )
				{
					pCombBox->AddString( pPrinterInfo2->pPrinterName );
					pCombBox->SetCurSel( 0 );
				}
 			}
		}
		delete [] pPrinter;
	}

	if( pCombBox->GetCount() == 0 )
	{
		AfxMessageBox( TEXT("Don't find the printer driver") );
	}	
	else
	{
		pCombBox->EnableWindow( TRUE );
	}
}
