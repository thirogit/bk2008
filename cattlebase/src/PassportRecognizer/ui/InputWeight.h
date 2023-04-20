#ifndef __INPUTWEIGHT_H__
#define __INPUTWEIGHT_H__

#include "../../classes/types/Validator.h"
#include "../../ctrls/WeightEdit.h"

class CInputWeight : public CDialog
{
	DECLARE_DYNAMIC(CInputWeight)

public:
	CInputWeight(CWnd* pParent = NULL);   
	virtual ~CInputWeight();
	enum { IDD = IDD_EDITBOXDLG };

	NullDouble AskForWeight();
	void SetWeight(const NullDouble& weight);
	void SetValidator(CValidator<const NullDouble&> *pValidator);

protected:
	void SetLabel(const CString& sLabel);
	void SetWindowCaption(const CString& sCaption);

protected:
	CWeightEdit  m_WeightEdit;
	CDialog::DoModal;

	NullDouble	m_Weight;
	CString		m_sLabel;
	CString		m_sCaption;
	CValidator<const NullDouble&>* m_pValidator;
	virtual BOOL OnInitDialog();
	void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
};

#endif