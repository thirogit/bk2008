#include "stdafx.h"
#include "PassImage.h"

BEGIN_MESSAGE_MAP(CPassImage,CStatic)
	ON_WM_PAINT()

END_MESSAGE_MAP()

CPassImage::CPassImage() : m_bNoImage(false)
{

	
	LOGFONT lf;
	
	m_noImageFont.Attach(::GetStockObject(DEFAULT_GUI_FONT));
	m_noImageFont.GetLogFont(&lf);
	m_noImageFont.Detach();
	lf.lfHeight = -24;
	lf.lfWidth = 0;
	lf.lfWeight = 900;
	m_noImageFont.CreateFontIndirect(&lf);



}


int CPassImage::Create(const RECT &rect,CWnd *pParentWnd)
{
	return CStatic::Create("",WS_CHILD|WS_VISIBLE,rect,pParentWnd);
}

bool CPassImage::UpdateImage(const fipWinImageEx &pix)
{
	bool bRet;
	if(m_img.m_hObject) m_img.DeleteObject();
	
	CDC *pDC = GetDC();
        bRet = pix.copyToBitmap(m_img,pDC);
	ReleaseDC(pDC);
	Invalidate();
	m_bNoImage = !bRet;
	return bRet;
}

void CPassImage::OnPaint()
{

	CStatic::OnPaint();
	CClientDC dc(this);

	if(m_bNoImage)
	{
		CRect rect,txtRect;
		CString noImageText;
		GetClientRect(&rect);	
		if(!m_sNoImageText.IsEmpty())
			noImageText = m_sNoImageText;
		else
			noImageText.LoadString(IDS_NOPASSIMAGE);
		
		CFont * pOldFont = dc.SelectObject( &m_noImageFont );

		CSize size = dc.GetTextExtent(noImageText);

		//INT nOldBkMode = dc.SetBkMode( TRANSPARENT );
		//COLORREF clrOldText = dc.SetTextColor( RGB( 0, 0, 0 ) );

		int x,y;
		x = ((rect.Width()-size.cx)/2);
		y = ((rect.Height()-size.cy)/2);
		txtRect.SetRect(x,y,x + size.cx,y + size.cy);
		dc.FillSolidRect(&rect,RGB(255,255,255));
		dc.DrawText(noImageText,txtRect,DT_SINGLELINE);

		//dc.SelectObject(pOldFont);
		//dc.SetBkColor(nOldBkMode);
		//dc.SetTextColor(clrOldText);
	}
	else
	{

		if(m_img.m_hObject)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);
			CBitmap* pOldBitmap = memDC.SelectObject(&m_img);
			CSize bmpSize = m_img.GetBitmapDimension();
			CRect rc;
			GetClientRect(&rc);

			if (bmpSize.cx < rc.Width() && bmpSize.cy < rc.Height())
			{
					dc.BitBlt(0, 0, bmpSize.cx, bmpSize.cy, &memDC, 0, 0, SRCCOPY);
			}
			else
			{
					double scale = min(double(rc.Width()) / bmpSize.cx, double(rc.Height()) / bmpSize.cy);
					dc.StretchBlt(0, 0 ,int(bmpSize.cx*scale), int(bmpSize.cy*scale), &memDC, 0, 0, bmpSize.cx, bmpSize.cy, SRCCOPY);
			}
			memDC.SelectObject(pOldBitmap);
		}
	}
}

void CPassImage::NoImage(CString sNoImageText)
{
	m_bNoImage = true;
	m_sNoImageText = sNoImageText;
	Invalidate();
}




