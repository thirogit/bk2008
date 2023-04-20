// ObjGroup.cpp : implementation file
//

#include "stdafx.h"
#include "DRAWCLI.h"
#include "ObjGroup.h"
#include "GroupMgrDlg.h"

// CObjGroup dialog

IMPLEMENT_DYNAMIC(CObjGroup, CDialog)
CObjGroup::CObjGroup(CDrawView* pView,CDrawObjGroup *pObjGroup)
	: CDialog(CObjGroup::IDD, pView) , m_View(pView),m_ObjGroup (pObjGroup),m_SelGroup(NULL)
{
	VERIFY(m_View);
}

CObjGroup::~CObjGroup()
{
	
}

void CObjGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GROUPLIST,m_list);
}
void CObjGroup::OnOK()
{
	int nSel = m_list.GetCurSel();
	if(nSel >= 0)
	{
		m_SelGroup = (CDrawObjGroup*)m_list.GetItemData(nSel);
	}
	CDialog::OnOK();
}

CDrawObjGroup* CObjGroup::GetSelectedGroup()
{
	return m_SelGroup;
}

BOOL CObjGroup::OnInitDialog()
{
	if(!CDialog::OnInitDialog()) return FALSE;
	ReloadGroupList(m_ObjGroup);


	return TRUE;
}



BEGIN_MESSAGE_MAP(CObjGroup, CDialog)
	ON_BN_CLICKED(IDC_GROUPMGR,OnGroupMgr)
END_MESSAGE_MAP()

void CObjGroup::OnGroupMgr()
{
	CGroupMgrDlg mgrdlg(m_View);
	mgrdlg.DoModal();
	CDrawObjGroup *pGrp = (CDrawObjGroup*)m_list.GetItemData(m_list.GetCurSel());
	ReloadGroupList(pGrp);


}

void CObjGroup::ReloadGroupList(CDrawObjGroup *pGrp)
{

	CGroupList *pList = m_View->GetDocument()->GetGroupList();
	CDrawObjGroup *pGroup;
	POSITION pos = pList->GetHeadPosition();
	int item,def;

	m_list.ResetContent();
	def = item = m_list.AddString("none");
	m_list.SetItemData(item,NULL);
	while(pos)
	{
		pGroup = pList->GetNext(pos);
		item = m_list.AddString(pGroup->m_groupname);
		m_list.SetItemData(item,(DWORD_PTR)pGroup);
		if(pGroup == pGrp) def = item;
	}
	m_list.SetCurSel(def);

}
// CObjGroup message handlers
