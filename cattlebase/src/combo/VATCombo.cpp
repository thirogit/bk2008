#include "stdafx.h"
#include "VATCombo.h"
#include "../missing/util.h"

BEGIN_MESSAGE_MAP(CVATCombo::CComboEnterEdit,CEnterEdit)	
	ON_WM_CHAR()	
END_MESSAGE_MAP()

CVATCombo::CComboEnterEdit::CComboEnterEdit(const TCHAR *pszRegEx) : CEnterEdit(pszRegEx)
{
}

void CVATCombo::CComboEnterEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_TAB)
	{
		CWnd *pNextWnd = GetParent()->GetNextWindow();
		if(pNextWnd)
			pNextWnd->SetFocus();
	}
	CEnterEdit::OnChar(nChar,nRepCnt,nFlags);
}

BEGIN_MESSAGE_MAP(CVATCombo,CComboBox)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE,OnSelChange)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_COMBOENTEREDIT,OnEditComplete)
END_MESSAGE_MAP()


CVATCombo::CVATCombo() : m_enteredit("[0-9]{1,2}([,.][0-9]{0,2})?")
{
}


void CVATCombo::Init()
{
	double VATRates[] = 
	{
		0.0,
		3.0,
		5.0,
		7.0,
		9.0		
	};
	CString floatStr;

	ResetContent();
	for(int i = 0;i < (sizeof(VATRates)/sizeof(double));i++)
	{
		floatStr.Format(IDS_FLOATFMT,VATRates[i]);
		AddString(floatStr);
	}

}

VATRate CVATCombo::GetVATRate()
{
	CString VATStr;
	GetWindowText(VATStr);
	if(VATStr.IsEmpty())
		return VATRate();

	return VATRate(CUtil::myatof(VATStr)/100.0);
}

void CVATCombo::SetVATRate(const VATRate& VAT)
{
	CString VATStr;
	if(!VAT.IsNull())
	{
		VATStr = VAT.To100String(NULL,2);
	}

	m_enteredit.SetCommitedText(VATStr);
}
void CVATCombo::PreSubclassWindow()
{
	m_enteredit.SubclassDlgItem(IDC_COMBOENTEREDIT, this);
	CComboBox::PreSubclassWindow();
	Init();
}

int CVATCombo::OnCreate(LPCREATESTRUCT lpCreateStuct)
{
	if(CComboBox::OnCreate(lpCreateStuct) == -1) return -1;

	m_enteredit.SubclassDlgItem(IDC_COMBOENTEREDIT, this);
	Init();
	return 0;
}

BOOL CVATCombo::OnSelChange()
{
	CString s;
	GetLBText(GetCurSel(),s);
	SetWindowText(s);
	return FALSE;
}
void CVATCombo::OnEditComplete(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHDR hdr;
	hdr.code = EN_EDITCOMPLETE;
	hdr.hwndFrom = GetSafeHwnd();
	hdr.idFrom = GetDlgCtrlID();
	GetParent()->SendMessage(WM_NOTIFY,hdr.idFrom,(LPARAM)&hdr);
	*pResult = 0;
}

bool CVATCombo::IsUncommited()
{
	return m_enteredit.IsUncommited();
}

