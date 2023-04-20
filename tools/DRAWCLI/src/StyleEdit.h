#pragma once

#include "TextStyle.h"
#include "FontCombo.h"
#include "StyleBar.h"
#include "OrientationCtrl.h"

// CStyleEdit dialog

class CStyleEdit : public CDialog
{
	DECLARE_DYNAMIC(CStyleEdit)

public:
	CStyleEdit(CTextStyle *pStyle,CWnd* pParent = NULL);   // standard constructor
	virtual ~CStyleEdit();

// Dialog Data
	enum { IDD = IDD_STYLE };

	CFontCombo m_fonts;
	CSpinButtonCtrl m_HeightSpin;
	UINT    m_Height;
	CExtColorButton	m_BtnTextColor;
	CString m_stylename;
	CTfontTbar   *m_styleBar;

	CHAlignCombo m_HAlignCombo;
	CVAlignCombo m_VAlignCombo;
	COrientationCtrl m_orientation;

	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	BOOL m_bStrikeout;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void SetStyle(CTextStyle *pStyle);
	void OnOK();


	CTextStyle *m_pStyle;

	afx_msg void OnBold();
	afx_msg void OnItalic();
	afx_msg void OnStrikeout();
	afx_msg void OnUnderline();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};
