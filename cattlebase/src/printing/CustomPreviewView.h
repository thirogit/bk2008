#ifndef __CUSTOMPREVIEWVIEW_H__
#define __CUSTOMPREVIEWVIEW_H__

#include "MyPrintPreview.h"

class CCustomPreviewView : public CMyPreviewView
{
	DECLARE_DYNCREATE(CCustomPreviewView)
public:
	CCustomPreviewView();
protected:
	virtual void PositionPage(UINT nPage);
	virtual void OnDraw(CDC* pDC);
	
	
};
#endif
