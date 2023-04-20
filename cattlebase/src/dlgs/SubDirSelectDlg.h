#ifndef  __SUBDIRSELECT_H__
#define __SUBDIRSELECT_H__

#include "../ctrls/BaseListCtrl.h"




class CSubDirList : public CBaseListCtrl
{
public:
	CSubDirList(CStringArray &subdirList);
	virtual void UpdateItemCount();
protected:
	
	virtual void GetItemDispText(int nItem,int nSubItem,char *pszItemText);

	CStringArray &m_array;

	const static GRIDHEADERS m_SubDirListCols[];



};


class CSubDirSelect : public CDialog
{
	DECLARE_DYNAMIC(CSubDirSelect)

public:
	CSubDirSelect(const CString &directoryWild,CWnd* pParent = NULL); 
	virtual ~CSubDirSelect();
	enum { IDD = IDD_LISTSELECTDLG };

	LPCSTR GetSelectedSubDirPath();
	void SetCaption(UINT nCaption);
	void SetCaption(const CString &sCaption);

protected:
	CString m_selectedSubDir;
	CStringArray m_subDirectories;
	CSubDirList m_subDirList;
	CString m_caption;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual BOOL OnInitDialog();
	void OnOK();
	DECLARE_MESSAGE_MAP()
};
#endif