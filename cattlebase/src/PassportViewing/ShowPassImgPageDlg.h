#pragma once
#include "PassImage.h"
#include "fiWinImage.h"


// CShowPassImgPageDlg dialog

class CShowPassImgPageDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CShowPassImgPageDlg)

public:
	CShowPassImgPageDlg();
	virtual ~CShowPassImgPageDlg();

// Dialog Data
	enum { IDD = IDD_SHOWPASSIMG };
	bool UpdateWithImage(fipWinImageEx &pix);

protected:
	CPassImage m_passShow;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
