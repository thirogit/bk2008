#ifndef __PASSPORTRECOGNITIONCONTEXT_H__
#define __PASSPORTRECOGNITIONCONTEXT_H__

#include "PassportRecognizer.h"
#include "../PassportCommon/fipWinImageEx.h"

class PassportRecognitionContext
{
	friend class PassportRecognizer;
private:
	PassportRecognitionContext();	
public:
	const fipWinImageEx& GetRawPassportImage() const;
	const fipWinImageEx& GetPreparedPassportImage() const;
	UINT GetResolution() const;
	double GetSkew() const;
	const CPoint& GetOrg() const;
private:
	fipWinImageEx m_RawPassportImage;
	fipWinImageEx m_PreparedPassportImage;	
	ImageSettings m_ImgSettings;

	double m_Skew; 
	CPoint m_Org;
	UINT m_dpiResolution;

};



#endif 
