#include "stdafx.h"
#include "CustomPrintDocOptDlg.h"


CCustomPrintDocOptDlg::CCustomPrintDocOptDlg(UINT nIDTemplate,CWnd *pParentWnd/* = NULL*/) : 
			CDialog(nIDTemplate,pParentWnd),m_nCfgID(0)
{
}

void CCustomPrintDocOptDlg::SetCfgItemID(UINT nID)
{
	m_nCfgID = nID;
}

UINT CCustomPrintDocOptDlg::GetCfgItemID() const
{
	return m_nCfgID;
}

bool CCustomPrintDocOptDlg::Validate()
{
	return true;
}