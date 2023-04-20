#ifndef __DOCTEXTOBJ_H__
#define __DOCTEXTOBJ_H__

#include "DocRectObj.h"
#include "TextStyle.h"

#define MAX_TEXT 255

class CDocObjText : public CDocRectObj
{
protected:
	DECLARE_SERIAL(CDocObjText);
	CDocObjText();
public:
	CDocObjText(const CRect& position,const CString& sText,CTextStyle &pStyle);
	CDocObjText(const CRect& position);
	~CDocObjText();

	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);

	virtual void DrawText(CDC *pDC,const CString &sText);
protected:
	CRect GetRectAlign(CRect &fitRect,CSize &txtSize,HOR_ALIGN halign,VER_ALIGN valign);

protected:
	CString m_text;
	BOOL m_bOwnStyle;
	CTextStyle m_OwnStyle;	
	CTextStyle *m_pStyle;	
};


class CDrawText : public CDocObjText
{
public:
	DECLARE_SERIAL(CDrawText);
	CDrawText() {};
};

#endif 
