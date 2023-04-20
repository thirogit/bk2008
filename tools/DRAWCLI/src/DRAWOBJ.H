
#ifndef __DRAWOBJ_H__
#define __DRAWOBJ_H__

class CDrawView;
class CDrawDoc;


#ifndef FOR_CATTLEBASE
#include "FontHelper.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#endif

#include "DrawObjGroup.h"
#include "Picture.h"
#include "TextStyle.h"

#define MAX_LABEL 32
/////////////////////////////////////////////////////////////////////////////
// CDrawObj - base class for all 'drawable objects'

class CDrawView;
class CDrawObjGroup;

class CDrawObj : public CObject
{
protected:
	DECLARE_SERIAL(CDrawObj);
	CDrawObj();

// Constructors
public:
	CDrawObj(const CRect& position);
#ifndef FOR_CATTLEBASE
	virtual void SerializeBody(xercesc::DOMElement * pParentElement);
	virtual void SerializeToXml(xercesc::DOMElement * pParentElement);
	virtual CString GetTagName();
#endif
// Attributes
	CRect m_position;
	CString m_label;
	CDrawDoc* m_pDocument;
	CDrawObjGroup *m_group;
	bool IsVisible();

#ifndef FOR_CATTLEBASE
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	CRect GetHandleRect(int nHandleID, CDrawView* pView);
	virtual HCURSOR GetHandleCursor(int nHandle);

	virtual void SetLineColor(COLORREF color);
	virtual void SetFillColor(COLORREF color);
	virtual COLORREF GetLineColor();
	virtual COLORREF GetFillColor();
	virtual BOOL IsNoFill();
	virtual BOOL IsNoOutline();
	virtual BOOL SetNoFill( BOOL bSet = TRUE );
	virtual BOOL SetNoOutline( BOOL bSet = TRUE );
	virtual BOOL SetLogPenSize(int nSize);
#endif
	
// Operations
	virtual void Draw(CDC* pDC);
#ifndef FOR_CATTLEBASE

	enum TrackerState { normal, selected, active };
	virtual void DrawTracker(CDC* pDC, TrackerState state);
	virtual void MoveTo(const CRect& positon, CDrawView* pView = NULL);
	virtual int HitTest(CPoint point, CDrawView* pView, BOOL bSelected);
	virtual BOOL Intersects(const CRect& rect);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	//virtual void OnOpen(CDrawView* pView);
	virtual void OnEditProperties(CPropertySheet &propSheet);
	virtual CDrawObj* Clone(CDrawDoc* pDoc = NULL);
	virtual void Remove();
	void Invalidate();
	virtual void CopyObjTo(CDrawObj *pObj);
#endif
// Implementation
public:
	virtual ~CDrawObj();
	virtual void Serialize(CArchive& ar);
#ifdef _DEBUG
	void AssertValid();
#endif

	// implementation data
public:
	void NormalizePosition();
protected:
	void NormalizePosition(CRect &rect);

	BOOL m_bPen;
	LOGPEN m_logpen;
	BOOL m_bBrush;
	LOGBRUSH m_logbrush;

	friend class CDrawView;
};

// special 'list' class for this application (requires afxtempl.h)
typedef CTypedPtrList<CObList, CDrawObj*> CDrawObjList;

//typedef CTypedPtrList<CObList, CDrawObjList*> CPagesList;
////////////////////////////////////////////////////////////////////////
// specialized draw objects

class CDrawRect : public CDrawObj
{
protected:
	DECLARE_SERIAL(CDrawRect);
	CDrawRect();

public:
	CDrawRect(const CRect& position);
	CPoint m_roundness; // for roundRect corners


#ifndef FOR_CATTLEBASE	
	virtual CString GetTagName(); 
	virtual void SerializeBody(xercesc::DOMElement * pParentElement);
#endif
	
public:
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);

#ifndef FOR_CATTLEBASE
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual BOOL Intersects(const CRect& rect);
	virtual CDrawObj* Clone(CDrawDoc* pDoc);
	virtual void CopyObjTo(CDrawRect *pObj);
	virtual void OnEditProperties(CPropertySheet &propSheet);
#endif


	enum Shape { rectangle, roundRectangle, ellipse, line };
	Shape m_nShape;
protected:	

	friend class CRectTool;
	friend class CTextTool;
	friend class CImgTool;
};

