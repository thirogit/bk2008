#ifndef __BASEDLG_H__
#define __BASEDLG_H__

#include "basetimesdlg.h"

template<class DLG,class DLGDATA_TYPE>
class CBaseDlg : public CDialog 
{
// Construction
public:
	CBaseDlg<DLG,DLGDATA_TYPE>(DLGDATA_TYPE *pData): 
	    CDialog(CBaseDlg::IDD),m_baseDlg(pData)
		{
		};

		CBaseDlg<DLG,DLGDATA_TYPE>(): 
	    CDialog(CBaseDlg::IDD),m_baseDlg()
		{
		};
	
	// Dialog Data
	//{{AFX_DATA(CFindCowDlg)
	enum { IDD = IDD_FRAMEDIALOG };
	//}}AFX_DATA

// Attributes
public:
	CNewDataDlg<DLG,DLGDATA_TYPE> m_baseDlg;
	//DLG m_baseDlg;
// Operations
public:

// Implementation
public:
	virtual ~CBaseDlg(){} ;

// Generated message map functions
protected:
	
	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();
		VERIFY(m_baseDlg.Create(m_baseDlg.IDD, this));
		
		ASSERT(m_baseDlg.GetStyle() & WS_CHILD);

		m_baseDlg.ModifyStyle(DS_MODALFRAME | WS_CAPTION | WS_SYSMENU | WS_BORDER,0);
		
		ASSERT(!(m_baseDlg.GetExStyle() & WS_EX_NOPARENTNOTIFY));

		CString wtext;
		m_baseDlg.GetWindowText(wtext);
		SetWindowText(wtext);

		CRect rc,itemrc,wrc,wclrc;
		m_baseDlg.GetWindowRect(&rc);
		
		GetWindowRect(wrc);
		GetClientRect(wclrc);

		m_baseDlg.SetWindowPos(&m_baseDlg.wndTop, wclrc.left, wclrc.top, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE);
		
		ClientToScreen(wclrc);

		int borderWidth2x = (wclrc.left-wrc.left) + (wrc.right-wclrc.right);
		int borderHeight2x = (wclrc.top-wrc.top) + (wrc.bottom-wclrc.bottom);
			
		
		SetWindowPos(&wndTop,0, 0, rc.Width()+borderWidth2x,rc.Height()+borderHeight2x, SWP_NOMOVE);
		m_baseDlg.ShowWindow(SW_SHOW);
	 	
		return TRUE;
	};

	/*void OnParentNotify(UINT message,LPARAM lParam)
	{

		switch(message)
		{
		case WM_DESTROY:
			TRACE("child destroy\n");
			break;
		}
	}*/

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP_TEMPLATE_PASS(CBaseDlg,CDialog,DLG,DLGDATA_TYPE)
	//{{AFX_MSG_MAP(COptionsSheet)
	ON_BN_CLICKED(IDOK,OnOK)
	ON_BN_CLICKED(IDCANCEL,OnCancel)
	//ON_WM_PARENTNOTIFY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#endif