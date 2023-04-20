#include "stdafx.h"
#include <winnls.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#include <objbase.h>

// the following header redefines
// the DEFINE_GUID macro to actually allocate data

#include <initguid.h>
#ifndef INITGUID
#define INITGUID
#endif

// the DEFINE_GUID macro in the following header now allocates data
#include "DocSummary.h"

const OLECHAR szSummInfo[] = OLESTR("\005SummaryInformation");

#if defined(_UNICODE)
LPCSTR tcstocs(LPCTSTR lpctStr) {                       // typed char (WCHAR) to CHAR
	static CHAR strTemp[1024];
	wcstombs(strTemp, lpctStr, 1024);
	return strTemp;
}
#else // !defined(_UNICODE)
#define tcstocs(lpctStr) (LPCSTR)(lpctStr)
#endif

CDocSummary::CDocSummary()
{
	m_propSet.SetFormatVersion(0);
	DWORD dwOSVer;
	dwOSVer = (DWORD)MAKELONG(LOWORD(GetVersion()), 2);
	m_propSet.SetOSVersion(dwOSVer);
	m_propSet.SetClassID(FMTID_SummaryInformation);
	m_pSection = m_propSet.AddSection(FMTID_SummaryInformation);
	UINT cp = GetACP();
	m_pSection->Set(PID_CODEPAGE, (void*)&cp, VT_I2);
	SetAuthor(_T(""));
	SetComments(_T(""));
	m_pSection->Set(PIDSI_REVNUMBER, (void*)_T("0"), VT_LPSTR);
	FILETIME zeroTime = {0L, 0L};
	m_pSection->Set(PIDSI_EDITTIME, (void*)&zeroTime, VT_FILETIME);
	m_pSection->Set(PIDSI_LASTPRINTED, (void*)&zeroTime, VT_FILETIME);
	m_pSection->Set(PIDSI_LASTSAVE_DTM, (void*)&zeroTime, VT_FILETIME);
	m_pSection->Set(PIDSI_CREATE_DTM, (void*)&zeroTime, VT_FILETIME);
}


BOOL CDocSummary::SetAuthor(LPCTSTR szAuthor)
{
	return m_pSection->Set(PIDSI_AUTHOR,
		(void*)tcstocs(szAuthor), VT_LPSTR);
}

CString CDocSummary::GetAuthor()
{
	return CString((LPCSTR)m_pSection->Get(PIDSI_AUTHOR));
}
BOOL CDocSummary::SetComments(LPCTSTR szComments)
{
	return m_pSection->Set(PIDSI_COMMENTS,
		(void*)tcstocs(szComments), VT_LPSTR);
}

CString CDocSummary::GetComments()
{
	return CString((LPCSTR)m_pSection->Get(PIDSI_COMMENTS));
}

BOOL CDocSummary::IncrRevNum()
{
	ULONG count;
	_stscanf((LPCTSTR)GetRevNum(), _T("%lu"), &count);
	count++;
	TCHAR buff[20];
	_stprintf(buff, _T("%lu"), count);
	return m_pSection->Set(PIDSI_REVNUMBER, (void*)buff, VT_LPSTR);
}

CString CDocSummary::GetRevNum()
{
	return CString((LPCSTR)m_pSection->Get(PIDSI_REVNUMBER));
}

BOOL CDocSummary::RecordCreateDate()
{
	FILETIME createDate;
	CoFileTimeNow(&createDate);
	return m_pSection->Set(PIDSI_CREATE_DTM,
		(void*)&createDate, VT_FILETIME);
}

CString CDocSummary::GetCreateDate()
{
	FILETIME* pCreateDate = (FILETIME*)m_pSection->Get(PIDSI_CREATE_DTM);
	if ((pCreateDate == NULL) ||
		((pCreateDate->dwLowDateTime == 0L) &&
		 (pCreateDate->dwHighDateTime == 0L)  ))
		return CString(_T(""));
	else
	{
		COleDateTime tempDate = *pCreateDate;
		return tempDate.Format();
	}
}

BOOL CDocSummary::RecordSaveDate()
{
	FILETIME saveDate;
	CoFileTimeNow(&saveDate);
	return m_pSection->Set(PIDSI_LASTSAVE_DTM,
		(void*)&saveDate, VT_FILETIME);
}

CString CDocSummary::GetLastSaveDate()
{
	FILETIME *pSaveDate = (FILETIME*)m_pSection->Get(PIDSI_LASTSAVE_DTM);
	if ((pSaveDate == NULL) ||
		((pSaveDate->dwLowDateTime == 0L) &&
		 (pSaveDate->dwHighDateTime == 0L)  ))
		return CString(_T(""));
	else
	{
		COleDateTime tempDate = *pSaveDate;
		return tempDate.Format();
	}
}

BOOL CDocSummary::WriteToStorage(LPSTORAGE lpRootStg)
{
	if (lpRootStg != NULL)
	{
		LPSTREAM lpStream = NULL;
		if (FAILED(lpRootStg->CreateStream(szSummInfo,
				   STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE,
				   0, 0, &lpStream)))
		{
			TRACE(_T("CreateStream failed\n"));
			return FALSE;
		}
		else
		{
			if(!m_propSet.WriteToStream(lpStream))
			{
				TRACE(_T("WriteToStream failed\n"));
				return FALSE;
			}
			lpRootStg->Commit(STGC_DEFAULT);
			lpStream->Release();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDocSummary::ReadFromStorage(LPSTORAGE lpRootStg)
{
	if (lpRootStg != NULL)
	{
		LPSTREAM lpStream = NULL;

		if (FAILED(lpRootStg->OpenStream(szSummInfo,
				   NULL, STGM_SHARE_EXCLUSIVE|STGM_READ,
				   0, &lpStream)))
		{
			TRACE(_T("OpenStream failed\n"));
			return FALSE;
		}
		else
		{
			if (!m_propSet.ReadFromStream(lpStream))
			{
				TRACE(_T("ReadFromStream failed\n"));
				return FALSE;
			}
			m_pSection = m_propSet.GetSection(FMTID_SummaryInformation);
			lpStream->Release();
			return TRUE;
		}
	}
	return FALSE;
}
