#include "stdafx.h"
#include "BaseListCtrl.h"
#include "MainCfg.h"

BEGIN_MESSAGE_MAP(CBaseListCtrl,CListCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO,GetDispInfo)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw )	
END_MESSAGE_MAP()

CBaseListCtrl::CBaseListCtrl(const GRIDHEADERS *ppHeaders, int pConfigItemId/* = -1*/) :
				pHeaders(ppHeaders),ConfigItemId(pConfigItemId)
{
	iHeaderCount = 0;
	while(ppHeaders[iHeaderCount].nColID || ppHeaders[iHeaderCount].nResID) 
		iHeaderCount++;
	
	ColSortAsc = new bool[iHeaderCount];
	memset(ColSortAsc,0,sizeof(bool)*iHeaderCount);
}

CBaseListCtrl::CBaseListCtrl(const GRIDHEADERS *ppHeaders, int iiHeaderCount,int pConfigItemId) :
				pHeaders(ppHeaders),ConfigItemId(pConfigItemId),iHeaderCount(iiHeaderCount)
{
	ColSortAsc = new bool[iHeaderCount];
	memset(ColSortAsc,0,sizeof(bool)*iHeaderCount);
}


CBaseListCtrl::~CBaseListCtrl()
{
	delete[]  ColSortAsc;
}
void CBaseListCtrl::OnDestroy()
{
	if(ConfigItemId != -1)
	for(int i =0; i < iHeaderCount;i++)
		AppCfg().SetNum(ConfigItemId,i,GetColumnWidth(i));
}

void CBaseListCtrl::SetListCols()
{

	CString szColText;
	int *colwidths = new int[iHeaderCount];
	memset(colwidths,0,sizeof(int)*iHeaderCount);

	CDC *dc = GetDC();
	int i,s;
	LV_COLUMN lvcolumn;

	CHeaderCtrl *pHeadCtrl = GetHeaderCtrl();
	if(pHeadCtrl)
	{
		for(i = 0,s = pHeadCtrl->GetItemCount();i < s;i++) 
			pHeadCtrl->DeleteItem(0);
	}

	UINT nColTextRes;

	for(i=0;i<iHeaderCount;i++)
	{
		nColTextRes = pHeaders[i].nResID;
		if(nColTextRes == (UINT)(-1))
			szColText = GetColText(pHeaders[i].nColID);
		else
			szColText.LoadString(nColTextRes);

		if(ConfigItemId == -1 || !(colwidths[i] = AppCfg().GetNum(ConfigItemId,i)))
				colwidths[i] = dc->GetTextExtent(szColText).cx;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = szColText.GetBuffer();
		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidths[i];
		InsertColumn(i, &lvcolumn);
		szColText.ReleaseBuffer();
	}

	ReleaseDC(dc);
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// call EnableToolTips to enable tooltip display
	EnableToolTips(TRUE);
	DeleteAllItems();
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	delete[] colwidths;	

}

void CBaseListCtrl::PostCreate()
{
	SetListCols();
	UpdateItemCount();
}

void CBaseListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();
	ASSERT(GetStyle() & LVS_OWNERDATA);
	PostCreate();
	
}
int CBaseListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int createResult = CListCtrl::OnCreate(lpCreateStruct);
	if(createResult != 0) return createResult;
	ASSERT(GetStyle() & LVS_OWNERDATA);
	PostCreate();
	return 0;
}

int CBaseListCtrl::GetItemImage(int iItem,int Column)
{
	return -1;
}

void CBaseListCtrl::GetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem = &(pDispInfo)->item;
	if (pItem->mask & LVIF_TEXT && pItem->iItem >= 0 && pItem->iSubItem >= 0) 
			GetItemDispText(pItem->iItem,pItem->iSubItem,pItem->pszText);

	pItem->iImage = GetItemImage(pItem->iItem,pItem->iSubItem);
	*pResult = 0;
}

bool CBaseListCtrl::GetCellRect(int Item, int Column,CRect &cellRect,bool bScroll/* = true*/,int iMIN_COL_WIDTH/* = MAXLONG*/)
{

	if (!EnsureVisible (Item, TRUE)) 
    		return false;
	    

		// Make sure that nCol is valid
		CHeaderCtrl* pHeader = GetHeaderCtrl();
		int nColumnCount = pHeader->GetItemCount();
		if (Column >= nColumnCount || GetColumnWidth (Column) < iMIN_COL_WIDTH)
			return false;                 

		// Get the column offset
		int Offset = 0;
		for (int iColumn = 0; iColumn < Column; iColumn++)
			Offset += GetColumnWidth (iColumn);

		CRect Rect;
		GetItemRect (Item, &Rect, LVIR_BOUNDS);

		// Now scroll if we need to expose the column
		CRect ClientRect;
		GetClientRect (&ClientRect);
		if (Offset + Rect.left < 0 || Offset + Rect.left > ClientRect.right)
		{
			CSize Size;
			if (Offset + Rect.left > 0)
				Size.cx = -(Offset - Rect.left);
			else
				Size.cx = Offset - Rect.left;
			Size.cy = 0;
			Scroll (Size);
			Rect.left -= Size.cx;
		}
		
		Rect.left += Offset/*+4*/ + 2;
		Rect.right = Rect.left + GetColumnWidth (Column) - 3;
		if (Rect.right > ClientRect.right)	Rect.right = ClientRect.right;

		cellRect = Rect;

return true;
}
BOOL CBaseListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;
	OnColClick(pnmlv->iSubItem);
	*pResult = 0;
	return TRUE;		// return FALSE to send message to parent also -
						// NOTE:  MSDN documentation is incorrect
}

int CBaseListCtrl::GetColumnCount()
{
	return GetHeaderCtrl()->GetItemCount();
}

int CBaseListCtrl::GetCurSel()
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = GetNextSelectedItem(pos);
	return nSelectedItem;
}

void CBaseListCtrl::OnColClick(int nCol)
{
	ColSortAsc[nCol] = !ColSortAsc[nCol];
	SortByCol(nCol,ColSortAsc[nCol]);
	Invalidate();
}

UINT CBaseListCtrl::GetColID(int nCol)
{
	if(nCol < iHeaderCount)
		return pHeaders[nCol].nColID;

	return (UINT)-1;
}

void CBaseListCtrl::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	COLORREF cellBkg;

    *pResult = CDRF_DODEFAULT;
    
    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
        {
        *pResult = CDRF_NOTIFYITEMDRAW;
        }
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
        {
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
        }
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
        {
		
			if(GetCellColor(pLVCD->nmcd.dwItemSpec,pLVCD->iSubItem,&cellBkg))
			{
					pLVCD->clrTextBk = cellBkg;					
			}
			else
				pLVCD->clrTextBk = RGB(255,255,255);


        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
        }
}

bool CBaseListCtrl::GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg)
{
	return false;
}

void CBaseListCtrl::Lock()
{
	SetRedraw(FALSE);
}
void CBaseListCtrl::Unlock()
{
	SetRedraw(TRUE);
}

