#ifndef __AGENTDLG_H__
#define __AGENTDLG_H__

#include "../datatypes/Agent.h"

class CAgentDlg : public CDialog
{
public:
	enum { IDD = IDD_AGENT };
	CAgentDlg(CWnd* pParent = NULL);
			
protected:
	
	CString m_AgentName;
	CString m_AgentCode;
	CString m_AgentPlateNo;
	
	virtual bool	CommitAgent() = 0;
	virtual CString GetWindowCaption() = 0;
	virtual void InitDialogControls();
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog() ;
	virtual void OnOK();
	virtual bool ValidateFields();	

};

#endif