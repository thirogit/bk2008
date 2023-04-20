#include "stdafx.h"
#include "AlignBar.h"
#include "resource.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
// CAlignBar
IMPLEMENT_DYNCREATE( CAlignBar, CToolBar )

BEGIN_MESSAGE_MAP( CAlignBar, CToolBar )
    //{{AFX_MSG_MAP(CAlignBar)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
// CAlignBar construction/destruction
CAlignBar::CAlignBar()
{
		
}


/******************************************************************************/

BOOL CAlignBar::Create(CWnd* pcParentWnd)
    {
    BOOL bRC = TRUE;
    int iNumButtons = 0;
    int iNumSeparators;

    UINT ButtonIDS[MAX_ALIGNBAR_ITEMS];

        ButtonIDS[iNumButtons++] = IDC_LEFT;
        ButtonIDS[iNumButtons++] = IDC_RIGHT;
        ButtonIDS[iNumButtons++] = IDC_CENTER;
    //	ButtonIDS[iNumButtons++] = IDC_JUSTIFY;
            
        iNumSeparators = 0;
          
       

    bRC = CToolBar::Create( pcParentWnd, CBRS_ALIGN_TOP );

    if (bRC == 0)
        {
        AfxThrowResourceException();
        /*DK* ##ERROR Could not create toolbar object*/
        }

		bRC = LoadBitmap(IDB_TEXT_TBAR);

    if (bRC == 0)
        {
        AfxThrowResourceException();
        /*DK* ##ERROR Could not load bitmap for toolbar*/
        }

    bRC = SetButtons(ButtonIDS, iNumButtons);

          // Set the style to be checkbox  style.
        SetButtonInfo(LEFT_TBAR_POS,      IDC_LEFT,       TBBS_CHECKBOX, LEFT_BMP_POS);
        SetButtonInfo(RIGHT_TBAR_POS,    IDC_RIGHT,     TBBS_CHECKBOX, RIGHT_BMP_POS);
        SetButtonInfo(CENTER_TBAR_POS, IDC_CENTER,  TBBS_CHECKBOX, CENTER_BMP_POS);
		//SetButtonInfo(JUSTIFY_TBAR_POS, IDC_JUSTIFY,  TBBS_CHECKBOX, JUSTIFY_BMP_POS);

    CSize size = CToolBar::CalcFixedLayout( FALSE, TRUE );

    if (GetSafeHwnd() != NULL)
        {
        SetWindowPos( &wndTop, 0, 0, size.cx, size.cy, SWP_NOMOVE );
        }

    return bRC;
    }

/******************************************************************************/

CAlignBar::~CAlignBar()
{
}
