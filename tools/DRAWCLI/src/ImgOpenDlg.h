#pragma once

#include "rectdlg.h"
// CImgOpenDlg dialog

class CImgOpenDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CImgOpenDlg)

public:
	CImgOpenDlg(CDrawImg *pImg);
	virtual ~CImgOpenDlg();

// Dialog Data
	enum { IDD = IDD_IMGOPEN };

	void DoChanges(CDrawObj *pObj);
	CString m_ImgFile;
	bool m_bNew;
	CPicture m_preview;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual BOOL OnInitDialog();
	

	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnOpenImg();
	afx_msg void OnPaint();
};
