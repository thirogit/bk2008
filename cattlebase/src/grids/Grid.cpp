#include "stdafx.h"
#include "Grid.h"
#include "MainCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define MAX_FONT_HEIGHT 16
#define MIN_FONT_HEIGHT 12

BEGIN_MESSAGE_MAP(CGrid, CListView)
	
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, GetDispInfo)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw )
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK,OnDbClick)

	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColClick)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	//ON_NOTIFY_REFLECT(LVN_GETINFOTIP, OnLvnGetInfoTip)
	
END_MESSAGE_MAP()


CGrid::CGrid(const GRIDHEADERS *ppHeaders, int pConfigItemId) 
{   
	VERIFY(ppHeaders);
	pHeaders = ppHeaders;
	iHeaderCount = 0;
	while(ppHeaders[iHeaderCount].nColID || ppHeaders[iHeaderCount].nResID)
		iHeaderCount++;

	
	ConfigItemId = pConfigItemId;

	ColSortAsc = new bool[iHeaderCount];
	memset(ColSortAsc,0,sizeof(bool)*iHeaderCount);
	m_bToolTipCtrlCustomizeDone = FALSE;
	m_shiftPressLastSort = false;
	m_pGridFont =  new CFont();;

}

void CGrid::SetCurPos(int nRow)
{
	CListCtrl &m_list = GetListCtrl();
	if(nRow >= 0 && nRow < m_list.GetItemCount())
	{
		CRect Rect;
		int headerHeight;
		m_list.GetHeaderCtrl()->GetItemRect(0,&Rect);
		headerHeight = Rect.Height();

		m_list.GetItemRect (nRow, &Rect, LVIR_BOUNDS);
		CSize Size(0,Rect.top-headerHeight);
		m_list.Scroll (Size);
	}	
}

int CGrid::GetCurSel(void)
{
	CListCtrl &m_list = GetListCtrl();
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = m_list.GetNextSelectedItem(pos);
	return nSelectedItem;
	//m_list.GetItemState(
}

void CGrid::SelectAll()
{
	
	CListCtrl &m_list = GetListCtrl();
	
	for(int i = 0;i< m_list.GetItemCount();i++)
		m_list.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
		//mlist.SetItem(i, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0);

}


void CGrid::SelectRow(int nRow)
{
	CListCtrl &m_list = GetListCtrl();
	if(nRow >= 0 && nRow < m_list.GetItemCount())
		m_list.SetItemState(nRow,LVIS_SELECTED,LVIS_SELECTED);
}
void CGrid::InvertSelection()
{
	CListCtrl &m_list = GetListCtrl();
	for(int i = 0;i< m_list.GetItemCount();i++)
	{
		if(m_list.GetItemState(i,LVIS_SELECTED) & LVIS_SELECTED)
			m_list.SetItemState(i,0,LVIS_SELECTED);
		else		
			m_list.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);

	}
}

BOOL CGrid::PreCreateWindow(CREATESTRUCT &cs)
{
	cs.style |= LVS_REPORT | LVS_OWNERDATA;
	return CListView::PreCreateWindow(cs);		
}

BOOL CGrid::Create(LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName, DWORD dwStyle,
	const RECT& rect,
	CWnd* pParentWnd, UINT nID,
	CCreateContext* pContext)
{
	if(!CListView::Create(lpszClassName,lpszWindowName,dwStyle,rect,pParentWnd,nID,pContext))
		return FALSE;
	
	

	return m_headerCtrl.SubclassWindow(GetListCtrl().GetHeaderCtrl()->GetSafeHwnd());

}

int CGrid::GetItemCount() const
{
	return GetListCtrl().GetItemCount();
}


int CGrid::GetMaxFontSize() const
{
	return MAX_FONT_HEIGHT;
}

int CGrid::GetMinFontSize() const
{
	return MIN_FONT_HEIGHT;
}

int CGrid::GetFontHeight() const
{
	return m_FontHeight;
}

void CGrid::SetFontHeight(int iHeight)
{
	int validHeight = max(min(iHeight,GetMaxFontSize()),GetMinFontSize());
	
	if(m_FontHeight != validHeight)
	{
		m_FontHeight = validHeight;

		CFont* pNewFont = new CFont();
		LOGFONT lf;

		m_pGridFont->GetLogFont(&lf);
		lf.lfHeight = -m_FontHeight;

		pNewFont->CreateFontIndirect(&lf);
		SetFont(pNewFont,TRUE);

		delete m_pGridFont;
		m_pGridFont = pNewFont;
	}
}

