#ifndef __BASETIMESDLG_H__
#define __BASETIMESDLG_H__
#include "timesdlg.h"

#ifdef _AFXDLL
#define BEGIN_MESSAGE_MAP_TEMPLATE_PASS(theClass,inherClass,baseClass,dataType) \
	template<class baseClass,class dataType> const AFX_MSGMAP* PASCAL theClass<baseClass,dataType>::GetThisMessageMap() \
		{ return &theClass<baseClass,dataType>::messageMap; } \
	template<class baseClass,class dataType> const AFX_MSGMAP* theClass<baseClass,dataType>::GetMessageMap() const \
		{ return &theClass<baseClass,dataType>::messageMap; } \
	template<class baseClass,class dataType> AFX_COMDAT const AFX_MSGMAP theClass<baseClass,dataType>::messageMap = \
	{ &inherClass::GetThisMessageMap, &theClass<baseClass,dataType>::_messageEntries[0] }; \
	template<class baseClass,class dataType> AFX_COMDAT const AFX_MSGMAP_ENTRY theClass<baseClass,dataType>::_messageEntries[] = \
	{ \

#else
#define BEGIN_MESSAGE_MAP_TEMPLATE_PASS(theClass,inherClass,baseClass,dataType) \
	template<class baseClass,class dataType>  AFX_MSGMAP* theClass<baseClass,dataType>::GetMessageMap() const \
		{ return &theClass<baseClass,dataType>::messageMap; } \
	template<class baseClass,class dataType> AFX_COMDAT const AFX_MSGMAP theClass<baseClass,dataType>::messageMap = \
	{ &inherClass::messageMap, &theClass<baseClass,dataType>::_messageEntries[0] }; \
	template<class baseClass,class dataType> AFX_COMDAT const AFX_MSGMAP_ENTRY theClass<baseClass,dataType>::_messageEntries[] = \
	{ \

#endif



template<class BASE_CLASS,class DATA_TYPE>
class CNewDataDlg : public BASE_CLASS
{
public:
	CNewDataDlg() : BASE_CLASS() {};
	CNewDataDlg(DATA_TYPE *data) : BASE_CLASS(data) {};
	~CNewDataDlg() {};
protected:
	void OnOK()
	{
		BASE_CLASS::OnOK();
		//GetParent()->SendMessage(WM_COMMAND,IDOK);
	};
	void OnCancel()
	{
		BASE_CLASS::OnCancel();
		//GetParent()->SendMessage(WM_COMMAND,IDCANCEL);
	};
	
	void EndDialog(int nResult)
	{
		BASE_CLASS::EndDialog(nResult);
	}
	
};





template<class DLG,class DLGDATA_TYPE>
class CBaseTimesDlg : public CDialog 
{
// Construction
public:
	CBaseTimesDlg<DLG,DLGDATA_TYPE>(DLGDATA_TYPE *pData): 
	    CDialog(CBaseTimesDlg::IDD),m_baseDlg(pData),m_timesDlg(pData)
		{
		};
	
	// Dialog Data
	//{{AFX_DATA(CFindCowDlg)
	enum { IDD = IDD_BASEDIALOG };
	//}}AFX_DATA

// Attributes
public:
	CNewDataDlg<DLG,DLGDATA_TYPE> m_baseDlg;
	CTimesDlg  m_timesDlg;
	CTabCtrl   m_tab;
// Operations
public:

// Implementation
public:
	virtual ~CBaseTimesDlg(){} ;

// Generated message map functions
protected:
	
	virtual void DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
		//{{AFX_DATA_MAP(CFindCowDlg)
			DDX_Control (pDX, IDC_TAB  , m_tab ) ;		
		//}}AFX_DATA_MAP
	}

	virtual BOOL OnInitDialog()
	{
		CDialog::OnInitDialog();
		VERIFY(m_baseDlg.Create(m_baseDlg.IDD, this));
		VERIFY(m_timesDlg.Create(CTimesDlg::IDD, this));

		m_baseDlg.ModifyStyle(WS_POPUP | DS_MODALFRAME | WS_CAPTION | WS_SYSMENU | WS_BORDER, WS_CHILD);

		//m_baseDlg.SetParent(this);
		//m_timesDlg.SetParent(this);

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
	};
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult)
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
	};
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP_TEMPLATE_PASS(CBaseTimesDlg,CDialog,DLG,DLGDATA_TYPE)
	//{{AFX_MSG_MAP(COptionsSheet)
	ON_BN_CLICKED(IDOK,OnOK)
	ON_BN_CLICKED(IDCANCEL,OnCancel)
	ON_NOTIFY(TCN_SELCHANGE,IDC_TAB,OnSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#endif
