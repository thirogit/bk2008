#ifndef __AGENTS_H__
#define __AGENTS_H__

#include "..\imgbutton\ImageButton.h"
#include "..\ctrls\AgentListCtrl.h"
#include "../datatypes/Agent.h"

class CAgentsDlg : public  CDialog 
{
public:
	CAgentsDlg (CWnd* pParent = NULL);
	enum { IDD = IDD_LISTEDITDLG };
protected:
	CImageButton  m_OKBtn;
	CImageButton  m_EditBtn;
	CImageButton  m_AddBtn;
	CImageButton  m_DeleteBtn;
	CAgentListCtrl  m_AgentList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	
protected:
	afx_msg void OnAddAgent();
	afx_msg void OnDelAgent();
	afx_msg void OnEditAgent();
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
};
#endif