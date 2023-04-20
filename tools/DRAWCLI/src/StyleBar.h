#ifndef __STYLEBAR_H__
#define __STYLEBAR_H__

// TOOLBAAR CONSTANTS -- Bitmap Positions and Toolbar Positions
#ifdef DBCS //VertEdit
#define MAX_TBAR_ITEMS        12 // positions 0 through 11
#else //DBCS
#define MAX_TBAR_ITEMS        11 // positions 0 through 10
#endif //DBCS

#define BOLD_BMP_POS          0
#define ITALIC_BMP_POS        1
#define UNDERLINE_BMP_POS     2
#define STRIKEOUT_BMP_POS     3

/*
#define PEN_BMP_POS           3
#define EDITTEXT_BMP_POS      4
#define KEYBOARD_BMP_POS      5   // still wasting space in image
#define INS_SPACE_BMP_POS     6
#define BACKSPACE_BMP_POS     7
#define NEWLINE_BMP_POS       8
*/

//#define SHADOW_BMP_POS        -1  // don't exist currently

#define BOLD_TBAR_POS         0
#define ITALIC_TBAR_POS       1
#define UNDERLINE_TBAR_POS    2
#define STRIKEOUT_TBAR_POS    3


//#define FONT_BMP_TXT_BORDER   2   // # pixels between font bmp (prn/tt) and text

//#define UM_DELAYED_TOOLBAR   WM_USER + 900


#define IDC_BACKSPACE                   124
#define IDC_NEWLINE                     125
#define IDC_EDITTEXT                    121
//#define IDC_KEYBOARD                    122
#define IDC_INS_SPACE                   123
#define IDC_PENEXT                      120


class CTfontTbar : public CToolBar
    {
    private:

    protected: // create from serialization only

    DECLARE_DYNCREATE(CTfontTbar)

    protected:

    //{{AFX_MSG(CTfontTbar)
  
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    public:
	void SetBarButtons(BOOL bBold,BOOL bItalic,BOOL bUnderline,BOOL bStrikeout);
	//void GetBarButtons(LOGFONT &lf);

    CTfontTbar(void);

    virtual ~CTfontTbar(void);
    BOOL    Create(CWnd* pcParentWnd);
    };
#endif