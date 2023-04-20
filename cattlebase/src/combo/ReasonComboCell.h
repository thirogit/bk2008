#ifndef __REASONCOMBOCELL_H__
#define __REASONCOMBOCELL_H__

//#include "DataTypes.h"
#include "BaseCombo.h"
#include "datatypes/ReasonDataArray.h"

typedef CBasePtrCombo<CComboBox,Reason> CBaseReasonCombo;

class gxReasonComboCell : public CBaseReasonCombo
{
public:

	gxReasonComboCell (Reason::REASON_INOUT_TYPE inoutcombo,CListCtrl* pCtrl, int iItem, int iSubItem, Reason *curSel = NULL);
    virtual ~gxReasonComboCell();
    void    SetListSubItem();
 
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//virtual void ReloadCombo(Reason *curSel = NULL);

protected:
	CListCtrl* pListCtrl;
    int			Item;
    int			SubItem;
    Reason	*initSelection;

	BOOL m_bESC;
	int  m_prevItem;
	CReasonDataArray m_reasons;
    

	int GetNewItemCount();
	Reason *GetItemText(int item,CString &itemText);
    
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnNcDestroy();
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnCloseup();
	
	DECLARE_MESSAGE_MAP()

};
#endif