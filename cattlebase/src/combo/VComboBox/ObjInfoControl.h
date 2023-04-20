/* 
 * Kenny Liu
 * http://www.codeproject.com/Members/yonken
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 */

#ifndef __OBJINFOCONTROL_H__
#define __OBJINFOCONTROL_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjInfoHolder.h"
#include "CustomDrawControl.h"
#include "VComboBox.h"

/*----------------------------------------------------------------------------*/
/* Global control
/*----------------------------------------------------------------------------*/

#define RETRIEVE_OBJINFO_IN_NEWTHREAD	// Create a separate thread to do the job

#define UPDATE_OBJINFO_VIA_TIMER		// Update the content of the list via the timer

#ifdef UPDATE_OBJINFO_VIA_TIMER
	#define UPDATE_INFOLIST_TIMER_ID		1
	#define UPDATE_INFOLIST_TIMER_ELAPSE	600
#endif // UPDATE_OBJINFO_VIA_TIMER

#define DERIVE_FROM_VCOMBO				// To switch the base class of CObjInfoComboBox and CObjInfoListBox

#pragma warning(push, 2)
#pragma warning(disable: 4786)

#include <map>	// For mapping the file extensions and the related image index inside CImageList
typedef std::map<tstring, int>	FileIconMap;

#pragma warning(pop)

/*----------------------------------------------------------------------------*/
/* IIconMapper
/*----------------------------------------------------------------------------*/
class IIconMapper : public ILockableObj
{
public:
	IIconMapper(int cx = 16, int cy = 16);
	virtual ~IIconMapper();
public:
	CImageList&	GetImageList()	{ return m_ImageList; }

	// return -1 means not found
	virtual int GetIconIndex(CObjInfoMatcher* pMatcher, WPARAM wParam, LPARAM lParam = 0) = 0;
protected:
	CImageList	m_ImageList;
};

/*----------------------------------------------------------------------------*/
/* CAsciiProgressGenerator
/*----------------------------------------------------------------------------*/
class CAsciiProgressGenerator
{
public:
	CAsciiProgressGenerator(DWORD dwMinTimeLimit = 600);
public:
	CString	GetProressAscii();
protected:
	UINT	m_nProgress;
	DWORD	m_dwMinTimeLimit;
	DWORD	m_dwCurrentTime;
};


/*----------------------------------------------------------------------------*/
/* CObjInfoListCtrl
/*----------------------------------------------------------------------------*/

#define WM_OBJINFO_THREADEVENT			(WM_USER+0x400)

/*
	An implementation of virtual list-control
*/

#define WM_OBJINFOLIST_UPDATE			(WM_USER+0x500)

#ifndef CObjInfoListCtrlBase
	#define CObjInfoListCtrlBase	CCustomDrawListCtrl
#endif // CObjInfoListCtrlBase

class CObjInfoListCtrl : public CObjInfoListCtrlBase, public IObserver
{
	DECLARE_DYNCREATE(CObjInfoListCtrl)
public:
	CObjInfoListCtrl();
	virtual ~CObjInfoListCtrl();
public:
	virtual void SetObjInfoMatcher(CObjInfoMatcher* pMatcher, BOOL bSubscribeThreadEvent = TRUE);
	inline CObjInfoMatcher* GetObjInfoMatcher() const								{ return m_pObjInfoMatcher; }
	inline CMultiColInfoHolder* GetObjInfoHolder() const
	{
		return m_pObjInfoMatcher ? (CMultiColInfoHolder*)m_pObjInfoMatcher->GetObjInfoHolder() : NULL;
	}

	virtual void			SetIconMapper(IIconMapper* pIconMapper)					{ m_pIconMapper = pIconMapper; }
	virtual IIconMapper*	GetIconMapper() const									{ return m_pIconMapper; }
public:
	virtual BOOL RefreshObjInfoList();

	virtual void AutoResizeColumn();
	virtual int GetItemIconIndex(int nItem);

#ifdef UPDATE_OBJINFO_VIA_TIMER
	void		SetUpdateListByTimer(BOOL bSet = TRUE)								{ m_bUpdateListByTimer = bSet; }
	void		OnTimer(UINT nIDEvent);
#endif // UPDATE_OBJINFO_VIA_TIMER