/////////////////////////////////////////////////////////////////////////////

class CDrawPoly;

class CDrawPoly : public CDrawObj
{
protected:
	DECLARE_SERIAL(CDrawPoly);
	CDrawPoly();

public:
	CDrawPoly(const CRect& position);

	void AddPoint(const CPoint& point, CDrawView* pView = NULL);
	BOOL RecalcBounds(CDrawView* pView = NULL);

#ifndef FOR_CATTLEBASE
	virtual CString GetTagName(); 
	virtual void SerializeBody(xercesc::DOMElement * pParentElement);
#endif


public:
	virtual ~CDrawPoly();
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);
#ifndef FOR_CATTLEBASE
	virtual void MoveTo(const CRect& position, CDrawView* pView = NULL);
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual BOOL Intersects(const CRect& rect);
	virtual CDrawObj* Clone(CDrawDoc* pDoc);
	virtual void CopyObjTo(CDrawPoly *pObj);
#endif
protected:
	int m_nPoints;
	int m_nAllocPoints;
	CPoint* m_points;
	CDrawPoly* m_pDrawObj;

	friend class CPolyTool;
};

class CDrawItem;    // COleClientItem derived class

class CDrawOleObj : public CDrawObj
{
protected:
	DECLARE_SERIAL(CDrawOleObj);
	CDrawOleObj();

public:
	CDrawOleObj(const CRect& position);

// Implementation
public:
	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);

	
#ifndef FOR_CATTLEBASE
	virtual CDrawObj* Clone(CDrawDoc* pDoc);
	virtual void OnOpen(CDrawView* pView);
	virtual void MoveTo(const CRect& positon, CDrawView* pView = NULL);
	virtual void OnEditProperties();
	virtual void Remove();
#endif
	virtual ~CDrawOleObj();

	static BOOL c_bShowItems;

	CDrawItem* m_pClientItem;
	CSize m_extent; // current extent is tracked separate from scaled position
};


class CDrawText : public CDrawRect
{
protected:
	DECLARE_SERIAL(CDrawText);
	CDrawText();
	CTextStyle* GetStyle(int styleId);
public:


	CDrawText::CDrawText(const CRect& position, CString& text,CTextStyle &pStyle);
	CDrawText(const CRect& position);
	~CDrawText();

#ifndef FOR_CATTLEBASE
	virtual CString GetTagName(); 
	virtual void SerializeBody(xercesc::DOMElement * pParentElement);
#endif
	virtual void Serialize(CArchive& ar);

	virtual void Draw(CDC* pDC);

	virtual void DrawText(CDC *pDC,CString &drawText);
	CRect GetRectAlign(CRect &fitRect,CSize &txtSize,HOR_ALIGN halign,VER_ALIGN valign);

#ifndef FOR_CATTLEBASE
	virtual int GetHandleCount();
	virtual CPoint GetHandle(int nHandle);
	virtual HCURSOR GetHandleCursor(int nHandle);
	virtual void MoveHandleTo(int nHandle, CPoint point, CDrawView* pView = NULL);
	virtual BOOL Intersects(const CRect& rect);
	virtual CDrawObj* Clone(CDrawDoc* pDoc);
	virtual void OnEditProperties(CPropertySheet &propSheet);
#endif

	CString m_text;
	
	BOOL m_bOwnStyle;
	CTextStyle m_OwnStyle;	
	CTextStyle *m_pStyle;	
	
private:
	friend class CTextTool;
};



class CDrawImg : public CDrawRect
{
protected:
	DECLARE_SERIAL(CDrawImg);
	CDrawImg();

public:
	CDrawImg(const CRect& position);
	~CDrawImg();

#ifndef FOR_CATTLEBASE
	virtual CString GetTagName(); 
	virtual void SerializeBody(xercesc::DOMElement * ppParentElement);
#endif

	virtual void Serialize(CArchive& ar);
	virtual void Draw(CDC* pDC);
	inline CPicture* GetPicture() { return &m_picture; };
#ifndef FOR_CATTLEBASE
	virtual CDrawObj* Clone(CDrawDoc* pDoc);
	virtual void OnEditProperties(CPropertySheet &propSheet);
#endif
	CPicture m_picture;
	friend class CImgTool;
};



#endif // __DRAWOBJ_H__
