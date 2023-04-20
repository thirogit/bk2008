#include "stdafx.h"
#include "cstringgrid.h"


#define HLS(h,l,s) ((HLSCOLOR)(((BYTE)(h)|((WORD)((BYTE)(l))<<8))|(((DWORD)(BYTE)(s))<<16)))

///////////////////////////////////////////////////////////////////////////////
#define HLS_H(hls) ((BYTE)(hls))
#define HLS_L(hls) ((BYTE)(((WORD)(hls)) >> 8))
#define HLS_S(hls) ((BYTE)((hls)>>16))

///////////////////////////////////////////////////////////////////////////////
HLSCOLOR RGB2HLS (COLORREF rgb);
COLORREF HLS2RGB (HLSCOLOR hls);

HLSCOLOR CStringGrid::RGB2HLS (COLORREF rgb)
{
    unsigned char minval = min(GetRValue(rgb), min(GetGValue(rgb), GetBValue(rgb)));
    unsigned char maxval = max(GetRValue(rgb), max(GetGValue(rgb), GetBValue(rgb)));
    float mdiff  = float(maxval) - float(minval);
    float msum   = float(maxval) + float(minval);
   
    float luminance = msum / 510.0f;
    float saturation = 0.0f;
    float hue = 0.0f; 

    if ( maxval != minval )
    { 
        float rnorm = (maxval - GetRValue(rgb)  ) / mdiff;      
        float gnorm = (maxval - GetGValue(rgb)) / mdiff;
        float bnorm = (maxval - GetBValue(rgb) ) / mdiff;   

        saturation = (luminance <= 0.5f) ? (mdiff / msum) : (mdiff / (510.0f - msum));

        if (GetRValue(rgb) == maxval) hue = 60.0f * (6.0f + bnorm - gnorm);
        if (GetGValue(rgb) == maxval) hue = 60.0f * (2.0f + rnorm - bnorm);
        if (GetBValue(rgb) == maxval) hue = 60.0f * (4.0f + gnorm - rnorm);
        if (hue > 360.0f) hue = hue - 360.0f;
    }
    return HLS ((hue*255)/360, luminance*255, saturation*255);
}

COLORREF CStringGrid::HLS2RGB (HLSCOLOR hls)
{
    float hue        = ((int)HLS_H(hls)*360)/255.0f;
    float luminance  = HLS_L(hls)/255.0f;
    float saturation = HLS_S(hls)/255.0f;

    if ( saturation == 0.0f )
    {
        return RGB (HLS_L(hls), HLS_L(hls), HLS_L(hls));
    }
    float rm1, rm2;
     
    if ( luminance <= 0.5f ) rm2 = luminance + luminance * saturation;  
    else                     rm2 = luminance + saturation - luminance * saturation;
    rm1 = 2.0f * luminance - rm2;   
    BYTE red   = _ToRGB (rm1, rm2, hue + 120.0f);   
    BYTE green = _ToRGB (rm1, rm2, hue);
    BYTE blue  = _ToRGB (rm1, rm2, hue - 120.0f);

    return RGB (red, green, blue);
}

BYTE CStringGrid::_ToRGB (float rm1, float rm2, float rh)
{
  if      (rh > 360.0f) rh -= 360.0f;
  else if (rh <   0.0f) rh += 360.0f;
 
  if      (rh <  60.0f) rm1 = rm1 + (rm2 - rm1) * rh / 60.0f;   
  else if (rh < 180.0f) rm1 = rm2;
  else if (rh < 240.0f) rm1 = rm1 + (rm2 - rm1) * (240.0f - rh) / 60.0f;      
                   
  return (BYTE)(rm1 * 255);
}

COLORREF CStringGrid::HLS_TRANSFORM (COLORREF rgb, int percent_L, int percent_S)
{
    HLSCOLOR hls = RGB2HLS (rgb);
    BYTE h = HLS_H(hls);
    BYTE l = HLS_L(hls);
    BYTE s = HLS_S(hls);

    if ( percent_L > 0 )
    {
        l = BYTE(l + ((255 - l) * percent_L) / 100);
    }
    else if ( percent_L < 0 )
    {
        l = BYTE((l * (100+percent_L)) / 100);
    }
    if ( percent_S > 0 )
    {
        s = BYTE(s + ((255 - s) * percent_S) / 100);
    }
    else if ( percent_S < 0 )
    {
        s = BYTE((s * (100+percent_S)) / 100);
    }
    return HLS2RGB (HLS(h, l, s));
}


