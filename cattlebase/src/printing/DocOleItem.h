#ifndef __DOCOLEITEM_H__
#define __DOCOLEITEM_H__

#include "CowDrwDoc.h"

class CDocOleObj;

class CDocOleItem : public COleClientItem
{
	DECLARE_SERIAL(CDocOleItem)
public:
	CDocOleItem(CCowDrwDoc* pContainer = NULL, CDocOleObj* pDrawObj = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.
public:
	//CDrawDoc* GetDocument()	{ return (CCowDrwDoc*)COleClientItem::GetDocument(); }	
protected:
	CDocOleObj* m_pDocObj;    // back pointer to OLE draw object

	//BOOL UpdateExtent();
public:
	~CDocOleItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);


};


#endif 