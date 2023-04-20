#include "stdafx.h"
#include "RepoPurchaseInvoicesPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRepoPurchaseInvoicesPage, ETSLayoutDialog)
CRepoPurchaseInvoicesPage::CRepoPurchaseInvoicesPage()
	: 
	ETSLayoutDialog(CRepoPurchaseInvoicesPage::IDD,NULL,NULL,false)
	//CDialog(CRepoPurchaseInvoicesPage::IDD,NULL)
{	

	

}

CRepoPurchaseInvoicesPage::~CRepoPurchaseInvoicesPage()
{
	
}

void CRepoPurchaseInvoicesPage::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);
	//CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PURACHE_INVOICE_LIST,m_InvoiceList);
	DDX_Control(pDX,IDC_PURACHE_INVOICE_COW_LIST,m_CowList);
}


BEGIN_MESSAGE_MAP(CRepoPurchaseInvoicesPage, ETSLayoutDialog)
	ON_NOTIFY(NM_CLICK,	IDC_PURACHE_INVOICE_LIST, OnInvoiceClick)
END_MESSAGE_MAP()


BOOL CRepoPurchaseInvoicesPage::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();	
	//CDialog::OnInitDialog();	

	CPane rootPane = CreateRoot(VERTICAL,0);
	rootPane->addItem(&m_InvoiceList);
	rootPane->addItem(&m_CowList);
		
	return TRUE;
}

void CRepoPurchaseInvoicesPage::BeforeUpdate()
{
	m_InvoiceList.BeforeUpdate();
	m_CowList.BeforeUpdate();
}	

void CRepoPurchaseInvoicesPage::UpdateFrom(RepoPurchase* pPurchase)
{
	m_pPurchase = pPurchase;
	m_InvoiceList.UpdateListWith(pPurchase);
	
	m_InvoiceList.EnableWindow(pPurchase != NULL);	

	if(pPurchase != NULL && pPurchase->GetInvoiceCount() == 1)
	{
		m_CowList.EnableWindow(TRUE);
		m_CowList.UpdateListWith(pPurchase->GetInvoice(0));
	}
	else
	{   
		m_CowList.UpdateListWith(NULL);
		m_CowList.EnableWindow(FALSE);
	}
}

void CRepoPurchaseInvoicesPage::OnUpdateRepo()
{
}

void CRepoPurchaseInvoicesPage::OnInvoiceClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE plvItemActive = (LPNMITEMACTIVATE)pNMHDR;
    
	if(plvItemActive->iItem >= 0)
	{
		RepoInvoice* pInvoice = m_InvoiceList.GetItemAt(plvItemActive->iItem);
		m_CowList.EnableWindow(TRUE);
		m_CowList.BeforeUpdate();		
		m_CowList.UpdateListWith(pInvoice);		
	}
	*pResult = 0;
}

