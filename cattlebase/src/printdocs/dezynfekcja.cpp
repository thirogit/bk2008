#include "stdafx.h"
#include "dezynfekcja.h"

#include "../logo/Logo.h"



IMPLEMENT_DYNCREATE(CDezynfDocOptDlg, CCustomPrintDocOptDlg)
CDezynfDocOptDlg::CDezynfDocOptDlg (CWnd *pParent/* = NULL*/) : 
	CCustomPrintDocOptDlg(IDD,pParent) 
{
	
}

BOOL CDezynfDocOptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_bDontFillPlateNo = PrintCfg().GetBool(m_nCfgID,CDezynfekcjaDoc::CFG_DONTFILLPLATENO);
	m_agentName  = PrintCfg().GetString(m_nCfgID,CDezynfekcjaDoc::CFG_AGENTNAME);

	return TRUE;
}
void CDezynfDocOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CCustomPrintDocOptDlg::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_AGENTNAME,m_agentName);
	DDX_Check(pDX,IDC_DONTFILLPLATENO,m_bDontFillPlateNo);
}
void CDezynfDocOptDlg::Save()
{
	UpdateData();
	PrintCfg().SetBool(m_nCfgID,CDezynfekcjaDoc::CFG_DONTFILLPLATENO,m_bDontFillPlateNo);
	PrintCfg().SetString(m_nCfgID,CDezynfekcjaDoc::CFG_AGENTNAME,m_agentName);
}

BEGIN_MESSAGE_MAP(CDezynfDocOptDlg, CCustomPrintDocOptDlg)
	
END_MESSAGE_MAP()
//#######################################################################################

IMPLEMENT_DYNCREATE(CDezynfekcjaDoc, CCowDrwDoc)

CDezynfekcjaDoc::CDezynfekcjaDoc() : m_nPages(0)
{    
}
int CDezynfekcjaDoc::LoadInputData(CPtrArray &docInputData)
{
	m_docs.RemoveAll();
	m_docs.Append((CDocDataArray&)docInputData);
	m_nPages = m_docs.GetSize();
	m_bDontFillPlateNo = PrintCfg().GetBool(GetCfgItemID(),CFG_DONTFILLPLATENO);
	m_agentName  = PrintCfg().GetString(GetCfgItemID(),CFG_AGENTNAME);

	return RELOAD_OK_CONTINUE;
}

int CDezynfekcjaDoc::GetPrintPages() const
{	
    return m_nPages;		
}

void CDezynfekcjaDoc::PreparePage(int nPage)
{	
	COleStreamFile logoStream;
	if(CLogoImage::GetLogo()->GetOleLogoImage(logoStream))
		DrawImage("LOGO",logoStream);
}

void CDezynfekcjaDoc::PrintPage(int page)
{
	Doc *pDoc = m_docs[page-1];
	DrawText("EVENTDAY",pDoc->GetLoadDate().GetDayDate());
	if(!m_bDontFillPlateNo)
		DrawText("PLATENO",pDoc->GetPlateNo());

	DrawText("CHEMICAL_AGENT",m_agentName);
}
