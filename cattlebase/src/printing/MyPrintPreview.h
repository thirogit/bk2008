#ifndef __MYPRINTPREVIEW_H__
#define __MYPRINTPREVIEW_H__


class  CMyPreviewView : public CPreviewView
{
protected: // create from serialization only
	CMyPreviewView();
	DECLARE_DYNCREATE(CMyPreviewView)

protected:
	afx_msg void OnPreviewPrint();
	afx_msg void OnPreviewClose();
	afx_msg void OnNumPageChange();
	afx_msg void OnNextPage();
	afx_msg void OnPrevPage();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	DECLARE_MESSAGE_MAP()
}; 

#endif
