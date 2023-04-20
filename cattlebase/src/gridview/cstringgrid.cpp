#include "stdafx.h"

#include "cstringgrid.h"

using namespace std;


IMPLEMENT_DYNCREATE(CStringGrid,  CView)

#ifdef _DEBUG
void CStringGrid::AssertValid() const
{
	CView::AssertValid();
}

void CStringGrid::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


int RowTypeHandle::_HanCounter = 1;


//-----------------------------------------------------------------------------
// Function: CStringGrid
// Use:      constructor
// Args:     HINSTANCE hInst - application instance
//           int       cols  - number of columns
//           int       rows  - number of rows
//-----------------------------------------------------------------------------
CStringGrid::CStringGrid(int cols, int rows) :
  m_hInst         (NULL),
  m_gridhWnd          (NULL),

  m_top           (0),
  m_left          (0),
  m_width         (0),
  m_height        (0),
  m_ScreenRows    (0),
  m_row_hlight    (-1),
  m_colCount      (minmax(0, cols, MAX_COLS)),
  m_rowCount      (minmax(0, rows, MAX_ROWS)),
  m_rowHeight     (DEFAULT_ROW_HEIGHT),
  m_titleHeight   (20),
  m_leftCol       (1),
  m_rightCol      (1),
  m_topRow        (1),
  m_bottomRow     (1),
  m_bFocused      (false),
  m_bGrayed       (true),
  
  
  m_dwOptions     (SGO_VISIBLE      |
                   SGO_EDITABLE     |
                   SGO_CELLSEL      |
                   SGO_SCROLLDOT    |
                   SGO_FIXEDCOL     |
                   SGO_FIXEDROW     |
                   SGO_SHOWFOCUS    |
                   0),

  m_dwExStyle     (0),
  m_dwStyle       (WS_CHILD         | 
                   WS_CLIPSIBLINGS  | 
                   WS_VISIBLE       | 
                   WS_CLIPCHILDREN  |
                   WS_VSCROLL       |
                   WS_HSCROLL       |
                   0),
                   
  m_titleString   (""),
  m_bSizing(false),
  m_bFreezHighlight(false),
  m_VisRows(m_rowCount-1),
  m_nWchichCol(-1),
  m_bWantToSort(false)

