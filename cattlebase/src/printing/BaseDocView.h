#ifndef __BASEDOCVIEW_H__
#define __BASEDOCVIEW_H__

#include "CowBasePrintDoc.h"

class CBaseDocView : public CScrollView
{
public:
	CBaseDocView();

	CFrameWnd *m_pOldFrame;
	int nCurPrintPage;
	CDialog *m_pCtrlDlg;
public: 
	virtual void PrintPreview() = 0;
	virtual void Print(int nCopies) = 0;
	virtual CCowBasePrintDoc* GetDocument();
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);

	afx_msg void OnFilePrint();
	DECLARE_MESSAGE_MAP()
}; 

#endif 
