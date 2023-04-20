#include "stdafx.h"
#include "InputWeight.h"
#include "../../classes/TempStringLoader.h"


BEGIN_MESSAGE_MAP(CInputWeight, CDialog)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CInputWeight, CDialog)
CInputWeight::CInputWeight(CWnd* pParent /*=NULL*/)
	: CDialog(CInputWeight::IDD, pParent),m_pValidator(NULL)
{
	m_sCaption = (CString)CTempStringLoader(IDS_ASKFORWEIGHTCAPTION);
	m_sLabel = (CString)CTempStringLoader(IDS_ASKFORWEIGHTLABEL);
}

CInputWeight::~CInputWeight()
{
}

void CInputWeight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDITBOX,m_WeightEdit);

}

BOOL CInputWeight::OnInitDialog()
{

	CDialog::OnInitDialog();

	SetWindowText(m_sCaption);
	SetDlgItemText(IDC_EDITBOXDLGLBL,m_sLabel);
	m_WeightEdit.SetWeight(m_Weight);

	return TRUE;
}


void CInputWeight::OnOK()
{
	NullDouble weight;
	weight = m_WeightEdit.GetWeight();
		
	if(m_pValidator && !m_pValidator->IsValid(weight))
	{
		AfxMessageBox(m_pValidator->TellMeWhy(weight));
		return;
	}
	else
	{
		m_Weight = weight;
		EndDialog(IDOK);
		return;
	}	
}


NullDouble CInputWeight::AskForWeight()
{
	if(DoModal() == IDOK)
		return m_Weight;

	return NullDouble();

}

void CInputWeight::SetWeight(const NullDouble& weight)
{
	m_Weight = weight;
}

void CInputWeight::SetValidator(CValidator<const NullDouble&> *pValidator)
{
	m_pValidator = pValidator;
}

void CInputWeight::SetLabel(const CString& sLabel)
{
	m_sLabel = sLabel;
}

void CInputWeight::SetWindowCaption(const CString& sCaption)
{
	m_sCaption = sCaption;
}

