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

#include "StdAfx.h"
#include "ObjInfoHolder.h"

#ifdef THREADED_OBJ_IN_CRT
#include <process.h>    /* _beginthreadex, _endthreadex */
#endif // CREATE_THREAD_CRT

// For StrStrI and StrFormatByteSize
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#define stristr		StrStrI

#include <algorithm>	// for sorting

/*----------------------------------------------------------------------------*/
/* IObserver
/*----------------------------------------------------------------------------*/
IObserver::~IObserver()
{
	LockableObjAccessLock(&m_Lock);
	UnSubscribeAll();
}

void IObserver::Subscribe( ISubject *pSubject, bool bAttachMe /*= true*/ )
{
	LockableObjAccessLock(&m_Lock);
	ASSERT(pSubject);
	ISubjects::iterator iter = m_Subjects.find(pSubject);
	if ( iter == m_Subjects.end() )	// not found
	{
		//TRACE("+++++++++++ Subscribe %x\n", this);
		m_Subjects.insert(pSubject);
		if ( bAttachMe )
			pSubject->Attach(this, false);	// we have subscribed it
	}
}

void IObserver::UnSubscribe( ISubject *pSubject, bool bDetachMe /*= true*/ )
{
	LockableObjAccessLock(&m_Lock);
	ASSERT(pSubject);
	ISubjects::iterator iter = m_Subjects.find(pSubject);
	if ( iter != m_Subjects.end() )	// found
	{
		//TRACE("----------- UnSubscribe %x\n", this);
		m_Subjects.erase(iter);
		if ( bDetachMe )
			pSubject->Detach(this, false);	// we have unsubscribed it
	}
}

void IObserver::UnSubscribeAll()
{
	LockableObjAccessLock(&m_Lock);
	for ( ISubjects::iterator iter = m_Subjects.begin(); iter != m_Subjects.end(); ++iter )
	{
		ASSERT(*iter);
		(*iter)->Detach(this, false);		// we have unsubscribed it
	}
	m_Subjects.clear();
}

/*----------------------------------------------------------------------------*/
/* ISubject
/*----------------------------------------------------------------------------*/
ISubject::ISubject()
	: m_wParam(-1)
	, m_lParam(-1)
{
	
}

ISubject::~ISubject()
{
	LockableObjAccessLock(&m_Lock);
	DttachAll();
}

void ISubject::Attach( IObserver *pObserver, bool bSubscribeMe /*= true*/ )
{
	LockableObjAccessLock(&m_Lock);
	ASSERT(pObserver);
	IObservers::iterator iter = m_Observers.find(pObserver);
	if ( iter == m_Observers.end() )	// not found
	{
		m_Observers.insert(pObserver);
		if ( bSubscribeMe )
			pObserver->Subscribe(this, false);		// we have attached it
	}
}

void ISubject::Detach( IObserver *pObserver, bool bUnSubscribeMe /*= true*/ )
{
	LockableObjAccessLock(&m_Lock);
	ASSERT(pObserver);
	IObservers::iterator iter = m_Observers.find(pObserver);
	if ( iter != m_Observers.end() )	// found
	{
		m_Observers.erase(pObserver);
		if ( bUnSubscribeMe )
			pObserver->UnSubscribe(this, false);	// we have detached it
	}
}

void ISubject::DttachAll()
{
	LockableObjAccessLock(&m_Lock);
	for ( IObservers::iterator iter = m_Observers.begin(); iter != m_Observers.end(); ++iter )
	{
		ASSERT(*iter);
		(*iter)->UnSubscribe(this, false);	// we are going to detach all
	}
	m_Observers.clear();
}

void ISubject::Notify( WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	LockableObjAccessLock(&m_Lock);
	m_wParam = wParam;
	m_lParam = lParam;
	for ( IObservers::iterator iter = m_Observers.begin(); iter != m_Observers.end(); ++iter )
	{
		ASSERT(*iter);
		(*iter)->OnNotify(this, wParam, lParam);
	}
}

/*----------------------------------------------------------------------------*/
/* CLockableObj
/*----------------------------------------------------------------------------*/
ILockableObj::ILockableObj()
{
	InitializeCriticalSection(&m_cs);
}

ILockableObj::~ILockableObj()
{
	DeleteCriticalSection(&m_cs);
}

bool ILockableObj::IsLockable() const
{
	return true;
}

