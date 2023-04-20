#include "stdafx.h"
#include "DocOleItem.h"
#include "DocOleObj.h"
//#include <AfxPriv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CDocOleItem, COleClientItem, 0)

CDocOleItem::CDocOleItem(CCowDrwDoc* pContainer, CDocOleObj* pDrawObj) : COleClientItem(pContainer)
{
	m_pDocObj = pDrawObj;
}

CDocOleItem::~CDocOleItem()
{
	//if (m_pDocObj != NULL)
	//	m_pDocObj->m_pClientItem = NULL;
}


void CDocOleItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	COleClientItem::Serialize(ar);

	// now store/retrieve data specific to CDocOleItem
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}



#ifdef _DEBUG
void CDocOleItem::AssertValid() const
{
	COleClientItem::AssertValid();
}

void CDocOleItem::Dump(CDumpContext& dc) const
{
	COleClientItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
