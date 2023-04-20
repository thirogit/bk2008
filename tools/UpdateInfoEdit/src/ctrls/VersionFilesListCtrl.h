#ifndef __VERSIONFILESLISTCTRL_H__
#define __VERSIONFILESLISTCTRL_H__

#include <ctrls\BaseListCtrl.h>

class CVersionFilesListCtrl : public CBaseListCtrl
{
public:
	CVersionFilesListCtrl(CStringArray &filesArray); 
	void UpdateItemCount();
protected:
	CStringArray &m_filesArray;
	
	static const GRIDHEADERS szColHeaders[];

	void SortByCol(int Column,bool bAsc);
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);

};




#endif