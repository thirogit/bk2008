// GroupMgrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRAWCLI.h"
#include "GroupMgrDlg.h"
#include "DrawDoc.h"


// CGroupMgrDlg dialog

IMPLEMENT_DYNAMIC(CGroupMgrDlg, CDialog)
CGroupMgrDlg::CGroupMgrDlg(CView* pView)
	: CDialog(CGroupMgrDlg::IDD, pView), m_View(pView),m_hAccel(NULL)
{
	VERIFY(m_View);
}

CGroupMgrDlg::~CGroupMgrDlg()
{
}

void CGroupMgrDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_GROUPS,m_groups);
	DDX_Text(pDX,IDC_GROUPNAME,m_groupname);
}


BEGIN_MESSAGE_MAP(CGroupMgrDlg, CDialog)
	ON_NOTIFY(LVN_GETDISPINFO,IDC_GROUPS, GetDispInfo)
	ON_BN_CLICKED(IDC_GROUPADD,OnAddGroup)
	ON_BN_CLICKED(IDC_GROUPDEL,OnDelGroup)
	ON_NOTIFY(NM_DBLCLK ,IDC_GROUPS, OnDblClick)
	ON_COMMAND(IDA_DELGROUP,OnDelGroup)

END_MESSAGE_MAP()


BOOL CGroupMgrDlg::OnInitDialog()
{
	if(!CDialog::OnInitDialog()) return FALSE;

	char szColumns[][20] = {
		"Group name",
		"ID",
		"Printout"
	};

	VERIFY(m_groups.GetStyle() & (LVS_REPORT | LVS_OWNERDATA));
	m_groups.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CDrawDoc *pDoc = (CDrawDoc*)m_View->GetDocument();
	m_grpList = pDoc->GetGroupList();

	CRect rc;
	m_groups.GetClientRect(&rc);
	int colWidth = rc.Width()/3;

	for(int i = 0; i < 3;i++)
	{
		m_groups.InsertColumn(i,szColumns[i],LVCFMT_LEFT,colWidth);
	}
	m_groups.SetItemCount(m_grpList->GetSize());

	// Load accelerator keys...
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(),
                              MAKEINTRESOURCE(IDR_GRPMGRACCEL));
	ASSERT(m_hAccel);


	return TRUE;
}


void CGroupMgrDlg::GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
//	LPARAM pData = pItem->lParam;
	char szBool[][4] = {"YES", "NO" };
	
	CDrawObjGroup *pGroup;
	char *szText = NULL;
	CString temp;

	if (pItem->mask & LVIF_TEXT) 
	{
		
		pGroup = m_grpList->GetAt(m_grpList->FindIndex(pItem->iItem));
		
		switch(pItem->iSubItem)
		{
			case 0:			
						
				szText = pGroup->m_groupname.GetBuffer();
			break;
			case 1:		
				temp.Format("%d",pGroup->m_groupID);
				szText = temp.GetBuffer();
			break;
			case 2:			
				szText = szBool[pGroup->m_PrintVis != true];
			break;
			
		default:
			ASSERT(0);
			break;
		}

		lstrcpy(pItem->pszText,szText);

	}
	
	*pResult = 0;
}

void CGroupMgrDlg::OnAddGroup()
{
	UpdateData();
	POSITION pos;
	CDrawObjGroup *pGroup ;
	if(m_groupname.GetLength() > 0)
	{
		pos = m_grpList->GetHeadPosition();
		while(pos)
		{
			pGroup = m_grpList->GetNext(pos);
			if(pGroup->m_groupname == m_groupname)
			{
				MessageBox("There is already group with this name");
				return;
			}
		}

		
		int id = 1;
		bool duplicate = false;

		while(1)
		{
			duplicate = false;
			pos = m_grpList->GetHeadPosition();
			while(pos)
			{
				pGroup = m_grpList->GetNext(pos);
				if(pGroup->m_groupID == id)
				{
					duplicate = true;
					break;
				}
	
			}
			if(!duplicate) break;
			else id++;
		}
		
		m_grpList->AddHead(new CDrawObjGroup(m_groupname,id));
		m_groups.SetItemCount(m_grpList->GetSize());
	}
	else MessageBox("Group name is empty");


}

void CGroupMgrDlg::OnDelGroup()
{
	CDrawObjGroup *pGroup;
	CDrawObj *pObj;
	POSITION pos = m_groups.GetFirstSelectedItemPosition(),obj,grp;

	int nSelectedItem = -1,i,si;
	if (pos != NULL) nSelectedItem = m_groups.GetNextSelectedItem(pos);
	
	if(nSelectedItem >= 0)
	{
		grp = m_grpList->FindIndex(nSelectedItem);
		pGroup = m_grpList->GetAt(grp);

		CDrawObjList *page;
		CDrawDoc *doc =  ((CDrawDoc*)m_View->GetDocument());
	

		for(i=0,si=doc->GetNumPages();i<si;i++)
		{
			page = doc->GetPageObjects(i);

			obj = page->GetHeadPosition();
			while(obj)
			{
				pObj = page->GetNext(obj);
				if(pObj->m_group == pGroup)
					pObj->m_group = NULL;
			}
		}

		m_grpList->RemoveAt(grp);
		delete pGroup;
		m_groups.SetItemCount(m_grpList->GetSize());

	}


}


void CGroupMgrDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1,nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		nSubItem = pNMIA->iSubItem;
	
	if(nItem >= 0)
	{
		

		if(nSubItem == 2)
		{
			CDrawObjGroup *pGroup;		
			pGroup = m_grpList->GetAt(m_grpList->FindIndex(nItem));
			pGroup->m_PrintVis = !pGroup->m_PrintVis;
			m_groups.Invalidate(FALSE);
		
		}
	
			
	}
	}

	*pResult = 0;
}

BOOL CGroupMgrDlg::PreTranslateMessage(MSG *pMsg)
{
	if(m_hAccel)
	{
		if(::TranslateAccelerator(GetSafeHwnd(), m_hAccel, pMsg))
		return(TRUE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

