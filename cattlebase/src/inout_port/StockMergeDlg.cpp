#include "stdafx.h"
#include "StockMergeDlg.h"

IMPLEMENT_DYNAMIC(CStockMergeDlg,CMergeDlg)

CStockMergeDlg::CStockMergeDlg(CWnd* pParent /*=NULL*/)
	: CMergeDlg(pParent),m_selectedStock(NULL)
{

}

CStockMergeDlg::~CStockMergeDlg()
{
}


void CStockMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CMergeDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_REPLACEITEMSCOMBO,m_stocks);

}


BEGIN_MESSAGE_MAP(CStockMergeDlg, CMergeDlg)
	
END_MESSAGE_MAP()


BOOL CStockMergeDlg::OnInitDialog()
{
	CMergeDlg::OnInitDialog();

	m_stocks.ReloadCombo();
	OnReplaceOptRadioBtn();
	return TRUE;
}

void CStockMergeDlg::OnOK()
{
	CMergeDlg::OnOK();
	MERGE_ACTION mergeAct = GetMergeAction();

	if(MERGE_ACTION_REPLACE == mergeAct || MERGE_ACTION_REPLACE_4ALL == mergeAct)
	{
		m_selectedStock = (Stock*)m_stocks.GetSelItemData();
		if(m_selectedStock == NULL)
		{
			AfxMessageBox(IDS_ERRSELSTOCK,MB_OK,0);
			return;
		}
	}
	EndDialog(IDOK);

}