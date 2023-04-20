#include "stdafx.h"
#include "Picture.h"
#include "NullStream.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CPicture::CPicture()
{
	m_pPicture	= NULL;
}

CPicture::~CPicture()
{
	UnLoad();
}

//bool CPicture::Load(CString sResourceType, CString sResource)
//{
//	bool bResult = false;
//
//	if (m_pPicture != NULL)
//		UnLoad();
//
//	if (m_pPicture == NULL)
//	{
//		BYTE*	pBuff = NULL;
//		int		nSize = 0;
//		if (GetResource(sResource.GetBuffer(0), sResourceType.GetBuffer(0), pBuff, nSize))
//		{
//			if (nSize > 0)
//			{
//				pBuff = new BYTE[nSize];
//
//				if (GetResource(sResource.GetBuffer(0), sResourceType.GetBuffer(0), pBuff, nSize))
//				{
//					if (LoadFromBuffer(pBuff, nSize))
//						bResult = true;
//				}
//
//				delete [] pBuff;
//			}
//		}
//	}
//	return bResult;
//}

//bool CPicture::Load(CString sFileName)
//{
//	bool bResult = false;
//
//	if (m_pPicture != NULL)
//		UnLoad();
//
//	if (m_pPicture == NULL)
//	{
//		CFile			cFile;
//		CFileException	e;
//
//		if (cFile.Open(sFileName, CFile::modeRead | CFile::typeBinary, &e))
//		{
//			BYTE* pBuff = new BYTE[cFile.GetLength()];
//
//			if (cFile.Read(pBuff, cFile.GetLength()) > 0)
//			{
//				if (LoadFromBuffer(pBuff, cFile.GetLength()))
//					bResult = true;
//			}
//
//			delete [] pBuff;
//		}
//	}
//	return bResult;
//}

void CPicture::UnLoad()
{
	if (m_pPicture != NULL)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
	}
}

bool CPicture::Draw(CDC* pDC)
{
	if (m_pPicture != NULL)
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
		int nHeight	= MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
		
		return Draw(pDC, 0, 0, nWidth, nHeight);
	}
	return false;
}

bool CPicture::Draw(CDC* pDC, CPoint Pos)
{
	if (m_pPicture != NULL)
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
		int nHeight	= MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
		
		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);
	}
	return false;
}

bool CPicture::Draw(CDC* pDC, CPoint Pos, CSize Size)
{
	if (m_pPicture != NULL)
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= Size.cx;
		int nHeight	= Size.cy;
		
		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);
	}
	return false;
}

bool CPicture::Draw(CDC* pDC, double nSizeRatio)
{
	if (m_pPicture != NULL)
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= int(MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH) * nSizeRatio);
		int nHeight	= int(MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH) * nSizeRatio);
		
		return Draw(pDC, 0, 0, nWidth, nHeight);
	}
	return false;
}

bool CPicture::Draw(CDC* pDC, CPoint Pos, double nSizeRatio)
{
	if (m_pPicture != NULL)
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		int nWidth	= int(MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH) * nSizeRatio);
		int nHeight	= int(MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH) * nSizeRatio);
		
		return Draw(pDC, Pos.x, Pos.y, nWidth, nHeight);
	}
	return false;
}