void ILockableObj::Lock()
{
	EnterCriticalSection(&m_cs);
}

void ILockableObj::UnLock()
{
	LeaveCriticalSection(&m_cs);
}

/*----------------------------------------------------------------------------*/
/* CAutoLocker
/*----------------------------------------------------------------------------*/
CAutoLocker::CAutoLocker( ILockableObj* pObj )
{
	if ( pObj && pObj->IsLockable() )
	{
		pObj->Lock();
		m_pObj = pObj;
	}
	else
		m_pObj = NULL;
}

CAutoLocker::~CAutoLocker()
{
	if ( m_pObj )
	{
		m_pObj->UnLock();
	}
}

/*----------------------------------------------------------------------------*/
/* CThreadedObj
/*----------------------------------------------------------------------------*/
CThreadedObj::CThreadedObj()
	: m_hStopEvent(NULL)
	, m_hThread(NULL)
{
	m_hStopEvent	= ::CreateEvent(NULL, TRUE, FALSE, NULL);
#ifdef _DEBUG
	m_dwDebugTimeElapsed	= 0;
	m_dwDebugStartTime		= -1;
#endif // _DEBUG
}

CThreadedObj::~CThreadedObj()
{
	if ( IsRunning() )
	{
		// We have to force the thread to stop now, otherwise it will try to access invalid data.
		VERIFY( ForceStopThread() );
	}
	if ( m_hStopEvent )
	{
		::CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}
	ASSERT(!m_hThread);
}

bool CThreadedObj::IsRunning() const
{
	//LockableObjAccessLock(this);
	DWORD dwExitCode = 0;
	return m_hThread && GetExitCodeThread(m_hThread, &dwExitCode) && (STILL_ACTIVE == dwExitCode);
}

bool CThreadedObj::IsTaskStopped()
{
	LockableObjAccessLock(this);
	return !IsRunning() || GetLastWParam() == EVENT_ONFINISH || GetLastWParam() == EVENT_ONFORCESTOP;
}

DWORD CThreadedObj::Suspend()
{
	return m_hThread ? SuspendThread(m_hThread) : -1;
}

DWORD CThreadedObj::Resume()
{
	return m_hThread ? ResumeThread(m_hThread) : -1;
}

bool CThreadedObj::RunInThread( PVOID pParam /*= NULL*/, HANDLE hStopEvent /*= NULL*/ )
{
	if ( SetStopEventHandle(hStopEvent) )
	{
		ASSERT( !IsRunning() && NULL == m_hThread);
		m_ThreadProcParam.pThreadedObj		= this;
		m_ThreadProcParam.pThreadParameter	= pParam;
		// create the thread suspended in case it returns before we ever catch the returned handle.
#ifdef THREADED_OBJ_IN_CRT
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, &m_ThreadProcParam, CREATE_SUSPENDED, NULL);
#else
		m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, &m_ThreadProcParam, CREATE_SUSPENDED, NULL);
#endif // CREATE_THREAD_CRT
		if ( m_hThread )
		{
			Notify(EVENT_PRESTART);
			ResumeThread(m_hThread);
		}
		return m_hThread != NULL;
	}
	return false;
}

bool CThreadedObj::SetStopEventHandle( HANDLE hStopEvent )
{
	//LockableObjAccessLock(this);
	if ( IsRunning() )
	{
		//ASSERT(0);	// a thread may has been started!
		return false;
	}
	if ( hStopEvent )
	{
		if ( m_hStopEvent )
		{
			CloseHandle(m_hStopEvent);
		}
		m_hStopEvent = hStopEvent;
	}
	return true;
}

bool CThreadedObj::SetStopEvent()
{
	LockableObjAccessLock(this);
	if (m_hStopEvent)
	{
		::SetEvent(m_hStopEvent);
	}
	return true;
}

