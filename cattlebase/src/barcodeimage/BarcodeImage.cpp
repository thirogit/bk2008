#include "stdafx.h"
#include "BarcodeImage.h"


CBarcodeImage::CBarcodeImage() 
{
	m_fiIo.read_proc = (FI_ReadProc)fipWinImageEx::FI_ReadProcNFS;
    m_fiIo.write_proc = (FI_WriteProc)fipWinImageEx::FI_WriteProcNFS;
    m_fiIo.seek_proc = (FI_SeekProc)fipWinImageEx::FI_SeekProcNFS;   
    m_fiIo.tell_proc = (FI_TellProc)fipWinImageEx::FI_TellProcNFS;
}


bool CBarcodeImage::DrawBarcode(const CBaseBarcode& barcode)
{	
	setSize(FIT_BITMAP,barcode.GetBarcodePixelWidth(),
					barcode.GetBarcodePixelHeight(),24);
	
	barcode.DrawBitmap(0,0,*this);
	
	return true;
}


bool CBarcodeImage::GetImageStream(COleStreamFile &logoStream)
{
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