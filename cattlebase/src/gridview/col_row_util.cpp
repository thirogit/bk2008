/******************************************************************************
* File Name:    col_row_util.cpp
* Author:       D. Van Leer
* Date:         11/01/03
* Copyright:    2003 D. Van Leer
* Description:  stringgrid class column and row utility functions
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
******************************************************************************/
#include "stdafx.h"
#include "cstringgrid.h"

#define MIN_HEIGHT 9
#define MAX_HEIGHT 101

// CellExists returns true if cell is in grid range
bool CStringGrid::CellExists(int col, int row)
{
  if(0 > col || col >= m_colCount || 0 > row || row >= m_rowCount)
    return false;
  else
    return true;
}

bool CStringGrid::ColExists(int col)
{
 return !(0 > col || col >= m_colCount);
}

/************************************************
* set column width functions
************************************************/

// set individual column width in pixels
bool CStringGrid::SetColWidth(int col, int width)
{
  if(0 <= col && col < m_colCount)
  {
    TEXTMETRIC  tm;
    int         cxChar;

    GetTextMetrics(m_memdc, &tm);
    cxChar = tm.tmAveCharWidth;
    int maxColWidth = cxChar * MAX_COLUMN_CHARS;
    
    if(width < 0) width = 0;
    if(width > maxColWidth) width = maxColWidth;
    m_columns[col].m_width = width;
    return true;
  }
  else
    return false;
}

// set widths from array of number of characters
void CStringGrid::SetColCharWidths(int widths[])
{
  TEXTMETRIC  tm;
  int         cxChar;
 
  GetTextMetrics(m_memdc, &tm);
  cxChar = tm.tmAveCharWidth;
  
  for(int i = 0; i < m_colCount; i++)
    m_columns[i].m_width = widths[i] * cxChar;
}

//-----------------------------------------------------------------------------
ROWHANDLE CStringGrid::fastAddRow()
{

  int row = m_rowCount;

  RowType *newRow = new RowType();
  m_drawRows.InsertAt(row,newRow);
  RowTypeHandle *rowHandle = new RowTypeHandle(newRow);
  m_rowHandles.Add(rowHandle);

  for(int i=0;i<m_colCount;i++)
	  newRow->m_cells.push_back(CellType());
	m_rowCount++;	
	
	newRow->visible = false;
	drawSetRowVisible(row,true);

	
	if(m_rowCount == 2) //one is fixed , second has just been added 
	{
		m_topRow = 1;
		m_bottomRow = 1; 
	}
    
   
	return rowHandle->rowHandle;

}
//-----------------------------------------------------------------------------
ROWHANDLE CStringGrid::AddRow()
{
  ROWHANDLE rowh =  fastAddRow();
  SetScrollbars();  
  return rowh;
}

//-----------------------------------------------------------------------------
// Function: InsertCol
// Use:      inserts column at specified point
// Args:     int col   - column number to insert at
//           int width - width of inserted column
//-----------------------------------------------------------------------------
void CStringGrid::fastInsertCol(int col,int width)
{
  if(col < 0) col = 0;
  if(col > m_colCount) col = m_colCount;
    
  int c = col;
  vector<CellType>::iterator p;
  RowType *rowPtr;
  for(int i = 0; i < m_rowCount; i++)
  {	
	rowPtr = m_drawRows[i];
    p = rowPtr->m_cells.begin()+c;
	rowPtr->m_cells.insert(p,1,CellType());
  }
  m_colCount++;
  vector<ColumnType>::iterator r;
  r = m_columns.begin() + col;
  m_columns.insert(r, 1, ColumnType(width));
}

void CStringGrid::InsertCol(int col, int width)
{
  fastInsertCol(col,width);
  SetScrollbars();
}
//-----------------------------------------------------------------------------
bool CStringGrid::drawDeleteRow(int row,bool bSetScroll)
{

  if((row < 1) || (row >= m_rowCount))
  {
    return false;
  }
  else
  {
    RowType *rowPtr = m_drawRows[row];
	rowPtr->visible = true;
	drawSetRowVisible(row,false);

	m_drawRows.RemoveAt(row);

	int i,s;

	for(i=0,s = m_rowHandles.GetSize();i<s;i++)
		if(m_rowHandles[i]->row == rowPtr)
		{
			delete m_rowHandles[i];
			m_rowHandles.RemoveAt(i);
			break;
		}

	delete rowPtr;
	m_rowCount--;
	AquireAfterDelete(row);

 	if(bSetScroll) SetScrollbars();
    return true;
  } 
}