	BOOL		LoadColumnLabels(UINT nResID);
protected:
	virtual int	GetFindObjInfoListColumnIndex() const								{ return 0; }
	virtual void OnInitList();
	virtual void OnDestroyList();

	virtual void OnUpdateObjInfoList(bool bAutoResizeColumns = true);

	virtual bool IsSortable() const;
	virtual BOOL OnSort(int nCol);
protected:
	afx_msg void OnGetObjListDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOdFindObjListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

#ifdef UPDATE_OBJINFO_VIA_TIMER
	BOOL			m_bUpdateListByTimer;
	UINT_PTR		m_nTimer;
#endif // UPDATE_OBJINFO_VIA_TIMER

	LRESULT			OnObjInfoThreadEvent(WPARAM wParam, LPARAM lParam);
	virtual void	OnNotify(ISubject* pSubject, WPARAM wParam = 0, LPARAM lParam = 0);
protected:
	CObjInfoMatcher*	m_pObjInfoMatcher;
	CStringArray		m_arrColumnLabels;
	IIconMapper*		m_pIconMapper;
protected:
	DECLARE_MESSAGE_MAP()
};

/*----------------------------------------------------------------------------*/
/* CFileIconMapper
/*----------------------------------------------------------------------------*/
class CFileIconMapper : public IIconMapper
{
public:
	CFileIconMapper(int cx = 16, int cy = 16);
	virtual ~CFileIconMapper();
public:
	int GetFileIconIndex(const CFileInfoEnumerator::FileInfo* pFileInfo);
	int GetFileIconIndex(LPCTSTR lpcszFilePath, LPCTSTR lpcszFileExt);
	int GetFileIconIndex(LPCTSTR lpcszFilePath);
protected:
	virtual int GetIconIndex(CObjInfoMatcher* pMatcher, WPARAM wParam, LPARAM lParam = 0);
protected:
	FileIconMap		m_FileIconIndexMap;
};

/*----------------------------------------------------------------------------*/
/* CFileInfoListCtrl
/*----------------------------------------------------------------------------*/

#ifndef CFileInfoListCtrlBase
	#define CFileInfoListCtrlBase	CObjInfoListCtrl
#endif // CFileInfoListCtrlBase

class CFileInfoListCtrl : public CFileInfoListCtrlBase
{
	DECLARE_DYNCREATE(CFileInfoListCtrl)
public:
	CFileInfoListCtrl();
	virtual ~CFileInfoListCtrl();
public:
	virtual IIconMapper*	GetIconMapper() const;
protected:
	LRESULT				OnObjInfoThreadEvent(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
};

/*----------------------------------------------------------------------------*/
/* CObjInfoListBox
/*----------------------------------------------------------------------------*/

#ifndef CObjInfoListBoxBase
	#ifdef DERIVE_FROM_VCOMBO
		#define CObjInfoListBoxBase	CVComboListBox
	#else
		#define CObjInfoListBoxBase	CComboListBox
	#endif // DERIVE_FROM_VCOMBO
#endif // CObjInfoListBoxBase

class CObjInfoListBox : public CObjInfoListBoxBase, public IObserver
{
	DECLARE_DYNCREATE(CObjInfoListBox)
public:
	CObjInfoListBox();
	virtual ~CObjInfoListBox();
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL CreateFromCtrl(CWnd* pParent, int nID, DWORD dwStyleAdd = 0);
public:
	virtual CString GetItemText(UINT nItem);

	virtual void SetObjInfoMatcher(CObjInfoMatcher* pMatcher, BOOL bSubscribeThreadEvent = TRUE);
	
	inline CObjInfoMatcher* GetObjInfoMatcher() const							{ return m_pObjInfoMatcher; }
	
	inline CMultiColInfoHolder* GetObjInfoHolder() const
	{
		return m_pObjInfoMatcher ? (CMultiColInfoHolder*)m_pObjInfoMatcher->GetObjInfoHolder() : NULL;
	}

	virtual void			SetIconMapper(IIconMapper* pIconMapper)				{ m_pIconMapper = pIconMapper; }
	virtual IIconMapper*	GetIconMapper() const								{ return m_pIconMapper; }

	virtual int			GetItemIconIndex(UINT nItem);

