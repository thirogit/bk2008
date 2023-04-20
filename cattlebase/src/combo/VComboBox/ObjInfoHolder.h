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

#ifndef __OBJINFOHOLDER_H__
#define __OBJINFOHOLDER_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileEnumerator.h"

#pragma warning(push, 2)
#pragma warning(disable: 4786)
#include <set>
#pragma warning(pop)

/*----------------------------------------------------------------------------*/
/* Global configs
/*----------------------------------------------------------------------------*/
#define THREADED_OBJ_ACCESS_LOCK

// See http://www.devarticles.com/c/a/Cplusplus/Multithreading-in-C/1/
#define THREADED_OBJ_IN_CRT					// safer to use in multi-threaded CRT environment

//#define FILEINFOHOLDER_NOTIFY_NEW_FILE	// to determine whether Notify(CThreadedObj::EVENT_ONPROCEED)

// If we use STL's sort, this error can sometime occur:
//	fatal error C1076: compiler limit : internal heap limit reached; use /Zm to specify a higher limit

// Somehow the qsort is faster than the sort implementation of the STL (P.J. Plauger) for VS6.0,
// so I turn off this follwing macro USE_STL_SORT.

#ifdef MSVC_NEW_VER
	#define USE_STL_SORT
#endif // MSVC_NEW_VER

/*----------------------------------------------------------------------------*/
/* class ILockableObj
/*----------------------------------------------------------------------------*/

class ILockableObj
{
public:
	ILockableObj();
	virtual ~ILockableObj();
protected:
	virtual bool IsLockable() const;
	void	Lock();
	void	UnLock();
private:
	CRITICAL_SECTION	m_cs;
	friend class CAutoLocker;
};

/*----------------------------------------------------------------------------*/
/* CAutoLocker
/*----------------------------------------------------------------------------*/
class CAutoLocker
{
public:
	CAutoLocker(ILockableObj* pObj);
	virtual ~CAutoLocker();
private:
	ILockableObj*	m_pObj;
};

#define LockableObjAccessLock(obj)		CAutoLocker	__objLock(obj)

/*----------------------------------------------------------------------------*/
/* IObserver
/*----------------------------------------------------------------------------*/

class IObserver
{
	friend class ISubject;
public:
	virtual ~IObserver();

	typedef std::set< ISubject* >	ISubjects;
public:
	virtual void	OnNotify(ISubject* pSubject, WPARAM wParam = 0, LPARAM lParam = 0) = 0;
public:
	void			Subscribe(ISubject *pSubject, bool bAttachMe = true);
	void			UnSubscribe(ISubject *pSubject, bool bDetachMe = true);
	void			UnSubscribeAll();
private:
	ISubjects		m_Subjects;
	ILockableObj	m_Lock;
};

/*----------------------------------------------------------------------------*/
/* ISubject
/*----------------------------------------------------------------------------*/

class ISubject
{
	friend class IObserver;
public:
	ISubject();
	virtual ~ISubject();

	typedef std::set<IObserver*>	IObservers;
public:
	void			Attach(IObserver *pObserver, bool bSubscribeMe = true);
	void			Detach(IObserver *pObserver, bool bUnSubscribeMe = true);
	void			DttachAll();
	void			Notify(WPARAM wParam = 0, LPARAM lParam = 0);

	inline WPARAM	GetLastWParam() const	{ return m_wParam; }
	inline LPARAM	GetLastLParam() const	{ return m_lParam; }
private:
	IObservers		m_Observers;
	WPARAM			m_wParam;
	LPARAM			m_lParam;
	ILockableObj	m_Lock;
};

/*----------------------------------------------------------------------------*/
/* class CThreadedObj
/*----------------------------------------------------------------------------*/

