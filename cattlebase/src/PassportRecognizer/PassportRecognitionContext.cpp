#include "stdafx.h"
#include "PassportRecognitionContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


PassportRecognitionContext::PassportRecognitionContext() : m_dpiResolution(0),m_Skew(0.0)
{
}

const fipWinImageEx& PassportRecognitionContext::GetRawPassportImage() const
{
	return m_RawPassportImage;
}

const fipWinImageEx& PassportRecognitionContext::GetPreparedPassportImage() const
{
	return m_PreparedPassportImage;
}

UINT PassportRecognitionContext::GetResolution() const
{
	return m_dpiResolution;
}

double PassportRecognitionContext::GetSkew() const
{
	return m_Skew;
}

const CPoint& PassportRecognitionContext::GetOrg() const
{
	return m_Org;
}


//
//bool COCRPass::GetBitmap(CDC *pDC,CBitmap &bitmap)
//{
//	if(!m_PassImg.isValid()) return false;
//	if (bitmap.m_hObject) bitmap.DeleteObject();
//
//	m_PassImg.copyToBitmap(bitmap,pDC);
//	
//	return true;
//
//}
//
//bool PassportRecognizer::GetCroppedPassImage(fipWinImageEx &destImg)
//{
//	return m_croppedPassImg.copySubImage(destImg,0,0,m_croppedPassImg.getWidth(),m_croppedPassImg.getHeight()) == TRUE;
//	
//}
//
//bool PassportRecognizer::GetRawPassImage(fipWinImageEx &destImg)
//{
//	return m_PassImg.copySubImage(destImg,0,0,m_PassImg.getWidth(),m_PassImg.getHeight()) == TRUE;
//}
