#ifndef __DRWDOC_H__
#define __DRWDOC_H__

#include "../../src/printing/CowDrwDoc.h"

class CDrwDoc : public CCowDrwDoc
{		
public:
	CDrwDoc();
	CDocObjList* GetObjects();
	CDocObj* ObjectAt(const CPoint& point);
	void Add(CDocObj* pObj);
	void Remove(CDocObj* pObj);
};

#endif