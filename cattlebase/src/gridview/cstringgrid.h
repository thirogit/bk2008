#ifndef CStringGridH
#define CStringGridH

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include <afxcview.h>
#undef  WINVER
#define WINVER 0x0500

#include <windows.h>
#include <string>
#include <vector>
using namespace std;

#define minmax(a,x,b) (min(max(x, a), b))
#define MAX_ROWS        2048
#define MAX_COLS          80
#define MAX_COLUMN_CHARS  80
#define NO_COLOR		  (COLORREF)(-1)

// notification messages sent to parent
#define SGN_CELLCHANGED (WM_USER + 1961)
#define SGN_RIGHTCLICK  (WM_USER + 1962)

// option flags
const DWORD SGO_NONE          = 0x00000000; // no options
const DWORD SGO_VISIBLE       = 0x00000001; // grid visible
const DWORD SGO_CELLSEL       = 0x00000002; // select single cell
const DWORD SGO_ROWSEL        = 0x00000004; // select whole row
const DWORD SGO_COLSEL        = 0x00000008; // select whole column
const DWORD SGO_EDITABLE      = 0x00000010; // allow editing cells
const DWORD SGO_SCROLLDOT     = 0x00000020; // grid scrolled indicators
const DWORD SGO_AUTOSIZE      = 0x00000040; // autosize columns
const DWORD SGO_FIXEDCOL      = 0x00000080; // show fixed column
const DWORD SGO_FIXEDROW      = 0x00000100; // show fixed row
const DWORD SGO_USERSIZE      = 0x00000200; // user sizable columns
const DWORD SGO_EXTENDLINES   = 0x00000400; // extend grid lines if last
                                            //    column/row offscreen
const DWORD SGO_EXTENDCELLS   = 0x00000800; // extend last col & row if cells
                                            //    don't fill client area
const DWORD SGO_NUMBERROWS    = 0x00001000; // row numbers in fixed column */
const DWORD SGO_SHOWFOCUS     = 0x00002000; // show not focused by graying text
const DWORD SGO_TITLE         = 0x00004000; // show title at top of grid
const DWORD SGO_XPSTYLE       = 0x00008000; // new style fixed cells and
                                            //    selected cell
const DWORD SGO_XPHORZ        = 0x00010000; // fixed col horizontal gradient
const DWORD SGO_ROWBARS       = 0x00020000; // color alternate rows

enum SGA_ALIGNSTYLE
{
SGA_NOSTYLE        = 0x0000,
SGA_ALIGNCENTER    = 0x0001,
SGA_ALIGNLEFT      = 0x0002,
SGA_ALIGNRIGHT     = 0x0004,
SGA_ALIGNVCENTER   = 0x0008,
SGA_ALIGNTOP	   = 0x0010,
SGA_ALIGNBOTTOM	   = 0x0020
};

// color indices
enum SGC_ENUM 
{
  SGC_TEXT_0 = 0, SGC_TEXT_1,       // 16 cell text colors
  SGC_TEXT_2,     SGC_TEXT_3,
  SGC_TEXT_4,     SGC_TEXT_5,
  SGC_TEXT_6,     SGC_TEXT_7,
  SGC_TEXT_8,     SGC_TEXT_9,
  SGC_TEXT_A,     SGC_TEXT_B,
  SGC_TEXT_C,     SGC_TEXT_D,
  SGC_TEXT_E,     SGC_TEXT_F,

  SGC_COLUMN_1,   SGC_COLUMN_2,     // 4 column colors
  SGC_COLUMN_3,   SGC_COLUMN_4,

  SGC_GRIDBKGND,                    // grid background
  SGC_CELLBKGND,                    // cell background
  SGC_EDITBKGND,                    // edit cell background
  SGC_FXDCELL,                      // fixed cell backgrouond
  SGC_SELCELL,                      // selected cell background

