#ifndef __FINDDOCDLG_H__
#define __FINDDOCDLG_H__

#include "ctrls\DateSpan.h"
#include "../dlgs/findbasedlg.h"
#include "..\combo\ReasonCombo.h"
#include "..\ctrls\NumFindCtrl.h"
#include "..\combo\TTComboBox.h"
#include "..\combo\SimpleHentcombo.h"
#include "../classes/forallactions/FindAction.h"
#include "../classes/match/DocMatcher.h"

class CFindDocBaseDlg : public  CFindBaseDlg
{
public:
	CFindDocBaseDlg (const CString& sCaption);
	enum { IDD = IDD_FINDDOC };
	int GoSearch(CFindSetup& setup);
protected:
	CDateSpan m_docdatespan;
	CDateSpan m_loaddatespan;

	BOOL      m_bByLoadDate;
	BOOL	  m_bByDocDate;
	
	CString	  m_docnofrom;
	CString	  m_docnoto;
	CFindHentCombo m_dochent;
	CString     m_docplateno;
	
	CIntFindCtrl     m_amount;
	CFindReasonCombo m_reason;
	
		
protected:
	virtual void SearchDoc(CFindAction<CDocDataArray::ArrayAction,CDocDataMatcher,Doc>& findAction,CFindSetup& setup) = 0;
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	virtual void InitControls() = 0;
	
protected:
	afx_msg void OnCheckByDate();
	DECLARE_MESSAGE_MAP()
};


#endif