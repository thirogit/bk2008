#pragma once


#include "drawObj.h"
#include "RectDlg.h"
// CFigPropDlg dialog

class CFigPropDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CFigPropDlg)

public:
	CFigPropDlg(CDrawRect *rectObj);
	virtual ~CFigPropDlg();
	virtual void DoChanges(CDrawObj *pObj);
	
// Dialog Data
	enum { IDD = IDD_PROP_FIG };

protected:
	CSliderCtrl m_roundX;
	CSliderCtrl m_roundY;
	int m_Height;
	int m_Width;
	CPoint m_roundnes;	
	CDrawRect::Shape m_Shape;
	void	UpdateRoundnes();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	BOOL OnApply();

	/*afx_msg void OnDeltaPosX(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaPosY(NMHDR* pNMHDR, LRESULT* pResult);*/
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};
