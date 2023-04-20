#include "stdafx.h"
#include "HentMergeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CHentMergeDlg,CMergeDlg)

CHentMergeDlg::CHentMergeDlg(CWnd* pParent /*=NULL*/)
	: CMergeDlg(pParent),m_selectedHent(NULL)
{

}

CHentMergeDlg::~CHentMergeDlg()
{
}


void CHentMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CMergeDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_REPLACEITEMSCOMBO,m_hents);

}


BEGIN_MESSAGE_MAP(CHentMergeDlg, CMergeDlg)
	
END_MESSAGE_MAP()


BOOL CHentMergeDlg::OnInitDialog()
{
	CMergeDlg::OnInitDialog();

	m_hents.ReloadCombo();
	OnReplaceOptRadioBtn();
	return TRUE;
}

void CHentMergeDlg::OnOK()
{
	CMergeDlg::OnOK();
	MERGE_ACTION mergeAct = GetMergeAction();

	if(MERGE_ACTION_REPLACE == mergeAct || MERGE_ACTION_REPLACE_4ALL == mergeAct)
	{
		m_selectedHent = (Hent*)m_hents.GetSelItemData();
		if(m_selectedHent == NULL)
		{
			AfxMessageBox(IDS_MERGEERRSELHENT,MB_OK,0);
			return;
		}
	}
	EndDialog(IDOK);

}

Hent* CHentMergeDlg::GetSelectedHent()
{
	return m_selectedHent; 
}