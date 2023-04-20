#include "stdafx.h"
#include "RepoPurchaseSummaryPage.h"
#include "../../classes/IntString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRepoPurchaseSummaryPage, ETSLayoutDialog)
CRepoPurchaseSummaryPage::CRepoPurchaseSummaryPage() : ETSLayoutDialog(CRepoPurchaseSummaryPage::IDD,NULL,NULL,false)
{	

}

CRepoPurchaseSummaryPage::~CRepoPurchaseSummaryPage()
{
	
}

void CRepoPurchaseSummaryPage::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);		
}


BEGIN_MESSAGE_MAP(CRepoPurchaseSummaryPage, ETSLayoutDialog)
	
END_MESSAGE_MAP()


BOOL CRepoPurchaseSummaryPage::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();	
	
	return TRUE;
}

void CRepoPurchaseSummaryPage::BeforeUpdate()
{

}

void CRepoPurchaseSummaryPage::UpdateFrom(RepoPurchase* pPurchase)
{

	int totalTransCowCount = 0;
	Weight totalTransWeight(0.0);
	Money totalTransNet(0.0);
	Money totalTransGross(0.0);

	int totalCashCowCount = 0;
	Weight totalCashWeight(0.0);
	Money totalCashNet(0.0);
	Money totalCashGross(0.0);

	if(pPurchase)
	{
		RepoInvoice* pInvoice = NULL;
			
		for(int i = 0; i < pPurchase->GetInvoiceCount();i++)
		{
			pInvoice = pPurchase->GetInvoice(i);

			switch(pInvoice->GetPayWay())
			{
			case Invoice::PAY_CASH:
				totalCashCowCount += pInvoice->GetCowCount();
				totalCashWeight += pInvoice->GetTotalWeight();
				totalCashNet += pInvoice->GetTotalNet();
				totalCashGross += pInvoice->GetTotalGross();
				break;
			case Invoice::PAY_TRANS:
				totalTransCowCount += pInvoice->GetCowCount();
				totalTransWeight += pInvoice->GetTotalWeight();
				totalTransNet += pInvoice->GetTotalNet();
				totalTransGross += pInvoice->GetTotalGross();
				break;
			}

		}
	}

 

	SetDlgItemText(IDC_PURCHASE_TOTAL_TRANS_COWCOUNT,CIntString(totalTransCowCount));
	SetDlgItemText(IDC_PURCHASE_TOTAL_TRANS_WEIGHT,totalTransWeight.ToString());
	SetDlgItemText(IDC_PURCHASE_TOTAL_TRANS_NET,totalTransNet.ToString());
	SetDlgItemText(IDC_PURCHASE_TOTAL_TRANS_GROSS,totalTransGross.ToString());

	SetDlgItemText(IDC_PURCHASE_TOTAL_CASH_COWCOUNT,CIntString(totalCashCowCount));
	SetDlgItemText(IDC_PURCHASE_TOTAL_CASH_WEIGHT,totalCashWeight.ToString());
	SetDlgItemText(IDC_PURCHASE_TOTAL_CASH_NET,totalCashNet.ToString());
	SetDlgItemText(IDC_PURCHASE_TOTAL_CASH_GROSS,totalCashGross.ToString());

	SetDlgItemText(IDC_PURCHASE_TOTAL_COWCOUNT,CIntString(totalCashCowCount+totalTransCowCount));
	SetDlgItemText(IDC_PURCHASE_TOTAL_NET,(totalCashNet+totalTransNet).ToString());
	SetDlgItemText(IDC_PURCHASE_TOTAL_WEIGHT,(totalTransWeight+totalCashWeight).ToString());
	SetDlgItemText(IDC_PURCHASE_TOTAL_GROSS,(totalCashGross+totalTransGross).ToString());

}