  SGC_FXDCOLTEXT,                   // fixed column text
  SGC_FXDROWTEXT,                   // fixed row text
  SGC_TITLETEXT,                    // title cell text
  SGC_EDITTEXT,                     // edit cell text
  SGC_SELTEXT,                      // selected cell text
  SGC_GRAYTEXT,                     // grayed text when unfocused

  SGC_SELBORDER,                    // selected cell border
  SGC_EDITBORDER,                   // edit cell border
  SGC_GRIDLINE,                     // grid lines
  SGC_SCROLLDOT,                    // scroll dot outline
  SGC_ODDROWBARS,                   // odd row color for rowstripe option
  SGC_GRADTOP,
  SGC_GRADBOTTOM,
  NUM_COLORS                        // last value = number of colors
};


const DWORD SGC_ERROR = 0xFF000000; // value to return if error

// font indices
enum SGF_ENUM
{
  SGF_FIXEDCELLS,                   // fixed column/row font
  SGF_CELLS,                        // normal cell font
  SGF_TITLE,                        // title font
  SGF_EDIT                          // edit mode font, must be fixed width font
};



// class declarations
// cell class
class CellType
{
 public:
	enum SG_CELLTYPE
	{
		PNUMERIC = 0x33, //random value
		PSTRING,
		PNULL,

	};

    CellType() :
     // m_string  (""),
     m_color   (NO_COLOR),
	 m_style   (SGA_NOSTYLE),
	 m_pStr(NULL),
	 m_pInt(NULL),
	 m_tType(PNULL),
	 m_bOwner(FALSE) //when NULL no need to delete
      {};

	 void FreePtr(void)
	 {
		 if(m_bOwner)
		{
			switch(m_tType)
			{
			case PNUMERIC:
				delete m_pInt;
				m_pInt = NULL;
				break;
			case PSTRING:
				delete m_pStr;
				m_pStr = NULL;
				break;
			}
			m_bOwner = FALSE;
			m_tType = PNULL;
		}
	 };
    ~CellType()
	{
		FreePtr();		
	};

	bool SetThisCell(void *ptr,CellType::SG_CELLTYPE cellType,BOOL b_isOwner)
	{
	
          FreePtr();
  
		  m_tType = cellType;
		  m_bOwner = b_isOwner;
		  switch(cellType)
		  {
			case PNUMERIC:	
				m_pInt = (int*)ptr;
				m_pStr = NULL;
			
			break;

			case PSTRING:
				m_pStr = (CString*)ptr;
				m_pInt = NULL;
				
			break;
			case PNULL:
				m_pStr = NULL;
				m_pInt = NULL;
     		 break;
		 }

		return true;
	}


	friend BOOL operator>(const CellType &left, const CellType &right);
	friend BOOL operator<(const CellType &left, const CellType &right);

    CString*    m_pStr;
	int*		m_pInt;	
	SG_CELLTYPE m_tType;
	BOOL		m_bOwner; // when true , destructor deletes pInt or pStr
    COLORREF    m_color;
	SGA_ALIGNSTYLE   m_style;
};

class CloakCellType : public CellType
{
public:
	CloakCellType() : CellType(),colspan(1),rowspan(1),in_span(false),corner(true) {};
	int colspan; 
	int rowspan; 
	bool in_span;
	bool corner;
};

class CloakRowType 
{
public:
	CloakRowType(int h = -1) : height_per100(h) , m_color(NO_COLOR){};
	vector <CloakCellType> m_cloakcells;

	int height_per100;
	int pixel_height;
	COLORREF    m_color;
    WORD        m_style;
};


class CloakColType
{
public:

	CloakColType(int w = -1) : width_per100(w) {};

	int width_per100;
	int percent_width;

};


#define DEFAULT_ROW_COLOR RGB(255,255,255)
#define DEFAULT_COL_COLOR DEFAULT_ROW_COLOR
#define DEFAULT_CLOAK_HEIGHT 48
#define DEFAULT_COLUMN_WIDTH 100


