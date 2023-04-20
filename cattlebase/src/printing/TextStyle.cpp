#include "stdafx.h"
#include "TextStyle.h"

CTextStyle::CTextStyle() : m_Id(0),m_font(NULL)
{
	SetDefaultStyle();
};

CTextStyle::~CTextStyle()
{
	if(m_font) ::DeleteObject(m_font);
}

CTextStyle::CTextStyle(const CString &sStyleName,UINT styleId) : m_stylename(sStyleName),m_Id(styleId)
{
	SetDefaultStyle();
}

CTextStyle::CTextStyle(const CTextStyle &copy)
{
	operator=(copy);		
};

CTextStyle& CTextStyle::operator=(const CTextStyle &right)
{
	m_Id  = right.m_Id;
	m_stylename = right.m_stylename;
	memcpy(&m_lf,&right.m_lf,sizeof(LOGFONT));
	m_txtclr = right.m_txtclr;
	m_halign = right.m_halign ;
	m_valign = right.m_valign;	
	m_font = NULL;
	m_TextAngle = right.m_TextAngle;

	RealizeStyle();

	return *this;
}

void CTextStyle::SetDefaultStyle()
{
	m_halign = CENTER;
	m_txtclr = DEFAULT_COLOR;
	m_valign = VCENTER;
	SetDefaultFont();
	m_TextAngle = 0;
}
void CTextStyle::SetDefaultFont()
{
	// define the logical parameters for the default font
	m_lf.lfHeight = -DEFAULT_FONT_SIZE; // size 8
	m_lf.lfWidth = 0;
	m_lf.lfEscapement = 0;
	m_lf.lfOrientation = 0;
	m_lf.lfWeight = FW_NORMAL;
	m_lf.lfItalic = 0;
	m_lf.lfUnderline = 0;
	m_lf.lfStrikeOut = 0;
	m_lf.lfCharSet = DEFAULT_CHARSET;
	m_lf.lfOutPrecision = OUT_STRING_PRECIS;
	m_lf.lfClipPrecision = CLIP_STROKE_PRECIS;
	m_lf.lfQuality = DEFAULT_QUALITY;
	m_lf.lfPitchAndFamily = FW_DONTCARE | VARIABLE_PITCH;
	_tcscpy(m_lf.lfFaceName, _T("Arial"));

	RealizeStyle();

}
void CTextStyle::Serialize(CArchive &ar)
{
	char name[MAX_STYLE_NAME+1];
	memset(name,0,sizeof(name));
	int enumInt;
	CObject::Serialize(ar);
	
	if (ar.IsStoring())
	{
		ar << m_Id;
		_snprintf(name,MAX_STYLE_NAME,"%s",(LPCSTR)m_stylename);
           ar.Write(name,sizeof(name));
		ar.Write(&m_lf,sizeof(LOGFONT));
		
		ar << m_txtclr;
		enumInt = m_halign;
		ar << enumInt;
		enumInt = m_valign;
		ar << enumInt;
	}
	else
	{
		ar >> m_Id;
		ar.Read(name,sizeof(name));
		m_stylename = name;
		ar.Read(&m_lf,sizeof(LOGFONT));
        ar >> m_txtclr;
		ar >> enumInt;
		m_halign = (HOR_ALIGN)enumInt;
		ar >> enumInt;
		m_valign = (VER_ALIGN)enumInt;

		RealizeStyle();		
	}
}

void CTextStyle::RealizeStyle()
{
	if(m_font)
	{
		::DeleteObject(m_font); 
		m_font = NULL;
	}

	if(m_font == NULL)
	{		
		m_font = ::CreateFontIndirect(&m_lf);
		if(!m_font)
			::GetStockObject(DEFAULT_GUI_FONT);
	}	
}
void CTextStyle::ResetId()
{
	m_Id = 0;
}

void CTextStyle::SetStyleName(const CString& sStyleName)
{
	m_stylename = sStyleName;
}

UINT CTextStyle::GetId() const
{
	return m_Id;
}

HFONT CTextStyle::GetFont()
{
	return m_font;
}

void CTextStyle::SetStyleFont(const CString& sFaceName,int iHeight)
{
	strncpy(m_lf.lfFaceName,sFaceName,LF_FACESIZE);
	m_lf.lfHeight = iHeight;
	RealizeStyle();
}

bool CTextStyle::IsUserDefined() const
{
	return GetId() == USERDEF_STYLEID;
}

COLORREF CTextStyle::GetTextColor() const
{
	return m_txtclr;
}
VER_ALIGN CTextStyle::GetVerAlign() const
{	
	return m_valign;
}
HOR_ALIGN CTextStyle::GetHorAlign() const
{
	return m_halign;
}