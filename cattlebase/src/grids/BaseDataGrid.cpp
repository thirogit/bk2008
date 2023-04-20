#include "stdafx.h"
#include "BaseDataGrid.h"

BaseDataGrid::BaseDataGrid(const GRIDHEADERS *ppHeaders, int pConfigItemId) :
	CGrid(ppHeaders,pConfigItemId)
{

}
bool BaseDataGrid::GetCellColor(int iItem,int nCol,COLORREF &cr)
{
	if(!GetDataCellColor(iItem,nCol,cr))
	{
		if(IsItemMarked(iItem))
		{
			cr = HLIGHTCOLOR;
			return true;
		}
		return false;
	}
	return true;
}
void BaseDataGrid::Lock()
{
	SetRedraw(FALSE);
}

void BaseDataGrid::Unlock()
{
	SetRedraw(TRUE);
}