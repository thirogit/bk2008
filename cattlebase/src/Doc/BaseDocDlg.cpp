#include "stdafx.h"
#include "BaseDocDlg.h"
#include "MainCfg.h"
#include "../datalimits.h"
#include "../Utils/StringFormat.h"
#include "../classes/TempStringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CBaseDocDlg, CDialog )
	
			ON_BN_CLICKED(IDC_ADDCOWS, OnAddCows)			
			ON_BN_CLICKED(IDC_COWDELETE,OnDeleteCow)
			ON_NOTIFY(NM_DBLCLK , IDC_COWLIST, OnDblClick)
			ON_BN_CLICKED(IDC_CLEARAGENT, OnClearAgent)				
			ON_BN_CLICKED(IDCANCEL,OnCancel)
			ON_BN_CLICKED(IDC_GETCOWS,OnGetCows)			
			ON_NOTIFY(NM_RCLICK, IDC_COWLIST, OnRClick)
			ON_WM_CLOSE()
			ON_COMMAND(IDM_COWLIST_EDITCOW,OnMenuEditCow)
			ON_COMMAND(IDM_COWLIST_SHOWPASSIMG,OnMenuShowPass)
			ON_BN_CLICKED(IDC_FINDPASSPORTS,OnFindPass)
			ON_COMMAND_RANGE(IDM_OUTGRPMENU_BASE,IDM_OUTGRPMENU_MAX,OnGrpChange)
			ON_COMMAND_RANGE(IDM_OUTGRPMENU_NOGRP,IDM_OUTGRPMENU_NOGRP,OnGrpChange)
			ON_BN_CLICKED(IDC_SETLOADTIMESTART,OnSetLoadTimeStart)
			ON_BN_CLICKED(IDC_SETLOADTIMEEND,OnSetLoadTimeEnd)
			ON_BN_CLICKED(IDC_SETPLATENOMENUBTN,OnPlateNoMenuBtn)
			ON_COMMAND(IDM_SETPLATENOFROMAGENT,OnSetPlateNoFromAgent)
			ON_COMMAND(IDM_SETPLATENOFROMHENT,OnSetPlateNoFromHent)
	
END_MESSAGE_MAP()

CBaseDocDlg::CBaseDocDlg(CBaseListCtrl *pListCtrl,Reason::REASON_INOUT_TYPE reasonType,CowAccessor* pGrpAccessor) : 
		CDialog(CBaseDocDlg::IDD),m_cowlist(pListCtrl),	m_reasonCombo(reasonType),m_GrpSummaryList(pGrpAccessor,m_cows)
{
	ASSERT(pListCtrl);	
}
CBaseDocDlg::~CBaseDocDlg()
{
}


void CBaseDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog ::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COWLIST, *m_cowlist);
	DDX_Control (pDX, IDOK  , m_OK  ) ;
	DDX_Control (pDX, IDC_COWDELETE, m_Delete  ) ;
	DDX_Control (pDX, IDC_DOCDATE, m_DocDate ) ;
	DDX_Control (pDX, IDC_TRANSPDATE, m_TransportDate ) ;
	DDX_Control (pDX,IDCANCEL,m_Cancel);
	DDX_Control (pDX,IDC_HENTCOMBO,m_hentCombo);
	DDX_Text	(pDX,IDC_PLATENO,m_plateNo);
	DDX_Control (pDX,IDC_EXTRAS,m_extras);
	DDX_Control (pDX,IDC_REASON,m_reasonCombo);
	DDX_Control(pDX,IDC_FINDPASSPORTS,m_FindPass);
	DDX_Control(pDX,IDC_OUTGRPSUMMARYLIST,m_GrpSummaryList);
	DDX_Control(pDX,IDC_CURRENTGROUP,m_activeGrpCombo);
	DDX_Control(pDX,IDC_LOADTIMESTART,m_loadStartTm);
	DDX_Control(pDX,IDC_LOADTIMEEND,m_loadEndTm);
	DDX_Control(pDX,IDC_AGENTCOMBO,m_AgentCb);
	
}

