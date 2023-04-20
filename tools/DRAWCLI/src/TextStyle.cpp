#include "stdafx.h"
#include "TextStyle.h"
#include "XStr.h"

CTextStyle::CTextStyle() : m_styleid(0),m_font(NULL)
{
	SetDefaultStyle();
};

CTextStyle::~CTextStyle()
{
	if(m_font) ::DeleteObject(m_font);
}

CTextStyle::CTextStyle(CString &stylename,int styleid) : m_stylename(stylename),m_styleid(styleid)
{
	SetDefaultStyle();
}

CTextStyle::CTextStyle(CTextStyle &copy)
{
	operator=(copy);		
};

CTextStyle& CTextStyle::operator=(CTextStyle &right)
{
	m_styleid  = right.m_styleid;
	m_stylename = right.m_stylename;
	memcpy(&m_lf,&right.m_lf,sizeof(LOGFONT));
	m_txtclr = right.m_txtclr;
	m_halign = right.m_halign ;
	m_valign = right.m_valign;	
	m_font = NULL;
	m_TextAngle = right.m_TextAngle;
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

}
void CTextStyle::Serialize(CArchive &ar)
{
	char name[MAX_STYLE_NAME+1];
	memset(name,0,sizeof(name));
	int enumInt;
	CObject::Serialize(ar);
		if (ar.IsStoring())
	{
		ar << m_styleid;
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
		ar >> m_styleid;
		ar.Read(name,sizeof(name));
		m_stylename = name;
		ar.Read(&m_lf,sizeof(LOGFONT));
        ar >> m_txtclr;
		ar >> enumInt;
		m_halign = (HOR_ALIGN)enumInt;
		ar >> enumInt;
		m_valign = (VER_ALIGN)enumInt;
		
		}
}

HFONT CTextStyle::RealizeStyle(bool bChanged/* = false*/)
{
	if(bChanged && m_font)
	{
		::DeleteObject(m_font); //i hope deleteing stock object wont cause error
		m_font = NULL;
	}

	if(m_font == NULL)
	{		
		m_font = ::CreateFontIndirect(&m_lf);
		if(!m_font)
			::GetStockObject(DEFAULT_GUI_FONT);
	}
	return m_font;
}

#ifndef FOR_CATTLEBASE

void CTextStyle::SerializeToXml(xercesc::DOMElement * pParentElement)
{

	xercesc::DOMDocument* pDOMDoc = pParentElement->getOwnerDocument();	

	pParentElement->setAttribute(X("id"),X(m_styleid));

	xercesc::DOMElement* pNameElement = pDOMDoc->createElement(X("name"));
	pNameElement->appendChild(pDOMDoc->createTextNode(X(m_stylename)));
	pParentElement->appendChild(pNameElement);

	xercesc::DOMElement* pTxtColorElement = pDOMDoc->createElement(X("textColor"));
	pTxtColorElement->appendChild(pDOMDoc->createTextNode(X(m_txtclr)));
	pParentElement->appendChild(pTxtColorElement);

	xercesc::DOMElement* pHorAlignElement = pDOMDoc->createElement(X("horAlign"));
	
	char *pszHorAlignXML = NULL;
	
	switch(m_halign)
	{
	default:
	case LEFT:
		pszHorAlignXML = "LEFT";
		break;
	case RIGHT:
		pszHorAlignXML = "RIGHT";
		break;
	case CENTER:
		pszHorAlignXML = "CENTER";
		break;
	}

	pHorAlignElement->appendChild(pDOMDoc->createTextNode(X(pszHorAlignXML)));
	pParentElement->appendChild(pHorAlignElement);

	xercesc::DOMElement* pVerAlignElement = pDOMDoc->createElement(X("verAlign"));
	
	char *pszVerAlignXML = NULL;
	
	switch(m_valign)
	{
	default:
	case TOP:
		pszVerAlignXML = "TOP";
		break;
	case BOTTOM:
		pszVerAlignXML = "BOTTOM";
		break;
	case VCENTER:
		pszVerAlignXML = "CENTER";
		break;
	}
	

	pVerAlignElement->appendChild(pDOMDoc->createTextNode(X(pszVerAlignXML)));
	pParentElement->appendChild(pVerAlignElement);


	xercesc::DOMElement* pFontElement = pDOMDoc->createElement(X("font"));
	

	xercesc::DOMElement* pFontHeightElement = pDOMDoc->createElement(X("height"));
	pFontHeightElement->appendChild(pDOMDoc->createTextNode(X(m_lf.lfHeight)));
	pFontElement->appendChild(pFontHeightElement);

	xercesc::DOMElement* pFontFaceElement = pDOMDoc->createElement(X("face"));
	pFontFaceElement->appendChild(pDOMDoc->createTextNode(X(m_lf.lfFaceName)));
	pFontElement->appendChild(pFontFaceElement);

	xercesc::DOMElement* pFontBoldElement = pDOMDoc->createElement(X("bold"));
	pFontBoldElement->appendChild(pDOMDoc->createTextNode(X(m_lf.lfWeight != FW_NORMAL)));
	pFontElement->appendChild(pFontBoldElement);

	xercesc::DOMElement* pFontItalicElement = pDOMDoc->createElement(X("italic"));
	pFontItalicElement->appendChild(pDOMDoc->createTextNode(X(m_lf.lfItalic == TRUE)));
	pFontElement->appendChild(pFontItalicElement);

	xercesc::DOMElement* pFontUnderlineElement = pDOMDoc->createElement(X("underline"));
	pFontUnderlineElement->appendChild(pDOMDoc->createTextNode(X(m_lf.lfUnderline == TRUE)));
	pFontElement->appendChild(pFontUnderlineElement);

	xercesc::DOMElement* pFontStriekeoutElement = pDOMDoc->createElement(X("strikeout"));
	pFontStriekeoutElement->appendChild(pDOMDoc->createTextNode(X(m_lf.lfStrikeOut == TRUE)));
	pFontElement->appendChild(pFontStriekeoutElement);


	pParentElement->appendChild(pFontElement);
}



