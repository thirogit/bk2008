// cattlebasefrmView.cpp : implementation of the CCattlebaseView class
//

#include "stdafx.h"
#include "cattlebase.h"
#include "cattlebaseDoc.h"
#include "cattlebaseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseView

IMPLEMENT_DYNCREATE(CCattlebaseView, CView)

BEGIN_MESSAGE_MAP(CCattlebaseView, CView)
	//{{AFX_MSG_MAP(CCattlebaseView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseView construction/destruction

CCattlebaseView::CCattlebaseView()
{
	

}

CCattlebaseView::~CCattlebaseView()
{
}

BOOL CCattlebaseView::PreCreateWindow(CREATESTRUCT& cs)
{
	

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseView drawing

void CCattlebaseView::OnDraw(CDC* pDC)
{
	CCattlebaseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
}

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseView diagnostics

#ifdef _DEBUG
void CCattlebaseView::AssertValid() const
{
	CView::AssertValid();
}

void CCattlebaseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCattlebaseDoc* CCattlebaseView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCattlebaseDoc)));
	return (CCattlebaseDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCattlebaseView message handlers
