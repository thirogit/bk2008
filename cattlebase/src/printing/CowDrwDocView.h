#ifndef __COWDRWDOCVIEW_H__
#define __COWDRWDOCVIEW_H__

#include "CowDrwDoc.h"
#include "BaseDocView.h"

class CCowDrwDocView : public CBaseDocView
{
	DECLARE_DYNCREATE(CCowDrwDocView)

public: 
	CCowDrwDocView();
	CCowDrwDoc* GetDocument()  { return (CCowDrwDoc*)m_pDocument; }
	void DocToClient(CRect& rect);
	void DocToClient(CPoint& point);
	void ClientToDoc(CPoint& point);
	void ClientToDoc(CRect& rect);
	static void SetUpDC(CDC *pDC);
	void SetPageSize(CSize size);
	virtual void PrintPreview();
	virtual void Print(int nCopies);

	virtual ~CCowDrwDocView();

protected:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);  
	virtual void OnDraw(CDC* pDC);  
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	
	virtual void OnInitialUpdate();

	void PrinteSeiko(int nCopies);
	void PrintWindows(int nCopies);

	DECLARE_MESSAGE_MAP()
}; 

#endif 
