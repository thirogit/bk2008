#ifndef __PASSIMAGE_H__
#define __PASSIMAGE_H__

#include "../PassportCommon/fipWinImageEx.h"

class CPassImage : public CStatic
{
public:
	CPassImage();
	virtual bool UpdateImage(const fipWinImageEx &pix);
	void NoImage(CString sNoImageText);

	int Create(const RECT &rect,CWnd *pParentWnd);
protected:
	CBitmap m_img;
	bool m_bNoImage;
	CString m_sNoImageText;
	CFont m_noImageFont;
	afx_msg void OnPaint();
	
	DECLARE_MESSAGE_MAP()
	
};



#endif // __PASSIMAGE_H__
