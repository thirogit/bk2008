#pragma once

#include "drawobj.h"
#include "Drawvw.h"
// CObjGroup dialog

class CObjGroup : public CDialog
{
	DECLARE_DYNAMIC(CObjGroup)

public:
	CObjGroup(CDrawView* pView,CDrawObjGroup *pObjGroup);   // standard constructor
	virtual ~CObjGroup();

// Dialog Data
	enum { IDD = IDD_OBJGROUP };
	CDrawObjGroup* GetSelectedGroup();

private:
	CListBox m_list;
	CDrawView *m_View;
    CDrawObjGroup* m_ObjGroup;
	CDrawObjGroup* m_SelGroup;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void OnOK();
	void ReloadGroupList(CDrawObjGroup *pGrp);

	afx_msg void OnGroupMgr();
    
	DECLARE_MESSAGE_MAP()
};
