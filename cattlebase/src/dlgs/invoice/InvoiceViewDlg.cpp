#include "stdafx.h"
#include "InvoiceViewDlg.h"
#include "../PassSearchDlg.h"

IMPLEMENT_DYNAMIC(CInvoiceViewDlg, CDialog)
CInvoiceViewDlg::CInvoiceViewDlg(Invoice *invoice,CWnd* pParent /*=NULL*/)
	: CDialog(CInvoiceViewDlg::IDD, pParent),m_viewprices(invoice)
{
		m_invoice = invoice;
}

CInvoiceViewDlg::~CInvoiceViewDlg()
{
}

void CInvoiceViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COWPRICELIST,m_viewprices);	
}

void CInvoiceViewDlg::ComputeTotalValues()
{
	Money totalvat,totalbrutto;
	
	totalvat = m_invoice->GetInvoiceNetValue()*m_invoice->GetVATRate();
	totalbrutto = m_invoice->GetInvoiceNetValue()+totalvat;
	
	SetDlgItemText(IDC_TOTALVALUE,totalbrutto.ToString());
	SetDlgItemText(IDC_VATVALUE,totalvat.ToString());
	SetDlgItemText(IDC_TOTALNETTO,m_invoice->GetInvoiceNetValue().ToString());
}



BEGIN_MESSAGE_MAP(CInvoiceViewDlg, CDialog)

	ON_BN_CLICKED(IDC_FINDPASSPORTS, OnBnClickedFindPassports)
	
END_MESSAGE_MAP()


BOOL CInvoiceViewDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog()) return FALSE;

	CString caption;
	caption.Format(IDS_INVVIEWCAPTION,(LPCSTR)m_invoice->GetInvoiceNoStr());
	SetWindowText(caption);

	m_viewprices.UpdateItemCount();
	
	CString countStr;
	countStr.Format(IDS_COWCOUNT,m_invoice->GetCowQty());
	SetDlgItemText(IDC_COWCOUNT,countStr);
	

	ComputeTotalValues();

	
	return TRUE;
}


/*
void CInvoiceViewDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	if (pNMIA)
		nItem = pNMIA->iItem;
	
	if(nItem >= 0)
	{
		    Cow *cowd = m_cowArray[nItem],
			*cowcopy = new Cow(*cowd);

		
				cowd->SQLUpdate.Reset();
				CNewCowDlg editcow(cowcopy,true);
				if(editcow.DoModal() == IDOK)
				{
					if(cowcopy->CanEditThisCow())
					{
						cowcopy->UpdateThisCow();
						(*cowd) = (*cowcopy);
					}
					else
					{
						AfxMessageBox(IDS_ERRCANTEDITCOW);
						//delete cowcopy;
					}
				}
				//else
					delete cowcopy;
		

	}

	*pResult = 0;
}
*/
void CInvoiceViewDlg::OnBnClickedFindPassports()
{
	CCowArray cowArray;
	m_invoice->GetInvoiceCows(cowArray);
	CowAccessor* pAccessor = CowAccessorFactory::createAccessor(m_invoice->GetBuyOrSell());

	CPassSearchDlg passSearch(cowArray,pAccessor,this);
	passSearch.DoModal();
	delete pAccessor;
}
