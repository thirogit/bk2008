#include "stdafx.h"
#include "DPU-S445.h"
#include "DPU-S445Dlg.h"
#include "afxdialogex.h"
#include "WinSpool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEVICE_BTH_NAME_PREFIX "DPU-S445-"
#define MAXPINLENGTH 4
#define MAXBTHDEVNAMESUFFIX 8  


CDPUS445Dlg::CDPUS445Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDPUS445Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bCancelPIN = FALSE;
	m_dwPrinterSessionId = 0;
}

void CDPUS445Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PRINTERCOMBO,m_printerCombo);
	DDX_Check(pDX,IDC_CANCELPINCHECK,m_bCancelPIN);
}

BEGIN_MESSAGE_MAP(CDPUS445Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SETPINBTN, &CDPUS445Dlg::OnBnClickedSetPINBtn)
	ON_BN_CLICKED(IDC_SETBTNNAMEBTN, &CDPUS445Dlg::OnBnClickedSetBtnNameBtn)
	ON_BN_CLICKED(IDC_CANCELPINCHECK, &CDPUS445Dlg::OnBnClickedCancelPINCheck)
	ON_BN_CLICKED(IDC_ACTIONPRINTERBTN, &CDPUS445Dlg::OnBnClickedActionPrinterBtn)
END_MESSAGE_MAP()


// CDPUS445Dlg message handlers

BOOL CDPUS445Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString btnCaption;
	btnCaption.LoadString(IDS_OPENPRINTER);
	SetDlgItemText(IDC_ACTIONPRINTERBTN,(LPCSTR)btnCaption);


	SetDlgItemText(IDC_FIXEDBTNNAMEPREFIX,DEVICE_BTH_NAME_PREFIX);
	SendDlgItemMessage(IDC_SETBTNNAMEBTN,EM_LIMITTEXT,MAXBTHDEVNAMESUFFIX);
	SendDlgItemMessage(IDC_BTHPIN,EM_LIMITTEXT,MAXPINLENGTH);

	EnableControls(false);

	if(m_Driver.LoadDriver())
	{	
		InitPrintersCombo();		
	}
	else
	{
		m_printerCombo.EnableWindow(false);
		GetDlgItem(IDC_ACTIONPRINTERBTN)->EnableWindow(false);
	}	

	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDPUS445Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDPUS445Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDPUS445Dlg::OnBnClickedSetPINBtn()
{	
	UpdateData();

	if(m_bCancelPIN)
	{
		BYTE btCancelBtPIN[3] = {0x12,0x69,0x36};
		m_Driver.SendToPrinter(m_dwPrinterSessionId,btCancelBtPIN,sizeof(btCancelBtPIN));
		AfxMessageBox(IDS_DONEPLEASERESTARTPRINTER);
	}
	else
	{
		
		CString sPIN;
		GetDlgItemText(IDC_BTHPIN,sPIN);

		if(sPIN.GetLength() != 4)
		{
			AfxMessageBox(IDS_PINHASTOBE4DIGITSINLENGTH);
			return;
		}
		else
		{
			BYTE btSetBtPINCmdPrefix[3] = {0x12,0x69,0x38};
			UINT uiBufferSize = sizeof(btSetBtPINCmdPrefix) + 1 + sPIN.GetLength();

			BYTE*  pCmdBuf = new BYTE[uiBufferSize];
			BYTE* pCmd = pCmdBuf;

			memcpy(pCmd,btSetBtPINCmdPrefix,sizeof(btSetBtPINCmdPrefix));
			pCmd += sizeof(btSetBtPINCmdPrefix);
			memcpy(pCmd,(LPCSTR)sPIN,sPIN.GetLength());
			pCmd += sPIN.GetLength();
			*pCmd = 0;
			pCmd += 1;

			m_Driver.SendToPrinter(m_dwPrinterSessionId,pCmdBuf,uiBufferSize);	
			AfxMessageBox(IDS_DONEPLEASERESTARTPRINTER);

		}
	}
}