//-----------------------------------------------------------------------------
bool CStringGrid::fastDeleteCol(int col)
{
  if((col < 1) || (col >= m_colCount))  return false;
  else
  {
    int c = col;
    vector<CellType>::iterator p;

    for(int i = 0; i < m_rowCount; i++)
    {
      p = m_drawRows[i]->m_cells.begin() + c;
    }
    m_colCount--;
    vector<ColumnType>::iterator r;
    r = m_columns.begin() + col;
    m_columns.erase(r);
    return true;
  }  
}


//-----------------------------------------------------------------------------
bool  CStringGrid::DeleteCol(int col)
{
	if(fastDeleteCol(col))
	{
		SetScrollbars();
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Function: AutoSizeCol
// Use:      adjusts size of column to fit widest string
// Args:     int col - column to autosize
// Returns:  bool - true if width changed, false if not changed
//-----------------------------------------------------------------------------
bool CStringGrid::AutoSizeCol(int col)
{
  HGDIOBJ     hOldFont    = NULL;
  HFONT       hFixedFont  = CreateFontIndirect(&m_fxFont);
  HFONT       hCellFont   = CreateFontIndirect(&m_cellFont);
  bool        changed     = false;
  int         maxwidth    = 0;
  int         cellsize    = 0;
  SIZE        size;

  if(m_width == 0)
  {
      /*::GetClientRect(m_gridhWnd, &m_clRect);
      m_top    = m_clRect.top;
      m_left   = m_clRect.left;
      m_height = m_clRect.bottom - m_clRect.top;
      m_width  = m_clRect.right - m_clRect.left;
	  m_ScreenRows = (m_height/m_rowHeight)-1;*/
	  ASSERT(0);
  }

  if(col >= 0 && col < ColCount())
  {
    // select fixed font
    hOldFont   = SelectObject(m_memdc, hFixedFont);

    // get width of fixed row text
	CString cellStr = drawCell(col, 0);
    if(CheckOptions(SGO_FIXEDROW))
    {
      GetTextExtentPoint32(m_memdc,cellStr ,
		  cellStr.GetLength(), &size);

      // allow for 5 pixel margins around text
      maxwidth = size.cx + 11;
    }
    
    //switch to cell font if not fixed column
    if(col > 0)
    {
      SelectObject(m_memdc, hCellFont); 
    }

    // get width of each row's text, keep widest value
    if(m_rowCount > 0)
    {
      for(int row = 1; row < m_rowCount; row++)
      {
		  cellStr = drawCell(col, row);
        GetTextExtentPoint32(m_memdc,cellStr ,
			cellStr.GetLength(), &size);
        cellsize = size.cx + 11;
        if(cellsize > maxwidth)
        {
          maxwidth = cellsize;
        }
      }
    }

    // limit to width of grid / 3
//    maxwidth = min(maxwidth, 20 + m_width / 3);
//    maxwidth = minmax(m_width / 3, maxwidth, 20);

    // update if different than current width
    if(maxwidth != m_columns[col].m_width)
    {
      changed = true;
      m_columns[col].m_width = maxwidth;
      SetScrollbars();
    }
  }
  SelectObject(m_memdc, hOldFont);
  DeleteObject(hFixedFont);
  DeleteObject(hCellFont);
  return changed;
}

//-----------------------------------------------------------------------------
// Function: VisibleRows
// Use:      returns number of visible rows
//-----------------------------------------------------------------------------
int CStringGrid::VisibleRows()
{
	return m_VisRows;
}
//-----------------------------------------------------------------------------
int CStringGrid::RowHeight(int row)
{
	if(row >= 0 && row < m_rowCount)
		return m_rowHeight+m_drawRows[row]->GetCloakHeight();
	return 0;
}
//-----------------------------------------------------------------------------
// Function: BottomRow
// Use:      returns bottom visible row
//           and sets m_clRect to client size
//-----------------------------------------------------------------------------
int CStringGrid::BottomRow()
{
	//::GetClientRect(m_gridhWnd, &m_clRect);

	int br = m_topRow,cr = 0;
	int x = HeadHeight() + RowHeight(m_topRow);
	while(br < (m_rowCount-1) &&  cr < m_ScreenRows && (x < m_height))
	{
		if(m_drawRows[br]->visible) cr++;
		br++;
		x += RowHeight(br);
   	}
	if(br > 1)	br--;
  return br;
}

//-----------------------------------------------------------------------------
// Function: RightColumn
// Use:      returns right visible column
//           and sets m_clRect to client size
//-----------------------------------------------------------------------------
int CStringGrid::RightColumn()
{
	
  int x = m_columns[0].m_width;

  int i;
  for(i = m_leftCol; i < m_colCount; i++)
  {
    x += m_columns[i].m_width;
    if(x > m_width)
      break;
  }
  i--;
  return i;
}

//-----------------------------------------------------------------------------
// Function: SetLeftColumn
// Use:      Sets left visible column
// Args:     col - column
//-----------------------------------------------------------------------------
void CStringGrid::SetLeftCol(int col)
{
  if(0 < col && col < m_colCount) m_leftCol = col;
}
//-----------------------------------------------------------------------------
// Function: SetTopRow
// Use:      Sets top visible row
// Args:     row - row
//-----------------------------------------------------------------------------
void CStringGrid::SetRowHeight(int height)
{
  if(MIN_HEIGHT < height && height < MAX_HEIGHT)
  {
    m_rowHeight = height;
  }
}

void CStringGrid::SetTitleHeight(int height)
{
  if(MIN_HEIGHT < height && height < MAX_HEIGHT)
  {
    m_titleHeight = height;
  }
}
bool RowType::CloakCellExists(int col,int row)
{
	return (col >= 0 && col < cloakcols && row >= 0 && row < cloakrows);
}

bool RowType::SetCloakCellStrPtr(int col,int row,CString *strPtr)
{
	return SetCloakCell(col,row,(void*)strPtr,CellType::PSTRING,FALSE);
}
bool RowType::SetCloakCellIntPtr(int col,int row,int *intPtr)
{
	return SetCloakCell(col,row,(void*)intPtr,CellType::PNUMERIC,FALSE);
}
bool RowType::SetCloakCellStr(int col,int row,CString &strVal)
{
	CString *newStr = new CString(strVal);
	if(!SetCloakCell(col,row,(void*)newStr,CellType::PSTRING,TRUE))
	{
		delete newStr;
		return false;
	}
	return true;
}
bool RowType::SetCloakCellInt(int col,int row,int intVal)
{
	int *newInt = new int;
	*newInt = intVal;
	if(!SetCloakCell(col,row,(void*)newInt,CellType::PNUMERIC,TRUE))
	{
		delete newInt;
		return false;
	}
	return true;
}

bool RowType::SetCloakCell(int col,int row,void *ptr,CellType::SG_CELLTYPE cellType,BOOL b_isOwner)
{
	if(CloakCellExists(col,row))
	{
		CellType *cell;
		cell = &m_CloakRows[row].m_cloakcells[col];
		return cell->SetThisCell(ptr,cellType,b_isOwner);
	}
	return FALSE;
}
//-----------------------------------------------------------------------------
// Function: SetCloakCellColor
// Use:      Sets cloak cell background color
// Args:     col,row,RGB
//-----------------------------------------------------------------------------
bool RowType::SetCloakCellColor(int col,int row,COLORREF color)
{
if(CloakCellExists(col,row))
{
	m_CloakRows[row].m_cloakcells[col].m_color = color;
	return TRUE;
}
return FALSE;
}
//-----------------------------------------------------------------------------
// Function:  SetCloakSize
// Use:     Set Soze of cloaked grid for all rows 
// Args:     number of cloaked rows,cols
//-----------------------------------------------------------------------------
bool RowType::SetCloakSize(int rows,int cols)
{
if(rows <= 0 && cols <= 0) return false;
else
{
	int i;
	cloakcols = cols;
	cloakrows = rows;
	m_CloakCols.resize(cols);
	m_CloakRows.resize(rows);
	for(i=0;i<rows;i++) m_CloakRows[i].m_cloakcells.resize(cols);
	MergeRows();
	MergeCols();
}
return true;
}

bool RowType::SetCloakCellSpan(int col,int row,int cspan = -1,int rspan = -1)
{

	int curcspan,currspan,i,ii,j,jj,t1;
	CloakCellType *cellptr;
	//CloakRowType *rowptr;


if(col >= 0 && col < cloakcols && row >= 0 && row < cloakrows)
{
	cellptr = &m_CloakRows[row].m_cloakcells[col];
//	rowptr = &m_CloakRows[row];
	curcspan = cellptr->colspan;
	currspan = cellptr->rowspan;

	if(cspan <= 0) cspan = curcspan;
	if(rspan <= 0) rspan = currspan;

	
		if(((col+cspan) <= cloakcols) && ((row+rspan) <= cloakrows))
		{

				if(curcspan > 1 || currspan > 1)
				{

					for(j=row,jj=row+currspan,ii=col+curcspan;j<jj;j++)		
					for(i=col;i<ii;i++)
					{
						m_CloakRows[j].m_cloakcells[i].in_span = false;
						m_CloakRows[j].m_cloakcells[i].corner = true;;
					}
					curcspan = 1;
					currspan = 1;


				}

					for(j=(row+currspan-1),jj=row+rspan,ii=col+cspan,t1=(col+curcspan-1);j<jj;j++)		
					for(i=t1;i<ii;i++)
					{
						m_CloakRows[j].m_cloakcells[i].in_span = true;

						if((i != col) || (j != row))
						m_CloakRows[j].m_cloakcells[i].corner = false;
					}
				/*} else if (cspan < curcspan || rspan < currspan)
				{

					for(j=(row+currspan-1),jj=row+rspan-1;j>jj;j--)		
					{
						for(i=col,ii=col+curcspan;i<ii;i++)
						{
							m_CloakRows[j].m_cloakcells[i].in_span = false;
							m_CloakRows[j].m_cloakcells[i].corner = true;
						}
					}

					for(j;j>=row;j--)
						for(i=(col+curcspan-1),ii=col+cspan-1;ii<i;i--)
						{
							m_CloakRows[j].m_cloakcells[i].in_span = false;
							m_CloakRows[j].m_cloakcells[i].corner = true;
						}
				}*/

				cellptr->colspan = cspan;
				cellptr->rowspan = rspan;

		} else return FALSE;

} else return FALSE;
/*

#ifdef _DEBUG
for(j=0;j<cloakrows;j++)
{
for(i=0;i<cloakcols;i++)
{
	DebugMsg("%c",m_CloakRows[j].m_cloakcells[i].corner ? 'x' : 'o');
}
DebugMsg("\n");
}

DebugMsg("---\n");
#endif
*/

return TRUE;
}


bool RowType::SetCloakColWidth(int col,int percent)
{

	if(col < cloakcols && percent <= 100 && percent >= 0)
	{
		m_CloakCols[col].width_per100 = percent;
		MergeCols();
		return TRUE;

	}
	return FALSE;

}


void RowType::MergeRows(void)
{

		int i,p100rcount = 0,rper100height = 0,t,correct = 0,h;

		for(i=0;i<cloakrows;i++)
		{
			t = m_CloakRows[i].height_per100;
			if(t<0) p100rcount++;
			else rper100height += t;
		}

		if(rper100height > 100)
		{
			for(i=0;i<cloakrows;i++)
			{
				t = m_CloakRows[i].height_per100;
				if(t < 0) m_CloakRows[i].pixel_height = 0;
				else 
				{
					h = (t*cloakHeight)/rper100height;
					m_CloakRows[i].pixel_height = h;
					correct += h;
				}
			}
		} else
		{
			for(i=0;i<cloakrows;i++)
			{
				t = m_CloakRows[i].height_per100;
				if(t < 0) 
				{
					h = ((100-rper100height)*cloakHeight)/(p100rcount*100);
					m_CloakRows[i].pixel_height = h;

				}
				else 
				{
					h = (t*cloakHeight)/100;
					m_CloakRows[i].pixel_height = h;
				}
				correct += h;
			}
		


		}

		if(cloakHeight > correct)
		{

			i = cloakrows-1;
			for(;i >= 0;i--)
				if(m_CloakRows[i].pixel_height > 0) 
				{
					m_CloakRows[i].pixel_height  += (cloakHeight-correct);
					break;
				}

		}




}

void RowType::MergeCols(void)
{


		int i,p100ccount = 0,cper100width = 0,t,correct = 0,w;

		for(i=0;i<cloakcols;i++)
		{
			t = m_CloakCols[i].width_per100;
			if(t<0) p100ccount++;
			else cper100width += t;
		}


		if(cper100width > 100)
		{
			for(i=0;i<cloakcols;i++)
			{
				t = m_CloakCols[i].width_per100;
				if(t < 0) w = 0;
				else w = (t*100)/cper100width;

				m_CloakCols[i].percent_width = w;
				correct += w;
			}
		} 
		else
		{
			for(i=0;i<cloakcols;i++)
			{
				t = m_CloakCols[i].width_per100;
				if(t < 0) w = (100-cper100width)/(p100ccount);
				else w = t;

				m_CloakCols[i].percent_width = w;
				correct += w;
			}
		


		}


	if(100 > correct)
	{

			i = cloakcols-1;
			for(;i >= 0;i--)
				if(m_CloakCols[i].percent_width > 0) 
				{
					m_CloakCols[i].percent_width  += (100-correct);
					break;
				}
}
}

bool RowType::SetCloakHeight(int iHeight) 
{ 
	if(iHeight > 0) 
	{
	   cloakHeight = iHeight; 
	   MergeRows();
	   return TRUE;
	}

return FALSE;
}

CString RowType::CloakCell(int col,int row)
{
	char buf[17];
	if(CloakCellExists(col,row))
	{
		CloakCellType *cellPtr = &m_CloakRows[row].m_cloakcells[col];
	  switch(cellPtr->m_tType)
	  {
		  case CellType::PSTRING:
              return (*cellPtr->m_pStr);
		  case CellType::PNUMERIC:
			  sprintf(buf,"%d",*cellPtr->m_pInt);
    		  return CString(buf);
	  }	  
	}

	return CString("");

}

int RowType::GetCloakHeight() 
{ 
	return (hidden ? 0 : cloakHeight); 
}

void CStringGrid::AquireAfterDelete(int deletedRow)
{
int row = deletedRow;
if(m_rowCount <= 1) 
	{
		m_bottomRow = -1;
		m_topRow = 0;
		
	}
	else
	{
		if(row <= m_bottomRow)
		{
			
				if((m_topRow-1) > 0)
				{
					m_topRow--;
				}
				m_bottomRow = BottomRow();
			
		}
	}
	
}

bool  CStringGrid::DeleteRow(ROWHANDLE row)
{
	int rowIdx;
RowType *rowPtr = GetRowByHandle(row,&rowIdx);
if(!rowPtr) return false;
  
	int i,s;

	for(i=0,s = m_drawRows.GetSize();i<s;i++)
		if(m_drawRows[i] == rowPtr)
		{

			  rowPtr->visible = true;
			  drawSetRowVisible(i,false);
              delete rowPtr;

	          m_drawRows.RemoveAt(i);

			  m_rowCount--;

			  AquireAfterDelete(i);

			  delete m_rowHandles[rowIdx];
			  m_rowHandles.RemoveAt(rowIdx);
			  SetScrollbars();
			  return true;
		}
return false;
}

ROWHANDLE CStringGrid::_GetRowHandle(BOOL bReset)
{
static int _index = 0;
if(bReset) _index = 0;
if(_index >= m_rowHandles.GetSize()) return 0;
return m_rowHandles[_index++]->rowHandle;
}

ROWHANDLE CStringGrid::GetFirstRowHandle()
{
	return _GetRowHandle(TRUE);
}
ROWHANDLE CStringGrid::GetNextRowHandle()
{
	return _GetRowHandle(FALSE);
}