CStyleCombo::CStyleCombo(CStyleList &StyleList) : List(StyleList) 
{
}
void CStyleCombo::InitStyleCombo(CTextStyle *preSel)
{
	int item;
	CTextStyle *pStyle;
	POSITION pos = List.GetHeadPosition();
	while(pos)
	{
		pStyle = List.GetNext(pos);
		item = AddString(pStyle->m_stylename);
		SetItemData(item,(DWORD_PTR)pStyle);
	}
	if(preSel)
		SelectString(-1,preSel->m_stylename);
}

CTextStyle* CStyleCombo::GetSelStyle()
{
	int nSel = GetCurSel();
	if(nSel < 0) return NULL;
	return (CTextStyle*)GetItemData(nSel);
}



CVAlignCombo::CVAlignCombo()
{
}

void CVAlignCombo::InitVAlignCombo(VER_ALIGN valign)
{
	struct VALIGNNAMES
	{
		char szAlignName[MAX_ITEM_NAME];
		VER_ALIGN eAlign;
	} tItems[3] =
	{
		{"TOP",TOP},
		{"BOTTOM",BOTTOM},
		{"VCENTER",VCENTER}
	};
	ASSERT(!(GetStyle() & CBS_SORT));

	ResetContent();
	int item,def = 0;
	for(int i = 0;i < 3;i++)
	{
		item = AddString(tItems[i].szAlignName);
		SetItemData(item,tItems[i].eAlign);
		if(valign == tItems[i].eAlign) def = item;
    }
	SetCurSel(def);

}
VER_ALIGN CVAlignCombo::GetSelVAlign()
{
	return (VER_ALIGN)GetItemData(GetCurSel());

}


bool CVAlignCombo::SelectAlign(VER_ALIGN align)
{

	for(int i = 0;i < GetCount();i++)
	{
		if((VER_ALIGN)GetItemData(i) == align)
		{
			SetCurSel(i);
			return true;
		}
	}
	return false;
}




CHAlignCombo::CHAlignCombo()
{

}

void CHAlignCombo::InitHAlignCombo(HOR_ALIGN halign)
{
	struct HALIGNNAMES
	{
		char szAlignName[MAX_ITEM_NAME];
		HOR_ALIGN eAlign;
	} tItems[3] =
	{
		{"LEFT",LEFT},
		{"RIGHT",RIGHT},
		{"CENTER",CENTER}
	};
	ASSERT(!(GetStyle() & CBS_SORT));

	ResetContent();
	int item,def = 0;
	for(int i = 0;i < 3;i++)
	{
		item = AddString(tItems[i].szAlignName);
		SetItemData(item,tItems[i].eAlign);
		if(halign == tItems[i].eAlign) def = item;
    }
	SetCurSel(def);

}
	
HOR_ALIGN CHAlignCombo::GetSelHAlign()
{
	return (HOR_ALIGN)GetItemData(GetCurSel());
}

bool CHAlignCombo::SelectAlign(HOR_ALIGN align)
{

	for(int i = 0;i < GetCount();i++)
	{
		if((HOR_ALIGN)GetItemData(i) == align)
		{
			SetCurSel(i);
			return true;
		}
	}
	return false;
}


#endif