	virtual WPARAM		GetObjInfoTextInMatchWPARAM(UINT nItem)					{ return nItem; }
	virtual LPARAM		GetObjInfoTextInMatchLPARAM(UINT nItem)					{ return 0; }

#ifdef UPDATE_OBJINFO_VIA_TIMER
	void				SetUpdateListByTimer(BOOL bSet = TRUE)					{ m_bUpdateListByTimer = bSet; }
	void				OnTimer(UINT nIDEvent);
#endif // UPDATE_OBJINFO_VIA_TIMER

	virtual void	AutoFitContentHScroll();

	virtual BOOL	RefreshObjInfoList();
protected:
	virtual void	OnInitListBox();
	virtual void	OnDestroyListBox();
	void			OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	LRESULT			OnObjInfoThreadEvent(WPARAM wParam, LPARAM lParam);
	virtual void	OnNotify(ISubject* pSubject, WPARAM wParam = 0, LPARAM lParam = 0);

	virtual void	OnUpdateObjInfoList();
protected:
#ifdef UPDATE_OBJINFO_VIA_TIMER
	BOOL				m_bUpdateListByTimer;
	UINT_PTR			m_nTimer;
#endif // UPDATE_OBJINFO_VIA_TIMER
	CFont				m_font;
	CObjInfoMatcher*	m_pObjInfoMatcher;
	IIconMapper*		m_pIconMapper;
protected:
	DECLARE_MESSAGE_MAP()
};

/*----------------------------------------------------------------------------*/
/* CFileInfoListBox
/*----------------------------------------------------------------------------*/

#ifndef CFileInfoListBoxBase
	#define CFileInfoListBoxBase	CObjInfoListBox
#endif // CFileInfoListBoxBase

class CFileInfoListBox : public CFileInfoListBoxBase
{
	DECLARE_DYNCREATE(CFileInfoListBox)
public:
	CFileInfoListBox();
	virtual ~CFileInfoListBox();
public:
	virtual IIconMapper*	GetIconMapper() const;
protected:
	virtual LPARAM		GetObjInfoTextInMatchLPARAM(UINT nItem)		{ return CFileInfoEnumerator::FIT_FILEPATH; }
	LRESULT				OnObjInfoThreadEvent(WPARAM wParam, LPARAM lParam);
protected:
	DECLARE_MESSAGE_MAP()
};

/*----------------------------------------------------------------------------*/
/* CObjInfoComboBox
/*----------------------------------------------------------------------------*/

#ifndef CObjInfoComboBoxBase
	#ifdef DERIVE_FROM_VCOMBO
		#define CObjInfoComboBoxBase	CVComboBox
	#else
		#define CObjInfoComboBoxBase	CCustomDrawComboBox
	#endif // DERIVE_FROM_VCOMBO
#endif // CObjInfoComboBoxBase

class CObjInfoComboBox : public CObjInfoComboBoxBase, public IObserver
{
	DECLARE_DYNCREATE(CObjInfoComboBox)
public:
	CObjInfoComboBox();
	virtual ~CObjInfoComboBox();
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	virtual void SetObjInfoMatcher(CObjInfoMatcher* pMatcher, BOOL bSubscribeThreadEvent = TRUE);

	inline CObjInfoMatcher* GetObjInfoMatcher() const							{ return m_pObjInfoMatcher; }

	inline CMultiColInfoHolder* GetObjInfoHolder() const
	{
		return m_pObjInfoMatcher ? (CMultiColInfoHolder*)m_pObjInfoMatcher->GetObjInfoHolder() : NULL;
	}

	virtual void			SetIconMapper(IIconMapper* pIconMapper);
	virtual IIconMapper*	GetIconMapper() const								{ return m_pIconMapper; }

	virtual int				GetItemIconIndex(UINT nItem);

	virtual CComboListBox& GetComboListBox()
	{
		if ( GetObjInfoListBox() )
			return *GetObjInfoListBox();
		return CObjInfoComboBoxBase::GetComboListBox();
	}

	virtual CObjInfoListBox* GetObjInfoListBox()								{ return NULL; }

	virtual CString GetItemText(UINT nItem);

	virtual size_t	GetMatchItemCount(LPCTSTR lpcszPattern, PINT pSelItem = NULL);
	
	virtual WPARAM	GetObjInfoTextInMatchWPARAM(UINT nItem)						{ return nItem; }
	virtual LPARAM	GetObjInfoTextInMatchLPARAM(UINT nItem)						{ return 0; }

