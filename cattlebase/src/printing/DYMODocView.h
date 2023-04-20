#ifndef __DYMODOCVIEW_H__
#define __DYMODOCVIEW_H__

#include "../printdocs/DYMOBaseDoc.h"
#include "CustomPaperView.h"

class CDYMODocView : public CCustomPaperView
{
	DECLARE_DYNCREATE(CDYMODocView)
public: 
	CDYMODocView();
	
	CDYMOBaseDoc* GetDocument()  { return (CDYMOBaseDoc*)m_pDocument; }
	virtual ~CDYMODocView();
	virtual void PrintPreview();
	virtual void Print(int nCopies);
	virtual CSize GetPaperSize();
protected:
	void SetPageSize(CSize size);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);  
	virtual void OnDraw(CDC* pDC);  
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate(); 

	void PrintDYMO(int nCopies);
	void PrintSeiko(int nCopies);
protected:
	CDC m_DCGlyphs;


	DECLARE_MESSAGE_MAP()
}; 

#endif 
