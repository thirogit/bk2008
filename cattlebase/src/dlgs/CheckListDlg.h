#ifndef __CHECKLISTDLG_H__
#define __CHECKLISTDLG_H__


#include "../ctrls/optionchecklistbox/CheckOption.h"
#include <map>
#include "../ctrls/optionchecklistbox/OptionCheckListBox.h"

class CCheckListDlg : public CDialog
{
	//DECLARE_DYNAMIC(CCheckListDlg)

public:
	enum { IDD = IDD_CHECKLISTDLG };

	CCheckListDlg(CWnd* pParent = NULL);   
	virtual ~CCheckListDlg();
	void AddOption(const CString& sOptDesc,bool* bValue);
	void SetOptions();
	
protected:
	virtual CString GetDlgCaption() = 0;
	virtual void Init() = 0;
	virtual void SaveAndUse() = 0;
	virtual void UseOnlyOnce() = 0;

	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();

	std::map<bool*,CCheckOption*> m_optionsMap;
	COptionCheckListBox m_checkList;

	afx_msg void OnUseOnlyOnce();
	afx_msg void OnSaveAndUse();
	DECLARE_MESSAGE_MAP()
};

#endif