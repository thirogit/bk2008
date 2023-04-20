#include "stdafx.h"
#include "ChooseCowEANDlg.h"
#include "MainCfg.h"

const GRIDHEADERS CChooseCowEANDlg::m_ChooseCowEANListHeader[] =
{	
	{IDS_HEADERCOWNO,COWS_SORT_BY_EAN},
	{0,0}
};

BEGIN_MESSAGE_MAP(CChooseCowEANDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK , IDC_COWLIST, OnDblClick)

END_MESSAGE_MAP()

CChooseCowEANDlg::CChooseCowEANDlg(CCowArray &cowArray,CWnd* pParent /*=NULL*/)
	: CDialog(CChooseCowEANDlg::IDD, pParent),m_cowList(&m_cowArray,m_ChooseCowEANListHeader,CFGID_CHOOSECOWLIST)
{
	m_cowArray.Append(cowArray);
	m_pSelectedCow = NULL;
}

CChooseCowEANDlg::~CChooseCowEANDlg()
{
}

void CChooseCowEANDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COWLIST,m_cowList);
}

BOOL CChooseCowEANDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_cowList.UpdateItemCount();
	return TRUE;
}

void CChooseCowEANDlg::OnOK()
{
	int curSel = m_cowList.GetCurSel();
	if(curSel < 0)
	{
		AfxMessageBox(IDS_ERRCHOOSECOWEAN,MB_OK,0);
		return;
	}
	m_pSelectedCow = m_cowArray[curSel];
	EndDialog(IDOK);
}

void CChooseCowEANDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
	
		if(nItem >= 0)
		{
			m_pSelectedCow = m_cowArray[nItem];
			EndDialog(IDOK);
		}
	}
	*pResult = 0;
}



Cow* CChooseCowEANDlg::ChooseCowEAN(CCowArray &inputArray,CWnd* pParent/* = NULL*/)
{
	CChooseCowEANDlg chooseCow(inputArray,pParent);
	if(chooseCow.DoModal() == IDOK)
	{		
		return chooseCow.m_pSelectedCow;
	}	
	return NULL;
}