class RowType
{
public:
	  RowType() :
	  hidden		(true), 
	  cloakHeight(DEFAULT_CLOAK_HEIGHT),
      m_color   (DEFAULT_ROW_COLOR),
      m_style   (SGA_NOSTYLE),
	  cloakcols(0),
	  cloakrows(0),
	  visible(true),
	  userptr(NULL),
	  selected(false),
	  bblue(false)
      {};

	  void toogle_select() { selected = !selected; };
	  void toogle() { hidden = !hidden; };
	  void show() { hidden = false; };
	  void hide() { hidden = true; };
	  int GetCloakHeight(); 
	  bool SetCloakHeight(int iHeight);
	  void MergeCols(void);
	  void MergeRows(void);
	  bool SetCloakRowHeight(int row, int percent);
	  bool SetCloakColWidth(int col,int percent);
	  bool SetCloakSize(int rows,int cols);
	  bool SetCloakCellColor(int col,int row,COLORREF color);
	  bool SetCloakCellSpan(int col,int row,int colspan,int rowspan);
	  bool CloakCellExists(int col,int row);

	  bool      SetCloakCell(int col,int row,void *ptr,CellType::SG_CELLTYPE cellType,BOOL b_isOwner);
	  bool      SetCloakCellStrPtr(int col,int row,CString *strPtr);
	  bool      SetCloakCellIntPtr(int col,int row,int *intPtr);
	  bool      SetCloakCellStr(int col,int row,CString &strVal);
	  bool      SetCloakCellInt(int col,int row,int intVal);
	  CString   CloakCell(int col,int row);

	  
	vector<CloakColType> m_CloakCols;
	vector<CloakRowType> m_CloakRows;

	bool selected;
	bool hidden; 
	vector<CellType>    m_cells;
    COLORREF    m_color;
    WORD        m_style;

  //private:
	int cloakcols;
	int cloakrows;
	int cloakHeight;
	bool visible;
	bool bblue;
	
	void * userptr;
};


// column class
class ColumnType
{
  public:
    
    ColumnType(int width = DEFAULT_COLUMN_WIDTH) :
      m_color  (NO_COLOR),
      m_style  (SGA_NOSTYLE),
	  m_width  (width), 
	  m_bAscending(TRUE)
	  {};

    int         m_width;
    COLORREF    m_color;
    SGA_ALIGNSTYLE    m_style;
	BOOL       m_bAscending;
};

#define NULLROWHANDLE (0)
typedef int ROWHANDLE ;

class RowTypeHandle
{
	static int _HanCounter;
public:
	RowTypeHandle(RowType *rowPtr) : row(rowPtr)
	{
		rowHandle = _HanCounter;
		_HanCounter++;
	};
	int rowHandle;
	RowType *row;
};

typedef DWORD HLSCOLOR;

typedef CTypedPtrArray< CPtrArray, RowType* > RowArray;
typedef CTypedPtrArray<CPtrArray,RowTypeHandle*> RowHandleArray;

#define DEFAULT_ROW_HEIGHT 25


extern UINT uMSG_MOUSEWHEEL;

// grid class
class CStringGrid : public CView
{
   DECLARE_DYNCREATE(CStringGrid)
protected:
	
    CStringGrid(int cols = 5, int rows = 5); 
		
	 virtual ~CStringGrid();

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
    	
	    
    BOOL      Create(HWND hParent, UINT uID, RECT* rect,
                     DWORD dwStyle = 0, DWORD dwExStyle = 0);
protected:
	bool SetCell(int col, int row, void *ptr,CellType::SG_CELLTYPE cellType,BOOL b_isOwner);
	CString   drawCell(int col,int row); //does the conversion fom int
	bool      HitTestCell(int x, int y, int& c, int& r);
	bool      CellExists(int col, int row);

	bool      ColExists(int col);
	bool      fastDeleteCol(int col);
	void      fastInsertCol(int col,int width);
	
public:
	//cells functions
    int       CellInt(int col, ROWHANDLE row);
    LPCSTR    CellString(int col, ROWHANDLE row);
	bool      SetCellStrPtr(int col,ROWHANDLE row,CString *strPtr);
	bool      SetCellIntPtr(int col,ROWHANDLE row,int *intPtr);
	bool      SetCellStr(int col,ROWHANDLE row,CString &strVal);
	bool      SetCellInt(int col,ROWHANDLE row,int intVal);
	COLORREF  GetCellColor(int col, ROWHANDLE row);
    SGA_ALIGNSTYLE      GetCellStyle(int col, ROWHANDLE row);
	void      SetCellStyle(int col, ROWHANDLE row, SGA_ALIGNSTYLE style);
    void      SetCellColor(int col, ROWHANDLE row, COLORREF color);
	bool      GetCellRect(int c, ROWHANDLE r, RECT& rc);

   
	//row functions
public:
	int       RowCount()  {return m_rowCount;}; // return number of rows
    bool      DeleteRow(ROWHANDLE row);               // delete row
	ROWHANDLE AddRow();
	//void      SetTopRow(ROWHANDLE row);               // set top displayed row
	void*	  GetHighlightedRowPtr(void);
	bool      SetRowCloakSize(ROWHANDLE row,int cloakrows,int cloakcols);
	void      SetRowVisible(ROWHANDLE iRow,bool bVis);
	void      ToggleVisible(ROWHANDLE iRow);
ROWHANDLE     GetHighlightedRowHandle();
	bool      GetRowVisible(ROWHANDLE row);
	bool      SetRowColor(ROWHANDLE row,COLORREF color);
	RowType*  GetRowPtr(ROWHANDLE row);
	void	  FreezHighlight(void);
	void	  UnFreezHighlight(void);
	void      SetRowHeight(int height);
	ROWHANDLE GetFirstRowHandle();
	ROWHANDLE GetNextRowHandle();
	void      ShowAllRows(BOOL bRedraw = TRUE);
	int       GetSelectedRows(CPtrArray &ary_sel);
	void      SelectHighlightedRow();
	void      UnselectHighlightedRow();
	void      SelectAll();
	void	  SelectVisibleRows();
	void      UnselectAll();
	void      InvertSelection();

protected:
	ROWHANDLE _GetRowHandle(BOOL bReset);
	inline int    VisibleRows();
	int       TopRow()    {return m_topRow;};   // return top visible row
	int       BottomRow(); 
	bool      drawDeleteRow(int row,bool bSetScroll = true);  
	RowType*  GetRowByHandle(ROWHANDLE rowHndl,int *index = NULL);
	RowType*  searchRow(int l, int r,ROWHANDLE v,int &idx);
	void      drawSetRowVisible(int iRow,bool bVis);
	int		  drawFindRow(RowType *rowPtr);
	void      AquireAfterDelete(int deletedRow);
	int       ComputePageSize();
	int       RowHeight(int row);
	ROWHANDLE fastAddRow();
		                 
    

//column functions
public:
inline int    ColCount()  {return m_colCount;}; // return number of columns
inline int    LeftColumn(){return m_leftCol;};  // return left visible column
    int       RightColumn();                    // return right visible column
    void      SetLeftCol(int col);              // set left displayed column
    void      InsertCol(int col, int width);    // insert column
    bool      DeleteCol(int col);               // delete column
    bool      AutoSizeCol(int col);             // fit column to text
    void      SetColCharWidths(int widths[]);
    bool      SetColWidth(int col, int width);
    int       AvgColWidth();
	int		  GetColWidth(int col);
	bool      SortByCol(int col);
protected:
	void      QuickSortRecursive(int col,int d, int h, BOOL bAscending);
	int		  HeaderColHit(int x,int y);
	int       m_nWchichCol;
	bool      m_bWantToSort;



public:
inline HWND   Handle(){return m_gridhWnd;};
    int       HeadHeight();
    int       TitleHeight();
	void      SetTitleString(LPCSTR title);
    LPCSTR    GetTitleString();
    void      SetTitleHeight(int height);
    bool      ResizeGrid(int cols, int rows);
    void      ClearGrid();
    void      AddOptions(DWORD options);        // add options
    void      DelOptions(DWORD options);        // delete options
    bool      CheckOptions(DWORD options);      // check if options set
inline DWORD  GetOptions() {return m_dwOptions;};
    void      SetOptions(DWORD options);        // set options
    void      SetVisible(bool v);
	COLORREF  SetColor(SGC_ENUM  index, COLORREF color);
    COLORREF  GetColor(SGC_ENUM  index);
    void      SetFont(SGF_ENUM font, LOGFONT &lf);
    bool      Visible();
	void	  ReDraw(void);


private:
	
COLORREF      HLS_TRANSFORM (COLORREF rgb, int percent_L, int percent_S);
HLSCOLOR      RGB2HLS (COLORREF rgb);
COLORREF	  HLS2RGB (HLSCOLOR hls);
BYTE          _ToRGB (float rm1, float rm2, float rh);
    void      DrawGrid();                       // draw the grid
    void      BtnCell(RECT rc);
    void      SetScrollbars(BOOL setVert = TRUE);

