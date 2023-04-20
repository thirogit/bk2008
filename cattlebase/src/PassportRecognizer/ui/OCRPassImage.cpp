#include "stdafx.h"
#include "OCRPassImage.h"

BEGIN_MESSAGE_MAP(COCRPassImage,CPassImage)
	ON_WM_PAINT()

END_MESSAGE_MAP()

COCRPassImage::~COCRPassImage()
{
	m_ocrFields.FreeAndFlush();
}

bool COCRPassImage::UpdateImage(const fipWinImageEx &pix)\
{
	m_ocrFields.FreeAndFlush();
	return CPassImage::UpdateImage(pix);
}

bool COCRPassImage::UpdateOCRImage(const fipWinImageEx &pix,const PassProfilePassProfile& passProfile)
{
	m_ocrFields.FreeAndFlush();


	PassProfilePassProfile& nonConstPassProfile = const_cast<PassProfilePassProfile&>(passProfile);

	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetBIRTHINFO().GetPassField()));
	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetEAN().GetPassField()));
	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetEANBARCODE().GetPassField()));
	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetFARMNOBARCODE().GetPassField()));
	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetFIRSTOWNER().GetPassField()));
	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetMOTHEREAN().GetPassField()));
	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetPASSDATE().GetPassField()));
	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetPASSNO().GetPassField()));
	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetSEX().GetPassField()));
	m_ocrFields.Add(new CPassField(nonConstPassProfile.GetSTOCK().GetPassField()));

	m_passSizeMM.SetSize(passProfile.GetPassDimX(),passProfile.GetPassDimY());

	return CPassImage::UpdateImage(pix);
}
void COCRPassImage::OnPaint()
{

	CPassImage::OnPaint();
	CClientDC dc(this);

	if(!m_bNoImage && m_img.m_hObject)
	{
		CRect rc;
		GetClientRect(&rc);
		CSize bmpSize = m_img.GetBitmapDimension();
		double imgScale = 1.0;	

		if (!(bmpSize.cx < rc.Width() && bmpSize.cy < rc.Height()))
		{
				imgScale = min(double(rc.Width()) / bmpSize.cx, double(rc.Height()) / bmpSize.cy);
		}

		CBrush brush(RGB(0,128,255));
		PtrFlushArray<CPassField>::ArrayIterator it = m_ocrFields.iterator();
		while(it.hasNext())
		{
			CPassField* pField = *(it++);

			double x1Scale = imgScale*((double)pField->xyOrd.x)/m_passSizeMM.cx;
			double y1Scale = imgScale*((double)pField->xyOrd.y)/m_passSizeMM.cy;
			
			double x2Scale = imgScale*((double)pField->xyOrd.x+pField->sSize.cx)/m_passSizeMM.cx;
			double y2Scale = imgScale*((double)pField->xyOrd.y+pField->sSize.cy)/m_passSizeMM.cy;
			
			dc.SetROP2(R2_MASKPEN );
			dc.SelectObject(&brush);
			int x1 = x1Scale*bmpSize.cx;
			int y1 = y1Scale*bmpSize.cy;
			int x2 = x2Scale*bmpSize.cx;
			int y2 = y2Scale*bmpSize.cy;

			dc.Rectangle(x1,y1,x2,y2);
							
				

		}
	}
}