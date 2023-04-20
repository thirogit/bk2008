#include "stdafx.h"
#include "NullStream.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CNullStream::CNullStream()
{

}
STDMETHODIMP_(ULONG)CNullStream::AddRef()
{
	return 1;
}

STDMETHODIMP_(ULONG)CNullStream::Release()
{
	return 0;
}

STDMETHODIMP CNullStream::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
	if (iid == IID_IUnknown || iid == IID_IStream)
	{
		if (ppvObj == NULL) 
		{
			return E_POINTER;
		}
		*ppvObj = this;
		return NOERROR;
	}
	return E_NOINTERFACE;
}

STDMETHODIMP CNullStream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
	return E_NOTIMPL;
}

STDMETHODIMP CNullStream::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
	if (pcbWritten != NULL)
		*pcbWritten = cb;
	return NOERROR;
}

STDMETHODIMP CNullStream::Seek(LARGE_INTEGER uliOffset, DWORD dwOrigin,
	ULARGE_INTEGER* puliNew)
{
	return E_NOTIMPL;
}

STDMETHODIMP CNullStream::SetSize(ULARGE_INTEGER)
{
	return E_NOTIMPL;
}

STDMETHODIMP CNullStream::CopyTo(LPSTREAM, ULARGE_INTEGER, ULARGE_INTEGER*,
	ULARGE_INTEGER*)
{
	return E_NOTIMPL;
}

STDMETHODIMP CNullStream::Commit(DWORD)
{
	return E_NOTIMPL;
}

STDMETHODIMP CNullStream::Revert()
{
	return E_NOTIMPL;
}

STDMETHODIMP CNullStream::LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)
{
	return E_NOTIMPL;
}

STDMETHODIMP CNullStream::UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER,
	DWORD)
{
	return E_NOTIMPL;
}

STDMETHODIMP CNullStream::Stat(STATSTG*, DWORD)
{
	return E_NOTIMPL;
}

STDMETHODIMP CNullStream::Clone(LPSTREAM*)
{
	return E_NOTIMPL;
}


