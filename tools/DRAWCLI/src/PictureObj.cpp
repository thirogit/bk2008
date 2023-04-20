#include "stdafx.h"
#include "PictureObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define HIMETRIC_INCH	2540

static const OLECHAR szDrawObjInfo[] = OLESTR("\005CPictureObjList");


void HimetricToPixels(CDC*pDC,CSize& size)
{
	ASSERT(pDC);
	// convert himetric to pixels
	size.cx	= MulDiv(size.cx, pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
	size.cy	= MulDiv(size.cy, pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);
}

void HimetricToPixels(CWnd*pWnd,CSize& size)
{
	CClientDC dc(pWnd);
	HimetricToPixels(&dc,size);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CPictureObj,CObject,0);

CPictureObj::CPictureObj():
	m_eHorizAlign(H_Left),
	m_eVertAlign(V_Top),
	m_bSelected(false),
	m_bVisible(true),
	m_rcPosition(0,0,0,0),
	m_startRect(0,0,0,0),
	m_trackerStyle(CRectTracker::hatchedBorder),
	m_zoomX(100),
	m_zoomY(100)
{
}

CPictureObj::CPictureObj(const CRect position):
	m_eHorizAlign(H_Left),
	m_eVertAlign(V_Top),
	m_bSelected(false),
	m_bVisible(true),
	m_rcPosition(position),
	m_startRect(position),
	m_trackerStyle(CRectTracker::hatchedBorder),
	m_zoomX(100),
	m_zoomY(100)
{

}

CPictureObj::~CPictureObj()
{
	ReleasePicture();
}

OLE_HANDLE	CPictureObj::GetHandle()
{
	OLE_HANDLE handle = NULL;
	if(m_pPict != NULL)
		m_pPict->get_Handle(&handle);
	return handle;
}

// In himetric
CSize CPictureObj::GetSize()
{
	CSize size(0,0);
	if(m_pPict != NULL)
	{
		m_pPict->get_Width(&size.cx);
		m_pPict->get_Height(&size.cy);
	}
	return size;
}

// In pixels
CSize CPictureObj::GetSize(CDC*pDC)
{
	ASSERT(pDC);
	CSize size(0,0);
	if (m_pPict != NULL)
	{
		m_pPict->get_Width(&size.cx);
		m_pPict->get_Height(&size.cy);
		// convert himetric to pixels
		::HimetricToPixels(pDC,size);
	}
	return size;
}

// In pixels
CSize CPictureObj::GetSize(CWnd*pWnd)
{
	CClientDC dc(pWnd);
	return GetSize(&dc);
}

void CPictureObj::Copy( CPictureObj &right)
{
	m_eHorizAlign	= right.m_eHorizAlign ;
	m_eVertAlign	= right.m_eVertAlign ;
	m_bSelected		= right.m_bSelected ;
	m_bVisible		= right.m_bVisible ;
	m_rcPosition	= right.m_rcPosition ;
	m_trackerStyle	= right.m_trackerStyle ;

	ReleasePicture();
	COleStreamFile file;
	if(file.CreateMemoryStream())
	{
		IStream* pStream = file.GetStream() ;
		ASSERT(pStream);
		if(pStream)
		{
			HRESULT hr = right.WriteToStream(pStream);
			ASSERT(SUCCEEDED(hr));
			if(FAILED(hr))
				return;
			LARGE_INTEGER dlibMove = {0,0};
			pStream->Seek(dlibMove,STREAM_SEEK_SET,0);
			hr = ReadFromStream(pStream);
			ASSERT(SUCCEEDED(hr));
		}
	}
	else
	{
		ASSERT(0);
	}
}

void CPictureObj::CalcZoom()
{
	CRect curRect	= m_rcPosition;
	CRect startRect	= m_startRect;
	m_zoomX = MulDiv(curRect.Width(),100,startRect.Width());
	m_zoomY = MulDiv(curRect.Height(),100,startRect.Height());
}

void CPictureObj::SetZoomToPosition()
{
	CRect rect	= m_startRect;
	CRect r		= m_rcPosition;
	int w = rect.Width();
	int h = rect.Height();
	w = MulDiv(w,m_zoomX,100);
	h = MulDiv(h,m_zoomY,100);

	if (m_rcPosition.left > m_rcPosition.right)
		r.left	= r.right + w;
	else
		r.right	= r.left + w;

	if (m_rcPosition.top < m_rcPosition.bottom)
		r.bottom = r.top + h;
	else
		r.top	= r.bottom + h;

	m_rcPosition = r;
}

void CPictureObj::ZoomIn()
{
	CalcZoom();
	if(m_zoomX < 500)
		m_zoomX += 10;
	if(m_zoomY < 500)
		m_zoomY += 10;
	SetZoomToPosition();
}

void CPictureObj::ZoomOut()
{
	CalcZoom();
	m_zoomX -= 10;
	if(m_zoomX < 10)
		m_zoomX = 10;
	m_zoomY -= 10;
	if(m_zoomY < 10)
		m_zoomY = 10;
	SetZoomToPosition();
}

HRESULT CPictureObj::WriteToStream(IStream* pStream)
{
	ATLASSERT(pStream);

	CIStream ostream(pStream);
	ostream
		<< (int)m_eHorizAlign
		<< (int)m_eVertAlign
		<< m_bSelected
		<< m_bVisible
		<< m_rcPosition
		<< m_trackerStyle
		<< m_startRect
		<< m_zoomX 
		<< m_zoomY;

	HRESULT hr = ostream.Result();
	if(FAILED(hr))
		return hr; 

	LARGE_INTEGER  dlibMove = {0,0};
	ULARGE_INTEGER SizePosition = {0,0};
	ULARGE_INTEGER CurPosition = {0,0};

	// save stream size position
	hr = pStream->Seek(dlibMove,STREAM_SEEK_CUR,&SizePosition);
	ATLASSERT(SUCCEEDED(hr));

	// write picture size = 0 to the stream
	LONG lSize = 0;
	CComVariant varSize(lSize);
	hr = varSize.WriteToStream(pStream);
	ATLASSERT(SUCCEEDED(hr));
	if(FAILED(hr))
		return hr; 

	if(m_pPict != NULL)
	{

		// save picture
		hr = m_pPict->SaveAsFile(pStream, FALSE,&lSize);
		ATLASSERT(SUCCEEDED(hr));

		// getting picture end position
		ULARGE_INTEGER EndPosition = {0,0};
		dlibMove.QuadPart = 0;
		hr = pStream->Seek(dlibMove,STREAM_SEEK_CUR,&EndPosition);
		ATLASSERT(SUCCEEDED(hr));

		// seek to the size position
		dlibMove.QuadPart = SizePosition.QuadPart;
		hr = pStream->Seek(dlibMove,STREAM_SEEK_SET,&CurPosition);
		ATLASSERT(SUCCEEDED(hr));
		ATLASSERT(SizePosition.QuadPart == CurPosition.QuadPart);

	
		// write picture size to the stream
		CComVariant var(lSize);
		VERIFY(SUCCEEDED(var.WriteToStream(pStream)));

		// restoring stream cursor
		dlibMove.QuadPart = EndPosition.QuadPart;
		hr = pStream->Seek(dlibMove,STREAM_SEEK_SET,&CurPosition);
		ATLASSERT(SUCCEEDED(hr));
		ATLASSERT(EndPosition.QuadPart == CurPosition.QuadPart);
	
	}
	return hr;
}

HRESULT CPictureObj::ReadFromStream(IStream* pStream)
{
	ATLASSERT(pStream);
	LONG lSize = 0;
	ReleasePicture();
	
	CIStream istream(pStream);
	istream
		>> (int&)m_eHorizAlign
		>> (int&)m_eVertAlign
		>> m_bSelected
		>> m_bVisible
		>> m_rcPosition
		>> m_trackerStyle
		>> m_startRect
		>> m_zoomX 
		>> m_zoomY;

	HRESULT hr = istream.Result();
	if(FAILED(hr))
		return hr; 

	// It is needed to know is picture in the stream
	// getting the size of the picture
	CComVariant varSize;
	hr = varSize.ReadFromStream(pStream);
	VERIFY(SUCCEEDED(hr));

	if(SUCCEEDED(hr))
	{
		ATLASSERT(varSize.vt == VT_I4);
		ATLASSERT(lSize >= 0 );
		lSize = varSize.lVal;

		if(lSize > 0)
		{
			// Warning !!! Microsoft bug!!! It is needed to set stream position manualy!!!
			LARGE_INTEGER Move = {0,0};
			ULARGE_INTEGER StartPosition = {0,0};
			// save stream position
			hr = pStream->Seek(Move,STREAM_SEEK_CUR,&StartPosition);
			ATLASSERT(SUCCEEDED(hr));

			hr = ::OleLoadPicture(pStream, lSize, FALSE, IID_IPicture, (LPVOID *)&m_pPict);
			ATLASSERT(SUCCEEDED(hr));
			
			// set correct next position
			Move.QuadPart = StartPosition.QuadPart + lSize;
			hr = pStream->Seek(Move,STREAM_SEEK_SET,&StartPosition);
			ATLASSERT(SUCCEEDED(hr));
		}
	}
	return hr;
}


BOOL CPictureObj::Load(LPCTSTR szFile)
{
	m_strPathName = szFile;

	HRESULT hr = S_OK;
	ReleasePicture();
	CFileStatus status;
	if(!CFile::GetStatus(szFile,status))
		return false;
	hr = ::OleLoadPicturePath(CComBSTR(szFile),NULL,0,0,IID_IPicture,(LPVOID *)&m_pPict);

	if(SUCCEEDED(hr))
		return true;

	return false;
}

BOOL CPictureObj::CreateFromFile(const CPoint pt)
{
	static char BASED_CODE szExt[]	= "jpg";
	static char BASED_CODE szName[] = "Image";
	static char BASED_CODE szFilter[] =
	"JPEG (*.jpg)|*.jpg|"
	"Windows Bitmap (*.bmp)|*.bmp|"
	"GIF Images (*.gif)|*.gif|"
	"Windows Icons (*.ico)|*.ico||"
	;


	CFileDialog dlg(TRUE,szExt,szName,
					OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);

	if(dlg.DoModal() != IDOK)
		return false;
	CString strPathName = dlg.GetPathName();
	if(Load(strPathName))
	{
		CSize size = GetSize((CWnd*)NULL);

		m_rcPosition.SetRect(pt.x,pt.y,size.cx,size.cy);
		m_startRect = m_rcPosition;
		CalcZoom();
		return true;
	}
	return false;
}

void CPictureObj::MoveTo(const CRect& position, CWnd* pView)
{
	ASSERT_VALID(this);

	if (position == m_rcPosition)
		return;
	const dw = 8;
	CRect rect = m_rcPosition;
	rect.InflateRect(dw,dw);

	if (pView != NULL)
		pView->InvalidateRect(rect);
	
	m_rcPosition = position;
	rect = m_rcPosition;
	rect.InflateRect(dw,dw);

	if (pView != NULL)
		pView->InvalidateRect(rect);
}

void CPictureObj::DrawTracker(CDC* pDC, const CRect& rect)
{
	if (m_pPict == NULL)
		return;
	CRectTracker tracker;
	tracker.m_rect = rect;
	pDC->LPtoDP(tracker.m_rect);
	tracker.m_nStyle |= m_trackerStyle;
	tracker.Draw(pDC);
}

void CPictureObj::DrawTracker(CDC* pDC)
{
	DrawTracker(pDC,GetPosition());
}


void CPictureObj::Draw(CDC* pDC, const CRect& rcPosition, const CRect& rcBounds)
{
	if (m_pPict == NULL)
		return;
	if(m_bVisible)
		Render(pDC,rcPosition,rcBounds);
	if(m_bSelected && !pDC->IsPrinting())
	{
		DrawTracker(pDC,rcBounds);
	}
}

void CPictureObj::Draw(CDC* pDC, CRect& rcBounds)
{
	Draw(pDC,rcBounds,rcBounds);
}


void CPictureObj::Draw(CDC* pDC) 
{
	if (m_pPict == NULL)
		return;

	// get size in pixels
	CSize size = GetSize(pDC);
	CRect rcBounds,rcRender(0,0,size.cx,size.cy);
	pDC->GetClipBox(&rcBounds);

	if(!m_rcPosition.IsRectEmpty())
		rcRender = m_rcPosition;

	// Offset picture using alignment style
	CPoint offset(0,0);
	switch(m_eHorizAlign)
	{
		default:
			ASSERT(0);
		case H_Center:
			offset.x = (rcBounds.Width() - size.cx)/2;
			break;
		case H_Left:
			break;
		case H_Right:
			offset.x = (rcBounds.Width() - size.cx);
			break;
	}

	switch(m_eVertAlign)
	{
		default:
			ASSERT(0);
		case V_Center:
			offset.y = ((rcBounds.Height() - size.cy)/2);
			break;
		case V_Top:
			break;
		case V_Bottom:
			offset.y = ((rcBounds.Height() - size.cy));
			break;
	}

	rcRender.OffsetRect(offset);
	Draw(pDC,rcRender);

}


//////////////////////////////////////////////////////////////////////
// CPictureObjList

CPictureObjList::~CPictureObjList()
{
	RemoveAndDestroy();
}

void CPictureObjList::RemoveAndDestroy()
{
	if(!GetCount())
		return;
	POSITION pos = GetHeadPosition();
	while (pos != NULL)
	{
		CPictureObj* pObj = GetNext(pos);
		delete pObj;
	}
	RemoveAll();
}

void CPictureObjList::DeselectAll() 
{
	POSITION pos = GetHeadPosition();
	while(pos != NULL)
	{
		CPictureObj* pObj = GetNext(pos);
		pObj->SetSelected(false);
	}
}

CPictureObj* CPictureObjList::FindSelected()
{
	POSITION pos = GetHeadPosition();
	while(pos != NULL)
	{
		CPictureObj* pObj = GetNext(pos);
		if(pObj->GetSelected())
			return pObj;
	}
	return NULL;
}

bool CPictureObjList::Remove(CPictureObj* pObj)
{
	POSITION pos = Find(pObj);
	if(pos != NULL)
	{
		RemoveAt(pos);
	}
	return (pos != NULL);
}

// in himetric
CSize CPictureObjList::ComputeMaxSize()
{
	CSize sizeMax(0,0);
	POSITION pos = GetHeadPosition();
	while(pos != NULL)
	{
		CPictureObj* pObj = GetNext(pos);
		CSize size = pObj->GetSize();
		
		sizeMax.cx = max(sizeMax.cx, size.cx);
		sizeMax.cy = max(sizeMax.cy, size.cy);
	}
	return sizeMax;
}

// in pixel
CSize CPictureObjList::ComputeMaxSize(CDC* pDC)
{
	CSize sizeMax(ComputeMaxSize());
	::HimetricToPixels(pDC,sizeMax);
	return sizeMax;
}

// in pixel
CSize CPictureObjList::ComputeMaxSize(CWnd* pWnd)
{
	CSize sizeMax(ComputeMaxSize());
	::HimetricToPixels(pWnd,sizeMax);
	return sizeMax;
}

CRect CPictureObjList::GetRect()
{
	CRect rectMax(0,0,0,0);
	POSITION pos = GetHeadPosition();
	while(pos != NULL)
	{
		CPictureObj* pObj = GetNext(pos);
		CRect rect = pObj->GetPosition();
		rectMax.UnionRect(rectMax,rect);
		
	}
	return rectMax;
}

void CPictureObjList::Draw(CDC* pDC)
{
	POSITION pos = GetTailPosition();
	while (pos != NULL)
	{
		CPictureObj* pObj = GetPrev(pos);
		pObj->Draw(pDC);
	}
}

// find object that contain point
CPictureObj* CPictureObjList::ObjectAt(const CPoint& pt)
{
	POSITION pos = GetHeadPosition();
	while (pos != NULL)
	{
		CPictureObj* pObj = GetNext(pos);
		CRect rect = pObj->GetPosition();
		rect.NormalizeRect();
		if (rect.PtInRect(pt))
			return pObj;
	}
	return NULL;
}

HRESULT CPictureObjList::WriteToStream(IStream* pStream)
{
	ATLASSERT(pStream);

	CIStream ostream(pStream);
	int count = GetCount();
	ostream << count;

	HRESULT hr = ostream.Result();
	if(FAILED(hr))
		return hr; 

	POSITION pos = GetHeadPosition();
	while (pos != NULL)
	{
		CPictureObj* pObj = GetNext(pos);
		hr = pObj->WriteToStream(pStream);
		if(FAILED(hr))
			break;
	}
	return hr;
}

HRESULT CPictureObjList::ReadFromStream(IStream* pStream)
{
	ATLASSERT(pStream);
	CIStream istream(pStream);
	int count = 0;
	istream >> count;

	HRESULT hr = istream.Result();
	if(FAILED(hr))
		return hr; 
	while(count--)
	{
		CPictureObj* pObj = new CPictureObj;
		hr = pObj->ReadFromStream(pStream);
		if(FAILED(hr))
			break;
		AddTail(pObj);
	}
	return hr;
}

BOOL CPictureObjList::ReadFromStorage(LPSTORAGE lpRootStg)
{
	if (lpRootStg != NULL)
	{
		CComPtr<IStream>spStream;

		if (FAILED(lpRootStg->OpenStream(szDrawObjInfo, 
				   NULL, STGM_SHARE_EXCLUSIVE|STGM_READ, 
				   0, &spStream)))
		{
			TRACE(_T("OpenStream failed\n"));
			return FALSE;
		}
		else
		{
			if(FAILED(ReadFromStream(spStream)))
			{
				TRACE(_T("ReadFromStream failed\n"));
				return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPictureObjList::WriteToStorage(LPSTORAGE lpRootStg)
{
	if (lpRootStg != NULL)
	{
		CComPtr<IStream>spStream;
		if (FAILED(lpRootStg->CreateStream(szDrawObjInfo, 
				   STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 
				   0, 0, &spStream)))
		{
			TRACE(_T("CreateStream failed\n"));
			return FALSE;
		}
		else
		{
			if(FAILED(WriteToStream(spStream)))
			{
				TRACE(_T("WriteToStream failed\n"));
				return FALSE;
			}
			lpRootStg->Commit(STGC_DEFAULT);
			return TRUE;
		}
	}
	return FALSE;
}

