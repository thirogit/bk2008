#ifndef __DOCIMGOBJ_H__
#define __DOCIMGOBJ_H__

#include "DocRectObj.h"
#include "Picture.h"

class CDocImgObj : public CDocRectObj
{
protected:
	DECLARE_SERIAL(CDocImgObj);
	CDocImgObj();

public:
	CDocImgObj(const CRect& position);
	~CDocImgObj();
public:
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);
	inline CPicture* GetPicture() { return &m_picture; };

protected:
	CPicture m_picture;
	
};

class CDrawImg : public CDocImgObj
{
public:
	DECLARE_SERIAL(CDrawImg);
	CDrawImg() {};
};


#endif 
