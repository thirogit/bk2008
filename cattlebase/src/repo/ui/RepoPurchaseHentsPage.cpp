#include "stdafx.h"
#include "RepoPurchaseHentsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRepoPurchaseHentsPage, ETSLayoutDialog)
CRepoPurchaseHentsPage::CRepoPurchaseHentsPage() : ETSLayoutDialog(CRepoPurchaseHentsPage::IDD,NULL,NULL,false)
{	

}

CRepoPurchaseHentsPage::~CRepoPurchaseHentsPage()
{
	
}

void CRepoPurchaseHentsPage::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);		
	DDX_Control(pDX,IDC_ITEMLIST,m_HentList);
}


BEGIN_MESSAGE_MAP(CRepoPurchaseHentsPage, ETSLayoutDialog)
	//ON_BN_CLICKED(IDC_UPDATEREPO, OnUpdateRepo)
	//ON_NOTIFY(NM_CLICK ,IDC_PURACHELIST, OnPuracheClick)	
END_MESSAGE_MAP()


BOOL CRepoPurchaseHentsPage::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();	
	
	CreateRoot(HORIZONTAL,0)->addItem(&m_HentList);
	
	return TRUE;
}

void CRepoPurchaseHentsPage::BeforeUpdate()
{
	m_HentList.BeforeUpdate();
}

void CRepoPurchaseHentsPage::UpdateFrom(RepoPurchase* pPurchase)
{
	m_HentList.UpdateListWith(pPurchase);
	m_HentList.EnableWindow(pPurchase != NULL);

}



