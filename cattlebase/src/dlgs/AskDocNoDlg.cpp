// AskDocNoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AskDocNoDlg.h"


// CAskDocNoDlg dialog

IMPLEMENT_DYNAMIC(CAskDocNoDlg, CDialog)
CAskDocNoDlg::CAskDocNoDlg(UINT nCaptionID,UINT nLabelID,CWnd* pParent /*=NULL*/)
	: CDialog(CAskDocNoDlg::IDD, pParent)
{

	m_caption.LoadString(nCaptionID);
	m_label.LoadString(nLabelID);
	m_docNo = 0;
}

UINT CAskDocNoDlg::AskForDocNo(UINT nCaption,UINT nLabel,CWnd* pParent/*=NULL*/)
{
	CAskDocNoDlg ask4docNo(nCaption,nLabel,pParent);
	if(ask4docNo.DoModal() == IDOK)
		return ask4docNo.GetDocNo();

	return 0;
}

CAskDocNoDlg::~CAskDocNoDlg()
{
}

void CAskDocNoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX,IDC_DOCNOEDIT,m_docNo);
}


BEGIN_MESSAGE_MAP(CAskDocNoDlg, CDialog)
END_MESSAGE_MAP()

UINT CAskDocNoDlg::GetDocNo()
{
	return m_docNo;
}

BOOL CAskDocNoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_caption);
	SetDlgItemText(IDC_ASKLABEL,m_label);

	CWnd * pEdit = GetDlgItem(IDC_DOCNOEDIT);
	pEdit->SendMessage( EM_SETSEL, 0, -1);
	pEdit->SendMessage( EM_SCROLLCARET, 0, 0); 
	return TRUE;
}

// CAskDocNoDlg message handlers
