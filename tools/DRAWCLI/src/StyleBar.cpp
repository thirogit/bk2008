#include "stdafx.h"
#include "StyleBar.h"
#include "resource.h"

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
// CTfontTbar
IMPLEMENT_DYNCREATE( CTfontTbar, CToolBar )

BEGIN_MESSAGE_MAP( CTfontTbar, CToolBar )
    //{{AFX_MSG_MAP(CTfontTbar)
   
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
// CTfontTbar construction/destruction
CTfontTbar::CTfontTbar(void)
{
		
}


/******************************************************************************/

BOOL CTfontTbar::Create(CWnd* pcParentWnd)
    {
    BOOL bRC = TRUE;
    int iNumButtons = 0;
    int iNumSeparators;

    UINT ButtonIDS[MAX_TBAR_ITEMS];

        ButtonIDS[iNumButtons++] = IDC_BOLD;
        ButtonIDS[iNumButtons++] = IDC_ITALIC;
        ButtonIDS[iNumButtons++] = IDC_UNDERLINE;
    	ButtonIDS[iNumButtons++] = IDC_STRIKEOUT;
            
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
        SetButtonInfo(BOLD_TBAR_POS,      IDC_BOLD,       TBBS_CHECKBOX, BOLD_BMP_POS);
        SetButtonInfo(ITALIC_TBAR_POS,    IDC_ITALIC,     TBBS_CHECKBOX, ITALIC_BMP_POS);
        SetButtonInfo(UNDERLINE_TBAR_POS, IDC_UNDERLINE,  TBBS_CHECKBOX, UNDERLINE_BMP_POS);
		SetButtonInfo(STRIKEOUT_TBAR_POS, IDC_STRIKEOUT,  TBBS_CHECKBOX, STRIKEOUT_BMP_POS);

    CSize size = CToolBar::CalcFixedLayout( FALSE, TRUE );

    if (GetSafeHwnd() != NULL)
        {
        SetWindowPos( &wndTop, 0, 0, size.cx, size.cy, SWP_NOMOVE );
        }

    return bRC;
    }

/******************************************************************************/

CTfontTbar::~CTfontTbar(void)
{
}

void CTfontTbar::SetBarButtons(BOOL bBold,BOOL bItalic,BOOL bUnderline,BOOL bStrikeout)
{
	SetButtonInfo(BOLD_TBAR_POS,      IDC_BOLD,       TBBS_CHECKBOX | (bBold ?  TBBS_CHECKED : 0), BOLD_BMP_POS);
    SetButtonInfo(ITALIC_TBAR_POS,    IDC_ITALIC,     TBBS_CHECKBOX | (bItalic ?  TBBS_CHECKED : 0), ITALIC_BMP_POS);
    SetButtonInfo(UNDERLINE_TBAR_POS, IDC_UNDERLINE,  TBBS_CHECKBOX | (bUnderline ?  TBBS_CHECKED : 0), UNDERLINE_BMP_POS);
	SetButtonInfo(STRIKEOUT_TBAR_POS, IDC_STRIKEOUT,  TBBS_CHECKBOX | (bStrikeout ?  TBBS_CHECKED : 0), STRIKEOUT_BMP_POS);
	


}
/*
void CTfontTbar::GetBarButtons(LOGFONT &lf)
{
	UINT nID,nStyle;
	int nImage;

	GetButtonInfo(BOLD_TBAR_POS,      nID,  nStyle , nImage);
	lf.lfWeight = (nStyle&TBBS_CHECKED ? FW_BOLD : FW_NORMAL);

    GetButtonInfo(ITALIC_TBAR_POS,    nID,  nStyle, nImage);
	lf.lfItalic = nStyle&TBBS_CHECKED;

    GetButtonInfo(UNDERLINE_TBAR_POS, nID,  nStyle, nImage);
	lf.lfUnderline = nStyle&TBBS_CHECKED;

	GetButtonInfo(STRIKEOUT_TBAR_POS, nID,  nStyle, nImage);
	lf.lfStrikeOut = nStyle&TBBS_CHECKED;
}*/



