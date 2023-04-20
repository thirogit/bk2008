#include "stdafx.h"
#include "comboenteredit.h"

BEGIN_MESSAGE_MAP(CComboEnterEdit,CComboBox)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE,OnSelChange)
END_MESSAGE_MAP()


CComboEnterEdit::CComboEnterEdit() : m_enteredit("[0-9]{1,2}([,.][0-9]{0,2})?")
{
}
void CComboEnterEdit::PreSubclassWindow()
{
	m_enteredit.SubclassDlgItem(IDC_COMBOENTEREDIT, this);
	CComboBox::PreSubclassWindow();
//	m_enteredit.SetWnd2Notify(GetParent());
}

int CComboEnterEdit::OnCreate(LPCREATESTRUCT lpCreateStuct)
{
	if(CComboBox::OnCreate(lpCreateStuct) == -1) return -1;

	m_enteredit.SubclassDlgItem(IDC_COMBOENTEREDIT, this);
//	m_enteredit.SetWnd2Notify(GetParent());
	return 0;
}

BOOL CComboEnterEdit::OnSelChange(/*NMHDR*,LRESULT *pResult*/)
{
	CString s;
	GetLBText(GetCurSel(),s);
	SetWindowText(s);
	//*pResult = 0;
	return FALSE;
}


/*Suppose we have a combobox-based control created in the usual manner (see http://www.codeproject.com/combobox/listboxex.asp or many others similar CodeProject articles). This control is used in a dialog and is notified about the control's activities using the common notify-message mapping

ON_CBN_SELCHANGE(IDC_MY_COMBO, OnSelchangeCombo)

All is fine until the moment the control requires notification as well. Without any problems we can map the new functionality and all works well:

ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)

From the user's view selecting a new combo item changes the actual value but after clicking OK the new values were not used. What's going on?

The problem is with recognizing the changes made. Because we are using ON_CONTROL_REFLECT the dialog's OnSelchangeCombo is not being called.

Of course it is not system bug but documented behaviour - see MSDN TN062: Message Reflection for Windows Controls - and a solution is easy: Instead of using ON_NOTIFY_REFLECT() use ON_NOTIFY_REFLECT_EX() or provide a more general handler. For example:

BOOL CMyComboBox::OnChildNotify(UINT message, WPARAM wParam, 
                                LPARAM lParam, LRESULT* pLResult) 
{
    if(message == WM_COMMAND)
    {
        int wNotifyCode = HIWORD(wParam);
        int wID = LOWORD(wParam); 
        HWND hwndCtl = (HWND) lParam;

        switch(wNotifyCode)
        {
        case CBN_SELCHANGE:
            //original OnSelchange code call 
            break;
        }
    }
    
    return CComboBox::OnChildNotify(message, wParam, lParam, pLResult);
}*/