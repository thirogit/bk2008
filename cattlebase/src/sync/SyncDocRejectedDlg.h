#ifndef __SYNCREJECTDDOCSDLH_H__
#define __SYNCREJECTDDOCSDLH_H__

#include "SyncHelper.h"
#include "RejectedDocsListCtrl.h" 
#include "SyncRejectedDlg.h"

#pragma once


// CSyncRejectedDocsDlg dialog

class CSyncRejectedDocsDlg : public CSyncRejectedDlg
{
	DECLARE_DYNAMIC(CSyncRejectedDocsDlg)

public:
	CSyncRejectedDocsDlg(CSyncDocDataArray &rejectedDocsArray,
		UINT nCaption,const GRIDHEADERS *ppHeaders,int nListCfgId,CWnd* pParent = NULL);   // standard constructor
	virtual ~CSyncRejectedDocsDlg();
	virtual UINT RejectedCount();
	virtual void DoRefresh();
	virtual void UpdateCount();
	
	

protected:
	CRejectedDocsListCtrl m_rejectedDocs;
	CSyncDocDataArray &m_rejectedDocsArray;
	CString FormatRejectedReasonsText(int nItemClicked);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
	afx_msg void OnRejCorrect (NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

};


class CSyncRejectedNewDocsDlg : public CSyncRejectedDocsDlg
{
public:
	CSyncRejectedNewDocsDlg(CSyncDocDataArray &rejectedDocsArray,UINT nCaption,int nListCfgId,CWnd* pParent = NULL) :
	CSyncRejectedDocsDlg(rejectedDocsArray,nCaption,RejectedNewDocHeader,nListCfgId,pParent)
	{
	}

private:
	static const GRIDHEADERS RejectedNewDocHeader[];


};

class CSyncRejectedUpdDocsDlg : public CSyncRejectedDocsDlg
{
public:
	CSyncRejectedUpdDocsDlg(CSyncDocDataArray &rejectedDocsArray,UINT nCaption,int nListCfgId,CWnd* pParent = NULL) :
	CSyncRejectedDocsDlg(rejectedDocsArray,nCaption,RejectedUpdDocHeader,nListCfgId,pParent)
	{
	}

private:
	static const GRIDHEADERS RejectedUpdDocHeader[];


};

#endif