{
 // SetRectEmpty(&m_clRect);
  int i;
RowType *rowPtr;
RowTypeHandle *rowHandle;

 for(i=0;i<m_rowCount;i++) 
 {
	 rowPtr = new RowType();
	rowHandle = new RowTypeHandle(rowPtr);
	m_rowHandles.Add(rowHandle);
 	 m_drawRows.Add(rowPtr);
	 rowPtr->m_cells.resize(cols);
 }


 if(m_rowCount == 1)
 {
	 m_topRow = 0;
	 m_bottomRow = -1;
 }
 

   

  // set default colors
  // 16 text colors
  m_colors[SGC_TEXT_0]      = GetSysColor(COLOR_WINDOWTEXT);
  m_colors[SGC_TEXT_1]      = RGB(  0,   0, 255);
  m_colors[SGC_TEXT_2]      = RGB(  0, 128,   0); 
  m_colors[SGC_TEXT_3]      = RGB(  0, 180, 180);
  m_colors[SGC_TEXT_4]      = RGB(255,   0,   0); 
  m_colors[SGC_TEXT_5]      = RGB(180,   0, 180);
  m_colors[SGC_TEXT_6]      = RGB(128,  64,   0); 
  m_colors[SGC_TEXT_7]      = RGB(220, 220, 220);
  m_colors[SGC_TEXT_8]      = RGB(128, 128, 128); 
  m_colors[SGC_TEXT_9]      = RGB(180, 180, 255);
  m_colors[SGC_TEXT_A]      = RGB(180, 255, 180); 
  m_colors[SGC_TEXT_B]      = RGB(128, 255, 255);
  m_colors[SGC_TEXT_C]      = RGB(255, 180, 180); 
  m_colors[SGC_TEXT_D]      = RGB(255, 128, 255);
  m_colors[SGC_TEXT_E]      = RGB(255, 255,   0); 
  m_colors[SGC_TEXT_F]      = RGB(255, 255, 255);
  // 4 column colors
  m_colors[SGC_COLUMN_1]    = RGB(240, 255, 255);
  m_colors[SGC_COLUMN_2]    = RGB(255, 240, 255);
  m_colors[SGC_COLUMN_3]    = RGB(255, 255, 240);
  m_colors[SGC_COLUMN_4]    = RGB(240, 255, 240);
  
  // other colors                              
  m_colors[SGC_GRIDBKGND]   = RGB(0xFF, 0xFF, 0xFF);
  m_colors[SGC_CELLBKGND]   = GetSysColor(COLOR_WINDOW);
  m_colors[SGC_EDITBKGND]   = RGB(250, 255, 255);
  m_colors[SGC_FXDCELL]     = RGB(0x90,0xC8,0x90);
  m_colors[SGC_SELCELL]     = GetSysColor(COLOR_INFOBK);

  m_colors[SGC_FXDCOLTEXT]  = GetSysColor(COLOR_BTNTEXT);
  m_colors[SGC_FXDROWTEXT]  = GetSysColor(COLOR_BTNTEXT);
  m_colors[SGC_TITLETEXT]   = RGB(0, 0, 200);
  m_colors[SGC_EDITTEXT]    = RGB(  0,   0,   0);
  m_colors[SGC_SELTEXT]     = GetSysColor(COLOR_INFOTEXT);
  m_colors[SGC_GRAYTEXT]    = GetSysColor(COLOR_GRAYTEXT);

  m_colors[SGC_SELBORDER]   = GetSysColor(COLOR_INFOTEXT);
  m_colors[SGC_EDITBORDER]  = RGB(  0,   0, 255);
  m_colors[SGC_GRIDLINE]    = RGB(0, 0, 0);
  m_colors[SGC_SCROLLDOT]   = RGB( 50,  50,  50);
  m_colors[SGC_ODDROWBARS]  = RGB(230, 255, 240);

  m_colors[SGC_GRADTOP]     = RGB(255, 255, 255);
  m_colors[SGC_GRADBOTTOM]  = RGB(204, 204, 236);


  for(i = 0; i < m_colCount; i++)   // rest of columns use
    m_columns.push_back(ColumnType());  // default of 100

  // set default fonts
  memset(&m_fxFont, 0, sizeof(LOGFONT));
  m_fxFont.lfHeight            = 12L;
  m_fxFont.lfWeight            = 0L;
  m_fxFont.lfQuality           = PROOF_QUALITY;
  m_fxFont.lfCharSet	       = DEFAULT_CHARSET;
  
  lstrcpy(m_fxFont.lfFaceName, "Verdana");

  memset(&m_cellFont, 0, sizeof(LOGFONT));
  m_cellFont.lfHeight          = 12L;
  m_cellFont.lfWeight          = 200L;
  m_cellFont.lfQuality         = PROOF_QUALITY;
  lstrcpy(m_cellFont.lfFaceName, "courier new");

  memset(&m_titleFont, 0, sizeof(LOGFONT));
  m_titleFont.lfHeight         = 18L;
  m_titleFont.lfWeight         = FW_SEMIBOLD;
  m_titleFont.lfQuality        = PROOF_QUALITY;
  m_titleFont.lfPitchAndFamily = FIXED_PITCH | FF_SWISS;
  lstrcpy(m_titleFont.lfFaceName, "ms sans serif");

  memset(&m_editFont, 0, sizeof(LOGFONT));
  m_editFont.lfHeight          = 12L;
  m_editFont.lfWeight          = 200L;
  m_editFont.lfQuality         = PROOF_QUALITY;
  m_editFont.lfPitchAndFamily  = FIXED_PITCH | FF_MODERN;
  lstrcpy(m_editFont.lfFaceName, "courier");

  m_imgList.DeleteImageList();
	 if( m_SelectImage.LoadBitmap( IDB_SELECTICON ) ) 
	 {
		m_SelectImage.GetObject( sizeof( m_SelectBitmap ) , &m_SelectBitmap );

		if( m_imgList.Create( m_SelectBitmap.bmWidth , m_SelectBitmap.bmWidth , ILC_COLORDDB | ILC_MASK ,  1 , 1 ) ) 
		{	
			m_imgList.Add( &m_SelectImage , RGB(255,0,255));
		}

	 }


 

};

void CStringGrid::SetFont(SGF_ENUM font, LOGFONT &lf)
{
  if(font == SGF_FIXEDCELLS)
  {
    m_fxFont = lf;
  }
  else if(font == SGF_CELLS)
  {
    m_cellFont = lf;
  }
  else if(font == SGF_TITLE)
  {
    m_titleFont = lf;
    return;   // skip resize
  }
  else if(font == SGF_EDIT)
  {
    m_editFont = lf;
    return;   // skip resize
  }

  // resize cells
  if(CheckOptions(SGO_AUTOSIZE))
  {
    for(int c = 0; c < m_colCount; ++c)
      AutoSizeCol(c);
  }

}