void CDPUS445Dlg::OnBnClickedSetBtnNameBtn()
{
	CString sBthNameSufix;
	GetDlgItemText(IDC_BTHNAMESUFFIX,sBthNameSufix);

	char ch;
	for(int c = 0;c < sBthNameSufix.GetLength();c++)
	{
		ch = sBthNameSufix.GetAt(c);
		if(!isgraph(ch) && !isdigit(ch))
		{
			AfxMessageBox(IDS_INVALIDCHARINBTHNAME);
			return;
		}
	}

	if(sBthNameSufix.GetLength() < 4)
	{
		AfxMessageBox(IDS_BTHNAMEHASTOBEATLEAST4CHARSINLENGTH);
		return;
	}
	else
	{
		BYTE btDevNameChangeCmdPrefix[3] = {0x12,0x69,0x35};
		CString sBthNamePrefix;
		GetDlgItemText(IDC_FIXEDBTNNAMEPREFIX,sBthNamePrefix);

		CString sBthName = sBthNamePrefix + sBthNameSufix;

		UINT uiBufferSize = sizeof(btDevNameChangeCmdPrefix) + 1 + sBthName.GetLength();

		BYTE*  pCmdBuf = new BYTE[uiBufferSize];
		BYTE* pCmd = pCmdBuf;

		memcpy(pCmd,btDevNameChangeCmdPrefix,sizeof(btDevNameChangeCmdPrefix));
		pCmd += sizeof(btDevNameChangeCmdPrefix);
		memcpy(pCmd,(LPCSTR)sBthName,sBthName.GetLength());
		pCmd += sBthName.GetLength();
		*pCmd = 0;
		pCmd += 1;

		m_Driver.SendToPrinter(m_dwPrinterSessionId,pCmdBuf,uiBufferSize);

		AfxMessageBox(IDS_DONEPLEASERESTARTPRINTER);
	}
}


void CDPUS445Dlg::OnBnClickedCancelPINCheck()
{
	UpdateData();
	GetDlgItem(IDC_BTHPIN)->EnableWindow(!m_bCancelPIN);
}


bool CDPUS445Dlg::IsSIIPrinter(LPCSTR lpszDriverName) const
{
	const char* szSIIPRinterDriverNamePrefix = "SII";

	return _tcsnccmp( szSIIPRinterDriverNamePrefix, lpszDriverName, lstrlen(szSIIPRinterDriverNamePrefix ) ) == 0; 
}

void CDPUS445Dlg::InitPrintersCombo()
{
	BYTE		*pPrinter;
 	DWORD		cbNeedPr, cbReturnPr;
	DWORD		dwIndex;
	m_printerCombo.ResetContent();

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
				if( IsSIIPrinter(pPrinterInfo2->pDriverName) )
				{
					m_printerCombo.AddString( pPrinterInfo2->pPrinterName );				
				}
 			}
		}
		delete [] pPrinter;
	}
	
}



void CDPUS445Dlg::OnBnClickedActionPrinterBtn()
{
	if(m_dwPrinterSessionId == 0)
	{
		int iSelectedPrinterIndex = m_printerCombo.GetCurSel();
		if(iSelectedPrinterIndex >= 0)
		{
			CString sPrinterName;
			m_printerCombo.GetLBText(iSelectedPrinterIndex,sPrinterName);	
			m_dwPrinterSessionId = m_Driver.OpenPrinterSession(sPrinterName);
			if(m_dwPrinterSessionId)
			{
				CString btnCaption;
				btnCaption.LoadString(IDS_CLOSEPRINTER);
				SetDlgItemText(IDC_ACTIONPRINTERBTN,(LPCSTR)btnCaption);
				EnableControls(true);
			}
		}
	}
	else
	{
		CString btnCaption;
		btnCaption.LoadString(IDS_OPENPRINTER);
		SetDlgItemText(IDC_ACTIONPRINTERBTN,(LPCSTR)btnCaption);
		EnableControls(false);

	}	
}

void CDPUS445Dlg::EnableControls(bool bEnable)
{
	UINT ControlIds[] = 
	{
		IDC_FIXEDBTNNAMEPREFIX,
		IDC_BTHNAMESUFFIX,
		IDC_SETBTNNAMEBTN,
		IDC_BTHPIN,
		IDC_CANCELPINCHECK,
		IDC_SETPINBTN,
		0
	};

	UINT* pControlId = ControlIds;
	while(*pControlId != 0)
	{
		GetDlgItem(*pControlId)->EnableWindow(bEnable);
		pControlId++;
	}
}
