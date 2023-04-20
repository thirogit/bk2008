#include "stdafx.h"
#include "cstringgrid.h"
#include "MainFrm.h"
#include <cctype>

UINT uMSG_MOUSEWHEEL;

BEGIN_MESSAGE_MAP(CStringGrid, CView)
	
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	
END_MESSAGE_MAP()


void CStringGrid::OnDestroy()
{
      if(m_memdc)
      {
        if(m_hOldBitmap)
        {
          DeleteObject(SelectObject(m_memdc, m_hOldBitmap));
        }
        DeleteDC(m_memdc);
      }
	  CView::OnDestroy();
}
    /*    
    case WM_GETDLGCODE:
      return DLGC_WANTALLKEYS;
    break;*/

void CStringGrid::OnMouseMove(UINT nFlags,CPoint point)    
{
  
     int xPos = point.x;  // horizontal position of cursor 
     int yPos = point.y;  // vertical position of cursor 
		
	  int   cellcol, cellrow;
      if(HitTestCell(xPos,yPos,cellcol,cellrow))
	  {
		  //TRACE("cellrow = %d, cellcol = %d\n",cellrow,cellcol);
				if((m_row_hlight != cellrow || m_row_hlight < 0) && !m_bFreezHighlight)
				{
					m_row_hlight = cellrow;
					DrawGrid();
				}

	  }
	  else if(m_row_hlight >= 0 && !m_bFreezHighlight)
	  {
		  m_row_hlight = -1;
		  DrawGrid();
	  }
		

      if(CheckOptions(SGO_USERSIZE))
      {      
          if(m_bSizing)
        {
          int w = m_sizingLeft - xPos;

          if(m_columns[m_sizingCol].m_width > w+2)
          {
            m_columns[m_sizingCol].m_width -= w;
            m_sizingLeft -= w;
          }
  
          SetCursor(LoadCursor(NULL, IDC_SIZEWE));
          DrawGrid();
        }
        else if(yPos > HeadHeight() - m_rowHeight && yPos < HeadHeight())
        {
          int left  = m_columns[0].m_width + m_columns[m_leftCol].m_width;
          int col   = m_leftCol;
          while(left < (xPos) && col < RightColumn())
          {
            col++;
            left += m_columns[col].m_width;
          }  
  
          if(xPos > (left - 5) && xPos < (left + 10))
          {
            m_bSizeGrip = true;
            m_sizingCol = col;
            m_sizingLeft = left;
            SetCursor(LoadCursor(NULL, IDC_SIZEWE));
          }
          else
          {
            m_bSizeGrip = false;
            SetCursor(LoadCursor(NULL, IDC_ARROW));
          }
        }
        else
        {
          m_bSizeGrip = false;
          SetCursor(LoadCursor(NULL, IDC_ARROW));
        }
      }
}


void CStringGrid::OnVScroll(UINT nSBCode,UINT nPos,CScrollBar *pScrollBar)
{
		SCROLLINFO      si;                   
		 bool redraw = true;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask  = SIF_ALL;
        GetScrollInfo(SB_VERT, &si);

		

        switch(nSBCode)
        {
          case SB_LINEUP:
            if(si.nPos > si.nMin) si.nPos--;
            
            break;

          case SB_LINEDOWN:
            if(m_bottomRow < m_rowCount - 1) si.nPos++;
                     
            break;

          case SB_PAGEUP:
          {
            if((si.nPos - ((int)si.nPage /*- 2*/)) > si.nMin)
            {
              si.nPos -= ((int)si.nPage /*- 2*/);
            }
            else
            {
              si.nPos = si.nMin;
            }
            break;
          }
          case SB_PAGEDOWN:
          {
           
            if(si.nPos + (int)si.nPage /*- 2*/ < si.nMax)
            {
              if(m_rowCount - m_bottomRow < (int)si.nPage)
              {
                si.nPos = m_rowCount - (si.nPage - 1);
              }
              else
              {
                si.nPos += (int)si.nPage /*- 2*/;
              }
            }
            break;
          }

          case SB_TOP:
            si.nPos = si.nMin;
            break;

          case SB_BOTTOM:
            si.nPos = m_rowCount - (si.nPage - 1);
            break;

          case SB_THUMBPOSITION:
          case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;

          default:
            redraw = false;
            break;
        }

        if(redraw == true) 
		{
			if(m_rowCount > 1)
			{           
				m_topRow  = si.nPos;
				m_bottomRow = BottomRow();
				SetScrollbars(FALSE);
			}
			DrawGrid();
           	
		}
		CView::OnVScroll(nSBCode,si.nPos,pScrollBar);
     
}

