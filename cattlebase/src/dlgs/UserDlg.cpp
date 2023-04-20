#include "stdafx.h"
#include "UserDlg.h"
#include "userdlg.h"
#include "global_arrays.h"
#include "AddUsrDlg.h"
#include "MainCfg.h"
#include "CattleSQL.h"
#include "../datatypes/UserDataComparator.h"


const GRIDHEADERS CUsrList::szUserListCols[] =
{
	{IDS_HEADERNAME,USERS_SORT_BY_USERNAME},
	{IDS_HEADERFORENAME,USERS_SORT_BY_FNAME},
	{IDS_HEADERSURNAME,USERS_SORT_BY_LNAME},
	{IDS_HEADERLASTLOGIN,USERS_SORT_BY_LASTLOGIN},
	{IDS_HEADERPRIVS,USERS_SORT_BY_PRIVS},
	{0,0}
};




CUsrList::CUsrList() : CBaseListCtrl(szUserListCols,CFGID_USERSLIST)
{
}
void CUsrList::UpdateItemCount()
{
		SetItemCount(ary_users.GetSize());
}

void CUsrList::SortByCol(int Column,bool bAsc)
{
	ary_users.SortBy(GetColID(Column),bAsc);

}


void CUsrList::GetItemDispText(int nItem,int nSubItem,char *pszItemText)
{
	char szPrivs[][5] = 
	{
		"VW","AMO","MF","DO","DF","PRT","ADM","INV"
	};
	UserData *pUser	= ary_users[nItem];
	const char *szText = NULL;
	CString temp;

			
		switch(GetColID(nSubItem))
		{
			case USERS_SORT_BY_USERNAME:			
					
				szText = (LPCSTR)pUser->username;
			break;
			case USERS_SORT_BY_FNAME:			
						
				szText = (LPCSTR)pUser->fname;
			break;
			case USERS_SORT_BY_LNAME:			
						
				szText = (LPCSTR)pUser->lname;
			break;
			case USERS_SORT_BY_LASTLOGIN:		
				if(pUser->lastlogin.IsNull())
				 temp.LoadString(IDS_FIRSTLOGIN);
				else
				 temp = pUser->lastlogin.GetFullDate();
				
				szText = (LPCSTR)temp;
			break;
			case USERS_SORT_BY_PRIVS:			
				temp = szPrivs[0];
				for(int i = 0;i< UserData::PRIVS_COUNT;i++)
					if(pUser->privledges & (1 << i))
					{
						temp += "\\";
						temp += szPrivs[i+1];
						//if(i < (PRIVS_COUNT-1))
					}
					szText = (LPCSTR)temp;
			break;
		
		default:
			ASSERT(0);
			break;
		}

		lstrcpy(pszItemText,szText);
	
}




// CUserDlg dialog

IMPLEMENT_DYNAMIC(CUserDlg, CPropertyPage)
CUserDlg::CUserDlg()
	: CPropertyPage(CUserDlg::IDD)
{
}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_USERLIST,m_users);
}


BEGIN_MESSAGE_MAP(CUserDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_ADDUSER, OnAdduser)
	ON_BN_CLICKED(IDC_DELUSER, OnDelUser)
	ON_BN_CLICKED(IDC_CHANGE, OnChange)
	ON_NOTIFY(NM_DBLCLK ,IDC_USERLIST, OnDblClick)
END_MESSAGE_MAP()


BOOL CUserDlg::OnInitDialog()
{

	if(!CDialog::OnInitDialog()) return FALSE;
	m_users.UpdateItemCount();

	return TRUE;
}

void CUserDlg::OnAdduser()
{
	UserData *pUser = new UserData;
	CAddUsrDlg addusrdlg(pUser);
	
	while(true)
	{
		if(addusrdlg.DoModal() == IDOK)
		{
			if(pUser->InsertThisUser())
			{
				ary_users.Add(pUser);
				m_users.UpdateItemCount();
				break;
			} else
			{
				SQLDB.SQLMessageBox(IDS_ERRUSERNAME);			 
				addusrdlg.EnableEditing();
				continue;
			}
		}
		else 
		{
			delete pUser;
			break;
		}
	}
		
}

void CUserDlg::OnDelUser()
{
	UserData *pUser;
	POSITION pos = m_users.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL) nSelectedItem = m_users.GetNextSelectedItem(pos);
	
	if(nSelectedItem >= 0)
	{
		pUser = ary_users[nSelectedItem];
		if(pUser == UserData::GetCurrentUser())
		{
			AfxMessageBox(IDS_CANDELCURUSER,MB_OK,0);
			return;
		}
		
	    if(ary_users[nSelectedItem]->DeleteThisUser())
		{		
			ary_users.RemoveAt(nSelectedItem);
			delete pUser;
			m_users.UpdateItemCount();
		}
		else
		{
			AfxMessageBox(IDS_ERRCANTDELUSR,MB_OK,0);
		}
	}	

}

void CUserDlg::OnChange()
{
	UserData *pUser = NULL,*pCopy = NULL;

	POSITION pos = m_users.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL) nSelectedItem = m_users.GetNextSelectedItem(pos);

	if(nSelectedItem >= 0)
	{
		pUser =  ary_users[nSelectedItem];
		pUser->SQLUpdate.Reset();
		pCopy = new UserData(*pUser);

	
       	CAddUsrDlg addusrdlg(pCopy);
		while(true)
		{
			if(addusrdlg.DoModal() == IDOK)
			{

					if(HAS_PRIV(pUser,UserData::ADMIN) && !HAS_PRIV(pCopy,UserData::ADMIN))
					{
						bool bNoMoreAdmin = true;
						for(int i = 0;i < ary_users.GetSize();i++)
							if(ary_users[i] != pUser &&
								HAS_PRIV(ary_users[i],UserData::ADMIN))
									bNoMoreAdmin = false;
						
						if(bNoMoreAdmin)
						{
							AfxMessageBox(IDS_ERRNEEDADMIN,MB_OK,0);
							continue;
						}
					}

				if(pCopy->UpdateThisUser())
				{
					*pUser = *pCopy;
					break;
				} else
				{
					AfxMessageBox(IDS_ERREDITUSR,MB_OK,0);			 
					addusrdlg.EnableEditing();
					continue;
				}
			}
			else break;
			
		}
		delete pCopy;
	}
	
}

void CUserDlg::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMIA)
		if(pNMIA->iItem >= 0)
			OnChange();
		
}