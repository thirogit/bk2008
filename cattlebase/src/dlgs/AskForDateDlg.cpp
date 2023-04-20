#include "stdafx.h"
#include "AskForDateDlg.h"


IMPLEMENT_DYNAMIC(CAskForDateDlg, CDialog)

BEGIN_MESSAGE_MAP(CAskForDateDlg, CDialog)
END_MESSAGE_MAP()

CAskForDateDlg::CAskForDateDlg(CWnd* pParent /*=NULL*/): CDialog(CAskForDateDlg::IDD, pParent)
{
	m_dt.MakeNull();
	m_pValidator = NULL;
}

CAskForDateDlg::~CAskForDateDlg()
{
}

void CAskForDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDITBOX,m_dtEditBox);
}


BOOL CAskForDateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(!m_sCaption.IsEmpty())
		SetWindowText(m_sCaption);

	if(!m_sLabel.IsEmpty())
		SetDlgItemText(IDC_EDITBOXDLGLBL,m_sLabel);

	if(!m_dt.IsNull())
		m_dtEditBox.SetDate(m_dt);	
	
	return TRUE;
}

void CAskForDateDlg::SetLabel(const CString& sLabel)
{
	m_sLabel = sLabel;
}

void CAskForDateDlg::SetWindowCaption(const CString& sCaption)
{
	m_sCaption = sCaption;
}

void CAskForDateDlg::SetDate(const CDateTime& dt)
{
	m_dt = dt;
}

void CAskForDateDlg::SetValidator(CValidator<const CDateTime&> *pValidator)
{
	m_pValidator = pValidator;
}

void CAskForDateDlg::OnOK()
{
	CDateTime dt;
	if(m_dtEditBox.GetDate(dt))
	{
		if(!dt.IsNull())
		{
			if(m_pValidator && !m_pValidator->IsValid(dt))
			{
				AfxMessageBox(m_pValidator->TellMeWhy(dt));
				return;
			}
			else
			{
				m_dt = dt;
				EndDialog(IDOK);
				return;
			}
		}
	}
	AfxMessageBox(IDS_ENTERCORRECTDATE);

}

CDateTime CAskForDateDlg::AskForDate()
{
	if(DoModal() == IDOK)
		return m_dt;

	return CDateTime();

}