void CStringGrid::OnHScroll(UINT nSBCode,UINT nPos,CScrollBar *pScrollBar)
{
		SCROLLINFO      si; 
        bool redraw = true;
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask  = SIF_ALL;
        GetScrollInfo( SB_HORZ, &si);

        switch(nSBCode)
        {
          case SB_LINELEFT:
            if(si.nPos > si.nMin) si.nPos--;
           
            break;
            
          case SB_LINERIGHT:
            m_rightCol = RightColumn();
            if(m_rightCol < m_colCount - 1) si.nPos++;
            break;
            
          case SB_PAGELEFT:
          {
            int x = LeftColumn();
            // save offset of current column from left edge
        

            while(x < RightColumn() && LeftColumn() > 1)
            {
              m_leftCol--;
              si.nPos--;
            }

          

            break;
          }
           
          case SB_PAGERIGHT:
          {
            int i = RightColumn();

           

            if(i > ColCount() - ((int)si.nPage + 1))
            {
              i = ColCount() - (si.nPage - 1);
            }
            else if(i == m_leftCol && i < (m_colCount - 1))
            {
              i ++;
            }

            m_leftCol = i;
            si.nPos = m_leftCol;

         
            break;
          }

          case SB_LEFT:
            si.nPos = si.nMin;
            break;
            
          case SB_RIGHT:
          
            m_leftCol = ColCount() - (si.nPage - 1);
            DrawGrid();
            //SetScrollbars();
            si.nPos = si.nMax;
            
            break;
            
          case SB_THUMBPOSITION:
          case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;
          
          default:
            redraw = false;
            break;    
        }
        if(redraw == true)
        {
			::SetScrollInfo(m_gridhWnd, SB_HORZ, &si, TRUE);
          m_leftCol = si.nPos;
         
          DrawGrid();
        }
    
	CView::OnHScroll(nSBCode,si.nPos,pScrollBar);

		
}


int CStringGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	

  m_gridhWnd = m_hWnd;
  m_top     = lpCreateStruct->y;
  m_left    = lpCreateStruct->x;
  m_width   = lpCreateStruct->cx;
  m_height  = lpCreateStruct->cy;

  m_ScreenRows = (m_height/m_rowHeight);
  if(CheckOptions(SGO_FIXEDROW)) m_ScreenRows--;
  

	  HBRUSH      hbBkgd  = CreateSolidBrush(m_colors[SGC_GRIDBKGND]);
      HFONT       hfEdit  = CreateFontIndirect(&m_editFont);
      int         maxX    = GetSystemMetrics(SM_CXSCREEN);
      int         maxY    = GetSystemMetrics(SM_CYSCREEN);
	  CDC         *pDC    = GetDC();  
	  HDC         hdc     = pDC->GetSafeHdc();
      
      m_memdc   = CreateCompatibleDC(hdc);
      m_hBitmap = CreateCompatibleBitmap(hdc, maxX, maxY);
      ReleaseDC(pDC);

      m_hOldBitmap = SelectObject(m_memdc, m_hBitmap);

      int         iSaveDC = SaveDC(m_memdc);
      // fill background
      SelectObject(m_memdc, hbBkgd);
      PatBlt(m_memdc, 0, 0, maxX, maxY, PATCOPY);

      RestoreDC(m_memdc, iSaveDC);
      DeleteObject(hbBkgd);
      DeleteObject(hfEdit);

	  ///EnableScrollBar(SB_VERT);
	  
	
  return 0;
}

