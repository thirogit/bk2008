#ifndef __REPOTOOLBAR_H__
#define __REPOTOOLBAR_H__

#include <boost/function.hpp>
#include "../../../PtrFlushArray.h"

class ToolbarButton
{
public:
	ToolbarButton(boost::function<void()> &onClick,HBITMAP hBtnImg,bool bDeleteImage);
	~ToolbarButton();
	void OnClick();
	HBITMAP GetBtnImage() const;
	bool GetDeleteImage() const;
private:
	boost::function<void()> m_onClick;
	HBITMAP m_hBtnImage;
	bool m_bDeleteImage;
};

class CallbackToolBar
{
public:
	virtual void AddButton(boost::function<void()> &onClick,HBITMAP hBtnImg,bool bDeleteImage = true) = 0;
	virtual void RemoveAllButtons() = 0;
	virtual void RemoveAt(int iBtnIndex) = 0;
	virtual int GetButtonCount() = 0;
	virtual ToolbarButton* GetButtonAt(int iBtnIndex) = 0;
};

class CRepoToolBar : public CStatic, public CallbackToolBar
{
public:
	CRepoToolBar();
	virtual ~CRepoToolBar();
	virtual void AddButton(boost::function<void()> &onClick,HBITMAP hBtnImg,bool bDeleteImage = true);
	virtual void RemoveAllButtons();
	virtual void RemoveAt(int iBtnIndex);
	virtual int GetButtonCount();
	virtual void EnableButton(int iBtnIndex,BOOL bEnable);
	virtual ToolbarButton* GetButtonAt(int iBtnIndex);
	BOOL Create(CWnd* pParentWnd,const RECT& rect,UINT nID);
protected:
	void AddButtonNoReToolBar(boost::function<void()> &onClick,HBITMAP hBtnImg,bool bDeleteImage);
	virtual void PreSubclassWindow();
	void	ReButtonToolBar();
	BOOL    CreateToolBar();
	
private:
	CToolBar m_ToolBar;
	CImageList	m_btnsImgList;
	PtrFlushArray< ToolbarButton > m_Buttons;
	BOOL m_bDynCreate;
protected:
	afx_msg void OnBtnClick(UINT btnId);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

};
#endif