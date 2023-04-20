#pragma once


// CGroupPos dialog

class CGroupPos : public CDialog
{
	DECLARE_DYNAMIC(CGroupPos)

public:
	CGroupPos(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGroupPos();

	CSize GetPos();
private:
	CSliderCtrl m_VSlider;
	CSliderCtrl m_HSlider;
	int PosX;
	int negPosY;

	void SetStatics();


// Dialog Data
	enum { IDD = IDD_GROUPPOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};