/******************************************************************************
* Function: ~CStringGrid
* Use:      Destructor
******************************************************************************/
CStringGrid::~CStringGrid()
{

	for(int i=0;i<m_rowCount;i++)
		delete (RowType*)m_drawRows[i];
	int s = m_rowHandles.GetSize();
	for(i=0;i<s;i++)
		delete (RowTypeHandle*)m_rowHandles[i];
	
	
//	if(m_gridhWnd  != NULL) ::DestroyWindow(m_gridhWnd);
}

SGA_ALIGNSTYLE CStringGrid::GetCellStyle(int col, ROWHANDLE row)
{
	RowType *rowPtr = GetRowByHandle(row);
   if(ColExists(col) && rowPtr) return rowPtr->m_cells[col].m_style;
  return SGA_NOSTYLE;
}

void CStringGrid::SetCellStyle(int col, ROWHANDLE row, SGA_ALIGNSTYLE style)
{
	RowType *rowPtr = GetRowByHandle(row);
   if(ColExists(col) && rowPtr) rowPtr->m_cells[col].m_style = style;
}

void CStringGrid::SetCellColor(int col, ROWHANDLE row, COLORREF color)
{
	RowType *rowPtr = GetRowByHandle(row);
   if(ColExists(col) && rowPtr)rowPtr->m_cells[col].m_color = color;
}


bool  CStringGrid::SetCellStrPtr(int col,ROWHANDLE row,CString *strPtr)
{
	return SetCell(col,row,(void*)strPtr,CellType::PSTRING,FALSE);
}

bool CStringGrid::SetCellIntPtr(int col,ROWHANDLE row,int *intPtr)
{
	return SetCell(col,row,(void*)intPtr,CellType::PNUMERIC,FALSE);
}

bool CStringGrid::SetCellStr(int col,ROWHANDLE row,CString &strVal)
{
	CString *newStr = new CString(strVal);
	if(!SetCell(col,row,(void*)newStr,CellType::PSTRING,TRUE))
	{
		delete newStr;
		return false;
	}
	return true;
}

bool  CStringGrid::SetCellInt(int col,ROWHANDLE row,int intVal)
{
	int *newInt = new int;
	*newInt = intVal;
	if(!SetCell(col,row,(void*)newInt,CellType::PNUMERIC,TRUE))
	{
		delete newInt;
		return false;
	}
	return true;
}




bool CStringGrid::SetCell(int col, ROWHANDLE row, void *ptr,CellType::SG_CELLTYPE cellType, BOOL b_isOwner)
{
  VERIFY(ptr);
  CellType *cellPtr;
  RowType *rowPtr = GetRowByHandle(row);
   if(!(ColExists(col) && rowPtr)) return false;
  cellPtr = &rowPtr->m_cells[col];
  return cellPtr->SetThisCell(ptr,cellType,b_isOwner);
}

int CStringGrid::CellInt(int col,ROWHANDLE row)
{
	RowType *rowPtr = GetRowByHandle(row);
   if(ColExists(col) && rowPtr)
  {
	  CellType *cellPtr = &rowPtr->m_cells[col];
	  if(cellPtr->m_tType == CellType::PNUMERIC)
		  return (*cellPtr->m_pInt);
  }
 
  return 0;
}

LPCSTR CStringGrid::CellString(int col, ROWHANDLE row)
{
 RowType *rowPtr = GetRowByHandle(row);
   if(ColExists(col) && rowPtr)
  {
	  CellType *cellPtr = &rowPtr->m_cells[col];
	  if(cellPtr->m_tType == CellType::PSTRING)
		  return (LPCSTR)(*cellPtr->m_pStr);
	  
  }
 
  return NULL;
}

CString CStringGrid::drawCell(int col,int row)
{
char buf[17];

  if(CellExists(col, row))
  {
	  CellType *cellPtr = &m_drawRows[row]->m_cells[col];
	  switch(cellPtr->m_tType)
	  {
		  case CellType::PSTRING:
              return (*cellPtr->m_pStr);
		  case CellType::PNUMERIC:
			  sprintf(buf,"%d",(*cellPtr->m_pInt));
    		  return CString(buf);
	  }	  
  }

  return CString("");
}

