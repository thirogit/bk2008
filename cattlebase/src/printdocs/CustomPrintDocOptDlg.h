#ifndef __CUSTOMPRINTDOCOPTDLG_H__
#define __CUSTOMPRINTDOCOPTDLG_H__


class CCustomPrintDocOptDlg : public  CDialog 
{
public:
	CCustomPrintDocOptDlg(UINT nIDTemplate,CWnd *pParentWnd = NULL);
	virtual UINT GetTemplateID()  = 0;
	void SetCfgItemID(UINT nID);
	UINT GetCfgItemID() const;
	virtual bool Validate();
	virtual void Save() = 0;

protected:
	UINT m_nCfgID;
};


#endif