BOOL  CBaseDocDlg::OnInitDialog()
{

	CDialog ::OnInitDialog();

	SetWindowText((LPCSTR)GetDocDlgCaption());
	SendDlgItemMessage(IDC_PLATENO,EM_LIMITTEXT,MAXPLATE);

	m_Cancel.SetButtonImage( IDB_CANCEL );
	m_OK.SetButtonImage( IDB_INDOCADD );
	m_Delete.SetButtonImage( IDB_TRASH );
	m_FindPass.SetButtonImage(IDB_BARCODE);

	m_listMenu.LoadMenu(IDR_COWLISTPOPMENU);

	ModifyListMenu(m_listMenu);

	SetupBtns();
	
	m_activeGrpCombo.InitCombo();

	m_loadStartTm.SetAbortWnd(&m_Cancel);
	m_loadEndTm.SetAbortWnd(&m_Cancel);

	m_AgentCb.ReloadCombo();

	

	return TRUE ;
}

void  CBaseDocDlg::OnAddCows()
{
	AddCow();
	UpdateCowCount();
}

void CBaseDocDlg::OnOK()
{
		UpdateData();
		if(!m_hentCombo.GetSelectedHent())
		{		
			AfxMessageBox(IDS_ERRNODELIVER,MB_OK,0);
			return;
		}


		if(AppCfg().GetBool(CFG_WARNEMPTYPLATENO) && m_plateNo.IsEmpty())
		{
			AfxMessageBox(IDS_ERREMPTYPLATENO,MB_OK,0);
		}

		if(!m_cowlist->GetItemCount())
		{
			AfxMessageBox(IDS_EMPTYCOWLIST,MB_OK,0);
			return;
		}

		if(m_DocDate.IsInputEmpty()) 
		{
				AfxMessageBox(IDS_ERRNODOCDATE,MB_OK,0);
				return;
		}
		else
		{
			if(!m_DocDate.IsDateValid())
			{
				AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
				m_DocDate.SetFocus();
				return;
			}
		}

		if(m_TransportDate.IsInputEmpty()) 
		{
				AfxMessageBox(IDS_ERRNOLOADDATE,MB_OK,0);
				return;
		}
		else
		{
			if(!m_TransportDate.IsDateValid())
			{
				AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
				m_TransportDate.SetFocus();
				return;
			}
		
		}

	
		if(!m_reasonCombo.GetSelReason())
		{
			AfxMessageBox(IDS_ERRNOINREASON,MB_OK,0);
			return;
		}
	
		if(CommitDoc())
			EndDialog(IDOK);
}
void CBaseDocDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if (pNMIA && pNMIA->iItem >= 0)
	{
		EditCow(pNMIA->iItem);	
		m_cowlist->Invalidate();
	}
	*pResult = 0;
}

void CBaseDocDlg::OnDeleteCow()
{
	int item = m_cowlist->GetCurSel();
	if(item >= 0)
	{
		m_cowlist->SetRedraw(FALSE);
		DeleteCow(item);
		m_cowlist->SetRedraw(TRUE);
		UpdateCowCount();
	}
}

void CBaseDocDlg::UpdateCowCount()
{
	m_cowlist->UpdateItemCount();	
	SetDlgItemInt(IDC_COWCOUNT,m_cows.GetSize());
}

void CBaseDocDlg::SetPlateNo(const CString& sPlateNo)
{
	SetDlgItemText(IDC_PLATENO,(LPCSTR)sPlateNo);
}

void CBaseDocDlg::OnGetCows()
{
	GetCows();
}

 void CBaseDocDlg::OnCancel()
 {
	 CancelDoc();
	 CDialog::OnCancel();
 }

void CBaseDocDlg::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMIA && pNMIA->iItem >= 0)
	{
		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		RightClick(ptCursor);				
	}
	*pResult = 0;
}

