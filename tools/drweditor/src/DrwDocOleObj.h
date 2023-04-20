#ifndef __DRWDOCOLEOBJ_H__
#define __DRWDOCOLEOBJ_H__

#include "../../src/printing/DocOleObj.h"
#include "EditDrwDocObj.h"

class CDrwDocOleObj : public CEditDocOleObj<CDocOleObj>
{
	DECLARE_SERIAL(CDrwDocOleObj);
	CDrwDocOleObj();
public:
	CDrwDocOleObj(const CRect& position);
	void SetOleItem(CDocOleItem *pClientItem);	
};

#endif 