void CStringGrid::OnDraw(CDC* pDC)
{
	TRACE("OnDraw()\n");

}

void CStringGrid::OnPaint() 
{
	
	PAINTSTRUCT ps;
	HWND        hWnd = this->GetSafeHwnd();
	HDC         hdc = ::BeginPaint(hWnd, &ps);
      
      BitBlt(hdc, ps.rcPaint.left, ps.rcPaint.top,
        ps.rcPaint.right-ps.rcPaint.left,
        ps.rcPaint.bottom-ps.rcPaint.top,
        m_memdc,
        ps.rcPaint.left, ps.rcPaint.top,
        SRCCOPY);
    
      ::EndPaint(hWnd, &ps); 

	
}

void CStringGrid::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	

     // m_bottomRow = BottomRow();
      m_rightCol  = RightColumn();
 // gry powiekszamy to ok , ale tylko wtedy 
	  //gdy leftCol = 1;
	  // przy powiekszaniu zrobic tak zeby szukalo wstecz kolumn 
	  //i cofalo leftCol
     if(m_rowCount > 1)
	 {
		if((m_bottomRow - m_topRow) < m_ScreenRows) 
		{
			int x = m_ScreenRows - (m_bottomRow - m_topRow);
			m_topRow = max(1, m_topRow - x); 
		}  
		m_bottomRow = BottomRow();
	 }
	  TRACE("toprow= %d, bottomrow = %d\n",m_topRow,m_bottomRow);

     SetScrollbars(); 
     DrawGrid();
 }
void CStringGrid::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);

	  m_bFocused  = true;
      m_bGrayed   = false;            
      DrawGrid();
      
}

void CStringGrid::OnKillFocus(CWnd *pNewWnd)
{
      m_bFocused = false;

      if(CheckOptions(SGO_SHOWFOCUS))
        m_bGrayed = true;
         
      DrawGrid();

}

void CStringGrid::OnLButtonDblClk(UINT nFlags,CPoint point)
{
 	
      int col, row;
      if(HitTestCell(point.x,point.y, col, row) == true)
      {
		  
		  RowType* rowPtr = m_drawRows[row];

		 if(col == 0) rowPtr->toogle_select();			 
		  else  
		  {
			  rowPtr->toogle();
  
			  SetScrollbars();
		  }
		  DrawGrid();

	  }
}
void CStringGrid::OnLButtonDown(UINT nFlags,CPoint point)
{     
      if(m_bSizeGrip)
      {
        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        m_bSizing = true;
      }

	  if((m_nWchichCol = HeaderColHit(point.x,point.y)) > 0)
	  {
		  m_bWantToSort = true;
		  TRACE("DOWN m_nWchichCol = %d\n",m_nWchichCol);
	  }
	  
     
      SetFocus();
 } 
	
void CStringGrid::OnLButtonUp(UINT nFlags,CPoint point)
{
      if(m_bSizing) 
	  {
		  SetScrollbars();
		  DrawGrid();
		  m_bSizing = false;
      }
	  if(m_bWantToSort)
	  {
		   TRACE("UP m_nWchichCol = %d\n",m_nWchichCol);
		  m_bWantToSort = false;
		  if(m_nWchichCol == HeaderColHit(point.x,point.y)) //cheching is this the same col 
			SortByCol(m_nWchichCol);
		  m_nWchichCol = -1;

	  }	
 }
    
     