UINT CGrid::GetColID(int nCol)
{

	ASSERT(nCol < iHeaderCount);
	if(nCol < iHeaderCount)
		return pHeaders[nCol].nColID;
	return (UINT)-1;
}

void CGrid::OnInitialUpdate()
{

	CString strColText;
	CListCtrl &m_list = GetListCtrl();

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES /*| LVS_EX_INFOTIP*/ );
	m_list.ModifyStyle(0,LVS_SHOWSELALWAYS);

	int *colwidths = new int[iHeaderCount];
	memset(colwidths,0,sizeof(int)*iHeaderCount);

	CDC *dc = m_list.GetDC();
	int i;
	LV_COLUMN lvcolumn;
	

	// add columns
	for (i = 0;i < iHeaderCount ; i++)
	{
		strColText.LoadString(pHeaders[i].nResID);
		if(!ConfigItemId) 
			colwidths[i] = dc->GetTextExtent(strColText).cx;
		else
		{
			colwidths[i] = AppCfg().GetNum(ConfigItemId,i+1);
			if(colwidths[i] <= 0)
				colwidths[i] = dc->GetTextExtent(strColText).cx;
		}

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = strColText.GetBuffer();		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidths[i];
		m_list.InsertColumn(i, &lvcolumn);
		strColText.ReleaseBuffer();
	}

	m_list.ReleaseDC(dc);
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	// call EnableToolTips to enable tooltip display
	EnableToolTips(TRUE);

	
	/*CToolTipCtrl* tooltip = m_list.GetToolTips();
	if (tooltip)
	{		
		tooltip->ModifyStyle(0, TTS_NOPREFIX);
		tooltip->SetDelayTime(TTDT_AUTOPOP, 20000);
		tooltip->SetDelayTime(TTDT_INITIAL, 3*1000);
		
	}*/

	m_list.DeleteAllItems();


	m_sortImageList.Create(14,14,ILC_COLORDDB | ILC_MASK,2,1);
	CBitmap sortArrowsBmp;
	sortArrowsBmp.LoadBitmap(IDB_SORTARROWS);
	m_sortImageList.Add(&sortArrowsBmp,RGB(255,0,255));
	//m_sortImageList.Create(IDB_SORTARROWS,14,1,RGB(255,0,255));

	CHeaderCtrl *m_pHdrCtrl = GetListCtrl().GetHeaderCtrl();
	m_pHdrCtrl->SetImageList(&m_sortImageList);	
	delete[] colwidths;

	
	
	LOGFONT logfont;
	CFont* pFont = GetFont();
	pFont->GetLogFont(&logfont);	
	m_FontHeight = abs(logfont.lfHeight);
	m_pGridFont->CreateFontIndirect(&logfont);
	
	int cfgFontHeight = m_FontHeight;
	if(ConfigItemId) 
		cfgFontHeight = AppCfg().GetNum(ConfigItemId,0);

	SetFontHeight(cfgFontHeight);

	PostInit();

}

