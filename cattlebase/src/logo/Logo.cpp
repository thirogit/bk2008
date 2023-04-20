#include "stdafx.h"
#include "Logo.h"
#include "../missing/paths.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "../missing/util.h"

CLogoImage CLogoImage::m_logo;

CLogoImage::CLogoImage() : m_bLoaded(false)
{
	m_fiIo.read_proc = (FI_ReadProc)fipWinImageEx::FI_ReadProcNFS;
    m_fiIo.write_proc = (FI_WriteProc)fipWinImageEx::FI_WriteProcNFS;
    m_fiIo.seek_proc = (FI_SeekProc)fipWinImageEx::FI_SeekProcNFS;   
    m_fiIo.tell_proc = (FI_TellProc)fipWinImageEx::FI_TellProcNFS;
	
}

void CLogoImage::LoadLogo()
{
	if(!m_bLoaded)
	{
		CString customLogoPath = paths_ConcatPath(CUtil::GetAppDir(),"LOGO.BMP");
		do
		{
			if(boost::filesystem::exists(boost::filesystem::path((LPCSTR)customLogoPath)))
			{	
				if(load((LPCSTR)customLogoPath))
				{
				
					if(getWidth() <= MAX_LOGO_WIDTH && getHeight() <= MAX_LOGO_HEIGHT)
					{
						m_bLoaded = true;
						break;	
					}
				}
			}

			clear();

			CBitmap defaultLogo;
			defaultLogo.LoadBitmap(IDB_LOGOCOW);
			m_bLoaded = copyFromBitmap(defaultLogo) == TRUE;

		}
		while(false);
	}
}


CLogoImage* CLogoImage::GetLogo()
{
	m_logo.LoadLogo();
	return &m_logo;
}

bool CLogoImage::GetOleLogoImage(COleStreamFile &logoStream)
{
	if(!m_bLoaded) return false;

	ASSERT(!logoStream.m_lpStream);
	
	logoStream.CreateMemoryStream();

	LONG lSize = 0;
	
	
	ULONGLONG CurPosition = 0;
	ULONGLONG EndPosition = 0;
	HRESULT hr;

	
	// write picture size = 0 to the stream,we dont know it now
	lSize = 0;
	CComVariant varSize(lSize);
	hr = varSize.WriteToStream(logoStream.GetStream());
	ATLASSERT(SUCCEEDED(hr));

	CurPosition = logoStream.Seek(0,STREAM_SEEK_CUR);
	
	saveToHandle(FIF_JPEG,&m_fiIo,&logoStream);	

	// getting picture end position
	EndPosition = logoStream.Seek(0,STREAM_SEEK_CUR);

	lSize = EndPosition-CurPosition;

	// seek to the size position
	logoStream.SeekToBegin();
		
	// write picture size to the stream
	CComVariant var(lSize);
	VERIFY(SUCCEEDED(var.WriteToStream(logoStream.GetStream())));

	
	logoStream.SeekToBegin();


	return true;
}