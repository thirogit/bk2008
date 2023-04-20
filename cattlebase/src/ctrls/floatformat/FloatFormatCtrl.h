#ifndef __FLOATFORMATCTRL_H__
#define __FLOATFORMATCTRL_H__

#include "../../classes/format/FloatFormat.h"

class CFloatFormatCtrl : public CStatic
{
public:
	CFloatFormatCtrl();
	virtual ~CFloatFormatCtrl();

	FloatFormat GetFloatFormat() const;
	void SetFloatFormat(const FloatFormat& fmt);

protected:

	virtual void PreSubclassWindow();	
	
	CButton *m_pGroupBox;
	CStatic *m_pDecimalPtLabel;
	CStatic *m_pPrecisionLabel;
	CComboBox* m_pPrecisionCb;
	CComboBox* m_pDecimalPtCb;	
	
	BOOL CreateControls();
	BOOL InitControls(const FloatFormat* pFormat = NULL);
	
	
	
protected:
	afx_msg void OnEnable(BOOL bEnable);

	DECLARE_MESSAGE_MAP()
};


#endif

