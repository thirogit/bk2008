#ifndef __ASKFORDATEDLG_H__
#define __ASKFORDATEDLG_H__

#include "../classes/types/DateTime.h"
#include "../ctrls/curdateedit/Curdateedit.h"
#include "../classes/types/Validator.h"

class CAskForDateDlg : public CDialog
{
	DECLARE_DYNAMIC(CAskForDateDlg)

public:
	CAskForDateDlg(CWnd* pParent = NULL);   
	virtual ~CAskForDateDlg();
	enum { IDD = IDD_EDITBOXDLG };

	CDateTime AskForDate();
	void SetLabel(const CString& sLabel);
	void SetWindowCaption(const CString& sCaption);
	void SetDate(const CDateTime& dt);
	void SetValidator(CValidator<const CDateTime&> *pValidator);


protected:
	CCurDateEdit  m_dtEditBox;
	CDialog::DoModal;

	CDateTime	m_dt;
	CString		m_sLabel;
	CString		m_sCaption;
	CValidator<const CDateTime&>* m_pValidator;


	virtual BOOL OnInitDialog();
	void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
};


#endif //__ASKFORDATEDLG_H__