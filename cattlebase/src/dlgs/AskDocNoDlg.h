#pragma once


// CAskDocNoDlg dialog

class CAskDocNoDlg : public CDialog
{
	DECLARE_DYNAMIC(CAskDocNoDlg)

public:
	CAskDocNoDlg(UINT nCaptionID,UINT nLabelID,CWnd* pParent = NULL);   // standard constructor
	virtual ~CAskDocNoDlg();
	UINT GetDocNo();
	static UINT AskForDocNo(UINT nCaption,UINT nLabel,CWnd* pParent/*=NULL*/);

// Dialog Data
	enum { IDD = IDD_ASKDOCNO };
protected:
	CString m_caption;
	CString m_label;
	UINT m_docNo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
