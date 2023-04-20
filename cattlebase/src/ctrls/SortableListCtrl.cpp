#include "stdafx.h"
#include "SortableListCtrl.h"

BEGIN_MESSAGE_MAP(CSortableListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
END_MESSAGE_MAP()


int CSortableListCtrl::GetColumnCount()
{
	return GetHeaderCtrl()->GetItemCount();
}

int CSortableListCtrl::GetCurSel()
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = GetNextSelectedItem(pos);
	return nSelectedItem;
}

BOOL CSortableListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;

	int nSubItem = pnmlv->iSubItem;

	TRACE("Column  = %d\n",nSubItem);

	*pResult = 0;
	return FALSE;		// return FALSE to send message to parent also -
						// NOTE:  MSDN documentation is incorrect
}



/*
void CSortableListCtrl::SortByCol(int Column,BOOL bAsc)
{





}



void CSortableListCtrl::SortItemsDescending()
{
    // Sort all items in descending aphabetical order using an STL set
    typedef std::set<LVITEM*, LVITEM_less> ItemSet;
    ItemSet setItems;
    int iCount = GetItemCount();
    for (int i = 0; i < iCount; i++)
    {
        LVITEM* pLVI = new LVITEM();
        ::memset(pLVI, 0, sizeof(LVITEM));
        pLVI->iItem = i;
        pLVI->mask = LVIF_IMAGE | LVIF_INDENT | LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
        pLVI->pszText = new TCHAR[MAX_ITEM_TEXT];
        pLVI->cchTextMax = MAX_ITEM_TEXT;
        GetItem(pLVI);
        setItems.insert(pLVI);
    }

	pLVI->
    // Remove all items from the list control
    DeleteAllItems();
	

    // Put the items back in the list control in reverse order
    int iIndex = 0;
    for (ItemSet::reverse_iterator it = setItems.rbegin(); it != setItems.rend(); ++it)
    {
        (*it)->iItem = iIndex++;
        InsertItem(*it);
        delete [] (*it)->pszText;
        delete *it;
    }
}*/
