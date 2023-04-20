#ifndef __PASSIMGVIEWDLG_H__
#define __PASSIMGVIEWDLG_H__

#include "PassImage.h"
#include "DataTypes.h"
// CPassImgViewDlg dialog

class CPassImgViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPassImgViewDlg)

public:
	//CPassImgViewDlg(fipWinImageEx &pix,CString &wndCaption,CWnd* pParent = NULL);   // standard constructor
	CPassImgViewDlg(CCowArray &cowArray,int iIndex,CWnd* pParent = NULL);

	virtual ~CPassImgViewDlg();

// Dialog Data
	enum { IDD = IDD_PASSIMGVIEWDLG };
protected:
	CPassImage m_passView;
	fipWinImageEx m_passPix;
	CCowArray &m_cowArray;
	int m_iIndex;

	virtual BOOL OnInitDialog();
	void UpdatePassImage();
	virtual bool GetPassImg(Cow *pCow);
	void EnableNext(BOOL bEnable);
	void EnablePrev(BOOL bEnable);
	virtual bool PassImgExists(Cow *pCow);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg BOOL PreTranslateMessage (MSG* pMsg);
	afx_msg void OnNext();
	afx_msg void OnPrev();

	afx_msg void OnNextNext();
	afx_msg void OnPrevPrev();
	DECLARE_MESSAGE_MAP()
};
#endif