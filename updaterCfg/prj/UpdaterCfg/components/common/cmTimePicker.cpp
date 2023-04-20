// cmTimePicker.h : header file
//


#include "stdafx.h"
#include "cmTimePicker.h"


HHOOK cmTimePicker::spinBtnHook_ = NULL;
cmTimePicker* cmTimePicker::hookOwner_ = NULL;
CWnd* cmTimePicker::spinBtn_ = NULL;

cmTimePicker::cmTimePicker():
	parentCell_(NULL),
	displaySeconds_(true)
{
}

cmTimePicker::~cmTimePicker()
{}

BEGIN_MESSAGE_MAP(cmTimePicker, CDateTimeCtrl)
	//{{AFX_MSG_MAP(cmTimePicker)
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void cmTimePicker::SetParentCell(cmBaseCell* parent)
{
	parentCell_ = parent;
}

cmBaseCell* cmTimePicker::GetParentCell() const
{
	return parentCell_;
}

void cmTimePicker::EnableSeconds()
{}

void cmTimePicker::DisableSeconds()
{}


void cmTimePicker::OnKillFocus(CWnd* pNewWnd) 
{
	CDateTimeCtrl::OnKillFocus(pNewWnd);
	
	parentCell_->KillFocus(true, true);
}

UINT cmTimePicker::OnGetDlgCode() 
{
	return DLGC_WANTARROWS | DLGC_WANTALLKEYS | DLGC_WANTCHARS;
	//return CDateTimeCtrl::OnGetDlgCode();
}


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