bool CThreadedObj::ForceStopThread( DWORD dwTimeOutMilliseconds /*= INFINITE*/ )
{
	//LockableObjAccessLock(this);
	if ( IsRunning() )
	{
		ASSERT(m_hStopEvent);
		SetStopEvent();
		ASSERT(m_hThread);
		DWORD dwWaitRet = WaitForSingleObject(m_hThread, dwTimeOutMilliseconds);
		if ( WAIT_TIMEOUT == dwWaitRet )
		{
			ASSERT(0);	// this is bad because we have to terminate the thread before it ever has the chance to clean up.
			TerminateThread(m_hThread, 1);
			Notify(EVENT_TERMINATED);
		}
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	return true;
}

UINT WINAPI CThreadedObj::ThreadProc( PVOID pThreadProcParam )
{
	PThreadProcParam pProcParam = static_cast<PThreadProcParam>(pThreadProcParam);
	if ( pProcParam && pProcParam->pThreadedObj )
	{
		pProcParam->pThreadedObj->_Run(pProcParam->pThreadParameter);
	}
#ifdef THREADED_OBJ_IN_CRT
	_endthreadex(0);
#endif // CREATE_THREAD_CRT
	return 0;
}

bool CThreadedObj::_Run( PVOID pParam /*= NULL*/ )
{
#ifdef _DEBUG
	UpdateTimeBegin();
#endif // _DEBUG

	Notify(EVENT_ONSTART);

	Lock();
	if ( m_hStopEvent )
	{
		::ResetEvent(m_hStopEvent);
	}
	UnLock();

	bool bRet = Run(pParam);

	if ( IsStopEventSignaled() )
		Notify(EVENT_ONFORCESTOP);
	else
		Notify(EVENT_ONFINISH);

	SetStopEvent();

	Lock();
	if ( m_hThread )
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	UnLock();
	return bRet;
}

/*----------------------------------------------------------------------------*/
/* CObjInfoHolder
/*----------------------------------------------------------------------------*/
CObjInfoHolder::CObjInfoHolder()
{
	
}

CObjInfoHolder::~CObjInfoHolder()
{
	
}

BOOL CObjInfoHolder::StopRetrievingObjInfo()
{
	return SetStopEvent();
}

bool CObjInfoHolder::Run( PVOID pParam /*= NULL*/ )
{
	return RetrieveObjInfo() ? true : false;
}

/*----------------------------------------------------------------------------*/
/* CMultiColInfoHolderCObjInfoHolder
/*----------------------------------------------------------------------------*/
CMultiColInfoHolder::CMultiColInfoHolder()
	: m_nSortColumn(INFOCOL_INVALID)
	, m_bSortAscending(DEFAULT_SORT_ASCENDING)
{
}

CMultiColInfoHolder::~CMultiColInfoHolder()
{
}

void CMultiColInfoHolder::SwitchSort( int infoType )
{
	if (IsSortable())
	{
		bool bAscending;
		if ( infoType == m_nSortColumn )
			bAscending = !m_bSortAscending;
		else
			bAscending	= DEFAULT_SORT_ASCENDING;
		Sort(infoType, bAscending);
	}
}

/*----------------------------------------------------------------------------*/
/* CObjInfoMatcher
/*----------------------------------------------------------------------------*/
CObjInfoMatcher::CObjInfoMatcher( CObjInfoHolder* pObjInfoHolder /*= NULL*/ )
	: m_pObjInfoHolder(pObjInfoHolder)
	, m_nMatchObjInfoCount(0)
{
	
}

CObjInfoMatcher::~CObjInfoMatcher()
{
	
}

void CObjInfoMatcher::Reset()
{
	m_nMatchObjInfoCount = 0;
}

BOOL CObjInfoMatcher::RetrieveObjInfo()
{
	Reset();
	if ( m_pObjInfoHolder )
		return m_pObjInfoHolder->RetrieveObjInfo();
	return FALSE;
}

BOOL CObjInfoMatcher::StopRetrievingObjInfo()
{
	if ( m_pObjInfoHolder )
		return m_pObjInfoHolder->StopRetrievingObjInfo();
	return FALSE;
}

bool CObjInfoMatcher::RunInThread( PVOID pParam /*= NULL*/, HANDLE hStopEvent /*= NULL*/ )
{
	Reset();
	if ( m_pObjInfoHolder )
		return m_pObjInfoHolder->RunInThread(pParam, hStopEvent);
	return FALSE;
}

size_t CObjInfoMatcher::GetObjInfoCount() const
{
	if ( m_pObjInfoHolder )
		return m_pObjInfoHolder->GetObjInfoCount();
	return 0;
}

bool CObjInfoMatcher::IsSortable() const
{
	if ( m_pObjInfoHolder )
		return m_pObjInfoHolder->IsSortable();
	return false;
}

/*----------------------------------------------------------------------------*/
/* CMultiPatternMatchHelper
/*----------------------------------------------------------------------------*/
CMultiColInfoMatcher::CMultiColInfoMatcher(CMultiColInfoHolder* pMultiColInfoHolder /*= NULL*/)
	: CObjInfoMatcher(pMultiColInfoHolder)
{
	
}

CMultiColInfoMatcher::~CMultiColInfoMatcher()
{
	
}

BOOL CMultiColInfoMatcher::SetPatterns( LPCTSTR lpcszPatterns )
{
	if ( lpcszPatterns )
	{
		ASSERT(*lpcszPatterns);
		bool bExpectingEndChar	= false;
		PatternInfo patternInfo;
		for (LPCTSTR pPatternBeg = lpcszPatterns; ; ++lpcszPatterns)
		{
			if ( _T(' ') == *lpcszPatterns || _T('\t') == *lpcszPatterns || _T('\0') == *lpcszPatterns )
			{
				if ( bExpectingEndChar )
				{
					if ( lpcszPatterns > pPatternBeg )
					{
						patternInfo.strPattern = pPatternBeg;
						size_t nPatLen = lpcszPatterns-pPatternBeg;
						//patternInfo.strPattern[nPatLen] = _T('\0');
						patternInfo.strPattern.resize(nPatLen);
						OnAddPattern(patternInfo);
					}
					bExpectingEndChar = false;
				}
				if ( _T('\0') == *lpcszPatterns )
					break;
			}
			else
			{
				if ( !bExpectingEndChar )
				{
					patternInfo.bWithMinus		= false;
					patternInfo.bWithBackSlash	= false;
					pPatternBeg = lpcszPatterns;
					bExpectingEndChar = true;
				}
				if ( lpcszPatterns-pPatternBeg <= 1 )
				{
					if ( _T('-') == *lpcszPatterns )
						patternInfo.bWithMinus = true;
					else if ( _T('\\') == *lpcszPatterns )
						patternInfo.bWithBackSlash = true;
					else
						continue;
					++pPatternBeg;
				}
			}
		}
	}
	return TRUE;
}

size_t CMultiColInfoMatcher::GetMatch(LPCTSTR lpcszPatterns /*= NULL*/, WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	UINT* pnActiveIndex = (PUINT)wParam;
	UINT nActiveIndex	= pnActiveIndex ? *pnActiveIndex : 0;
	UINT nObjInfoCount	= GetObjInfoCount();
	if ( !lpcszPatterns || !*lpcszPatterns )
	{
		// Empty pattern string means to display everything.
		m_nMatchObjInfoCount = nObjInfoCount;
		if (nActiveIndex < (int)m_matchedObjInfoIndices.size() && nActiveIndex < m_nMatchObjInfoCount)
			nActiveIndex = m_matchedObjInfoIndices[nActiveIndex];
	}
	else
	{
		int nActiveObjIndex = nActiveIndex;
		if ( m_nMatchObjInfoCount < GetObjInfoCount() )
		{
			if (nActiveIndex < (int)m_matchedObjInfoIndices.size() && nActiveIndex < m_nMatchObjInfoCount)
				nActiveObjIndex = m_matchedObjInfoIndices[nActiveIndex];
		}
		nActiveIndex = 0;

		SetPatterns(lpcszPatterns);
		m_nMatchObjInfoCount = 0;
		for (size_t ii = 0; ii < nObjInfoCount; ++ii)
		{
			if ( IsObjInfoMatch(ii) )
			{
				if (m_nMatchObjInfoCount >= (int)m_matchedObjInfoIndices.size())
				{
					m_matchedObjInfoIndices.push_back(ii);
				}
				else
				{
					m_matchedObjInfoIndices[m_nMatchObjInfoCount] = ii;
				}
				if ( ii == nActiveObjIndex )
				{
					nActiveIndex = m_nMatchObjInfoCount;
				}
				++m_nMatchObjInfoCount;
			}
		}
	}
	if ( pnActiveIndex )
		*pnActiveIndex = nActiveIndex;
	//TRACE("------------ match count %d\n", m_nMatchObjInfoCount);
	return m_nMatchObjInfoCount;
}

int CMultiColInfoMatcher::GetObjInfoIndexInMatches( int nItem ) const
{
	if ( m_nMatchObjInfoCount < GetObjInfoCount() 
		&& m_nMatchObjInfoCount > 0 
		&& nItem < (int)m_matchedObjInfoIndices.size()	// We have to check this in case the holder is retrieving info in a thread.
		)
	{
		ASSERT( nItem >= 0 );
		ASSERT((UINT)nItem < m_nMatchObjInfoCount);
		nItem = m_matchedObjInfoIndices[nItem];
	}
	return nItem;
}

LPCTSTR CMultiColInfoMatcher::GetObjInfoTextInMatches( WPARAM wParam, LPARAM lParam )
{
	int& nIndex = (int&)wParam;
	int& nCol = (int&)lParam;
	ASSERT(m_pObjInfoHolder);
	int nObjInfoIndex = GetObjInfoIndexInMatches(nIndex);
	return m_pObjInfoHolder->GetObjInfoText(nObjInfoIndex, nCol);
}

void CMultiColInfoMatcher::Reset()
{
	CObjInfoMatcher::Reset();
	m_matchedObjInfoIndices.clear();
}

bool CMultiColInfoMatcher::HasFilteredObjinfo() const
{
	return !m_matchedObjInfoIndices.empty();
}

/*----------------------------------------------------------------------------*/
/* CFileInfoEnumerator
/*----------------------------------------------------------------------------*/
CFileInfoEnumerator::CFileInfoEnumerator()
{
	
}

CFileInfoEnumerator::~CFileInfoEnumerator()
{
	
}

const CFileInfoEnumerator::FileInfo* CFileInfoEnumerator::GetFileInfo( size_t nIndex )
{
	if ( nIndex < (int)m_arrFileInfo.size() )
	{
		return &m_arrFileInfo.at(nIndex);
	}
	return NULL;
}

void CFileInfoEnumerator::HandleFile( LPCTSTR lpcszPath, const FindFileData& ffd )
{
	FileInfo fileInfo;
	fileInfo.sFileInfo[FIT_FILENAME] = ffd.cFileName;
	
	fileInfo.sFileInfo[FIT_FILEPATH] = lpcszPath;
	
	tstring::size_type nDotPos = fileInfo.sFileInfo[FIT_FILENAME].rfind(_T('.'));
	if ( nDotPos != tstring::npos )
		fileInfo.sFileInfo[FIT_FILEEXT] = fileInfo.sFileInfo[FIT_FILENAME].c_str()+nDotPos+1;
	
	SYSTEMTIME st;

	#define FTIME_BUFFER_SIZE	255
	
	TCHAR szModified[FTIME_BUFFER_SIZE+FTIME_BUFFER_SIZE], szLocalDate[FTIME_BUFFER_SIZE], szLocalTime[FTIME_BUFFER_SIZE];
	FILETIME ft = ffd.ftLastWriteTime;
	
	FileTimeToLocalFileTime( &ft, &ft );
	FileTimeToSystemTime( &ft, &st );
	GetDateFormat( LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, szLocalDate, FTIME_BUFFER_SIZE );
	GetTimeFormat( LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, szLocalTime, FTIME_BUFFER_SIZE );
	
#ifdef MSVC_NEW_VER
	_sntprintf_s(szModified, FTIME_BUFFER_SIZE+FTIME_BUFFER_SIZE, _TRUNCATE, _T("%s %s"), szLocalDate, szLocalTime);
#else
	_sntprintf(szModified, FTIME_BUFFER_SIZE+FTIME_BUFFER_SIZE, _T("%s %s"), szLocalDate, szLocalTime);
#endif // MSVC_NEW_VER

	fileInfo.sFileInfo[FIT_MODIFIED] = szModified;

	DWORDLONG dwHighBase = MAXDWORD;
	++dwHighBase;
	fileInfo.nFileSize = (ffd.nFileSizeHigh * dwHighBase) + ffd.nFileSizeLow;

	#define FILESIZEBUFFER_SIZE		100

	TCHAR szBuffer[FILESIZEBUFFER_SIZE];
#ifdef MSVC_NEW_VER
	StrFormatByteSize64(fileInfo.nFileSize, szBuffer, FILESIZEBUFFER_SIZE);
#else
	StrFormatByteSize(fileInfo.nFileSize, szBuffer, FILESIZEBUFFER_SIZE);
#endif // MSVC_NEW_VER

	fileInfo.sFileInfo[FIT_FILESIZE] = szBuffer;
	
	m_arrFileInfo.push_back(fileInfo);
}

/*----------------------------------------------------------------------------*/
/* class CFileInfoHolderSortHelper
/*----------------------------------------------------------------------------*/
#ifdef USE_STL_SORT
class CFileInfoHolderSortHelper
{
public:
	CFileInfoHolderSortHelper(CFileInfoHolder* pFileInfoHolder)
		: m_pFileInfoHolder(pFileInfoHolder)
	{
	}
public:
	bool operator()( const CFileInfoEnumerator::FileInfo& left, const CFileInfoEnumerator::FileInfo& right ) const
	{
		return m_pFileInfoHolder->IsSortAscending() 
			&& _tcsicmp(left.sFileInfo[m_pFileInfoHolder->GetSortColumn()].c_str(), 
						right.sFileInfo[m_pFileInfoHolder->GetSortColumn()].c_str()) < 0;
	}
protected:
	CFileInfoHolder* m_pFileInfoHolder;
};
#else
	bool CFileInfoHolder::s_bSortAscending	= DEFAULT_SORT_ASCENDING;
	int CFileInfoHolder::s_nSortInfoType	= INFOCOL_INVALID;
#endif // USE_STL_SORT

/*----------------------------------------------------------------------------*/
/* CFileInfoHolder
/*----------------------------------------------------------------------------*/

CFileInfoHolder::CFileInfoHolder( const EnumerateFolderArray* pEnumerateFolders /*= NULL*/ )
	: m_pEnumerateFolders(pEnumerateFolders)
{
	;
}

CFileInfoHolder::~CFileInfoHolder()
{
	
}

LPCTSTR CFileInfoHolder::GetObjInfoText( WPARAM wParam, LPARAM lParam )
{
	int& nIndex = (int&)wParam;
	int& nCol = (int&)lParam;
	ObjInfoHolderAccessLock(this);
#ifdef _DEBUG
	UINT nObjInfoCount = GetObjInfoCount();
#endif // _DEBUG
	ASSERT( 0 <= nIndex && (UINT)nIndex < nObjInfoCount );
	ASSERT( CFileInfoEnumerator::FIT_FIRST <= nCol && nCol < CFileInfoEnumerator::FIT_MAX );
	const CFileInfoEnumerator::FileInfo* pFileInfo = CFileInfoEnumerator::GetFileInfo(nIndex);
	if ( pFileInfo )
		return const_cast<TCHAR*>(pFileInfo->sFileInfo[nCol].c_str());
	return NULL;
}

BOOL CFileInfoHolder::RetrieveObjInfo()
{
	ASSERT(m_pEnumerateFolders);
	m_arrFileInfo.clear();
	BOOL bRet = TRUE;
	for (size_t ii = 0; ii < m_pEnumerateFolders->size(); ++ii)
	{
		const EnumerateFolder& folder = m_pEnumerateFolders->at(ii);
		SetFilterPatterns( folder.strFilterPatterns.c_str() );
		BOOL bOK = Enumerate( folder.strFolder.c_str(), true, m_hStopEvent );
		bRet = bRet && bOK;
	}
	return bRet;
}

void CFileInfoHolder::Sort( int infoType, bool bAscending /*= true*/ )
{
	ObjInfoHolderAccessLock(this);
	if ( m_arrFileInfo.empty() )
		return;
	ASSERT(FIT_FIRST <= infoType && infoType < FIT_MAX);

	Notify(EVENT_SORT_BEGIN);

#ifdef _DEBUG
	UpdateTimeBegin();
#endif // _DEBUG

	if ( infoType == m_nSortColumn && (bAscending ^ m_bSortAscending) )
	{
		std::reverse(m_arrFileInfo.begin(), m_arrFileInfo.end());
		m_bSortAscending = bAscending;
	}
	else
	{
		m_nSortColumn		= infoType;
		m_bSortAscending	= bAscending;
#ifdef USE_STL_SORT
		std::sort( m_arrFileInfo.begin(), m_arrFileInfo.end(), CFileInfoHolderSortHelper(this) );
#else
		s_bSortAscending	= bAscending;
		s_nSortInfoType		= infoType;
		GENERICCOMPAREFN pfnSort	= reinterpret_cast<GENERICCOMPAREFN>(CompareFileInfo);
		qsort(&m_arrFileInfo.front(), m_arrFileInfo.size(), sizeof(FileInfo), pfnSort);
#endif // USE_STL_SORT
	}

	Notify(EVENT_SORT_END);
}

#ifndef USE_STL_SORT
int CFileInfoHolder::CompareFileInfo( const FileInfo* left, const FileInfo* right )
{
	int nRet;
	if ( FIT_FILESIZE == s_nSortInfoType )
	{
		nRet = (int)(left->nFileSize - right->nFileSize);
	}
	else
	{
		nRet = _tcsicmp(left->sFileInfo[s_nSortInfoType].c_str(), right->sFileInfo[s_nSortInfoType].c_str());
	}
	if ( !s_bSortAscending )
		nRet = -nRet;
	return nRet;
}
#endif // USE_STL_SORT

void CFileInfoHolder::HandleFile( LPCTSTR lpcszPath, const FindFileData& ffd )
{
	ObjInfoHolderAccessLock(this);
	CFileInfoEnumerator::HandleFile(lpcszPath, ffd);
#ifdef FILEINFOHOLDER_NOTIFY_NEW_FILE
	Notify(CThreadedObj::EVENT_ONPROCEED);
#endif // FILEINFOHOLDER_NOTIFY_NEW_FILE
}

void CFileInfoHolder::SetEnumerateFolderArray( const EnumerateFolderArray* pEnumerateFolders )
{
	m_pEnumerateFolders = pEnumerateFolders;
}

/*----------------------------------------------------------------------------*/
/* CFileInfoMatcher
/*----------------------------------------------------------------------------*/
CFileInfoMatcher::CFileInfoMatcher()
{
	
}

CFileInfoMatcher::~CFileInfoMatcher()
{
	
}

BOOL CFileInfoMatcher::IsObjInfoMatch( UINT nIndex )
{
	ObjInfoHolderAccessLock(m_pObjInfoHolder);
	CFileInfoHolder* pFileInfoHolder = static_cast<CFileInfoHolder*>(m_pObjInfoHolder);
	ASSERT(pFileInfoHolder);
	const CFileInfoEnumerator::FileInfo* pFileInfo = pFileInfoHolder->GetFileInfo(nIndex);
	ASSERT(pFileInfo);
	LPCTSTR lpcszFileName			= const_cast<TCHAR*>(pFileInfo->sFileInfo[CFileInfoEnumerator::FIT_FILENAME].c_str());
	LPCTSTR lpcszFilePath			= const_cast<TCHAR*>(pFileInfo->sFileInfo[CFileInfoEnumerator::FIT_FILEPATH].c_str());
	const TCHAR* cszColumnText[]	= {lpcszFileName, lpcszFilePath};
	for (FilePatternArray::iterator iter = m_arrFilePatterns.begin(); iter != m_arrFilePatterns.end(); ++iter)
	{
		LPCTSTR lpcszMatchTargetText = cszColumnText[iter->bMatchPath];
		bool bFind	= stristr(lpcszMatchTargetText, iter->strPattern.c_str()) != NULL;
		bool bMatch	= bFind ^ iter->bExclude;
		if ( !bMatch )
			return FALSE;
	}
	return TRUE;
}

BOOL CFileInfoMatcher::SetPatterns( LPCTSTR lpcszPatterns )
{
	m_arrFilePatterns.clear();
	return CMultiColInfoMatcher::SetPatterns(lpcszPatterns);
}

void CFileInfoMatcher::OnAddPattern( const PatternInfo& patternInfo )
{
	m_arrFilePatterns.push_back( FilePattern(patternInfo) );
}

/*----------------------------------------------------------------------------*/
/* CComboFileInfoMatcher
/*----------------------------------------------------------------------------*/
BOOL CComboFileInfoMatcher::IsObjInfoMatch( UINT nIndex )
{
	ObjInfoHolderAccessLock(m_pObjInfoHolder);
	CFileInfoHolder* pFileInfoHolder = static_cast<CFileInfoHolder*>(m_pObjInfoHolder);
	ASSERT(pFileInfoHolder);
	const CFileInfoEnumerator::FileInfo* pFileInfo = pFileInfoHolder->GetFileInfo(nIndex);
	ASSERT(pFileInfo);
	LPCTSTR lpcszFilePath			= const_cast<TCHAR*>(pFileInfo->sFileInfo[CFileInfoEnumerator::FIT_FILEPATH].c_str());
	for (FilePatternArray::iterator iter = m_arrFilePatterns.begin(); iter != m_arrFilePatterns.end(); ++iter)
	{
		bool bFind	= stristr(lpcszFilePath, iter->strPattern.c_str()) != NULL;
		if ( !bFind )
			return FALSE;
	}
	return TRUE;
}