/******************************************************************************
* Function: GetCellRect
* Use:      gets rectangle with cell coordinates in rc
*           returns false if cell not visible
* Args:     int c - colunn
*           int r - row
*
******************************************************************************/
bool CStringGrid::GetCellRect(int c, ROWHANDLE r, RECT& rc)
{
  /*if(c < m_leftCol || c >= m_colCount || r < m_topRow || r >= m_rowCount)
    return false;

  int top = HeadHeight() + m_rowHeight * (r - m_topRow);
  if(top >= m_height) return false;

  int left = m_columns[0].m_width;
  for(int i = m_leftCol; i < c; i++)
    left += m_columns[i].m_width;

  if(left >= m_width) return false;

  rc.left    = left + 1;
  rc.right   = left + m_columns[c].m_width;
  rc.top     = top + 1;
  rc.bottom  = top + m_rowHeight;
  */
	throw;

  return true;
}

/******************************************************************************
* Function: HitTestCell
* Use:      finds which cell point (x, y) is in 
* Args:     int x   - x coord of point to test
*           int y   - y coord of point to test
*           int& c  - reference to variable to receive column point is in
*           int& r  - reference to variable to receive row point is in
******************************************************************************/
bool CStringGrid::HitTestCell(int x, int y, int& c, int& r)
{
	int col,left;

	if( x > m_width || y < HeadHeight() || y > m_height)
    return false;
  
	if(x < m_columns[0].m_width)
		col = 0;
	else
	{
	  left  = m_columns[0].m_width  + m_columns[m_leftCol].m_width;
	  col   = m_leftCol;
	  while(left < x && col < m_rightCol + 1)
	  {
		col++;
		left += m_columns[col].m_width;
	  }  
	}

  int row = m_topRow;
  RowType *rowPtr = m_drawRows[row];
  int top =  HeadHeight() + ((int)rowPtr->visible)*(m_rowHeight + rowPtr->GetCloakHeight());
  

  while(top < y && row < m_bottomRow)
  {
	row++;
	rowPtr = m_drawRows[row];
    top += ((int)rowPtr->visible)*(m_rowHeight+rowPtr->GetCloakHeight());
  }

  if(top < y) return false; //still out off range

  if((col <= m_rightCol) && (row <= m_bottomRow))
  {
    c = col;
    r = row;
    return true;
  }
  
    return false;   
 
}
//*****************************************************************************
int CStringGrid::HeaderColHit(int x,int y)
{
	int col,left;
	if( x > m_width || y > HeadHeight() || y > m_height)
    return -1;
  
	if(x < m_columns[0].m_width)
		col = 0;
	else
	{
	  left  = m_columns[0].m_width  + m_columns[m_leftCol].m_width;
	  col   = m_leftCol;
	  while(left < x && col < m_rightCol + 1)
	  {
		col++;
		left += m_columns[col].m_width;
	  }  
	}
	return col;

}
/******************************************************************************
* Function: BtnCell
* Use:      Draws cells with 3D button look
* Args:     RECT rc - rectangle with cell coordinates
******************************************************************************/
void CStringGrid::BtnCell(RECT rc)
{
//  HGDIOBJ oldPen;
  HBRUSH br = CreateSolidBrush(m_colors[SGC_FXDCELL]);
//  COLORREF gradcolor;
  // fill cell
  //rc.left++;
  rc.top++; 
  
  //  if not xp style, solid fill
  //if(!CheckOptions(SGO_XPSTYLE))
 // {
    FillRect(m_memdc, &rc, br);
 // }
 //  else gradient fill
  //else
  //{
//    TRIVERTEX verts[2];
  //  GRADIENT_RECT gr  = {0, 1};

  /*  gradcolor         = m_colors[SGC_GRADTOP];
    verts[0].x        = rc.left;
    verts[0].y        = rc.top;
    verts[0].Red      = (WORD) ((gradcolor & 0x000000FF) << 8);
    verts[0].Green    = (WORD) (gradcolor & 0x0000FF00);       
    verts[0].Blue     = (WORD) ((gradcolor & 0x00FF0000) >> 8);
    verts[0].Alpha    = 0;
    
    gradcolor         = m_colors[SGC_GRADBOTTOM];
    verts[1].x        = rc.right;
    verts[1].y        = rc.bottom;
    verts[1].Red      = (WORD) ((gradcolor & 0x000000FF) << 8);
    verts[1].Green    = (WORD) (gradcolor & 0x0000FF00);       
    verts[1].Blue     = (WORD) ((gradcolor & 0x00FF0000) >> 8);
    verts[1].Alpha    = 0;
    
    if(rc.top > HeadHeight() && CheckOptions(SGO_XPHORZ)
        && rc.left < m_columns[0].m_width)
    GradientFill(m_memdc, verts, 2, &gr, 1, GRADIENT_FILL_RECT_H);
    else
    GradientFill(m_memdc, verts, 2, &gr, 1, GRADIENT_FILL_RECT_V);*/
  //}
  // adjust rect
  //rc.right--;
 // rc.bottom--;

  // draw hilights
 /* oldPen = SelectObject(m_memdc, CreatePen(PS_SOLID, 0, 
                        GetSysColor(COLOR_BTNHILIGHT)));
  MoveToEx(m_memdc, rc.left, rc.bottom, NULL);
  LineTo(m_memdc, rc.left, rc.top);
  LineTo(m_memdc, rc.right, rc.top);

  //draw shadows
  DeleteObject(SelectObject(m_memdc, CreatePen(PS_SOLID, 0, 
                            GetSysColor(COLOR_BTNSHADOW))));
  LineTo(m_memdc, rc.right, rc.bottom);
  LineTo(m_memdc, rc.left, rc.bottom);
    
  DeleteObject(SelectObject(m_memdc, oldPen));*/
  DeleteObject(br);
}