bool CPicture::Draw(CDC* pDC, int x, int y, int cx, int cy)
{
	long hmWidth;
	long hmHeight;
	m_pPicture->get_Width(&hmWidth);
	m_pPicture->get_Height(&hmHeight);
	return m_pPicture->Render(pDC->m_hDC, x, y, cx, cy, 0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK;		
}

//bool CPicture::LoadFromBuffer(BYTE* pBuff, int nSize)
//{
//	bool bResult = false;
//
//	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
//	void* pData = GlobalLock(hGlobal);
//	memcpy(pData, pBuff, nSize);
//	GlobalUnlock(hGlobal);
//
//	IStream* pStream = NULL;
//
//	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
//	{
//		HRESULT hr;
//		if ((hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_pPicture)) == S_OK)
//			bResult = true;
//	
//		pStream->Release();
//	}
//
//	return bResult;
//}

//bool CPicture::GetResource(LPSTR lpName, LPSTR lpType, void* pResource, int& nBufSize)
//{ 
//	HRSRC		hResInfo;
//	HANDLE		hRes;
//	HMODULE		hInst	= NULL; 
//	LPSTR		lpRes	= NULL; 
////	int			nLen	= 0;
//	bool		bResult	= FALSE;
//
//	// Find the resource
//	hResInfo = FindResource(hInst, lpName, lpType);
//
//	if (hResInfo == NULL) 
//		return false;
//
//	// Load the resource
//	hRes = LoadResource(hInst, hResInfo);
//
//	if (hRes == NULL) 
//		return false;
//
//	// Lock the resource
//	lpRes = (char*)LockResource(hRes);
//
//	if (lpRes != NULL)
//	{ 
//		if (pResource == NULL)
//		{
//			nBufSize = SizeofResource(hInst, hResInfo);
//			bResult = true;
//		}
//		else
//		{
//			if (nBufSize >= (int)SizeofResource(hInst, hResInfo))
//			{
//				memcpy(pResource, lpRes, nBufSize);
//				bResult = true;
//			}
//		} 
//
//		UnlockResource(hRes);  
//	}
//
//	// Free the resource
//	FreeResource(hRes);
//
//	return bResult;
//}

CSize CPicture::GetSize(CDC* pDC)
{
	CSize rcResult = CSize(0,0);
	
	if (m_pPicture != NULL)
	{
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		rcResult.cx	= MulDiv(hmWidth,	pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
		rcResult.cy	= MulDiv(hmHeight,	pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
	}

	return rcResult;
}


bool CPicture::Serialize(CArchive& ar)
{
	
	CArchiveStream arcstream(&ar);
	IStream* pStream = (IStream*)&arcstream;
		
	if (ar.IsStoring())
	{
		WriteToStream(pStream);	
	
	}
	else
	{
		ReadFromStream(pStream);
		
	}
	return true;
}

bool CPicture::WriteToStream(IStream *pStream)
{
	LONG lSize = 0;
	CNullStream nullStream;

	HRESULT hr = m_pPicture->SaveAsFile(&nullStream,FALSE,&lSize);
	VERIFY(SUCCEEDED(hr));
	
	CComVariant varSize(lSize);
	hr = varSize.WriteToStream(pStream);
	ATLASSERT(SUCCEEDED(hr));
	
	hr = m_pPicture->SaveAsFile(pStream,FALSE,&lSize);
	VERIFY(SUCCEEDED(hr));
	
	return true;


}
bool CPicture::ReadFromStream(IStream *pStream)
{
	LONG lSize = 0;
	LARGE_INTEGER  Move = {0,0};
	ULARGE_INTEGER SizePosition = {0,0};
	
	ULARGE_INTEGER StartPosition = {0,0};
	HRESULT hr;
		
	CComVariant varSize;
	hr = varSize.ReadFromStream(pStream);
	ASSERT(SUCCEEDED(hr));

	if(SUCCEEDED(hr))
	{
		ATLASSERT(varSize.vt == VT_I4);
		ATLASSERT(lSize >= 0 );
		lSize = varSize.lVal;

		if(lSize > 0)
		{
			// Warning !!! Microsoft bug!!! It is needed to set stream position manualy!!!
			
			// save stream position
			hr = pStream->Seek(Move,STREAM_SEEK_CUR,&StartPosition);
			ATLASSERT(SUCCEEDED(hr));

			hr = ::OleLoadPicture(pStream, lSize, FALSE, IID_IPicture, (LPVOID *)&m_pPicture);
			ATLASSERT(SUCCEEDED(hr));
			
			// set correct next position
			Move.QuadPart = StartPosition.QuadPart + lSize;
			hr = pStream->Seek(Move,STREAM_SEEK_SET,&StartPosition);
			ATLASSERT(SUCCEEDED(hr));
			return true;
		}
	}
	return false;
}


//void CPicture::Copy(CPicture &copy)
//{
//COleStreamFile file;
//HRESULT hr;
//	if(file.CreateMemoryStream())
//	{
//		IStream* pStream = file.GetStream() ;
//		ASSERT(pStream);
//		if(pStream)
//		{
//			hr = copy.WriteToStream(pStream);
//			ASSERT(SUCCEEDED(hr));
//			if(FAILED(hr)) 	return;
//
//			LARGE_INTEGER dlibMove = {0,0};
//			pStream->Seek(dlibMove,STREAM_SEEK_SET,0);
//			hr = ReadFromStream(pStream);
//			ASSERT(SUCCEEDED(hr));
//		}
//	}
//	else
//	{
//		ASSERT(0);
//	}
//
//
//}