#include "stdafx.h"
#include "gridtabs.h"
#include "grids/CowGrid.h"
#include "grids/HentsGrid.h"
#include "grids/DocsGrid.h"
#include "grids/InvoiceGrid.h"
#include "grids/ExpensesGrid.h"

#include <afx.h>


CGridTab::CGridTab(UINT nId,UINT nResGridName,CRuntimeClass *pGridClass,UINT nHotKey,
		UserData::PRIVLEDGES uPrivledges,bool bActiveOnStartup)
{
	m_nId = nId;
	m_nResGridName = nResGridName;
	m_pGridClass = pGridClass;	
	m_nHotKey = nHotKey;	
	m_uPrivledges = uPrivledges;
	m_bActiveOnStartup = bActiveOnStartup;
}

UINT CGridTab::GetGridId() const
{
	return m_nId;
}
UINT CGridTab::GetResGridName() const
{
	return m_nResGridName;
}
CRuntimeClass * CGridTab::GetGridClass() const
{
	return m_pGridClass;
}
UINT CGridTab::GetHotKey() const
{
	return m_nHotKey;
}
UserData::PRIVLEDGES CGridTab::GetGridPrivledges() const
{
	return m_uPrivledges;
}
bool CGridTab::GetActiveOnStartup() const
{
	return m_bActiveOnStartup;
}
/////////////////////////////////////////////////////////////////////////////////////////

GridTabs GridTabs::instance;
GridTabs::AutoArray GridTabs::arrayFlush(GridTabs::instance);

GridTabs::GridTabs()
{
	Add(new CGridTab(INCOWSGRID,		IDS_GRIDNAME_INCOWS,		RUNTIME_CLASS(CInCowsGrid),			VK_F1,	(UserData::PRIVLEDGES)-1,	true ));
	Add(new CGridTab(ALLCOWSGRID,		IDS_GRIDNAME_ALLCOWS,		RUNTIME_CLASS(CAllCowsGrid),		VK_F2,	(UserData::PRIVLEDGES)-1,	false));
	Add(new CGridTab(HENTSGRID,			IDS_GRIDNAME_HENTS,			RUNTIME_CLASS(CHentsGrid),			VK_F3,	(UserData::PRIVLEDGES)-1,	false));
	Add(new CGridTab(COMPANYHENTGRID,	IDS_GRIDNAME_COMPANYHENTS,	RUNTIME_CLASS(CCompanyHentsGrid),	VK_F4,	(UserData::PRIVLEDGES)-1,	false));
	Add(new CGridTab(FARMERHENTGRID,	IDS_GRIDNAME_FARMERSHENTS,	RUNTIME_CLASS(CFarmerHentsGrid),	VK_F5,	(UserData::PRIVLEDGES)-1,	false));
	Add(new CGridTab(INDOCSGRID,		IDS_GRIDNAME_INDOCS,		RUNTIME_CLASS(CInDocsGrid),			VK_F6,	(UserData::PRIVLEDGES)-1,	false));
	Add(new CGridTab(OUTDOCSGRID,		IDS_GRIDNAME_OUTDOCS,		RUNTIME_CLASS(COutDocsGrid),		VK_F7,	(UserData::PRIVLEDGES)-1,	false));
	Add(new CGridTab(BUYINVSGRID,		IDS_GRIDNAME_BUYINVS,		RUNTIME_CLASS(CBuyVATInvoiceGrid),	VK_F8,	UserData::INVOICE,			false));
	Add(new CGridTab(SELLINVSGRID,		IDS_GRIDNAME_SELLINVS,		RUNTIME_CLASS(CSellVATInvoiceGrid),	VK_F9,	UserData::INVOICE,			false));
	Add(new CGridTab(RRINVSGRID,		IDS_GRIDNAME_RRINVS,		RUNTIME_CLASS(CRRInvoiceGrid),		VK_F10,	UserData::INVOICE,			false));
	Add(new CGridTab(EXPENSESGRID,		IDS_GRIDNAME_EXPENSES,		RUNTIME_CLASS(CExpensesGrid),		VK_F11,	(UserData::PRIVLEDGES)-1,	false));
}

GridTabs* GridTabs::GetTabs()
{
	return &instance;
}


