// cattlebasefrmDoc.cpp : implementation of the CCattlebaseDoc class
//

#include "stdafx.h"
#include "cattlebase.h"
#include "cattlebaseDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseDoc

IMPLEMENT_DYNCREATE(CCattlebaseDoc, CDocument)

BEGIN_MESSAGE_MAP(CCattlebaseDoc, CDocument)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseDoc construction/destruction

CCattlebaseDoc::CCattlebaseDoc()
{
	

}

CCattlebaseDoc::~CCattlebaseDoc()
{
}

BOOL CCattlebaseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCattlebaseDoc serialization

void CCattlebaseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		
	}
	else
	{
		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseDoc diagnostics

#ifdef _DEBUG
void CCattlebaseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCattlebaseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseDoc commands