int CStringGrid::AvgColWidth()
{
  int sum = 0;
  if(m_colCount > 0)
  {
    for(int i = 0; i < m_colCount; i++)
    {
      sum += m_columns[i].m_width;
    }
    return (sum / m_colCount);
  }
  return 1;
}

int CStringGrid::GetColWidth(int col)
{
 
  if(col >= 0 && col < m_colCount)
  {
     return m_columns[col].m_width;
   }
  return 0;

}



/******************************************************************************
* Function: AddOptions
* Use:      Sets option flags
* Args:     DWORD options - option flags to be set, can be ORed together
******************************************************************************/
void CStringGrid::AddOptions(DWORD options)
{
  // if autosize, turn off usersize and vice-versa 
  if(options & SGO_AUTOSIZE)
  {
    m_dwOptions = m_dwOptions & ~SGO_USERSIZE;
    options = options & ~SGO_USERSIZE;
  }
  else if(options & SGO_USERSIZE)
  {
    m_dwOptions = m_dwOptions & ~SGO_AUTOSIZE;
    options = options & ~SGO_AUTOSIZE;
  }
    
  m_dwOptions = m_dwOptions | options;
}

/******************************************************************************
* Function: DelOptions
* Use:      Clears option flags
* Args:     DWORD options - option flags to be cleared, can be ORed together
******************************************************************************/
void CStringGrid::DelOptions(DWORD options)
{
  m_dwOptions = m_dwOptions & ~options;
}

/******************************************************************************
* Function: CheckOptions
* Use:      Checks if option flags are set
* Args:     DWORD options - option flags to be checked, can be ORed together
* Returns:  bool - true if flags set, false if not set
******************************************************************************/
bool CStringGrid::CheckOptions(DWORD options)
{
  return ((m_dwOptions & options) == options) ? true : false;
}

/******************************************************************************
* Function: SetOptions
* Use:      Sets options
* Args:     DWORD options - option flags to be set, can be ORed together
******************************************************************************/
void CStringGrid::SetOptions(DWORD options)
{
  // disallow usersize if autosize 
  if(options & SGO_AUTOSIZE)
    options = options & ~SGO_USERSIZE;
    
  m_dwOptions = options;
}

/******************************************************************************
* Function: SetColor
* Use:      Sets indexed color in color table
* Args:     DWORD index - index of color to set
* Returns:  COLORREF - old color, or SGC_ERROR if index out of range
******************************************************************************/
COLORREF CStringGrid::SetColor(SGC_ENUM index, COLORREF color)
{
  if(index < NUM_COLORS)
  {
    COLORREF temp   = m_colors[index];
    m_colors[index] = color;
    return temp;
  }
  else
  {
    return SGC_ERROR;
  }
}

/******************************************************************************
* Function: GetColor
* Use:      Gets indexed color from color table
* Args:     DWORD index - index of color to get
* Returns:  COLORREF - color, or SGC_ERROR if index out of range
******************************************************************************/
COLORREF CStringGrid::GetColor(SGC_ENUM index)
{
  if(index < NUM_COLORS)
  {
    return m_colors[index];
  }
  else
  {
    return SGC_ERROR;
  }
}

/******************************************************************************
* Function: GetCellColor
* Use:      Gets text color of cell(col, row)
* Args:     int col - column of cell
*           int row - row of cell  
* Returns:  COLORREF - text color for cell, defaults to first color in table
******************************************************************************/
COLORREF CStringGrid::GetCellColor(int col, ROWHANDLE row)
{
 RowType *rowPtr = GetRowByHandle(row);
 if(ColExists(col) && rowPtr)
  {
  	  return rowPtr->m_cells[col].m_color;
  }
  return NO_COLOR;
}