void CBaseDocDlg::RightClick(CPoint &ptCursor)
{
	CMenu* popup = m_listMenu.GetSubMenu(0);
	popup->TrackPopupMenu(TPM_LEFTALIGN,ptCursor.x,ptCursor.y,this,NULL);
}


void CBaseDocDlg::OnMenuEditCow()
{
	int item = m_cowlist->GetCurSel();
	if(item >= 0)
	{
		EditCow(item);
		m_cowlist->Invalidate();
	}	
}

void CBaseDocDlg::OnMenuShowPass()
{
	int item = m_cowlist->GetCurSel();
	if(item >= 0)
	{
		ShowPass(item);
	}
	
}

void CBaseDocDlg::OnClose()
{
	OnCancel();
}
void CBaseDocDlg::OnFindPass()
{
	FindPass();
}

NullDoc* CBaseDocDlg::CreateNewDoc()
{
	NullDoc *pDoc = new NullDoc();
	pDoc->SetPlateNo(m_plateNo);

	CDateTime docDt;
	m_DocDate.GetDate(docDt);
	pDoc->SetDocDate(docDt);

	CDateTime transportDt;
	m_TransportDate.GetDate(transportDt);
	pDoc->SetLoadDate(transportDt);
	
	CString sExtras;
	m_extras.GetWindowText(sExtras);
	pDoc->SetExtras(sExtras);

	pDoc->SetHent(m_hentCombo.GetSelectedHent());
	pDoc->SetReason(m_reasonCombo.GetSelReason());

	pDoc->SetLoadStartTime(m_loadStartTm.GetTime());
	pDoc->SetLoadEndTime(m_loadEndTm.GetTime());

	pDoc->SetAgent(m_AgentCb.GetSelItemData());
	return pDoc;

}


void CBaseDocDlg::UpdateDoc(Doc *pDoc)
{
	Doc *pNewDoc = CreateNewDoc();

	pDoc->ChangePlateNo(pNewDoc->GetPlateNo());
	pDoc->ChangeDocDate(pNewDoc->GetDocDate());
	pDoc->ChangeLoadDate(pNewDoc->GetLoadDate());
	pDoc->ChangeHent(pNewDoc->GetHent());
	pDoc->ChangeExtras(pNewDoc->GetExtras());
	pDoc->ChangeReason(pNewDoc->GetReason());
	pDoc->ChangeLoadStartTime(pNewDoc->GetLoadStartTime());
	pDoc->ChangeLoadEndTime(pNewDoc->GetLoadEndTime());
	pDoc->ChangeAgent(pNewDoc->GetAgent());
	
	delete pNewDoc;
}

void CBaseDocDlg::UpdateSummary()
{
	m_GrpSummaryList.UpdateSummary();
}
int CBaseDocDlg::GetActiveGroup()
{
	return m_activeGrpCombo.GetSelOutGroup();	
}
void CBaseDocDlg::ModifyListMenu(CMenu &listMenu)
{
	CMenu grpMenu;
	grpMenu.CreatePopupMenu();
	CString menuStr;
	for(int i = MAX_INOUT_GRP;i >= 0 ;i--)
	{
		menuStr.Format("%d",i);
		grpMenu.InsertMenu(0, MF_BYPOSITION,IDM_OUTGRPMENU_BASE+i ,(LPCSTR)menuStr);	
	}

	menuStr.LoadStringA(IDS_NOINOUTGRP);
	grpMenu.InsertMenu(0, MF_BYPOSITION,IDM_OUTGRPMENU_NOGRP ,(LPCSTR)menuStr);

	menuStr.LoadString(IDS_OUTDOC_CHNGGRP_MENUTEXT);
	CMenu* pSubMenu = m_listMenu.GetSubMenu(0);
	pSubMenu->InsertMenu(-1,MF_BYPOSITION | MF_POPUP, (UINT)grpMenu.GetSafeHmenu(), (LPCSTR)menuStr);
}