class CThreadedObj : public ISubject
	#ifdef THREADED_OBJ_ACCESS_LOCK
		 , public ILockableObj
	#endif // THREADED_OBJ_ACCESS_LOCK
{
public:
	// Some messages are sent when the thread is not active, e.g. right before the thread is started,
	// they can be useful when *more* than 1 Observers that are interested in this Subject.
	// In practice, those Objservers can be different UI window that need to do some action to reflect
	// the change, and that action normally would rely on SendMessage(), which may causes dead lock if 
	// those messages are sent via the thread while the UI is waiting the thread.
	enum ThreadEventWPARAM
	{
		EVENT_PRESTART,		// before the thread is started

		EVENT_ONSTART,		// the thread is started
		EVENT_ONFINISH,		// the thread finishes normally
		EVENT_ONFORCESTOP,	// the thread is stopped on purpose
		EVENT_ONPROCEED,	// the thread is in progress, not really useful

		EVENT_TERMINATED,	// after the thread is terminated

		EVENT_CUSTOM_FIRST,	// custom event
	};
public:
	CThreadedObj();
	virtual ~CThreadedObj();
public:
	virtual bool Run(PVOID pParam = NULL)			{ return false; };
	DWORD		Suspend();
	DWORD		Resume();
public:
	bool		IsRunning() const;		// return true when the thread is still active

	// IsRunning() simply check to see if the thread is still in active status,
	// but since stopping a thread might be not that fast as we expect, this can help
	// the caller (normally in a GUI thread) response faster.
	bool		IsTaskStopped();

	bool		RunInThread(PVOID pParam = NULL, HANDLE hStopEvent = NULL);
	bool		SetStopEventHandle(HANDLE hStopEvent);
	bool		SetStopEvent();
	bool		ForceStopThread(DWORD dwTimeOutMilliseconds = INFINITE);

#ifdef _DEBUG
	inline DWORD		UpdateTimeBegin()			{ return m_dwDebugStartTime = GetTickCount(); }
	inline DWORD		GetTimeElapsed()			{ return m_dwDebugTimeElapsed; }
	inline DWORD		UpdateTimeElapsed()			{ return m_dwDebugTimeElapsed = (-1 == m_dwDebugStartTime ? 0 : GetTickCount() - m_dwDebugStartTime); }
#endif // _DEBUG
protected:
	inline bool			IsStopEventSignaled()		{ return m_hStopEvent && ::WaitForSingleObject(m_hStopEvent, 0) == WAIT_OBJECT_0; }
	static UINT WINAPI	ThreadProc(PVOID pThreadProcParam);
	bool				_Run(PVOID pParam = NULL);
protected:
	HANDLE					m_hThread;
	HANDLE					m_hStopEvent;
	
	typedef struct ThreadProcParam 
	{
		CThreadedObj*	pThreadedObj;
		PVOID			pThreadParameter;
	}ThreadProcParam, *PThreadProcParam;

	ThreadProcParam			m_ThreadProcParam;

#ifdef _DEBUG
	DWORD					m_dwDebugStartTime;
	DWORD					m_dwDebugTimeElapsed;
#endif // _DEBUG
};

/*----------------------------------------------------------------------------*/
/* class CObjInfoHolder
/*----------------------------------------------------------------------------*/
/*
	A "Object Info Holder" is a class that used to load (normally in a separate thread) and keep
	various info of specific type.
*/

class CObjInfoHolder : public CThreadedObj
{
public:
	CObjInfoHolder();
	virtual ~CObjInfoHolder();
public:
	virtual BOOL	RetrieveObjInfo() = 0;	// Refresh
	virtual BOOL	StopRetrievingObjInfo();
public:
	virtual size_t	GetObjInfoCount() const = 0;
	virtual bool	IsSortable() const			{ return false; }
	virtual LPCTSTR	GetObjInfoText(WPARAM wParam, LPARAM lParam) = 0;
protected:
	virtual bool	Run(PVOID pParam = NULL);
};

#ifdef THREADED_OBJ_ACCESS_LOCK
	#define ObjInfoHolderAccessLock(pObjInfoHolder)		LockableObjAccessLock(pObjInfoHolder)
#else
	#define ObjInfoHolderAccessLock(pObjInfoHolder)
#endif // THREADED_OBJ_ACCESS_LOCK

/*----------------------------------------------------------------------------*/
/* class CMultiColInfoHolder
/*----------------------------------------------------------------------------*/
#define DEFAULT_SORT_ASCENDING	true

typedef int (__cdecl *GENERICCOMPAREFN)(const void * elem1, const void * elem2);

class CMultiColInfoHolder : public CObjInfoHolder
{
public:
	CMultiColInfoHolder();
	virtual ~CMultiColInfoHolder();
	enum {
		INFOCOL_INVALID	= -1,
		INFOCOL_FIRST,
	};
	enum {
		EVENT_SORT_BEGIN = EVENT_CUSTOM_FIRST,
		EVENT_SORT_END,
	};
public:
	virtual size_t	GetObjInfoColumnCount() const = 0;
public:
	virtual void	Sort(int infoType, bool bAscending = true) = 0;
	