bool CStringGrid::ResizeGrid(int cols, int rows)
{
  bool flag = false;
  
  if(cols < ColCount() && cols > 1)
  {
    for(int c = ColCount(); c > cols; c--)
    {
      fastDeleteCol(cols);
    }
    flag = true;
  }
  else if(cols > ColCount() && cols <= MAX_COLS)
  {
    int cc = ColCount();
    for(int c = cc; c < cols; c++)
    {
      fastInsertCol(cc, DEFAULT_COLUMN_WIDTH); //fastInsertCol
    }
    flag = true;
  }

  if(rows < RowCount() /*&& rows > 1*/)
  {
    for(int r = RowCount(); r > rows; r--)
    {		
		drawDeleteRow(rows,false);
    }
    flag = true;
  }
  else if(rows > RowCount() && rows <= MAX_ROWS)
  {
    int rc = RowCount();
    for(int r = rc; r < rows; r++)
    {
      fastAddRow();
    }
    flag = true;
  }

  if(flag == false)
  {
    return false;
  }
  else
  {

    m_leftCol = 1;
    SetScrollbars();
    return true;
  }
}

/*! Clears all cells except fixed and redraws grid. \n Does not change size of grid. */
void CStringGrid::ClearGrid()
{
  for(int c = 1; c < ColCount(); c++)
    for(int r = 1; r < RowCount(); r++)
		SetCell(c, r, NULL,CellType::PNULL,FALSE);
      
  DrawGrid();
}  

int CStringGrid::ComputePageSize()
{
	int cH = 0,ret;
  RowType *rowPtr;
  TRACE("count = %d, top = %d, bottom = %d\n",m_rowCount,m_topRow,m_bottomRow);
  for(int i = m_topRow; i <= m_bottomRow;i++) 
  {
	 //TRACE("i idx = %d\n",i);
	  rowPtr = m_drawRows[i];
	  if(rowPtr->visible) cH += rowPtr->GetCloakHeight();
  }

  ret = min(m_VisRows,((m_height - TitleHeight() - cH) / m_rowHeight));
  return  (ret < 0 ? 0 : ret);

}
void CStringGrid::MesureClientRect()
{
	RECT rc;
	  GetClientRect(&rc);
      m_top    = rc.top;
      m_left   = rc.left;
      m_width  = rc.right - rc.left;
      m_height = rc.bottom - rc.top;
	  m_ScreenRows = (m_height/m_rowHeight);
	  if(CheckOptions(SGO_FIXEDCOL)) m_ScreenRows--;
}


void CStringGrid::SetScrollbars(BOOL setVert)
{

  MesureClientRect();	
  SCROLLINFO      si;                   
	
  si.cbSize = sizeof(SCROLLINFO);
  si.fMask  = 
              SIF_RANGE | 
              SIF_PAGE  |
              SIF_POS   |
              0;
  si.nMin   = 1;
  si.nMax   = m_VisRows;

  TRACE("m_VisRows    = %d\n",m_VisRows);

  si.nPage  = ComputePageSize();
  
  TRACE("page = %d\n",si.nPage);

  si.nPos   = m_topRow;
  SetScrollInfo(SB_VERT, &si, TRUE);

  if(setVert)
  {
  
	int c = ColCount() - 1;
	int x = m_columns[0].m_width;
	while(x < (m_width) && c >= 0){
		x += m_columns[c].m_width;
		c--;
	}
	if(c == -1) m_leftCol = 1;

	si.nMin   = 1;
	si.nPage  = ColCount() - (c + 1);
	si.nMax   = m_colCount;
	si.nPos   = m_leftCol;
	SetScrollInfo( SB_HORZ, &si, TRUE);
  }


}





int CStringGrid::HeadHeight()
{
  // add title and row heights dependant on option settings
  int height = ((CheckOptions(SGO_TITLE))     ? m_titleHeight : 0) +
               ((CheckOptions(SGO_FIXEDROW))  ? m_rowHeight   : 0);
  
  return height;
}

int CStringGrid::TitleHeight()
{
  return (CheckOptions(SGO_TITLE)) ? m_titleHeight : 0;
}

void CStringGrid::SetTitleString(LPCSTR title)
{
  m_titleString = title;
}

LPCSTR CStringGrid::GetTitleString()
{
  return (LPCSTR)m_titleString;
}



void  CStringGrid::FreezHighlight(void)
{
//	DebugMsg("Freezing...\n");
	m_bFreezHighlight = true;
}


void CStringGrid::UnFreezHighlight(void)
{
	m_bFreezHighlight = false;
}

