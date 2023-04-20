#if !defined(FASTSELEDIT_H__INCLUDED_)
#define FASTSELEDIT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EnterEdit.h"
#include "../../barcodescanner/BCScannerDrv.h"

class CFastSelEdit : public CEnterEdit, public IBarcodeReceiver
{
	enum FASTSELECT_CONTENT_TYPE
	{
		NONE,
		EAN,
		NUMBER,
		TEXT
	};
public:
	CFastSelEdit();
	void SelAllText();
	virtual void OnReturnKey();
	virtual bool IsTextValid (const TCHAR *pszText);

	void SetContentToText();
	void SetContentToNumber();
	void SetContentToEAN();
protected:
	void ResetEdit();
	virtual void OnBarcode(const CString& sBarcode);

	FASTSELECT_CONTENT_TYPE m_currentContent;

	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

};


#endif 