	void      MesureClientRect();
	

protected:
    HINSTANCE       m_hInst;                // instance handle
    HWND            m_gridhWnd;                 // window handle
    HDC             m_memdc;                // virtual canvas dc
   // BOOL            m_bWindowClosed;        // window close flag ?
  //  RECT            m_clRect;               // client rectangle
  //  UINT            m_uID;                  // unique ID
    HBITMAP         m_hBitmap;              // virtual canvas bitmap
    HGDIOBJ         m_hOldBitmap;           // handle of default dc bitmap

    int             m_top;                  // top of grid
    int             m_left;                 // left of grid
    int             m_width;                // window width
    int             m_height;               // window height
	int             m_colCount;             // number of columns
    int             m_rowCount;             // number of rows
	int             m_rowHeight;            // height of a row
    int             m_titleHeight;          // height of title
    int             m_leftCol;              // left visible col
    int             m_rightCol;             // right visible col
    int             m_topRow;               // top visible row
    int             m_bottomRow;            // bottom visible row
	int			    m_ScreenRows ;			//how many rows on screen
	int				m_row_hlight;			// highlighted row
	int				m_VisRows;
  
    
      
    bool            m_bFocused;             // grid focused flag
    bool            m_bGrayed;              // grayed if not focused flag
    bool            m_bSizing;              // column sizing flag
    bool            m_bSizeGrip;
    int             m_sizingCol;
    int             m_sizingLeft;
	
    DWORD           m_dwOptions;            // option flags
    DWORD           m_dwExStyle;            // extended window style
    DWORD           m_dwStyle;              // window style
   
	LOGFONT         m_fxFont;               // fixed col/row font
    LOGFONT         m_cellFont;             // grid cell font
    LOGFONT         m_editFont;             // edit mode font
    LOGFONT         m_titleFont;            // title font
    COLORREF        m_colors[NUM_COLORS];   // array of colors
      
    CString         m_titleString;

	RowHandleArray  m_rowHandles;
   
    vector<ColumnType>  m_columns;              

	RowArray        m_drawRows;

	CImageList m_imgList;
	CBitmap     m_SelectImage;
	BITMAP      m_SelectBitmap;
	bool m_bFreezHighlight;
	
	  

  	// returns a pointer to the window (stored as the WindowLong)
    /*inline static CStringGrid *GetObjectFromWindow(HWND hWnd)
    {
      return (CStringGrid *)GetWindowLong(hWnd, GWL_USERDATA);
    }*/




protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CDummyList)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd); 
	//afx_msg void OnContextMenu(CWnd *pWnd,CPoint pos);
	afx_msg void OnDestroy();
	afx_msg void OnVScroll(UINT nSBCode,UINT nPos,CScrollBar *pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode,UINT nPos,CScrollBar *pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags,CPoint point);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags,CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt,UINT nFlags);
	//afx_msg LONG OnWheel( UINT nFlags, LONG pValue );
	afx_msg BOOL OnMouseWheel(UINT nFlags,short zDelta,CPoint pt);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif



