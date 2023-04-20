// StyleMgr.cpp : implementation file
//

#include "stdafx.h"
#include "DRAWCLI.h"
#include "StyleMgr.h"
#include "DrawDoc.h"
#include ".\stylemgr.h"
#include "styleedit.h"




// CStyleMgr dialog

IMPLEMENT_DYNAMIC(CStyleMgr, CDialog)
CStyleMgr::CStyleMgr(CView *pView)
	: CDialog(CStyleMgr::IDD, pView)//,m_style2edit(NULL)
{
	CDrawDoc *pDoc = (CDrawDoc*)pView->GetDocument();
	m_styleList = pDoc->GetStyleList();
	VERIFY(m_styleList);
}

CStyleMgr::~CStyleMgr()
{
}

void CStyleMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STYLELIST,m_styles);	
}


BEGIN_MESSAGE_MAP(CStyleMgr, CDialog)
	ON_NOTIFY(LVN_GETDISPINFO,IDC_STYLELIST, GetDispInfo)
	ON_BN_CLICKED(IDC_STYLEDEL,OnStyleDel)
	ON_BN_CLICKED(IDC_NEWSTYLE,OnNewStyle)
	ON_NOTIFY(NM_DBLCLK , IDC_STYLELIST, OnDblClick)
	
END_MESSAGE_MAP()

#define MAX_COL_NAME 20

BOOL CStyleMgr::OnInitDialog()
{
	if(!CDialog::OnInitDialog()) return FALSE;

	char szColumns[][MAX_COL_NAME] = {
		"Style Name",
		"Font Face",
		"Font Size",
		"Style",
		"Color",
		"Ver. Align",
		"Hor. Align",
		"Orientation"
	};
	const int nCols = (sizeof(szColumns)/MAX_COL_NAME);

	VERIFY(m_styles.GetStyle() & (LVS_REPORT | LVS_OWNERDATA));
	m_styles.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CRect rc;
	m_styles.GetClientRect(&rc);
	int colWidth = rc.Width()/nCols;

	for(int i = 0; i < nCols;i++)
	{
		m_styles.InsertColumn(i,szColumns[i],LVCFMT_LEFT,colWidth);
	}
	m_styles.SetItemCount(m_styleList->GetSize());

		
	 return TRUE;
}
void CStyleMgr::GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
//	LPARAM pData = pItem->lParam;
//	char szBool[][4] = {"YES", "NO" };
	
	CTextStyle *pStyle;
	char *szText = NULL;
	CString temp;

	if (pItem->mask & LVIF_TEXT) 
	{
		pStyle = m_styleList->GetAt(m_styleList->FindIndex(pItem->iItem));
		
		switch(pItem->iSubItem)
		{
			case 0:			
						
				szText = pStyle->m_stylename.GetBuffer();
			break;
			case 1:		
				szText = pStyle->m_lf.lfFaceName;
			break;
			case 2:			
				temp.Format("%d",-pStyle->m_lf.lfHeight);
				szText = temp.GetBuffer();
			break;
			case 3:
				if(pStyle->m_lf.lfWeight == FW_BOLD)
					temp += "B";
				if(pStyle->m_lf.lfItalic)
					temp += "I";
				if(pStyle->m_lf.lfUnderline)
					temp += "U";
				if(pStyle->m_lf.lfStrikeOut)
					temp += "K";
				szText = temp.GetBuffer();

			break;
			case 4:
				temp.Format("#%02X%02X%02X",GetRValue(pStyle->m_txtclr),
											GetGValue(pStyle->m_txtclr),
											GetBValue(pStyle->m_txtclr));
				szText = temp.GetBuffer();
			break;

			case 5:
				switch(pStyle->m_valign)
				{
					case TOP:
						szText = "TOP";
						break;
					case BOTTOM:
						szText = "BOTTOM";
						break;
					case VCENTER:
						szText = "VCENTER";
						break;
				}
			break;	
			case 6:
				switch(pStyle->m_halign)
				{
					case LEFT:
						szText = "LEFT";
						break;
					case RIGHT:
						szText = "RIGHT";
						break;
					case CENTER:
						szText = "CENTER";
						break;
				}
			break;
			case 7:
				temp.Format("%d",pStyle->m_TextAngle);
					szText = (char*)(LPCSTR)temp;
			break;

			
		default:
			ASSERT(0);
			break;
		}

		lstrcpy(pItem->pszText,szText);

	}
	
	*pResult = 0;
}

void CStyleMgr::OnStyleDel()
{
	POSITION pos = m_styles.GetFirstSelectedItemPosition(),stylePos;

	int nSel;
	CTextStyle *pStyle = NULL;
	while(pos)
	{
		nSel = m_styles.GetNextSelectedItem(pos);
		stylePos = m_styleList->FindIndex(nSel);
		pStyle = m_styleList->GetAt(stylePos);
		if(pStyle->m_styleid != USER_DEF_STYLE_ID)
		{
			m_styles.DeleteItem(nSel);
			m_styleList->RemoveAt(stylePos);
			delete pStyle;
		}
		else 
		{
			MessageBox("You can't delete user defined style");
			return;
		}

	}

}

int CStyleMgr::FindNextStyleID(CStyleList *pStyleList)
{
	int ID = 1;
	POSITION pos;
	CTextStyle *pStyle;
	bool duplicate = false;

		while(1)
		{
			duplicate = false;
			pos = pStyleList->GetHeadPosition();
			while(pos)
			{
				pStyle = pStyleList->GetNext(pos);
				if(pStyle->m_styleid == ID)
				{
					duplicate = true;
					break;
				}
	
			}
			if(!duplicate) break;
			else ID++;
		}
	
	return ID;
}

void CStyleMgr::OnNewStyle()
{
	CString newstylename;
	int newstyleid = FindNextStyleID(m_styleList);
	newstylename.Format("NEWSTYLE%d",newstyleid);
	CTextStyle *pStyle = new CTextStyle(newstylename,newstyleid);

	CStyleEdit styleEdit(pStyle);
	while(1)
	{
superior_while:
		if(styleEdit.DoModal() == IDOK)
		{
			
			POSITION pos = m_styleList->GetHeadPosition();
			CTextStyle *pPosStyle;
			while(pos)
			{
					pPosStyle = m_styleList->GetNext(pos);
					if(pStyle->m_stylename == pPosStyle->m_stylename)
					{
						MessageBox("There is already style with this name");
						goto superior_while;
					}				
			}
			m_styleList->AddTail(pStyle);
			m_styles.SetItemCount(m_styleList->GetSize());	
			break;
		}
		else
		{
			delete pStyle;
			break;
		}
	}
}

void CStyleMgr::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = -1;
	//int nSubItem = -1;
	if (pNMIA)
	{
		nItem = pNMIA->iItem;
		//nSubItem = pNMIA->iSubItem;
		if(nItem >=0)
		{
			
			CTextStyle *m_style2edit = m_styleList->GetAt(m_styleList->FindIndex(nItem));
			if(m_style2edit)
			{
				CStyleEdit styleEdit(m_style2edit);
				if(styleEdit.DoModal() == IDOK)
					Invalidate();
			}
		}

	}


	
	*pResult = 0;
}

