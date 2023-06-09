#if !defined(__PICTURE_H__)
#define __PICTURE_H__

class CPicture  
{
public:
	CPicture();
	virtual ~CPicture();

//	bool			Load(CString sResourceType, CString sResource);
//	bool			Load(CString sFileName);
	bool			Draw(CDC* pDC);
	bool			Draw(CDC* pDC, CPoint Pos);
	bool			Draw(CDC* pDC, CPoint Pos, CSize Size);
	bool			Draw(CDC* pDC, double nSizeRatio);
	bool			Draw(CDC* pDC, CPoint Pos, double nSizeRatio);
	CSize			GetSize(CDC* pDC);
	bool			Serialize(CArchive& ar);
//	void			Copy(CPicture &copy);
	bool			WriteToStream(IStream *pStream);
	bool			ReadFromStream(IStream *pStream);
private:
//	static	bool	GetResource(LPSTR lpName, LPSTR lpType, void* pResource, int& nBufSize);
	void			UnLoad();
//	bool			LoadFromBuffer(BYTE* pBuff, int nSize);
	bool			Draw(CDC* pDC, int x, int y, int cx, int cy);
	
	IPicture*		m_pPicture;
	enum
	{
		HIMETRIC_INCH	= 2540
	};
	
};

#endif // !defined(AFX_PICTURE_H__6098A4C3_D6D5_4711_BC7B_1595F459B480__INCLUDED_)
