#include "stdafx.h"

#include "NumFindCtrl.h"


#define IDC_NUMFINDCOMBO 1001
#define IDC_NUMFINDFILTER 1002

CNumFindCtrl::CNumFindCtrl(LPCSTR filterExp) :  m_number(filterExp)
{
}

CNumFindCtrl::~CNumFindCtrl()
{
}


BEGIN_MESSAGE_MAP(CNumFindCtrl, CStatic)
	//{{AFX_MSG_MAP(CNumFindCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CNumFindCtrl::PreSubclassWindow()
{

	CStatic::PreSubclassWindow();

	ModifyStyleEx(GetExStyle(),WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP);
	CreateControls();
//	PosCtrl();
	
}


BOOL CNumFindCtrl::CreateControls()
{

	RECT rect;

	struct tagComboItems
	{
		const char *comboItem;
		CMP_ACTION_TYPE cmpAction;
	} ComboItems[3] =
	{
		{"Dok³adnie",CMP_PRECISE},
		{"Co najwy¿ej",CMP_ATMOST},
		{"Co najmniej",CMP_ATLEAST}
	};

	

  CWnd* pParent = GetParent();
  if (pParent != NULL)  SetFont(pParent->GetFont());
  
  SetWindowText("");
  GetClientRect(&rect);

	if(!m_number.CreateEx(WS_EX_CLIENTEDGE,_T("EDIT"),"",ES_AUTOHSCROLL | WS_CHILD | WS_TABSTOP | WS_VISIBLE  |  WS_TABSTOP | WS_BORDER ,CRect(85,0,130,25),this,IDC_NUMFINDFILTER))
		return FALSE;

	m_number.SetFont(GetFont());
	
	if(!m_combo.Create(CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE | WS_CHILD, CRect(0,0,80,100),this,IDC_NUMFINDCOMBO))
		return FALSE;

	m_combo.SetFont(GetFont());
	int item;
	for(int i = 0;i < 3;i++)
	{
		item = m_combo.AddString(ComboItems[i].comboItem);
		m_combo.SetItemData(item,ComboItems[i].cmpAction);
	}

	m_combo.SetCurSel(0);

	m_number.EnableWindow(TRUE);
	m_number.ShowWindow(SW_SHOW);

 	m_combo.EnableWindow(TRUE);
	m_combo.ShowWindow(SW_SHOW);

	return TRUE;
}

int CNumFindCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CStatic::OnCreate(lpCreateStruct) == -1 || !CreateControls()) return -1;
	return 0;
}
CMP_ACTION_TYPE  CNumFindCtrl::GetRangeType()
{
	return (CMP_ACTION_TYPE)m_combo.GetItemData(m_combo.GetCurSel());
}

bool CNumFindCtrl::HasValue()
{
	return m_number.GetWindowTextLength() > 0;
}

