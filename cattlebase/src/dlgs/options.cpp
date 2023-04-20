#include "stdafx.h"
#include "options.h"


COptionsSheet::COptionsSheet(CWnd* pWndParent)
	 : CPropertySheet(IDS_OPTWNDCAPTION, pWndParent)
{
	AddPage(&m_MainOptPage);
	AddPage(&m_FirmInfoPage);
	AddPage(&m_PreSel);
	AddPage(&m_BackupPage);

	if(HAS_PRIVLEDGE(ADMIN))
	{
		AddPage(&m_UsersOpt);
	}	
	
	AddPage(&m_ArchiveOpt);
	AddPage(&m_PassFileOpt);
	AddPage(&m_ImportExportOpt);
	
}

COptionsSheet::~COptionsSheet()
{
}


BEGIN_MESSAGE_MAP(COptionsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(COptionsSheet)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet message handlers



BOOL COptionsSheet::OnInitDialog() 
{

	CPropertySheet::OnInitDialog();
	//GetPage(0)->SetModified ();



	
	return TRUE;
}
/*
BOOL COptionsSheet::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
   {
       NMHDR* pnmh = (NMHDR*)lParam;
       // tab is about to change
      if (TCN_SELCHANGE == pnmh->code)
       {
           // get the current page index
		      CString rString;
		    int nCurrentPage = GetActiveIndex ();
			//this->GetPage(nCurrentPage)->m_strCaption;
		
					 
		    SetWindowText(rString);           
       }
       return CPropertySheet::OnNotify(wParam, lParam, pResult);
}
   */