	virtual void	SwitchSort(int infoType);

	inline int		GetSortColumn() const	{ return m_nSortColumn; }
	inline bool		IsSortAscending() const	{ return m_bSortAscending; }
protected:
	int				m_nSortColumn;
	bool			m_bSortAscending;
};

/*----------------------------------------------------------------------------*/
/* CObjInfoMatcher
/*----------------------------------------------------------------------------*/
/*
	In practice, we want to "filter out" some info for searching, that's the why we need this class
	In the initial version of design, I simply make it derive from CObjInfoHolder, but then I realize I have to make it a new class.

	About why not make CObjInfoMatcher derive from CObjInfoHolder:
	1) CObjInfoHolder is designed to simply "loads" and "holds" the info only, it does not care about the other thing like "filtering"
	2) Decoupling data and algorithm. Suppose we have 1 instance of CObjInfoHolder but we want to display it on multiple place (such 
	as two different controls) and also we want to have them work independently, then we will need this CObjInfoMatcher.
	Example: We have 1 CFileInfoHolder that keeps the file list and we want to display it on two CListCtrls.
*/

typedef vector<int>	IndicesArray;

class CObjInfoMatcher
{
public:
	CObjInfoMatcher(CObjInfoHolder* pObjInfoHolder = NULL);
	virtual ~CObjInfoMatcher();

	struct PatternInfo
	{
		tstring	strPattern;
		bool	bWithMinus;
		bool	bWithBackSlash;
	};
public:
	inline void				SetObjInfoHolder(CObjInfoHolder* pObjInfoHolder)	{ m_pObjInfoHolder = pObjInfoHolder;}

	inline CObjInfoHolder*	GetObjInfoHolder() const							{ return m_pObjInfoHolder; }
public:
	// Match methods
	inline size_t	GetMatchObjInfoCount() const								{ return HasFilteredObjinfo() ? m_nMatchObjInfoCount : GetObjInfoCount(); }

	virtual void	Reset();

	virtual LPCTSTR	GetObjInfoTextInMatches(WPARAM wParam, LPARAM lParam = 0) = 0;
	// returns the matched count
	virtual size_t	GetMatch(LPCTSTR lpcszPatterns = NULL, WPARAM wParam = 0, LPARAM lParam = 0) = 0;

	// helper functions, simply call m_pObjInfoHolder's corresponding functions.
	BOOL			RetrieveObjInfo();	// Refresh
	BOOL			StopRetrievingObjInfo();

	bool			RunInThread(PVOID pParam = NULL, HANDLE hStopEvent = NULL);

	size_t			GetObjInfoCount() const;
	bool			IsSortable() const;
protected:
	virtual BOOL	SetPatterns(LPCTSTR lpcszPatterns) = 0;
	virtual bool	HasFilteredObjinfo() const									{ return false; }
protected:
	CObjInfoHolder*		m_pObjInfoHolder;
	// this object is not supposed to be used by two objects at the same time, so we do not need to lock this member.
	size_t				m_nMatchObjInfoCount;
};

/*----------------------------------------------------------------------------*/
/* class CMultiPatternMatchHelper
/*----------------------------------------------------------------------------*/

class CMultiColInfoMatcher : public CObjInfoMatcher
{
public:
	CMultiColInfoMatcher(CMultiColInfoHolder* pMultiColInfoHolder = NULL);
	virtual ~CMultiColInfoMatcher();
public:
	virtual size_t	GetMatch(LPCTSTR lpcszPatterns = NULL, WPARAM wParam = 0, LPARAM lParam = 0);

	virtual LPCTSTR	GetObjInfoTextInMatches(WPARAM wParam, LPARAM lParam = 0);
	int				GetObjInfoIndexInMatches(int nItem) const;

	virtual void	Reset();
protected:
	virtual BOOL	SetPatterns(LPCTSTR lpcszPatterns);
	virtual BOOL	IsObjInfoMatch(UINT nIndex) = 0;
	virtual void	OnAddPattern(const PatternInfo& patternInfo) = 0;

	virtual bool	HasFilteredObjinfo() const;
protected:
	IndicesArray	m_matchedObjInfoIndices;
};

