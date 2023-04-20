#ifndef __DATADIFFLIST_H__
#define  __DATADIFFLIST_H__

#include "../ctrls/BaseListCtrl.h"
#include "../PtrFlushArray.h"
#include "DataDiffMerge.h"

typedef PtrFlushArray< CBaseDataDiffMerge > DiffMemberArray;

class CDataDiffList : public CBaseListCtrl
{
public:
	CDataDiffList(DiffMemberArray &diffList);

	void UpdateItemCount();
	enum DATADIFFLISTCOLS
	{
		COL_MEMBER_NAME = 1000,
		COL_MEMBER_TO,
		COL_MEMBER_FROM
	};
private:
    static const GRIDHEADERS DataDiffListCols[]; 
	
	DiffMemberArray &m_diffList;
	CImageList m_checkboxImageList;
	
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	virtual void PostCreate();
	virtual int GetItemImage(int iItem,int Column);
	
protected:
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP();
};

#endif