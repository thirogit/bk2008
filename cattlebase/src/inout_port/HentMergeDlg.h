#ifndef __HENTMERGEDLG_H__
#define __HENTMERGEDLG_H__

#include "MergeDlg.h"
#include "../combo/SimpleHentCombo.h"

class CHentMergeDlg : public CMergeDlg
{
	DECLARE_DYNAMIC(CHentMergeDlg)

public:
	CHentMergeDlg(CWnd* pParent = NULL);  
	virtual ~CHentMergeDlg();	

	Hent* GetSelectedHent(); 
private:

	CSimpleHentCombo m_hents;
	Hent *m_selectedHent;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	void OnOK();

	DECLARE_MESSAGE_MAP()
};

#endif