void* CStringGrid::GetHighlightedRowPtr(void)
{
	if(m_row_hlight > 0) return m_drawRows[m_row_hlight]->userptr;
	return NULL;
}

bool  CStringGrid::SetRowCloakSize(ROWHANDLE row,int cloakrows,int cloakcols)
{
	RowType *rowPtr = GetRowByHandle(row,NULL);
	if(rowPtr) return rowPtr->SetCloakSize(cloakrows,cloakcols);
	return false;
}

int CStringGrid::drawFindRow(RowType *rowPtr)
{
	for(int i=0,s = m_drawRows.GetSize();i<s;i++)
		if(m_drawRows[i] == rowPtr) return i;

	return -1;
}

void  CStringGrid::SetRowVisible(ROWHANDLE iRow,bool bVis)
{

	RowType *rowPtr = GetRowByHandle(iRow);
	if(rowPtr) drawSetRowVisible(drawFindRow(rowPtr),bVis);

}
void CStringGrid::drawSetRowVisible(int iRow,bool bVis)
{
	
	RowType *rowPtr;
	if(iRow > 0 && iRow < RowCount())
	{
		rowPtr = m_drawRows[iRow];

		if((iRow <= m_bottomRow && iRow >= m_topRow) || ((m_VisRows-1) < m_ScreenRows)) 
		{
			if(!bVis && rowPtr->visible)
			{
				if((m_bottomRow+1) < m_rowCount)
					m_bottomRow++;
				else
				{
					if((m_topRow-1) > 0) m_topRow--;
				}

				

			} else if (!rowPtr->visible)
			{

				if((m_VisRows > m_ScreenRows) )
					m_bottomRow--;
				else if ((m_bottomRow+1) < m_rowCount) m_bottomRow++;
			}
		}

		if(rowPtr->visible != bVis) 	m_VisRows += ( bVis ? 1 : (-1));

		rowPtr->visible = bVis;
			
			//DebugMsg("A bottom = %d, top = %d\n",m_bottomRow,m_topRow);
		}
}

void CStringGrid::ToggleVisible(ROWHANDLE iRow)
{
	RowType *rowPtr = GetRowByHandle(iRow,NULL);
	if(rowPtr)
	{
	    drawSetRowVisible(drawFindRow(rowPtr),!rowPtr->visible);
	}
}

bool  CStringGrid::SetRowColor(ROWHANDLE row,COLORREF color)
{
	RowType *rowPtr = GetRowByHandle(row,NULL);
	if(rowPtr) 
	{
		rowPtr->m_color = color;
		return true;
	}
	return false;
}

RowType *CStringGrid::GetRowPtr(ROWHANDLE row)
{
return GetRowByHandle(row,NULL);
}


ROWHANDLE  CStringGrid::GetHighlightedRowHandle()
{
	int i,s;
	if(m_row_hlight <= 0) return NULLROWHANDLE;
	RowType *rowPtr = m_drawRows[m_row_hlight];

	for(i=0,s = m_rowHandles.GetSize();i<s;i++)
		if(rowPtr == m_rowHandles[i]->row)
		{
			return m_rowHandles[i]->rowHandle;
		}
return NULLROWHANDLE;
}

bool CStringGrid::GetRowVisible(ROWHANDLE row)
{
	RowType *rowPtr = GetRowByHandle(row,NULL);
	if(rowPtr) return rowPtr->visible;
	return false;
}

RowType *CStringGrid::GetRowByHandle(ROWHANDLE rowHndl,int *index)
{
	int m;
	RowType *rowPtr = searchRow(0,m_rowHandles.GetSize()-1,rowHndl,m);
	if(index) *index = m;
	return rowPtr;
}

RowType* CStringGrid::searchRow(int l, int r,ROWHANDLE v,int &idx)
{
	if(l > r) return NULL;
	int m = (l+r)/2;
	RowTypeHandle *rowHandle = m_rowHandles[m];
	if(v == rowHandle->rowHandle)
	{
		idx = m;
		return rowHandle->row;
	}
	if(v < rowHandle->rowHandle) return searchRow(l,m-1,v,idx);
	else return searchRow(m+1,r,v,idx);
	return NULL;
}

bool  CStringGrid::SortByCol(int col)
{
int iSize = m_drawRows.GetSize();
if (iSize > 1 && ColExists(col)) {

		try {

			int	low = 1,
				high = iSize - 1;

			QuickSortRecursive(col,low,high,m_columns[col].m_bAscending);

		} catch (...) {
		  return false;
		}

	} else 	return false;
	
	m_columns[col].m_bAscending = !m_columns[col].m_bAscending;
	DrawGrid();

	return true;
}

