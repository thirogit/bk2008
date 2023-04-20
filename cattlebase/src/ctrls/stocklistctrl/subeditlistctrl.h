#ifndef __SUBEDITLISTCTRL_H__
#define __SUBEDITLISTCTRL_H__

#define IDC_EDITCELL 9876
#define IDC_COMBOCELL 9877

class CSubEditListCtrl : public CListCtrl
{
public:
	CSubEditListCtrl() : CListCtrl() {};
	virtual ~CSubEditListCtrl() {};
	CEdit*  EditSubItem (int Item, int Column);

protected:
    //{{AFX_MSG(gxListCtrl)
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

   DECLARE_MESSAGE_MAP()
};

#endif