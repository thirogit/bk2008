#include "stdafx.h"
#include "MoneyTransferDlg.h"
#include "../../consts.h"
#include "../../global_arrays.h"
#include "../../Utils/StringFormat.h"
#include "../../missing/util.h"
#include "../../taskprogress/TaskProgressDlg.h"
#include "../../classes/TempStringLoader.h"
#include "../../CattleSQL.h"
#include "../formatters/ElixirFormatter.h"
#include "../formatters/KBFormatter.h"
#include "../formatters/PolbankFormatter.h"
#include "../formatters/BZWBKFormatter.h"
#include "../formatters/AliorFormatter.h"
#include "../formatters/PBSFormatter.h"
#include "../formatters/PekaoFormatter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMoneyTransferDlg::CMoneyTransferDlg(CInvoiceDataArray& invoices,CWnd* pParent /*=NULL*/)
	: CDialog(CMoneyTransferDlg::IDD, pParent),
	m_moneyTransferList(m_moneyTransferEntries),
	m_ExchangeRateEdit(g_szRegExUnsigned)
{

	m_exchangeRate.SetCountry(ary_countries.GetCountryByCountryNum(ISO_POL_NUMBER));
	m_exchangeRate.SetExchangeRate(1.0);

	MoneyTransferEntry* pEntry = NULL;

	CInvoiceDataArray::ArrayIterator it = invoices.iterator();
	while(it.hasNext())
	{
		m_moneyTransferEntries.Add(new MoneyTransferEntry(*(it++),&m_exchangeRate));
	}

	m_moneyTransferFormatters.Add(new KBFormatter());
	m_moneyTransferFormatters.Add(new ElixirFormatter());
	m_moneyTransferFormatters.Add(new PolbankFormatter());
	m_moneyTransferFormatters.Add(new BZWBKFormatter());
	m_moneyTransferFormatters.Add(new AliorFormatter());
	m_moneyTransferFormatters.Add(new PBSFormatter());
	m_moneyTransferFormatters.Add(new PekaoFormatter());
}


CMoneyTransferDlg::~CMoneyTransferDlg()
{	
	m_moneyTransferEntries.FreeAndFlush();
	m_moneyTransferFormatters.FreeAndFlush();
}

void CMoneyTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_FORMATSELECTCOMBO,m_moneyTransferFormatsCb);
	DDX_Control(pDX,IDC_CURRENCYCOMBO,m_CurrenciesCb);
	DDX_Control(pDX,IDC_EXCHANGERATE,m_ExchangeRateEdit);
	DDX_Control(pDX,IDC_MONEYTRANSERLIST,m_moneyTransferList);
	DDX_Control(pDX,IDC_TRANSFERDATE,m_TransferDate);
}


BEGIN_MESSAGE_MAP(CMoneyTransferDlg, CDialog)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_EXCHANGERATE,OnExchangeRate)
	ON_BN_CLICKED(IDC_DELETEENTRY,OnDeleteMoneyTransferEntry)
	ON_CBN_SELCHANGE(IDC_CURRENCYCOMBO, OnCurrencyChange)
END_MESSAGE_MAP()


BOOL CMoneyTransferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	m_moneyTransferList.UpdateItemCount();
	m_CurrenciesCb.InitCurrencyCombo(m_exchangeRate.GetCountry()->GetCountryISONumber());
	m_ExchangeRateEdit.SetCommitedText(CStringFormat("%.4f",m_exchangeRate.GetExchangeRate()));
	Update();
	InitMoneyTransferFormatsCombo();
	
	return TRUE;
}

void CMoneyTransferDlg::InitMoneyTransferFormatsCombo()
{
	int iItem = -1;
	Formatter* pFormatter = NULL;
	PtrFlushArray<Formatter>::ArrayIterator formatterIt = m_moneyTransferFormatters.iterator();
	while(formatterIt.hasNext())
	{
		pFormatter = *(formatterIt++);		
		iItem = m_moneyTransferFormatsCb.AddString(pFormatter->GetFormatterName());
		m_moneyTransferFormatsCb.SetItemData(iItem,(DWORD_PTR)pFormatter);
	}

}

void CMoneyTransferDlg::OnDeleteMoneyTransferEntry()
{
	m_moneyTransferList.Lock();

	int iSelectedItem = m_moneyTransferList.GetCurSel();
	if(iSelectedItem >= 0)
	{
		MoneyTransferEntry* pEntryToDelete = m_moneyTransferEntries.GetAt(iSelectedItem);
		m_moneyTransferEntries.RemoveAt(iSelectedItem);
		delete pEntryToDelete;
	}

	m_moneyTransferList.UpdateItemCount();
	m_moneyTransferList.Unlock();
	Update();
}

void CMoneyTransferDlg::Update()
{
	CheckIfOK();
	SetTotal();
}

