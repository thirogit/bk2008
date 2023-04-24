// PictureObj.h: interface for the CPictureObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTUREOBJ_H__59DA2CBD_8B9F_431C_85BE_5C17F2529AAD__INCLUDED_)
#define AFX_PICTUREOBJ_H__59DA2CBD_8B9F_431C_85BE_5C17F2529AAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXCTL_H__
#include <afxctl.h>
#endif

#include <atlbase.h>
#include <afxtempl.h>
#include "IStream.h"

typedef enum 
{
	H_Left,			// left-justified
	H_Center,		// center-justified
	H_Right,		// right-justified

} HorzAlign;

typedef enum 
{
	V_Top,			// aligned to top
	V_Center,		// aligned to center
	V_Bottom,		// aligned to bottom

} VertAlign;




class CPictureObj  :	public CPictureHolder,
						public CObject
{
	DECLARE_SERIAL(CPictureObj);

public:
	CPictureObj();
	CPictureObj(const CRect position);
	virtual ~CPictureObj();
	void RemoveAndDestroy();

	virtual void Copy( CPictureObj &right);
	virtual HRESULT WriteToStream(IStream* pStream);
	virtual HRESULT ReadFromStream(IStream* pStream);
	
// Attributes
public:
	HorzAlign	GetHorzAlign( void ) const;
	void		SetHorzAlign( HorzAlign eHorzAlign );
	VertAlign	GetVertAlign( void ) const;
	void		SetVertAlign( VertAlign eVertAlign );
	BOOL		GetSelected( void ) const;
	void		SetSelected( BOOL bValue );
	void		SetVisible(BOOL bValue);
	BOOL		GetVisible();
	CRect		GetPosition();
	void		SetPosition(const CRect pos);
	CRect		GetStartRect();
	void		SetStartRect(const CRect pos);
	CString		GetPathName();
	void		SetPathName(const CString pathname);

	OLE_HANDLE	GetHandle();
	CSize		GetSize();			// in himetric
	CSize		GetSize(CDC*pDC);	// in pixel
	CSize		GetSize(CWnd*pWnd);	// in pixel
	BOOL		IsValidPicture();

// Operations
public:

	BOOL Load(LPCTSTR szFile);
	BOOL CreateFromFile(const CPoint pt);
	void ReleasePicture();
	void MoveTo(const CRect& position, CWnd* pView);

	// Drawing picture
	void Draw(CDC* pDC);
	void Draw(CDC* pDC, CRect& rcBounds);
	void Draw(CDC* pDC, const CRect& rcPosition, const CRect& rcBounds);
	void DrawTracker(CDC* pDC);
	void DrawTracker(CDC* pDC, const CRect& rect);
	void ZoomIn();
	void ZoomOut();

protected:

	void CalcZoom();
	void SetZoomToPosition();

protected:

	HorzAlign		m_eHorizAlign;
	VertAlign		m_eVertAlign;
	BOOL			m_bSelected;
	BOOL			m_bVisible;
	CRect			m_rcPosition;		// in pixels
	CRect			m_startRect;		// in pixels
	int				m_trackerStyle;
	int				m_zoomX,m_zoomY;

	// Not serialized
	CString			m_strPathName;

};

inline
void CPictureObj::RemoveAndDestroy()
{
	delete this;
}
inline
CString CPictureObj::GetPathName()
{
	return m_strPathName;
}
inline
void CPictureObj::SetPathName(const CString pathname)
{
	m_strPathName = pathname;
}

inline
void CPictureObj::SetVisible(BOOL bValue)
{
	m_bVisible = bValue;
}
inline
BOOL CPictureObj::GetVisible()
{
	return m_bVisible;
}

inline
CRect CPictureObj::GetStartRect()
{
	return m_startRect;
}
inline
void CPictureObj::SetStartRect(const CRect pos)
{
	m_startRect = pos;
}

inline
CRect CPictureObj::GetPosition()
{
	return m_rcPosition;
}
inline
void CPictureObj::SetPosition(const CRect pos)
{
	m_rcPosition = pos;
}
inline
BOOL CPictureObj::GetSelected( void ) const
{
	return m_bSelected;
}
inline
void CPictureObj::SetSelected( BOOL bValue )
{
	m_bSelected = bValue;
}
inline
HorzAlign CPictureObj::GetHorzAlign( void ) const		
{ 
	return m_eHorizAlign; 
}
inline
VertAlign CPictureObj::GetVertAlign( void ) const		
{ 
	return m_eVertAlign; 
}
inline
void CPictureObj::SetHorzAlign( HorzAlign eHorizAlign )			
{ 
	m_eHorizAlign = eHorizAlign; 
}
inline
void CPictureObj::SetVertAlign( VertAlign eVertAlign )			
{ 
	m_eVertAlign = eVertAlign; 
}
inline
void CPictureObj::ReleasePicture()
{
	if(m_pPict != NULL)
		m_pPict->Release();
	m_pPict = NULL;
}
inline
BOOL CPictureObj::IsValidPicture()
{
	return m_pPict != NULL;
}

class CPictureObjList : public CTypedPtrList<CObList, CPictureObj*>
{
public:
	~CPictureObjList();

	void RemoveAndDestroy();
	void DeselectAll();
	
	CSize ComputeMaxSize();				// in himetric
	CSize ComputeMaxSize(CDC* pDC);		// in pixel
	CSize ComputeMaxSize(CWnd* pWnd);	// in pixel
	CRect GetRect();					// in pixel

	CPictureObj* FindSelected();
	CPictureObj* ObjectAt(const CPoint& pt);
	bool Remove(CPictureObj* pObj);
	void Draw(CDC* pDC);

	// Streaming
	HRESULT WriteToStream(IStream* pStream);
	HRESULT ReadFromStream(IStream* pStream);
	BOOL	WriteToStorage(LPSTORAGE lpRootStg);
	BOOL	ReadFromStorage(LPSTORAGE lpRootStg);

};


void HimetricToPixels(CDC*pDC,CSize& size);
void HimetricToPixels(CWnd*pWnd,CSize& size);


#endif // !defined(AFX_PICTUREOBJ_H__59DA2CBD_8B9F_431C_85BE_5C17F2529AAD__INCLUDED_)
