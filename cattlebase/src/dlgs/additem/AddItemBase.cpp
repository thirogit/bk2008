#include "stdafx.h"
#include "AddItemBase.h"


CAddItemBaseDlg::CAddItemBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddItemBaseDlg::IDD, pParent),m_itemList(NULL),m_selIndex(-1)
{
}

CAddItemBaseDlg::~CAddItemBaseDlg()
{
	if(m_itemList)
		delete m_itemList;
}

void CAddItemBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST,*m_itemList);
}


BEGIN_MESSAGE_MAP(CAddItemBaseDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK ,IDC_LIST, OnDblClick)
END_MESSAGE_MAP()


BOOL CAddItemBaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_itemList->UpdateItemCount();
	
	return TRUE;
}
int CAddItemBaseDlg::GetSelectedItemIndex()
{
	return m_selIndex;
}

void CAddItemBaseDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMIA)
	{		
		if(pNMIA->iItem >= 0)
		{
			m_selIndex  = pNMIA->iItem;
			EndDialog(IDOK);			
		}
	}

	*pResult = 0;


}

void CAddItemBaseDlg::OnOK()
{
	m_selIndex = m_itemList->GetCurSel();
	if(m_selIndex < 0)
	{
		AfxMessageBox(IDS_CHOOSEDOCTOADD,MB_OK,0);
		return;
	}


	EndDialog(IDOK);

}