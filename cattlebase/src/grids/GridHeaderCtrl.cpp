#include "stdafx.h"
#include "GridHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CGridHeaderCtrl::CGridHeaderCtrl()
{
}

CGridHeaderCtrl::~CGridHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CGridHeaderCtrl, CHeaderCtrl)
     ON_MESSAGE(HDM_LAYOUT, OnHeaderLayout)
END_MESSAGE_MAP()

LRESULT CGridHeaderCtrl::OnHeaderLayout(WPARAM, LPARAM lParam)
{
     LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;

     LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);

     lphdlayout->pwpos->cy = 25;
     lphdlayout->prc->top = 25;

     return lResult;
}
