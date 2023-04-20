#ifndef OCRINDOCUPDATEDLG_H_INCLUDED
#define OCRINDOCUPDATEDLG_H_INCLUDED
#pragma once

#include "OCRPassDlg.h"

class COCRInDocUpdateDlg : public COCRPassDlg
{
public:
	COCRInDocUpdateDlg(CCowArray &inArray,CCowArray &sesArray,CCowDataPassImgMap &passMap,PassportRecognizerSettings* pSettings,CWnd* pParent = NULL);	
	//~COCRInDocUpdateDlg();

protected:
	virtual bool DoManualInput();
	virtual  bool EditCow(Cow *pCow);
protected:
	afx_msg void OnUpdateCow();
	DECLARE_MESSAGE_MAP()
};
#endif