void CBaseDocDlg::OnGrpChange(UINT grpItemId)
{
	
	POSITION pos;
	int selItem;
	Cow *pCow;
	
		pos = m_cowlist->GetFirstSelectedItemPosition();
		while(pos)
		{
			selItem = m_cowlist->GetNextSelectedItem(pos);
			pCow = m_cows[selItem];

			if(grpItemId == IDM_OUTGRPMENU_NOGRP)
				ChangeGrp4Cow(pCow,NULL_INOUT_GRP);
			else if((grpItemId-IDM_OUTGRPMENU_BASE) <= MAX_INOUT_GRP)
			{	
				ChangeGrp4Cow(pCow,(grpItemId-IDM_OUTGRPMENU_BASE));
			}
		}	
		m_cowlist->Invalidate();
	
	UpdateSummary();

}

void CBaseDocDlg::CommitChangesForCows(CCowArray& cows)
{
	CCowArray::ArrayIterator it = cows.iterator();

	while(it.hasNext()) 
		(*(it++))->CommitChanges();
}
void CBaseDocDlg::RollbackChangesForCows(CCowArray& cows)
{
	CCowArray::ArrayIterator it = cows.iterator();

	while(it.hasNext()) 
		(*(it++))->RollbackChanges();
}

void CBaseDocDlg::OnSetLoadTimeStart()
{
	m_loadStartTm.SetTime(Time::now());
}

void CBaseDocDlg::OnSetLoadTimeEnd()
{
	m_loadEndTm.SetTime(Time::now());
}

void CBaseDocDlg::OnPlateNoMenuBtn()
{
	m_PlateNoMenu.DestroyMenu();
	m_PlateNoMenu.CreatePopupMenu();
	LPCSTR pszItemTextFormat = "%s - %s";

	Hent* pHent = m_hentCombo.GetSelectedHent();
	if(pHent != NULL)
	{
		CString sPlateNo = pHent->GetPlate();
		if(!sPlateNo.IsEmpty())
		{
			m_PlateNoMenu.AppendMenu(MF_STRING, IDM_SETPLATENOFROMHENT, 
				(LPCTSTR)CStringFormat(pszItemTextFormat,(LPCSTR)sPlateNo,(LPCSTR)pHent->GetAlias()));
		}
	}

	Agent* pAgent = m_AgentCb.GetSelItemData();
	if(pAgent)
	{
		CString sPlateNo = pAgent->GetPlateNo();
		if(!sPlateNo.IsEmpty())
		{
			m_PlateNoMenu.AppendMenu(MF_STRING, IDM_SETPLATENOFROMAGENT, 
				(LPCTSTR)CStringFormat(pszItemTextFormat,(LPCSTR)sPlateNo,(LPCSTR)pAgent->GetAgentCode()));
		}
	}

	
	if(m_PlateNoMenu.GetMenuItemCount() == 0)
	{
		m_PlateNoMenu.AppendMenu(MF_STRING | MF_DISABLED, WM_APP, 
				(LPCSTR)CTempStringLoader(IDS_NO_PLATE_NUMBERS));
	}


	CRect rc;
	GetDlgItem(IDC_SETPLATENOMENUBTN)->GetWindowRect(rc);

	int x = rc.left;
	int y = rc.top;

	m_PlateNoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,x,y,this);

}

void CBaseDocDlg::OnSetPlateNoFromHent()
{
	Hent* pHent = m_hentCombo.GetSelectedHent();
	if(pHent != NULL)
	{
		CString sPlateNo = pHent->GetPlate();
		if(!sPlateNo.IsEmpty())
		{
			SetPlateNo(sPlateNo);		
		}
	}
}

void CBaseDocDlg::OnSetPlateNoFromAgent()
{
	Agent* pAgent = m_AgentCb.GetSelItemData();
	if(pAgent)
	{
		CString sPlateNo = pAgent->GetPlateNo();
		if(!sPlateNo.IsEmpty())
		{
			SetPlateNo(sPlateNo);	
		}
	}
}

void CBaseDocDlg::OnClearAgent()
{
	m_AgentCb.SetCurSel(-1);
}