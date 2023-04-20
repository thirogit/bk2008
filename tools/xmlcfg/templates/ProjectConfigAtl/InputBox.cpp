// InputBox.cpp : implementation file
//

#include "stdafx.h"
#include "InputBox.h"
#include "cmNumericEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBox dialog


CInputBox::CInputBox(CWnd* pParent /*=NULL*/)
	: CDialog(CInputBox::IDD, pParent),
	txtValue_(NULL),
	stringMode_(false),
	valueDouble_(0),
	precision_(0)
{
	//{{AFX_DATA_INIT(CInputBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CInputBox::~CInputBox()
{
	if (txtValue_) {
		if (txtValue_->GetSafeHwnd())
			txtValue_->Detach();
		delete txtValue_;
	}
}

void CInputBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBox)
	DDX_Control(pDX, IDC_LBL_CAPTION, lblCaption_);
	//DDX_Control(pDX, IDC_TXT_VALUE, txtValue_);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBox, CDialog)
	//{{AFX_MSG_MAP(CInputBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBox message handlers

void CInputBox::OnOK() 
{
	cmString tmp;

	txtValue_->GetWindowText(tmp);

	if (stringMode_) {
		valueString_ = tmp;
	} else {
		valueDouble_ = tmp;
	}
	
	CDialog::OnOK();
}

void CInputBox::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CInputBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(title_);
	lblCaption_.SetWindowText(caption_);
	
	CWnd* child;
	CRect rc;

	child = GetDlgItem(IDC_TXT_VALUE);
	child->GetWindowRect(rc);
	ScreenToClient(rc);
	if (stringMode_) {
		txtValue_ = new CEdit;
		
		txtValue_->Attach(CreateWindowEx(WS_EX_STATICEDGE, _T("Edit"), _T(""), WS_CHILD | WS_VISIBLE, rc.left, rc.top, rc.Width(), rc.Height(), this->m_hWnd, NULL, AfxGetInstanceHandle(), NULL));
		txtValue_->SetWindowText(valueString_);
	} else {
		cmNumericEdit* numEdit;

		numEdit = new cmNumericEdit;
		numEdit->Attach(CreateWindowEx(WS_EX_STATICEDGE, _T("Edit"), _T(""), WS_CHILD | WS_VISIBLE, rc.left, rc.top, rc.Width(), rc.Height(), this->m_hWnd, NULL, AfxGetInstanceHandle(), NULL));

		numEdit->SetPrecision(precision_);
		numEdit->SetValue(valueDouble_);

		txtValue_ = numEdit;
	}
	txtValue_->SetFont(GetFont());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputBox::Init(const cmString& title, const cmString& caption, const cmString& initialValue)
{
	title_ = title;
	caption_ = caption;
	valueString_ = initialValue;

	stringMode_ = true;
}

void CInputBox::Init(const cmString& title, const cmString& caption, double initialValue, int precision)
{
	title_ = title;
	caption_ = caption;
	valueDouble_ = initialValue;
	precision_ = precision;

	stringMode_ = false;
}

cmString CInputBox::GetValueString() const
{
	return valueString_;
}

double CInputBox::GetValueDouble() const
{
	return valueDouble_;
}