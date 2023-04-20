#include "stdafx.h"
#include "DocImgObj.h"

IMPLEMENT_SERIAL(CDrawImg, CDocImgObj, 0)

IMPLEMENT_SERIAL(CDocImgObj, CDocRectObj, 0)

CDocImgObj::CDocImgObj() 
{
}

CDocImgObj::CDocImgObj(const CRect& position) : CDocRectObj(position)
{
	
}


CDocImgObj::~CDocImgObj() 
{

}

void CDocImgObj::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CArchiveStream arcstream(&ar);
	IStream* pStream = (IStream*)&arcstream;
	CDocRectObj::Serialize(ar);
	m_picture.Serialize(ar);
	
}

void CDocImgObj::Draw(CDC* pDC)
{
	ASSERT_VALID(this);
	CDocRectObj::Draw(pDC);
	CRect rect = GetPosition();
	pDC->LPtoDP(&rect);
	rect.NormalizeRect();
	pDC->DPtoLP(&rect);
	m_picture.Draw(pDC,CPoint(rect.left,rect.top),CSize(rect.Width(),rect.Height()));

}