void CStringGrid::QuickSortRecursive(int col,int d, int h, BOOL bAscending)
{
	int i,j;
	
	i = h;
	j = d;

	

	CellType *cell = &m_drawRows[ (d+h) / 2]->m_cells[col];
	 
	do {

		if (bAscending) {
			while (m_drawRows[j]->m_cells[col] < *cell) j++;
			while (m_drawRows[i]->m_cells[col] > *cell) i--;
		} else {
			while (m_drawRows[j]->m_cells[col] > *cell) j++;
			while (m_drawRows[i]->m_cells[col] < *cell) i--;
		}

		if ( i >= j ) {

			if ( i != j ) 
			{
				

				RowType *rowPtri,*rowPtrj;
				rowPtri = m_drawRows[i];
				rowPtrj = m_drawRows[j];
				
				m_drawRows.RemoveAt(i);
				m_drawRows.InsertAt(i,rowPtrj);

				m_drawRows.RemoveAt(j);
				m_drawRows.InsertAt(j,rowPtri);

			
			}

			i--;
			j++;
		}
	} while (j <= i);

	if (d < i) QuickSortRecursive(col,d,i,bAscending);
	if (j < h) QuickSortRecursive(col,j,h,bAscending);
}


BOOL operator>(const CellType &left, const CellType &right)
{
	if(left.m_tType == right.m_tType)
	{

		switch(left.m_tType)
		{
		case CellType::PNULL:
			return true; //what does it matter
		case CellType::PNUMERIC:
			return (*left.m_pInt > *right.m_pInt);

		case CellType::PSTRING:
			return ((*left.m_pStr).Compare((LPCSTR)(*right.m_pStr)) < 0);
		}
	} else {
	
		if((left.m_tType == CellType::PNUMERIC || left.m_tType == CellType::PNULL) && right.m_tType == CellType::PSTRING)
				return false;
		if(left.m_tType == CellType::PNULL) return false;
		if(right.m_tType == CellType::PNULL) return true;
	
	}

	ASSERT(0); //i dont know when program can get here
	return false;
}

BOOL operator<(const CellType &left, const CellType &right)
{
	return operator>(right,left);
}


void  CStringGrid::ShowAllRows(BOOL bRedraw)
{
int rcount = RowCount(),i;

if(m_VisRows < rcount)
{
	for(i=1/*skiping fixed*/;i<rcount;i++)
		drawSetRowVisible(i,true);

	if(bRedraw)
	{
		DrawGrid();
		SetScrollbars();
	}
}


}

int CStringGrid::GetSelectedRows(CPtrArray &ary_sel)
{

	RowType *rowPtr;
	void *data;

	for(int i=1/*skiping first fixed row*/,ii=m_drawRows.GetSize();i<ii;i++)
	{
		rowPtr = m_drawRows[i]; 
		if(rowPtr->selected && rowPtr->visible)
		{
			data = (void*)rowPtr->userptr;			
			VERIFY(data);
			ary_sel.Add(data);
		}

	}
return ary_sel.GetSize();
}

void CStringGrid::SelectHighlightedRow()
{
	if(m_row_hlight  > 0)
		((RowType*)m_drawRows[m_row_hlight])->selected = true;
	DrawGrid();
}
void CStringGrid::UnselectHighlightedRow()
{
		if(m_row_hlight  > 0)
		((RowType*)m_drawRows[m_row_hlight])->selected = false;
	DrawGrid();

}
void CStringGrid::SelectAll()
{
		for(int i =0,s = m_drawRows.GetSize();i<s;i++)
			((RowType*)m_drawRows[i])->selected = true;
	DrawGrid();

}
void CStringGrid::SelectVisibleRows()
{
	RowType *rowPtr;
	for(int i =0,s = m_drawRows.GetSize();i<s;i++)
	{
		rowPtr = (RowType*)m_drawRows[i];
			if(rowPtr->visible) rowPtr->selected = true;
	}
	ReDraw();
}
void CStringGrid::UnselectAll()
{
		for(int i =0,s = m_drawRows.GetSize();i<s;i++)
			((RowType*)m_drawRows[i])->selected = false;
	ReDraw();

}

void CStringGrid::InvertSelection()
{
	RowType *rowPtr;
	for(int i =0,s = m_drawRows.GetSize();i<s;i++)
	{
		rowPtr = (RowType*)m_drawRows[i];
		rowPtr->selected = !rowPtr->selected;
	}
	ReDraw();
}

