#include "stdafx.h"
#include "ExpenseCategoriesDlg.h"
#include "../MainCfg.h"
#include "../global_arrays.h"
#include "NewExpenseCategoryDlg.h"
#include "EditExpenseCategoryDlg.h"
#include "../CattleSQL.h"
#include "../consts.h"
#include "../classes/TempStringLoader.h"
/////////////////////////////////////////////////////////////////////////////////////////

#define EXPENSECATEGORYLIST_NAMECOL 101
#define EXPENSECATEGORYLIST_DESCCOL 102
#define EXPENSECATEGORYLIST_VATCOL	103

const GRIDHEADERS CExpenseCategoryListCtrl::m_expenseCategoriesListColumns[] = 
{
	{IDS_HEADEREXPENSECATEGORYNAME,		EXPENSECATEGORYLIST_NAMECOL},
	{IDS_HEADEREXPENSECATEGORYDESC,		EXPENSECATEGORYLIST_DESCCOL},
	{IDS_HEADEREXPENSECATEGORYVATRATE,	EXPENSECATEGORYLIST_VATCOL},
	{0,0}
};


CExpenseCategoryListCtrl::CExpenseCategoryListCtrl(CExpenseCategoryArray &expenseCategories) :
			m_expenseCategories(expenseCategories), CBaseListCtrl(m_expenseCategoriesListColumns,CFGID_EXPENSECATEGORYLIST)
{
}

void CExpenseCategoryListCtrl::UpdateItemCount()
{
	SetItemCount(m_expenseCategories.GetSize());
}

void CExpenseCategoryListCtrl::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	CString sText;

	ExpenseCategoryPtr ptrCategory = m_expenseCategories[nItem];
	switch(GetColID(nSubItem))
	{
		case EXPENSECATEGORYLIST_NAMECOL:
			sText = ptrCategory->GetExpenseCategoryName();
			break;
		case EXPENSECATEGORYLIST_DESCCOL:
			sText = ptrCategory->GetExpenseCategoryDesc();
			break;
		case EXPENSECATEGORYLIST_VATCOL:
			if(ptrCategory->HasDefaultVATRate())
			{
				sText = ptrCategory->GetDefaultVATRate().To100String();
			}
			else
				sText = szMissing;
			break;
	}
	strcpy(pszItemText,(LPCSTR)sText);
}


/////////////////////////////////////////////////////////////////////////////////////////
	

BEGIN_MESSAGE_MAP(CExpenseCategoriesDlg,CDialog)
	ON_BN_CLICKED(IDC_ADDBTN,OnNewExpenseCategroy)
	ON_BN_CLICKED(IDC_DELBTN,OnDelExpenseCategory)
	ON_BN_CLICKED(IDC_EDITBTN,OnEditExpenseCategory)
	ON_NOTIFY(NM_DBLCLK , IDC_LIST, OnDblClick)
END_MESSAGE_MAP()

CExpenseCategoriesDlg::CExpenseCategoriesDlg(CWnd* pParent/* = NULL*/) : 
CDialog(CExpenseCategoriesDlg::IDD,pParent),m_expenseCategories(ary_expensecategories)
{

}

void CExpenseCategoriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST,m_expenseCategories);
	DDX_Control (pDX, IDOK  , m_OKBtn ) ;
	DDX_Control (pDX, IDC_EDITBTN,m_EditBtn);
	DDX_Control (pDX, IDC_ADDBTN,m_AddBtn);
	DDX_Control (pDX, IDC_DELBTN,m_DeleteBtn);
}
BOOL CExpenseCategoriesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_OKBtn.SetButtonImage( IDB_STOCKOK);	
	m_AddBtn.SetButtonImage( IDB_STOCKADD);	
	m_DeleteBtn.SetButtonImage( IDB_STOCKDEL);	
	m_EditBtn.SetButtonImage( IDB_STOCKEDIT);	

	SetWindowText(CTempStringLoader(IDS_EDITEXPENSECATEGORIES));
	return TRUE;

}

void CExpenseCategoriesDlg::OnNewExpenseCategroy()
{
	CNewExpenseCategoryDlg newCategoryDlg(this);
	if(newCategoryDlg.DoModal() == IDOK)
	{
		ExpenseCategory *pNewCategory = newCategoryDlg.GetNewExpenseCategory();
		if(pNewCategory->InsertThisExpenseCategory(true))
		{
			ary_expensecategories.Add(pNewCategory);
			m_expenseCategories.UpdateItemCount();
		}
		else
		{
			SQLDB.SQLMessageBox(IDS_ERRINSERTINGEXPENSECATEGORY);
			delete pNewCategory;
		}
	}

}
void CExpenseCategoriesDlg::OnDelExpenseCategory()
{
	m_expenseCategories.Lock();
	int nSelectedCategory = m_expenseCategories.GetCurSel();
	ExpenseCategoryPtr pCategory = ary_expensecategories[nSelectedCategory];
	if(pCategory.Count() > 2) //one in array, one here
	{
		AfxMessageBox(IDS_EXPENSECATEGORYISUSED);
	}
	else
	{
		if(pCategory->DeleteThisExpenseCategory())
		{
			VERIFY(!ary_expensecategories.RemoveAt(nSelectedCategory));
			m_expenseCategories.UpdateItemCount();
			delete pCategory.GetPtr();
		}
		else
		{
			SQLDB.SQLMessageBox(IDS_ERRDELETINGEXPENSECATEGORY);
		}
	}
	m_expenseCategories.Unlock();

}

void CExpenseCategoriesDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnEditExpenseCategory();
	*pResult = 0;
}

void CExpenseCategoriesDlg::OnEditExpenseCategory()
{
	int nSelectedCategory = m_expenseCategories.GetCurSel();
	ExpenseCategoryPtr pCategory = ary_expensecategories[nSelectedCategory];
	ExpenseCategory *ptrCategory = pCategory.GetPtr();

	CEditExpenseCategoryDlg editCategoryDlg(ptrCategory,this);
	if(editCategoryDlg.DoModal() == IDOK)
	{
		if(ptrCategory->UpdateThisExpenseCategory())
		{
			ptrCategory->CommitChanges();
			m_expenseCategories.Invalidate();
		}
		else
		{
			ptrCategory->RollbackChanges();
			SQLDB.SQLMessageBox(IDS_ERRUPDATEINGEXPENSECATEGORY);
		}
	}
	else
	{
		ptrCategory->RollbackChanges();
	}
}
