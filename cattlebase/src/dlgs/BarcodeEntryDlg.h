#ifndef __BARCODEENTRYDLG_H__
#define __BARCODEENTRYDLG_H__

//#include "AniDialog.h"
#include "../barcodescanner/BCScannerDrv.h"
#include "../ctrls/XColorStatic.h"
#include "../classes/types/Validator.h"

class CBarcodeEntryDlg : public CDialog, public IBarcodeReceiver
{
	DECLARE_DYNAMIC(CBarcodeEntryDlg)

public:
	CBarcodeEntryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBarcodeEntryDlg();
	enum { IDD = IDD_BARCODEENTRYDLG };

	void SetEntryText(const CString& entryText);
	const CString& GetBarcode();
	void SetValidator(CValidator<const CString&> *pValidator);
protected:
	CValidator<const CString&>*	  m_pValidator;
	CString		  m_barcodeValueStr;
	CString		  m_entryTextStr;
	CXColorStatic m_entryText;
	CXColorStatic m_barcodeValue;
	CXColorStatic m_barcodeStatus;
protected:
	virtual void OnBarcode(const CString& sBarcode);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


	afx_msg void OnOverride();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif