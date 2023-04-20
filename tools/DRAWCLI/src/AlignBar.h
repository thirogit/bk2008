#ifndef __ALIGNBAR_H__
#define __ALIGNBAR_H__

// TOOLBAAR CONSTANTS -- Bitmap Positions and Toolbar Positions

#define MAX_ALIGNBAR_ITEMS        11 // positions 0 through 10

#define LEFT_BMP_POS          4
#define RIGHT_BMP_POS        5
#define CENTER_BMP_POS     6
#define JUSTIFY_BMP_POS    7


#define LEFT_TBAR_POS         0
#define RIGHT_TBAR_POS       1
#define CENTER_TBAR_POS    2
#define JUSTIFY_TBAR_POS    3

#define IDC_LEFT   555
#define IDC_RIGHT  556
#define IDC_CENTER 557
#define IDC_JUSTIFY 558


class CAlignBar : public CToolBar
    {
    private:

    protected: // create from serialization only

    DECLARE_DYNCREATE(CAlignBar)

    protected:

    //{{AFX_MSG(CAlignBar)
  
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    public:

    CAlignBar();

    virtual ~CAlignBar();
    BOOL    Create(CWnd* pcParentWnd);
    };
#endif