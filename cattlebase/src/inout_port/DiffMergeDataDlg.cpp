#include "stdafx.h"
#include "DiffMergeDataDlg.h"

IMPLEMENT_DYNAMIC(CDiffMergeDataDlg, CDialog)

BEGIN_MESSAGE_MAP(CDiffMergeDataDlg, CDialog)
	ON_BN_CLICKED(IDC_SELECTALL,OnSelectAll)
END_MESSAGE_MAP()


CDiffMergeDataDlg::CDiffMergeDataDlg(DiffMemberArray& diffArray,CWnd* pParent /*=NULL*/)
	: CDialog(CDiffMergeDataDlg::IDD, pParent),m_diffList(diffArray),m_diffArray(diffArray)
{	
}

CDiffMergeDataDlg::~CDiffMergeDataDlg()
{
}

void CDiffMergeDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_DIFFLIST,m_diffList);
}

BOOL CDiffMergeDataDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_diffList.UpdateItemCount();

	return TRUE;
}
void CDiffMergeDataDlg::OnSelectAll()
{
	DiffMemberArray::ArrayIterator it = m_diffArray.iterator();
	while(it.hasNext())
		(*(it++))->SetDoCopy(true);
	m_diffList.Invalidate(FALSE);
}