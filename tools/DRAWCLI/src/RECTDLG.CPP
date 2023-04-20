// rectdlg.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "drawcli.h"
#include "rectdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define __LOCALMSG_InternalUpdateControls (WM_USER+333)

/////////////////////////////////////////////////////////////////////////////
// CRectDlg dialog


IMPLEMENT_DYNCREATE( CRectDlg, CBaseDlg )

COLORREF CRectDlg::g_clrTransparent = COLORREF(-1);

CRectDlg::CRectDlg(CWnd* /*pParent = NULL*/)
	: CBaseDlg(CRectDlg::IDD),m_bInitialized(false)
{
	m_bNoFill = FALSE;
	m_nPenSize = 0;
	m_clrFill = m_clrOutline = g_clrTransparent;
	m_bInitialized = true;
}

CRectDlg::CRectDlg(BOOL bNoFill,UINT nPenSize,COLORREF clrFill,COLORREF clrOutline,CString &label)  
 : CBaseDlg(CRectDlg::IDD),m_bInitialized(false)
{
	Init(bNoFill,nPenSize,clrFill,clrOutline,label);
	m_bInitialized = true;
}

bool CRectDlg::Init(BOOL bNoFill,UINT nPenSize,COLORREF clrFill,COLORREF clrOutline,CString &label)  
{
	if(!m_bInitialized)
	{
		m_bNoFill = bNoFill;
		m_nPenSize = nPenSize;
		m_clrFill = clrFill;
		m_clrOutline = clrOutline;
		m_label = label;
		return true;
	}
	return false;
}

void CRectDlg::DoChanges(CDrawObj *pObj)
{
	pObj->m_label = m_label;

	pObj->SetNoFill(m_bNoFill);
	if( !pObj->IsNoFill() )
		pObj->SetFillColor(m_clrFill );

	if(m_nPenSize == 0 )
		pObj->SetNoOutline();
	else
	{
		pObj->SetNoOutline( FALSE );
		pObj->SetLogPenSize(m_nPenSize);
		pObj->SetLineColor(m_clrOutline );
	}
}


void CRectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRectDlg)
	DDX_Control(pDX, IDC_SPIN1, m_SpinCtrl);
	DDX_Check(pDX, IDC_NOFILL, m_bNoFill);
	DDX_Text(pDX, IDC_WEIGHT, m_nPenSize);
	DDX_Text(pDX,IDC_LABELEDIT,m_label);
	DDV_MinMaxUInt(pDX, m_nPenSize, 0, 100);
	DDX_Control(pDX, IDC_BUTTON_COLOR_OUTLINE, m_BtnColorOutline);
	DDX_Control(pDX, IDC_BUTTON_COLOR_FILL, m_BtnColorFill);


	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRectDlg, CDialog)
	//{{AFX_MSG_MAP(CRectDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_NOFILL, _UpdateControls)
	ON_EN_CHANGE(IDC_WEIGHT, _UpdateControls)
	ON_MESSAGE(
		__LOCALMSG_InternalUpdateControls,
		OnInternalUpdateControls
		)
	ON_REGISTERED_MESSAGE(
		CExtPopupColorMenuWnd::g_nMsgNotifyColorChangedFinally,
		OnColorChangedFinally
		)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRectDlg message handlers

BOOL CRectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	m_SpinCtrl.SetRange(0, 100);
	m_SpinCtrl.SetBase(10);
	m_SpinCtrl.SetPos( /*1*/ m_nPenSize );

	m_BtnColorOutline.m_bEnableBtnColorDefault
		= m_BtnColorFill.m_bEnableBtnColorDefault
		= false;
	m_BtnColorOutline.m_clrDefault
		= m_BtnColorOutline.m_clrSelected
		= m_clrOutline;
	m_BtnColorFill.m_clrDefault
		= m_BtnColorFill.m_clrSelected
		= m_clrFill;

	PostMessage( __LOCALMSG_InternalUpdateControls );

	return TRUE;
}

LRESULT CRectDlg::OnColorChangedFinally(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;
	m_clrFill = m_BtnColorFill.m_clrSelected;
	m_clrOutline = m_BtnColorOutline.m_clrSelected;
	PostMessage( __LOCALMSG_InternalUpdateControls );
	return 0;
}

LRESULT CRectDlg::OnInternalUpdateControls(WPARAM wParam, LPARAM lParam)
{
	wParam;
	lParam;
	UpdateData();
	if( m_BtnColorOutline.GetSafeHwnd() != NULL )
		m_BtnColorOutline.EnableWindow( m_nPenSize != 0 );
	if( m_BtnColorFill.GetSafeHwnd() != NULL )
		m_BtnColorFill.EnableWindow( !m_bNoFill );
	return 0;
}

void CRectDlg::_UpdateControls()
{
	PostMessage( __LOCALMSG_InternalUpdateControls );
}
