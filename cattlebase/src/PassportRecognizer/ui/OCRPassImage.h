#ifndef __OCRPASSIMAGE_H__
#define __OCRPASSIMAGE_H__

#include "../../PassportViewing/PassImage.h"
#include "../../passOcrProfile/include/PassProfilePassProfile.h" 
#include "PtrFlushArray.h"
#include "../PassField.h"

class COCRPassImage : public CPassImage
{
public:
	~COCRPassImage();
	bool UpdateOCRImage(const fipWinImageEx &pix,const PassProfilePassProfile& passProfile);
	virtual bool UpdateImage(const fipWinImageEx &pix);
protected:
	PtrFlushArray<CPassField> m_ocrFields;
	CSize m_passSizeMM;
protected:
	afx_msg void OnPaint();
	
	DECLARE_MESSAGE_MAP()
};


#endif