void CMoneyTransferDlg::OnExchangeRate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString sExchangeRate;
	m_ExchangeRateEdit.GetWindowText(sExchangeRate);
	m_exchangeRate.SetExchangeRate(CUtil::myatof(sExchangeRate));
	m_moneyTransferList.Invalidate();
	*pResult = (LRESULT)0;
}

void CMoneyTransferDlg::CheckIfOK()
{
	MoneyTransferEntry* pEntry = NULL;
	PtrFlushArray<MoneyTransferEntry>::ArrayIterator it = m_moneyTransferEntries.iterator();
	while(it.hasNext())
	{
		pEntry = *(it++);
		if(pEntry->IsInvalid())
		{
			GetDlgItem(IDOK)->EnableWindow(FALSE);
			return;
		}		
	}

	GetDlgItem(IDOK)->EnableWindow(m_moneyTransferEntries.GetCount() > 0);
}

void CMoneyTransferDlg::SetTotal()
{
	MoneyTransferEntry* pEntry = NULL;
	PtrFlushArray<MoneyTransferEntry>::ArrayIterator it = m_moneyTransferEntries.iterator();
	Money total(0.0);
	while(it.hasNext())
	{
		pEntry = *(it++);
		total += pEntry->GetTransferValue();			
	}

	SetDlgItemText(IDC_TOTAL,total.ToString());
}

void CMoneyTransferDlg::OnOK()
{

	int iFormatSelected = m_moneyTransferFormatsCb.GetCurSel();
	if(iFormatSelected < 0)
	{
		AfxMessageBox(IDS_SELECTMONEYTRANSFERFORMAT);
		m_moneyTransferFormatsCb.SetFocus();
		return;
	}

	if(!m_TransferDate.IsInputEmpty() && !m_TransferDate.IsDateValid())
	{
		AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
		m_TransferDate.SetFocus();
		return;
	}

	Formatter* pFormatter = (Formatter*)m_moneyTransferFormatsCb.GetItemData(iFormatSelected);

	CString extension(pFormatter->GetFileExtension()),fileFilter;
	fileFilter.Format(IDS_SINGLE_FILEFILTER,(LPCSTR)extension,(LPCSTR)extension,(LPCSTR)extension);
	CFileDialog saveFileDlg(FALSE,(LPCSTR)extension,NULL,OFN_OVERWRITEPROMPT,(LPCSTR)fileFilter,this);

	if(saveFileDlg.DoModal() == IDOK)
	{
		CString outputFilePathName = saveFileDlg.GetPathName();
		MoneyTransferContext context;

		CDateTime transferDate;
		m_TransferDate.GetDate(transferDate);

		if(transferDate.IsNull())
			transferDate = CDateTime::now();

		context.SetTransferDate(transferDate);

		CFile moneyTransferFile;
		CTaskProgressDialog taskDlg(CTempStringLoader(IDS_CREATINGMONEYTRANSFERFILE),NULL);		

		taskDlg.Create();

		if(moneyTransferFile.Open((LPCSTR)outputFilePathName,CFile::modeCreate | CFile::modeWrite))
		{
			pFormatter->FormatMoneyTransfers(m_moneyTransferEntries,&context,moneyTransferFile,&taskDlg);
			if(taskDlg.CheckCancelButton())
			{
				moneyTransferFile.Close();
				::DeleteFile((LPCSTR)outputFilePathName);
				return;
			}

			if(AfxMessageBox(IDS_ASKMARKPAID,MB_YESNO) == IDYES)
			{
				if(!SetPaidDate(m_moneyTransferEntries))
				{
					AfxMessageBox(IDS_FAILEDTOMARKPAIDARRAY);
				}
			}
		}
	}
	EndDialog(IDOK);
}

bool CMoneyTransferDlg::SetPaidDate(PtrFlushArray<MoneyTransferEntry>& moneyTransfers)
{
	CInvoiceDataArray invoices;
	PtrFlushArray<MoneyTransferEntry>::ArrayIterator moneyTransferIt = moneyTransfers.iterator();
	while(moneyTransferIt.hasNext())
		invoices.Add((*(moneyTransferIt++))->GetInvoice());

	Invoice* pInv = NULL;
	CInvoiceDataArray::ArrayIterator invIt = invoices.iterator();
	while(invIt.hasNext())
	{
		pInv = *(invIt++);		
		pInv->ChangePaidDate(CDateTime::now());
		if(!pInv->UpdateThisInvoice(false))
		{
			SQLDB.Rollback();
			SQLDB.SQLMessageBox(IDS_ERRFAILTOMARKPAID);
			invoices.RollbackChanges();
			
			
			return false;
		}
	}	
	SQLDB.Commit();
	invoices.CommitChanges();
	return true;
}

void CMoneyTransferDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}


void CMoneyTransferDlg::OnCurrencyChange()
{
	
	int currCurrency = m_CurrenciesCb.GetCurSel();
	Country *cc = (Country*)m_CurrenciesCb.GetItemData(currCurrency);
	if(cc)
	{
		m_exchangeRate.SetCountry(cc);
		m_moneyTransferList.Invalidate();
	}
	
}