	BOOL			IsAutoCompleteEnabled() const								{ return m_bAutoComplete; }
	void			EnableAutoComplete(BOOL bEnable = TRUE)						{ m_bAutoComplete = bEnable; }

	virtual BOOL	RefreshObjInfoList();

#ifdef UPDATE_OBJINFO_VIA_TIMER
	void			SetUpdateListByTimer(BOOL bSet = TRUE)						{ m_bUpdateListByTimer = bSet; }
	void			OnTimer(UINT nIDEvent);
#endif // UPDATE_OBJINFO_VIA_TIMER
protected:
	virtual void	OnUpdateObjInfoList();
	LRESULT			OnObjInfoThreadEvent(WPARAM wParam, LPARAM lParam);
	virtual void	OnNotify(ISubject* pSubject, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual BOOL	OnInitCombo();
	virtual void	OnDestroyCombo();

	void			OnEditDispChange();
protected:
	// For auto complete
	void		OnDropDown();
	LRESULT		OnSetCurSel(WPARAM nIndex, LPARAM);
	void		OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );

#ifndef DERIVE_FROM_VCOMBO
	void		OnEditUpdate();
#endif // DERIVE_FROM_VCOMBO
	
	void		OnSelChange();
	void		OnSelEndOK();
protected:
	CObjInfoMatcher*	m_pObjInfoMatcher;
	IIconMapper*		m_pIconMapper;
	BOOL				m_bAutoComplete;
	BOOL				m_bFilterListOnDropDown;

#ifdef UPDATE_OBJINFO_VIA_TIMER
	BOOL				m_bUpdateListByTimer;
	UINT_PTR			m_nTimer;
#endif // UPDATE_OBJINFO_VIA_TIMER
protected:
	DECLARE_MESSAGE_MAP()
};

/*----------------------------------------------------------------------------*/
/* CFileInfoComboBox
/*----------------------------------------------------------------------------*/

#ifndef CFileInfoComboBoxBase
	#define CFileInfoComboBoxBase	CObjInfoComboBox
#endif // CFileInfoComboBoxBase

class CFileInfoComboBox : public CFileInfoComboBoxBase
{
	DECLARE_DYNCREATE(CFileInfoComboBox)
public:
	CFileInfoComboBox();
	virtual ~CFileInfoComboBox();
public:
	virtual IIconMapper*		GetIconMapper() const;

	virtual CFileInfoListBox&	GetFileInfoListBox()									{ return m_FileInfoListBox; }
protected:
	virtual LPARAM				GetObjInfoTextInMatchLPARAM(UINT nItem)					{ return CFileInfoEnumerator::FIT_FILEPATH; }
	LRESULT						OnObjInfoThreadEvent(WPARAM wParam, LPARAM lParam);

	virtual CObjInfoListBox*	GetObjInfoListBox();
protected:
	CFileInfoListBox	m_FileInfoListBox;
protected:
	DECLARE_MESSAGE_MAP()
};

/*----------------------------------------------------------------------------*/
/* IObjInfoController
/*----------------------------------------------------------------------------*/
class IObjInfoController
{
public:
	virtual CWnd&				GetControl()		= 0;
	virtual CObjInfoHolder&		GetInfoHolder()		= 0;
	virtual CObjInfoMatcher&	GetMatcher()		= 0;
};

template <typename TCtrl, typename TInfoHolder, typename TMatcher>
class CObjInfoController : public IObjInfoController
{
public:
	CObjInfoController(BOOL bSubscribeThreadEvent = TRUE)
	{
		Matcher().SetObjInfoHolder( &InfoHolder() );
		Control().SetObjInfoMatcher( &Matcher(), bSubscribeThreadEvent );
	}
public:
	TCtrl&				Control()			{ return m_ctrl; }
	TInfoHolder&		InfoHolder()		{ return m_infoHolder; }
	TMatcher&			Matcher()			{ return m_matcher; }
protected:
	CWnd&				GetControl()		{ return m_ctrl; }
	CObjInfoHolder&		GetInfoHolder()		{ return m_infoHolder; }
	CObjInfoMatcher&	GetMatcher()		{ return m_matcher; }
protected:
	TCtrl			m_ctrl;
	TInfoHolder		m_infoHolder;
	TMatcher		m_matcher;
};

#else
	#error ### repeated include of ObjInfoControl.h!!!
#endif // __OBJINFOCONTROL_H__