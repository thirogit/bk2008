#ifndef __PASSPORTSEGREGATIONDLG_H__
#define __PASSPORTSEGREGATIONDLG_H__

#include "../datatypes/DocDataArray.h"


class CPassportSegregationDlg : public CDialog
{
	DECLARE_DYNAMIC(CPassportSegregationDlg)

public:
	CPassportSegregationDlg(CDocDataArray& docs,CWnd* pParent = NULL);   
	virtual ~CPassportSegregationDlg();	
	enum { IDD = IDD_PSSPORT_SEGREGATION };

private:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	


	DECLARE_MESSAGE_MAP()

};

#endif