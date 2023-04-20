#ifndef __PASSDBMNGMNTOPT_H__
#define __PASSDBMNGMNTOPT_H__

#include <afxeditbrowsectrl.h>
#include "../ctrls/XColorStatic.h"
#include "DataTypes.h"
#include "../ctrls/BaseListCtrl.h"
#include "../PassportStorage/PassImgBase.h"


class CMountedFilesList : public CBaseListCtrl
{
public:
	enum COLUMNS
	{
		COL_STATUS = 1,
		COL_DOMOUNT,
		COL_FILEPATH
	};

	CMountedFilesList(CMountedFilesArray& mountedFiles);
	void UpdateItemCount();
private:
	void GetItemDispText(int nItem,int nSubItem,char *pszItemText);
	bool GetCellColor(int nItem,int nSubItem, COLORREF *cellBkg);

	CMountedFilesArray & m_mountedFiles;

	static const GRIDHEADERS m_mountedPassListCols[];
};


class CPassDBMngmntOpt : public CPropertyPage
{
	DECLARE_DYNAMIC(CPassDBMngmntOpt)

public:
	CPassDBMngmntOpt();
	virtual ~CPassDBMngmntOpt();
	static bool MovePassDBFile(const CString & passMoveDir);

// Dialog Data
	enum { IDD = IDD_PASSIMGDBMNGMNT };
protected:
	CMFCEditBrowseCtrl   m_movePassFileDir;
	CComboBox			m_fileSizes;

	BOOL m_bAskBeforePassFileMove;
	CXColorStatic m_warnLabel;
	CString m_passMoveDir;
	CMountedFilesList m_mountedList;
	CMountedFilesArray m_mountedFiles;
	CMenu m_mountlistMenu;





protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	BOOL OnApply();
	
protected:
	afx_msg void OnMount();
	afx_msg void OnUnmount();
	afx_msg void OnChangePath();
	afx_msg void OnBnClickedMoveNow();
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};
#endif