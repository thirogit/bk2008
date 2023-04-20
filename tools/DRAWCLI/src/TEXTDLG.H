
#ifndef __TEXTDLG_H__
#define __TEXTDLG_H__
/////////////////////////////////////////////////////////////////////////////
// CTextDlg dialog
#include "fontcombo.h"
#include "rectdlg.h"
#include "StyleBar.h"
#include "OrientationCtrl.h"


class CTextDlg : public CBaseDlg
{
	void _UpdateControls();

// Construction
public:
	
	DECLARE_DYNCREATE(CTextDlg);

	CTextDlg(); //try to use it i dare you

	CTextDlg(CDrawText *pTxtObj);
	//CTextDlg(CString &text,CTextStyle &style,CStyleList &StyleList);
	virtual ~CTextDlg();
	void DoChanges(CDrawObj *pObj);
	
// Dialog Data
	//{{AFX_DATA(CTextDlg)
	enum { IDD = IDD_PROP_TEXT };
	//}}AFX_DATA
private:
	COLORREF m_clrText;
	CString	m_text;

    CExtColorButton	m_BtnTextColor;
	CFontCombo m_fontCombo;
	CStyleCombo m_stylecombo;

	CSpinButtonCtrl m_HeightSpin;
		
	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	BOOL m_bStrikeout;

	CVAlignCombo m_valigncombo;
	VER_ALIGN m_valign;

	CHAlignCombo m_haligncombo;
	HOR_ALIGN m_halign;

	UINT    m_Height;
	LOGFONT m_LogFont;
	BOOL m_bNotOwnStyle;

	COrientationCtrl m_orientation;
	UINT m_txtAngle;


	//style from list
	CTextStyle *m_pStyle;

	void ChooseFont(LOGFONT* font);
	void DoStyle(CTextStyle &style);
	void GetSelectedFont(LOGFONT *lf);
	/*void OnOK();*/
	BOOL OnApply();

	CTfontTbar    *m_pcTfontTbar; // must be dynamic for changing buttons (delete/new)
	

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CTextDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnInternalUpdateControls(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColorChangedFinally(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBold();
	afx_msg void OnItalic();
	afx_msg void OnStrikeout();
	afx_msg void OnUnderline();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif