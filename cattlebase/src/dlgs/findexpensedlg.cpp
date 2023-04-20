#include "stdafx.h"
#include "findexpensedlg.h"
#include "mainfrm_ctrl.h"
#include "../classes/forallactions/FindAction.h"
#include "../global_arrays.h"
#include "../classes/match/ExpenseMatcher.h"

BEGIN_MESSAGE_MAP(CFindExpenseDlg, CFindBaseDlg)
	ON_BN_CLICKED(IDC_BYEXPENSEDATE,OnBtnByExpenseDate)
END_MESSAGE_MAP()

CFindExpenseDlg::CFindExpenseDlg() : CFindBaseDlg(CFindExpenseDlg::IDD),m_bByExpenseDate(FALSE)
{
 
}

void CFindExpenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CFindBaseDlg::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_BYEXPENSETITLEEDIT,m_byTitle);
	DDX_Text(pDX,IDC_BYEXPENSEINVNOEDIT,m_byInvoiceNo);
	DDX_Check(pDX,IDC_BYEXPENSEDATE,m_bByExpenseDate);
	DDX_Control(pDX,IDC_BYEXPENSETYPECOMBO,m_ExpenseType);
	DDX_Control(pDX,IDC_EXPENSEDATE,m_expenseDateSpan);
	DDX_Control(pDX,IDC_BYVATRATE,m_vatRate);
	DDX_Control(pDX,IDC_BYGROSVALUE,m_expenseGrosValue);
	DDX_Control(pDX,IDC_BYEXPENSECATEGORY,m_ExpenseCategory);
}


BOOL CFindExpenseDlg::OnInitDialog()
{
	CFindBaseDlg::OnInitDialog();	
	m_ExpenseCategory.InitFindCombo();
	m_ExpenseType.InitFindCombo();
	UpdateData(FALSE);
	OnBtnByExpenseDate();

	return TRUE;
}
void CFindExpenseDlg::OnBtnByExpenseDate()
{
	UpdateData();
	m_expenseDateSpan.EnableWindow(m_bByExpenseDate);
}


int CFindExpenseDlg::GoSearch(CFindSetup& setup)
{
	UpdateData();
	CExpenseDataMatcher expenseMatcher;

	if(m_byTitle.GetLength() > 0)
			expenseMatcher.FindExpenseTitle(m_byTitle);
	if(m_byInvoiceNo.GetLength())
			expenseMatcher.FindInvoiceNo(m_byInvoiceNo);

	if(m_bByExpenseDate)
	{
		CDateTime date_t1,date_t2;
		TIMESPAN date_span_type = m_expenseDateSpan.GetSpan(date_t1,date_t2);
		expenseMatcher.FindExpenseDate(date_t1,date_t2,date_span_type);
	}

	if(!m_ExpenseType.IsNeutral())
	{
		expenseMatcher.FindExpenseType(m_ExpenseType.GetSelItemData());
	}
	
	if(m_vatRate.HasValue())
	{
		expenseMatcher.FindVATRate(m_vatRate.GetValue()/100.0,m_vatRate.GetRangeType());
	}
	if(m_expenseGrosValue.HasValue())
	{
		expenseMatcher.FindGrosValue(m_expenseGrosValue.GetValue(),m_expenseGrosValue.GetRangeType());
	}

	if(!m_ExpenseCategory.IsNeutral())
	{
		expenseMatcher.FindExpenseCategory(m_ExpenseCategory.GetSelItemData());
	}

	if(setup.ByCreationTime())
	{
		CDateTime time1,time2;
		TIMESPAN creationSearchSpan = setup.CreationTimeSpan(time1,time2);
		expenseMatcher.FindCreationTime(time1,time2,creationSearchSpan);
	}
	
	if(setup.ByLastModificationTime())
	{
		CDateTime time1,time2;
		TIMESPAN lastModificationSearchSpan = setup.LastModificationTimeSpan(time1,time2);
		expenseMatcher.FindLastModificationTime(time1,time2,lastModificationSearchSpan);
	}

	UserData *pCreator = setup.SelectedCreator();
	if(pCreator)
		expenseMatcher.FindCreator(pCreator);
		
	UserData* pLastModifier = setup.SelectedLastModifier();
	if(pLastModifier)
		expenseMatcher.FindLastModifier(pLastModifier);
	
	if(setup.GetInResult())
		expenseMatcher.FindMarked();

	if(setup.ByExtras())
		expenseMatcher.FindExtras(setup.GetExtras());

	CFindAction<CExpenseDataArray::ArrayAction,CExpenseDataMatcher,Expense> findExpense(setup.GetAddToResult(),&expenseMatcher);

	ary_expenses.ForAllAction(findExpense);
	setup.ChangeMainGrid(EXPENSESGRID );
		
	return findExpense.GetFoundCount();		
}