void CStringGrid::OnKeyDown(UINT nChar, UINT nRepCnt,UINT nFlags)  
{   
        switch(nChar)
        {

		/*case VK_RETURN:

			for(int i =0;i<m_drawRows.GetSize();i++)
				drawSetRowVisible(i,true);
			break;
		case VK_SPACE:
			{
			TRACE("row hlight = %d\n",m_row_hlight);
			ROWHANDLE rowh = GetHighlightedRowHandle();
			if(rowh)
			{
				DeleteRow(rowh);
				ReDraw();
			}
			}	
		break;

		case VK_F1:

			AddRow();
			ReDraw();
			break;

		case VK_F2:
			drawDeleteRow(2);
			ReDraw();
			break;*/
          case VK_UP:
          {
            // shift-up, move line up, cursor doesn't move
            if(GetKeyState(VK_SHIFT) < 0)
                     SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, NULL), (LPARAM)NULL);
                        
            // ctrl-up, move page up, cursor doesn't move
            else if(GetKeyState(VK_CONTROL) < 0)
                     SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, NULL),(LPARAM)NULL);
                
            break;
		  }
            
          case VK_DOWN:
          {
            // shift-down, move line down, cursor doesn't move
            if(GetKeyState(VK_SHIFT) < 0)
                SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN, NULL), (LPARAM)NULL);
            // ctrl-down, move page down, cursor doesn't move
            else if(GetKeyState(VK_CONTROL) < 0)
              SendMessage( WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, NULL), (LPARAM)NULL);
            break;
          }
          
          case VK_RIGHT:
          {
            if(GetKeyState(VK_SHIFT) < 0)
                 SendMessage( WM_HSCROLL, MAKEWPARAM(SB_LINERIGHT, NULL), (LPARAM)NULL);
            else if(GetKeyState(VK_CONTROL) < 0)
                 SendMessage( WM_HSCROLL, MAKEWPARAM(SB_PAGERIGHT, NULL), (LPARAM)NULL);
            break;
          }
            
          case VK_LEFT:
            // shift-left, move col left, cursor doesn't move
            if(GetKeyState(VK_SHIFT) < 0)
                SendMessage(WM_HSCROLL, MAKEWPARAM(SB_LINELEFT, NULL), (LPARAM)NULL);
            // ctrl-left, move page left, cursor doesn't move
            else if(GetKeyState(VK_CONTROL) < 0)
                SendMessage( WM_HSCROLL, MAKEWPARAM(SB_PAGELEFT, NULL), (LPARAM)NULL);
            
            break;
          
          case VK_PRIOR:
          {           
            SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEUP, NULL), (LPARAM)NULL);
            break;
          }
           
          case VK_NEXT:
          {
             SendMessage( WM_VSCROLL,MAKEWPARAM(SB_PAGEDOWN, NULL), (LPARAM)NULL);
	           break;
          }
           
          case VK_HOME:
          {
            if(GetKeyState(VK_CONTROL) < 0)
               SendMessage( WM_VSCROLL, MAKEWPARAM(SB_TOP, NULL), (LPARAM)NULL);
            else if(GetKeyState(VK_SHIFT) < 0)
			   SendMessage( WM_HSCROLL, MAKEWPARAM(SB_TOP, NULL), (LPARAM)NULL);
      
            break;
          }
          
          case VK_END:
          {
            if(GetKeyState(VK_CONTROL) < 0)
                SendMessage( WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), (LPARAM)NULL);
			else if (GetKeyState(VK_SHIFT) < 0)
				SendMessage( WM_HSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), (LPARAM)NULL);
        
            break;
          }      
          
	 }               
         
     
 

}

BOOL CStringGrid::OnMouseWheel(UINT nFlags,short zDelta,CPoint pt)
{
	TRACE("delete = %d\n",zDelta);
	if( zDelta > 0 ) // rolled up
	{
		OnVScroll( SB_LINEUP, 0, NULL );
		
	}
	else // rolled down
	{
		OnVScroll( SB_LINEDOWN, 0, NULL );
	}

	ScreenToClient(&pt);
	OnMouseMove(0,pt);
	return 0;
}