void CGrid::GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
	LPARAM pData = pItem->lParam;

	try
	{
		if (pItem->mask & LVIF_TEXT) 
		{
			GetData(pItem->iItem,pItem->iSubItem,pItem->pszText,(LPVOID)pItem->lParam);
		}
	}
	catch(CException *e)
	{
		TRACE("GOT EXCEPTION!\n");
		throw e;
	}
	
	*pResult = 0;
}
//***************************************************************************************
void CGrid::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
    {
		*pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
    {
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
    }
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
    {
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell 
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.

		COLORREF crBkg = RGB(255,255,255);
		if(!GetCellColor(pLVCD->nmcd.dwItemSpec,pLVCD->iSubItem,crBkg))
		{
			if ( (pLVCD->nmcd.dwItemSpec & 1) )
				crBkg = ODDCOLOR;
			else 
				crBkg = EVENCOLOR;
		}
	
		// Store the color back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrTextBk = crBkg;

		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;
    }

}
CGrid::~CGrid()
{
	delete[] ColSortAsc;
	delete m_pGridFont;

}
//***************************************************************************************
void CGrid::OnDestroy()
{
	CListCtrl &m_list = GetListCtrl();

	if(ConfigItemId)
	{
		AppCfg().SetNum(ConfigItemId,0,GetFontHeight());
		for(int i = 1;i < iHeaderCount ;i++)
			AppCfg().SetNum(ConfigItemId,i,m_list.GetColumnWidth(i-1));
	}
}
//***************************************************************************************
void CGrid::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMIA)
	{
		if(pNMIA->iItem >= 0 && pNMIA->iSubItem >= 0)
		{
			OnRightClick(pNMIA->iItem,pNMIA->iSubItem);
		}		
	}
	*pResult = 0;
}
//***************************************************************************************
void CGrid::OnColClick(NMHDR* pNMHDR, LRESULT* pResult)
{

	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;
	int nSubItem = pnmlv->iSubItem;

	ColSortAsc[nSubItem] = !ColSortAsc[nSubItem];

	SortByCol(nSubItem,ColSortAsc[nSubItem]);
	Invalidate();
	*pResult = 0;

}
//***************************************************************************************
void CGrid::OnDbClick(NMHDR* pNMHDR, LRESULT* pResult)
{

	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;
	if(pnmlv->iSubItem >= 0 && pnmlv->iItem >= 0)
		OnDoubleClick(pnmlv->iItem,pnmlv->iSubItem);	
	
	pResult = 0;
}
//***************************************************************************************
int CGrid::OnToolHitTest(CPoint point, TOOLINFO * pTI) const 
{
   //See if the point falls onto a list item
	//UINT nFlags = 0;

	LVHITTESTINFO lvhitTestInfo;

	lvhitTestInfo.pt	= point;

	int nItem = ListView_SubItemHitTest(
			this->m_hWnd,
			&lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;

	UINT nFlags =   lvhitTestInfo.flags;

   //nFlags is 0 if the SubItemHitTest fails
   //Therefore, 0 & <anything> will equal false
   if (nFlags & LVHT_ONITEMLABEL){
      //If it did fall on a list item,
      //and it was also hit one of the
      //item specific sub-areas we wish to show tool tips for

      //Get the client (area occupied by this control
      RECT rcClient;
      GetClientRect( &rcClient );

      //Fill in the TOOLINFO structure
      pTI->hwnd = m_hWnd;
      pTI->uId = (UINT) (nItem * 100 + nSubItem);
      pTI->lpszText = LPSTR_TEXTCALLBACK;
      pTI->rect = rcClient;
	  //pTI->lParam = MAKELPARAM(nItem,nSubItem);
	  //TRACE("MAKELPARAM(l = %d,h = %d)\n",nItem,nSubItem);

      return pTI->uId; //By returning a unique value per listItem,
                       //we ensure that when the mouse moves over another list item,
                       //the tooltip will change
   }else{
      //Otherwise, we aren't interested, so let the message propagate
      return -1;
   }
}
//***************************************************************************************
BOOL CGrid::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{

	 TOOLTIPTEXT *pText = (TOOLTIPTEXT *)pNMHDR;
       
       pText->lpszText = MAKEINTRESOURCE(IDS_BTNADD);
       pText->hinst = AfxGetInstanceHandle();
       return TRUE;
   


	//Handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

	//Ignore messages from the built in tooltip, we are processing them internally
	if( (pNMHDR->idFrom == (UINT)m_hWnd) &&
		 ( ((pNMHDR->code == TTN_NEEDTEXTA) && (pTTTA->uFlags & TTF_IDISHWND)) ||
         ((pNMHDR->code == TTN_NEEDTEXTW) && (pTTTW->uFlags & TTF_IDISHWND)) ) ){
      return FALSE;
   }

//	if(pNMHDR->idFrom == 0)	  	// Notification in NT from automatically
//		return FALSE;   	// created tooltip


	//if(!m_bToolTipCtrlCustomizeDone)
	//{
		//AFX_MODULE_THREAD_STATE* pThreadState = AfxGetModuleThreadState();
		//CToolTipCtrl *pToolTip = pThreadState->m_pToolTip;
		// Set max tip width in pixel.
		// you can change delay time, tip text or background color also. enjoy yourself!
		//pToolTip->SetMaxTipWidth(MAX_TOOLTIP_WIDTH);
		//pToolTip->
	//	m_bToolTipCtrlCustomizeDone = TRUE;
	//}

	*pResult = 0;

	CString strTipText;
	CListCtrl &listctrl = GetListCtrl();
	
	//Get the mouse position
	const MSG* pMessage;
   pMessage = GetCurrentMessage();
	ASSERT ( pMessage );
   CPoint pt;
   pt = pMessage->pt; //Get the point from the message
	ScreenToClient( &pt ); //Convert the point's coords to be relative to this control

   //See if the point falls onto a list item

	LVHITTESTINFO lvhitTestInfo;

	lvhitTestInfo.pt	= pt;

	int nItem = listctrl.SubItemHitTest(&lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;

	UINT nFlags =   lvhitTestInfo.flags;

   //nFlags is 0 if the SubItemHitTest fails
   //Therefore, 0 & <anything> will equal false
   if( nFlags & LVHT_ONITEMLABEL ){
	  //If it did fall on a list item,
      //and it was also hit one of the
      //item specific sub-areas we wish to show tool tips for

     if(GetToolTipText(nItem,nSubItem,strTipText))
	  {
		
       #ifndef _UNICODE
	      if (pNMHDR->code == TTN_NEEDTEXTA)
		      lstrcpyn(pTTTA->szText, (LPCSTR)strTipText, 80);
	      else
			 _mbstowcsz(pTTTW->szText, (LPCSTR)strTipText, 80);
      #else
	      if (pNMHDR->code == TTN_NEEDTEXTA)
			 _wcstombsz(pTTTA->szText, (LPCSTR)strTipText, 80);
	      else
		      lstrcpyn(pTTTW->szText, (LPCSTR)strTipText, 80);
      #endif
		  return FALSE;
        					
      }
   }

   return FALSE;
}

//***************************************************************************************
void CGrid::RepaintSortArrows()
{
	int i,j;
	CHeaderCtrl *header = GetListCtrl().GetHeaderCtrl();
	UINT colID;
	HDITEM hdItem;
	bool bFound = false;
        
	for(i = 0;i < iHeaderCount;i++)
	{
		colID = GetColID(i);
		hdItem.mask = HDI_IMAGE | HDI_FORMAT;
		VERIFY( header->GetItem(i, &hdItem) );
		bFound = false;

		for(j = 0;j < m_sortCols.GetSize();j++)
		{
			if(m_sortCols[j].GetDataMember() == colID)
			{
				hdItem.iImage = (m_sortCols[j].IsAscending() ? 0 : 1);
				hdItem.fmt |= HDF_IMAGE;
				bFound = true;				
			}
		}

		if(!bFound)
		{
			//hdItem.mask =  HDI_FORMAT;
			hdItem.fmt &= (~HDF_IMAGE);
		}
		header->SetItem(i,&hdItem);

	}
}
//***************************************************************************************
void CGrid::SortByCol(int nCol,bool bAsc)
{
	DWORD shiftKeyState = GetAsyncKeyState(VK_SHIFT); 
	if(shiftKeyState & 0x8000)
	{
		UINT colId = GetColID(nCol);
		bool bRep = false;

		if(!m_shiftPressLastSort) m_sortCols.RemoveAll();

		for(int i = 0;i < m_sortCols.GetSize();i++)
			if(m_sortCols[i].GetDataMember() == colId)
			{
				m_sortCols[i].SetAscending(bAsc);
				bRep = true;
				break;
			}
		if(!bRep)
		{
			m_sortCols.Add(DataMemberSortParams(colId,bAsc));
		}
		m_shiftPressLastSort = true;
	}
	else
	{
		m_sortCols.RemoveAll();
		m_sortCols.Add(DataMemberSortParams(GetColID(nCol),bAsc));
		m_shiftPressLastSort = false;
	}
	GridSortBy(m_sortCols);
	RepaintSortArrows();	
}


void CGrid::OnLvnGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);
	if (pGetInfoTip->iSubItem == 0)
	{
		LVHITTESTINFO hti = {0};
		::GetCursorPos(&hti.pt);
		ScreenToClient(&hti.pt);
		if (GetListCtrl().SubItemHitTest(&hti) == -1 || hti.iItem != pGetInfoTip->iItem || hti.iSubItem != 0){
			// don't show the default label tip for the main item, if the mouse is not over the main item
			if ((pGetInfoTip->dwFlags & LVGIT_UNFOLDED) == 0 && pGetInfoTip->cchTextMax > 0 && pGetInfoTip->pszText[0] != _T('\0'))
				pGetInfoTip->pszText[0] = _T('\0');
			return;
		}

		//pGetInfoTip->iItem);
		if (pGetInfoTip->pszText && pGetInfoTip->cchTextMax > 0)
		{
			CString info;

			info = "dupa";			

			//info += TOOLTIP_AUTOFORMAT_SUFFIX_CH;
			_tcsncpy(pGetInfoTip->pszText, info, pGetInfoTip->cchTextMax);
			pGetInfoTip->pszText[pGetInfoTip->cchTextMax-1] = _T('\0');
		}
	}
	*pResult = 0;
}