/*----------------------------------------------------------------------------*/
/* CFileEnumerator
/*----------------------------------------------------------------------------*/

class CFileInfoEnumerator : public CFilteredFileEnumerator
{
public:
	CFileInfoEnumerator();
	virtual ~CFileInfoEnumerator();
public:
	enum FileInfoType
	{
		FIT_INVALID		= -1,
		FIT_FIRST,
		FIT_FILENAME	= FIT_FIRST,
		FIT_FILEPATH,
		FIT_FILEEXT,
		FIT_MODIFIED,
		FIT_FILESIZE,
		
		FIT_MAX,
	};

	struct FileInfo
	{
		tstring sFileInfo[FIT_MAX];
		LONGLONG nFileSize;	// just for sorting.
	};
public:
	size_t			GetFileCount() const	{ return m_arrFileInfo.size(); }

	const FileInfo*	GetFileInfo(size_t nIndex);
protected:
	virtual void	HandleFile(LPCTSTR lpcszPath, const FindFileData& ffd);
protected:
	typedef std::vector<FileInfo>	FileInfoArray;
	FileInfoArray	m_arrFileInfo;
};

/*----------------------------------------------------------------------------*/
/* CFileInfoHolder
/*----------------------------------------------------------------------------*/
struct EnumerateFolder
{
	EnumerateFolder(LPCTSTR lpcszFolder = NULL, LPCTSTR lpcszFilterPatterns = NULL)
		: strFolder(lpcszFolder)
		, strFilterPatterns(lpcszFilterPatterns)
	{
	}
	tstring strFolder;
	tstring strFilterPatterns;
};

typedef vector<EnumerateFolder>		EnumerateFolderArray;

class CFileInfoHolder : public CFileInfoEnumerator, public CMultiColInfoHolder
{
public:
	CFileInfoHolder(const EnumerateFolderArray* pEnumerateFolders = NULL);
	virtual ~CFileInfoHolder();
public:
	virtual size_t	GetObjInfoColumnCount() const		{ return CFileInfoEnumerator::FIT_MAX; }

	virtual size_t	GetObjInfoCount() const				{ return CFileInfoEnumerator::GetFileCount(); }

	virtual LPCTSTR	GetObjInfoText(WPARAM wParam, LPARAM lParam);

	virtual BOOL	RetrieveObjInfo();

	virtual bool	IsSortable() const					{ return true; }
	virtual void	Sort(int infoType, bool bAscending = true);

	void			SetEnumerateFolderArray(const EnumerateFolderArray* pEnumerateFolders);
protected:
#ifndef USE_STL_SORT
	static bool s_bSortAscending;
	static int s_nSortInfoType;
	static int CompareFileInfo(const FileInfo* left, const FileInfo* right);
#endif // USE_STL_SORT
	virtual void	HandleFile(LPCTSTR lpcszPath, const FindFileData& ffd);
protected:
	const EnumerateFolderArray* m_pEnumerateFolders;
};

/*----------------------------------------------------------------------------*/
/* CFileInfoMatcher
/*----------------------------------------------------------------------------*/

class CFileInfoMatcher : public CMultiColInfoMatcher
{
public:
	CFileInfoMatcher();
	virtual ~CFileInfoMatcher();
protected:
	BOOL IsObjInfoMatch(UINT nIndex);
	
	BOOL SetPatterns( LPCTSTR lpcszPatterns );
	
	void OnAddPattern(const PatternInfo& patternInfo);
protected:
	struct FilePattern 
	{
		FilePattern(const CObjInfoMatcher::PatternInfo& patternInfo)
			: strPattern(patternInfo.strPattern)
			, bExclude(patternInfo.bWithMinus)
			, bMatchPath(patternInfo.bWithBackSlash)
		{
			;
		}
		std::string strPattern;
		bool		bExclude;
		bool		bMatchPath;
	};
	typedef std::vector<FilePattern>	FilePatternArray;
	FilePatternArray	m_arrFilePatterns;
};

/*----------------------------------------------------------------------------*/
/* CComboFileInfoMatcher
/*----------------------------------------------------------------------------*/
class CComboFileInfoMatcher : public CFileInfoMatcher
{
protected:
	BOOL IsObjInfoMatch(UINT nIndex);
};

#else
	#error ### repeated include of ObjInfoHolder.h!!!
#endif // __OBJINFOHOLDER_H__