#include "stdafx.h"
#include "BaseTimesDlg.h"
/*
BEGIN_MESSAGE_MAP_TEMPLATE(CNewDataDlg,BASE_CLASS,DATA_TYPE)
	//{{AFX_MSG_MAP(COptionsSheet)
	ON_BN_CLICKED(IDOK,OnOK)
	ON_BN_CLICKED(IDCANCEL,OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BEGIN_MESSAGE_MAP_TEMPLATE_PASS(CBaseTimesDlg, CDialog,DLG,DLGDATA_TYPE)
	//{{AFX_MSG_MAP(COptionsSheet)
	ON_NOTIFY(TCN_SELCHANGE,IDC_TAB,OnSelChange)
	ON_BN_CLICKED(IDOK,OnOK)
	ON_BN_CLICKED(IDCANCEL,OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



template<class DLG,class DLGDATA_TYPE>
BOOL CBaseTimesDlg<DLG,DLGDATA_TYPE>::OnInitDialog() 
{

	CDialog::OnInitDialog();
 /*  VERIFY(m_baseDlg.Create(m_baseDlg.IDD, NULL));
   VERIFY(m_timesDlg.Create(CTimesDlg::IDD, this));

   m_baseDlg.ModifyStyle(WS_POPUP | DS_MODALFRAME | WS_CAPTION | WS_SYSMENU | WS_BORDER, WS_CHILD);

   m_baseDlg.SetParent(this);

   CString wtext;
   m_baseDlg.GetWindowText(wtext);
   SetWindowText(wtext);

	CRect rc,itemrc,wrc,wclrc;
	m_baseDlg.GetWindowRect(&rc);
	
	GetWindowRect(wrc);
	GetClientRect(wclrc);
	ClientToScreen(wclrc);

	int borderWidth2x = (wclrc.left-wrc.left) + (wrc.right-wclrc.right);
	int borderHeight2x = (wclrc.top-wrc.top) + (wrc.bottom-wclrc.bottom);
		           
	
	m_tab.GetItemRect(0,itemrc); //get item height

	rc.InflateRect(itemrc);
	m_tab.AdjustRect(TRUE,rc);

	m_tab.SetWindowPos(&m_tab.wndTop,0, 0, rc.Width(), rc.Height(), SWP_NOMOVE);
	
	CRect l_rectClient;
	CRect l_rectWnd;
	

	m_tab.GetClientRect(l_rectClient);
	m_tab.AdjustRect(FALSE,l_rectClient);
	m_tab.GetWindowRect(l_rectWnd);
	m_tab.GetParent()->ScreenToClient(l_rectWnd);
	l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top+itemrc.Height());

	m_baseDlg.SetWindowPos(&m_baseDlg.wndTop, l_rectClient.left, l_rectClient.top, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE);
	m_timesDlg.SetWindowPos(&m_baseDlg.wndTop, l_rectClient.left, l_rectClient.top, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE);

	SetWindowPos(&wndTop,0, 0, rc.Width()+borderWidth2x,rc.Height()+borderHeight2x, SWP_NOMOVE);
 
	m_tab.InsertItem(0,"Dane");
	m_tab.InsertItem(1,"W³aœciwoœci");
	m_tab.SetCurSel(0);
	LRESULT Result;
	OnSelChange(NULL,&Result);  
	
	return TRUE;
}

//Selection change event for the class derived from CTabCtrl
template<class DLG,class DLGDATA_TYPE>
void CBaseTimesDlg<DLG,DLGDATA_TYPE>::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
  int nSel = m_tab.GetCurSel();

   switch(nSel)
   {
	case 0:
		m_baseDlg.ShowWindow(SW_SHOW);
		m_timesDlg.ShowWindow(SW_HIDE);
	break;
	case 1:
		m_baseDlg.ShowWindow(SW_HIDE);
		m_timesDlg.ShowWindow(SW_SHOW);
	break;
   }

   *pResult = 0;
}*/




