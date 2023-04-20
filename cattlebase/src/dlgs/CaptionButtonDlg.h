#ifndef __CAPTIONBUTTONDLG_H__
#define __CAPTIONBUTTONDLG_H__

#include "..\ctrls\CaptionButton.h"




//
//template<class BASEDLG>
//class CCaptionButtonDlg : public BASEDLG
//{
//public:
//
//	CCaptionButtonDlg() : BASEDLG(),m_bRestoring(FALSE),bCapInit(FALSE) {};
//
//	virtual INT_PTR RestoreDoModal()
//	{
//		m_bRestoring = TRUE;
//		return DoModal();
//	}
//
//	virtual ~CCaptionButtonDlg() {};
//private:
//	BOOL m_bRestoring;
//protected:
//	virtual BOOL OnInitDialog() 
//	{
//		if(!BASEDLG::OnInitDialog()) return FALSE;
//
//		SetupCaptionButton();
//
//		return TRUE;
//	}
//
//	inline BOOL IsRestoring() { return m_bRestoring; }
//
//	void SetupCaptionButton()
//	{
//		CCaptionButton::InitCapBtn(m_hWnd);		
//		m_pCapBtn.SetBmpID(1, IDB_CAPMIN, IDB_CAPPUSHED);
//		m_pCapBtn.ResetButtonState();
//		SendMessage( WM_NCACTIVATE, TRUE,0L);	
//	}
//
//	CCaptionButton m_pCapBtn;
//	BOOL bCapInit;
//	
//
//	LRESULT  OnWMU_HitCapButton(WPARAM wParam,LPARAM lParam)
//	{
//		m_bRestoring = FALSE;
//		SaveDialog();
//		EndDialog(IDCONTINUE);
//		return (LRESULT)0;
//	}
//
//	virtual void SaveDialog()  = 0;
//
//
//	
//	
//	DECLARE_MESSAGE_MAP()
//	
//};
//
//
//// announce the user defined message:
//DECLARE_USER_MESSAGE(WMU_HITCAPBUTTON)
//
//
//BEGIN_MESSAGE_MAP_TEMPLATE(CCaptionButtonDlg, BASEDLG,BASEDLG)
//	ON_REGISTERED_MESSAGE(WMU_HITCAPBUTTON, OnWMU_HitCapButton)
//END_MESSAGE_MAP()
//
//typedef CCaptionButtonDlg<CDialog> HoldDlg;
//
//
//

#endif