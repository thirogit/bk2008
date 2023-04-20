#ifndef __DOCOBJ_H__
#define __DOCOBJ_H__

#include "DocObjGroup.h"

#define MAX_DOCOBJ_LABEL 32

class CDocObj : public CObject
{
protected:
	CDocObj();

public:
	CDocObj(const CRect& position);
	bool IsVisible();
	virtual void Draw(CDC* pDC) = 0;
	virtual ~CDocObj();
	virtual void Serialize(CArchive& ar);

	CDocObjGroup * GetGroup();
	CString GetLabel() const;
	const CRect& GetPosition();

protected:
	void NormalizePosition();
	

//	CCowDrwDoc* m_pDocument;
	CDocObjGroup *m_pGroup;

	BOOL m_bPen;
	LOGPEN m_logpen;
	BOOL m_bBrush;
	LOGBRUSH m_logbrush;
private:
	CRect m_Position;
	CString m_sLabel;
	
};



#endif 