void CStringGrid::DrawGrid()
{
 
  int i;

  // if no fixed col set width to zero
  if(!CheckOptions(SGO_FIXEDCOL))
  {
    m_columns[0].m_width = 0;
  }
 
  m_rightCol  = RightColumn();

  // set up GDI objects
  int         iSaveDC = SaveDC(m_memdc);
  RECT        rc;
  RECT        selrc;

  HPEN        hpGrid      = CreatePen(PS_SOLID, 0, m_colors[SGC_GRIDLINE]);
  HBRUSH      hbSelect    = CreateSolidBrush(m_colors[SGC_SELCELL]);
  HBRUSH      hbCellBkg   = CreateSolidBrush(m_colors[SGC_CELLBKGND]);


  HBRUSH	  hHighlight  = CreateSolidBrush(RGB(0xba,0xde,0xba)); //!!!
  HBRUSH      hbBkColor   = CreateSolidBrush(m_colors[SGC_GRIDBKGND]); //default rowbackground when RowType.m_color == -1


 HFONT       hfFxFont    = CreateFontIndirect(&m_fxFont);
  HFONT       hfCellFont  = CreateFontIndirect(&m_cellFont);
  HFONT       hfTitleFont = CreateFontIndirect(&m_titleFont);
  HBRUSH oldBrush;
  COLORREF crBorder = ::GetSysColor (COLOR_HIGHLIGHT);

   const UINT  dtStyle     = DT_SINGLELINE     |
                            DT_VCENTER        |
                            DT_END_ELLIPSIS   |
                            0;

  int maxX  = GetSystemMetrics(SM_CXSCREEN);
  int maxY  = GetSystemMetrics(SM_CYSCREEN);

  SetBkMode(m_memdc, TRANSPARENT);
  SelectObject(m_memdc, hfCellFont); //default font
  SelectObject(m_memdc, hpGrid); //pen
  SelectObject(m_memdc, hbBkColor); //brush

  PatBlt(m_memdc, 0, 0, maxX, maxY, PATCOPY);
  SelectObject(m_memdc, hfFxFont);

   // set rect for corner cell													
  rc.left   = 0;
  rc.top    = TitleHeight();
  rc.right  = m_columns[0].m_width;
  rc.bottom = HeadHeight();

HBRUSH br = CreateSolidBrush(m_colors[SGC_FXDCELL]);
oldBrush = (HBRUSH)SelectObject(m_memdc,br);

  // draw corner cell
  if(CheckOptions(SGO_FIXEDROW | SGO_FIXEDCOL))
  {
    //BtnCell(rc);

	Rectangle(m_memdc,rc.left,rc.top,rc.right+1,rc.bottom+1);
    selrc = rc;
    selrc.right -= 5;    // allow margin of 5 pixels on right
    selrc.left  += 5;    // allow margin of 10 pixels on left
    SetTextColor(m_memdc,
                 m_colors[!m_bGrayed ? SGC_FXDCOLTEXT : SGC_GRAYTEXT]);
    DrawText(m_memdc, drawCell(0, 0), -1, &selrc, dtStyle | DT_CENTER);
  }

/*
  if(CheckOptions(SGO_SCROLLDOT | SGO_FIXEDROW | SGO_FIXEDCOL) == true)
  {
    // if not showing first col/row put indicator arrows in corner cell
    // don't draw arrows if column too narrow or short
    if(m_columns[0].m_width > 20 && m_rowHeight >= 10)
    {
      SelectObject(m_memdc, hpDots);
      if(m_topRow > 1)
      {
        SelectObject(m_memdc, hbRowDot);
        const POINT vertices[] =
        {
          {rc.left, rc.bottom},
          {rc.left + 10, rc.bottom},
          {rc.left + 5, rc.bottom - 5}
        };
        Polygon(m_memdc, vertices, 3);
      }
      if(m_leftCol > 1)
      {
        SelectObject(m_memdc, hbColDot);
        const POINT vertices[] =
        {
          {rc.right, rc.top},
          {rc.right, rc.top + 10},
          {rc.right - 5, rc.top + 5}
        };
        Polygon(m_memdc, vertices, 3);
      }
    }
  }*/

  // draw top row
  rc.left = m_columns[0].m_width;
  SetTextColor(m_memdc, m_colors[!m_bGrayed ? SGC_FXDCOLTEXT : SGC_GRAYTEXT]);

  for(i = m_leftCol; i <= m_rightCol ; ++i)
  {
    rc.right = rc.left + m_columns[i].m_width;
    Rectangle(m_memdc,rc.left,rc.top,rc.right+ (i != m_rightCol) ,rc.bottom+1);
	//BtnCell(rc);

    selrc = rc;
    selrc.right -= 5;     // allow margin of 5 pixels
    selrc.left  += 5;     // on both sides of text
    DrawText(m_memdc, drawCell(i, 0), -1, &selrc, dtStyle | DT_CENTER);
    rc.left += m_columns[i].m_width;
  }


SelectObject(m_memdc,oldBrush);
DeleteObject(br);

 
UINT align = DT_CENTER;
int r;  
int right = m_columns[0].m_width;
int iright = right;
int bottom = rc.top;
SelectObject(m_memdc, hpGrid);

for(i = m_leftCol; i < m_rightCol + 1; ++i)
{
    right += m_columns[i].m_width;
}


MoveToEx(m_memdc, 0, 0, NULL); //?
LineTo  (m_memdc, right, 0);

int y = TitleHeight()+(m_rowHeight);
int yextra;
bool hlightON = false;
COLORREF tempcell = NO_COLOR,temprow = NO_COLOR,
         tempccloak = NO_COLOR,temprcloak = NO_COLOR,tempfxdcol = NO_COLOR;
HBRUSH hColTypeBrush;
HBRUSH hRowTypeBrush; // specified color for row
HBRUSH hCloakRow;
HBRUSH hCloakCell;
HBRUSH hFxdCol,hOld;
int cc,yy,off,cr,coloff,currcolw,ccols;;
CloakCellType *ccellptr;
int spani,ii,spanj,jj,cspan,rspan,currrowh;
CDC pDC;
POINT pt;
int ix,iy;
RowType *rowPtr;

//TRACE("topRow = %d, bootomRow = %d\n",m_topRow,m_bottomRow);
for(r = m_topRow; r <= m_bottomRow; ++r)
{
    
	rowPtr = m_drawRows[r];

	if(!rowPtr->visible) continue;

	hlightON = (m_row_hlight == r);
	yy = y+m_rowHeight;
	yextra =  yy + !rowPtr->hidden + rowPtr->GetCloakHeight()+1;
 
	temprow = rowPtr->m_color; 
	if(temprow != NO_COLOR)
			 hRowTypeBrush   = CreateSolidBrush(temprow);
		 	else hRowTypeBrush = hbBkColor;
		
	SelectObject(m_memdc, hRowTypeBrush);	
	Rectangle(m_memdc, 0, y,right, yextra);

//FXDCOL
tempfxdcol = rowPtr->m_cells[0].m_color; //!!!
if(tempfxdcol != NO_COLOR) hFxdCol = CreateSolidBrush(tempfxdcol);
else hFxdCol = hRowTypeBrush;

hOld = (HBRUSH)SelectObject(m_memdc,hFxdCol);

ix = m_columns[0].m_width+1;
iy = yy+1;
Rectangle(m_memdc,0,y,ix,iy);

if(rowPtr->selected)
{
pDC.Attach(m_memdc);
pt.x = (ix + 0)/2 - 8; //assuming 16x16 picture
pt.y = (iy + y)/2 - 8;
m_imgList.Draw(&pDC,0,pt,ILD_NORMAL);
pDC.Detach();
}

SelectObject(m_memdc,hOld);


	if(hlightON) SelectObject(m_memdc, hHighlight);

	for(i = m_leftCol,iright = m_columns[0].m_width; i <= m_rightCol ; i++)
	{

		//select cell color 
		if(!hlightON)
		{
			tempcell = rowPtr->m_cells[i].m_color;
			if(tempcell != NO_COLOR) hColTypeBrush   = (HBRUSH)CreateSolidBrush(tempcell);
			else hColTypeBrush  = hRowTypeBrush;  //if no color use row color
			SelectObject(m_memdc, hColTypeBrush);
		}
		
		SetRect(&selrc,iright,y,iright+m_columns[i].m_width + (i != m_rightCol) ,yy+1);
		Rectangle(m_memdc,selrc.left,selrc.top,selrc.right,selrc.bottom);
		//draw cell text

		DrawText(m_memdc, drawCell(i, r), -1, &selrc, dtStyle | align);
   
		
		//SelectObject(m_memdc, hbBkColor);
		if(tempcell != NO_COLOR) DeleteObject(hColTypeBrush);

		iright += m_columns[i].m_width; 
    }
	
	//draw cloak rows
	off = yy;
	for(cr = 0;(cr < rowPtr->cloakrows) && (!rowPtr->hidden); cr++)
	{
		ccols = rowPtr->cloakcols-1;
		//select brush
		temprcloak = rowPtr->m_CloakRows[cr].m_color;
		
			if(temprcloak != NO_COLOR)	hCloakRow = (HBRUSH)CreateSolidBrush(temprcloak);
			else hCloakRow = hRowTypeBrush;

			SelectObject(m_memdc,hCloakRow);
		
		for(cc = 0,coloff = 0;cc <= ccols ;cc++)
		{
			//select brush
			ccellptr = &rowPtr->m_CloakRows[cr].m_cloakcells[cc];
			cspan = ccellptr->colspan;
			rspan = ccellptr->rowspan;

			if(ccellptr->in_span && !ccellptr->corner) continue;

			tempccloak = ccellptr->m_color;
			
			if(tempccloak != NO_COLOR)	
			{
			//	pen.Color (crBorder);
				//if(m_NoHideMakeBlue && !rowPtr->visible)
					// tempccloak  = HLS_TRANSFORM (crBorder, +80, -66);
          	    hCloakCell = (HBRUSH)CreateSolidBrush(tempccloak);
			
			}
			else hCloakCell = hCloakRow;



			SelectObject(m_memdc,hCloakCell);
			currcolw = (rowPtr->m_CloakCols[cc].percent_width * right)/100;

			if(cspan > 1)
			{

				if((cc+cspan-1) == ccols)
				{
					currcolw = right-coloff-1;
				}
				else for(spani=cc+1,ii=cc+cspan;spani < ii;spani++)
					currcolw += ((rowPtr->m_CloakCols[spani].percent_width * right)/100);
			
			}

			if(cc == ccols) 
				currcolw = right-coloff-1;
		
			selrc.left = coloff;
			selrc.top = off;
			selrc.right = coloff + currcolw +1;

			currrowh = rowPtr->m_CloakRows[cr].pixel_height;
			if(rspan > 1)
			{

				for(spanj=cr+1,jj=cr+rspan;spanj<jj;spanj++)
					currrowh += rowPtr->m_CloakRows[spanj].pixel_height;
			}

			selrc.bottom = off+currrowh+1;

			//selrc.bottom = off+m_rows[r].m_CloakRows[cr].pixel_height+1;
			
			Rectangle(m_memdc,selrc.left ,selrc.top,selrc.right ,selrc.bottom);

			DrawText(m_memdc,rowPtr->CloakCell(cc,cr) , -1, &selrc, dtStyle | align);

			coloff += currcolw;
			

		}
		off += rowPtr->m_CloakRows[cr].pixel_height;

		if(temprcloak != NO_COLOR) DeleteObject(hCloakRow);
		if(tempccloak != NO_COLOR) DeleteObject(hCloakCell);

	}

y += (m_rowHeight) + rowPtr->GetCloakHeight() ;

if(temprow != NO_COLOR) DeleteObject(hRowTypeBrush);
if(tempcell != NO_COLOR) DeleteObject(hColTypeBrush);


}


  if(CheckOptions(SGO_TITLE))
  {
    // set rect for title cell
    rc.left   = 0;
    rc.top    = 0;
    rc.right  = right;
    rc.bottom = m_titleHeight;

    // draw title cell
    BtnCell(rc);
    selrc = rc;
    selrc.right -= 5;     // allow margin of 5 pixels on right
    selrc.left  += 10;    // allow margin of 10 pixels on left
    SelectObject(m_memdc, hfTitleFont);
    SetTextColor(m_memdc,
                 m_colors[!m_bGrayed ? SGC_TITLETEXT : SGC_GRAYTEXT]);
    DrawText(m_memdc, m_titleString, -1, &selrc, dtStyle | DT_CENTER);
  }

 	/*
  m_si.fMask  = SIF_POS;
  m_si.nPos   = m_leftCol;
  ::SetScrollInfo(m_gridhWnd, SB_HORZ, &m_si, TRUE);

  m_si.fMask  = SIF_POS;
  m_si.nPos   = m_topRow;
  ::SetScrollInfo(m_gridhWnd, SB_VERT, &m_si, TRUE);
*/
  //::InvalidateRect(m_gridhWnd, NULL, TRUE);
	Invalidate(FALSE);
  // clean up
  RestoreDC(m_memdc, iSaveDC);
  DeleteObject(hbBkColor);
  DeleteObject(hbSelect);
  DeleteObject(hbCellBkg);
  DeleteObject(hfFxFont);
  DeleteObject(hfCellFont);
  DeleteObject(hfTitleFont);
  DeleteObject(hpGrid);
  DeleteObject(hHighlight);
}

void CStringGrid::ReDraw(void)
{
	DrawGrid();
}