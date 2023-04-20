#ifndef __TEXTSTYLE_H__
#define __TEXTSTYLE_H__

#ifndef FOR_CATTLEBASE
#include <xercesc/dom/DOM.hpp>
#endif

enum HOR_ALIGN
{
	LEFT = DT_LEFT,
	RIGHT = DT_RIGHT,
	CENTER = DT_CENTER
};

enum VER_ALIGN
{
	TOP = DT_TOP,
	BOTTOM = DT_BOTTOM,
	VCENTER = DT_VCENTER
};


#define DEFAULT_COLOR RGB(0,0,0)
#define DEFAULT_FONT_SIZE (30)
#define MIN_FONT_HEIGHT (2)
#define MAX_FONT_HEIGHT (80)

#define USER_DEF_STYLE_ID (-1)

#define MAX_STYLE_NAME 32


class CTextStyle : public CObject
{
public:
	CTextStyle();
	virtual ~CTextStyle();

	CTextStyle(CString &stylename,int styleid);

	CTextStyle(CTextStyle &copy);

	CTextStyle& operator=(CTextStyle &right);
	
	void SetDefaultStyle();
		
	void SetDefaultFont();

	void Serialize(CArchive &ar);

#ifndef FOR_CATTLEBASE
	void SerializeToXml(xercesc::DOMElement * pParentElement);
#endif

	HFONT RealizeStyle(bool bChanged = false);

	int m_styleid;
	CString m_stylename;
	LOGFONT m_lf;
	COLORREF m_txtclr;
	HOR_ALIGN m_halign;
	VER_ALIGN m_valign;
	UINT m_TextAngle;

	HFONT m_font;
};

typedef CTypedPtrList<CObList, CTextStyle*> CStyleList;


#ifndef FOR_CATTLEBASE
class CStyleCombo : public CComboBox
{
public:
	CStyleCombo(CStyleList &StyleList);
	void InitStyleCombo(CTextStyle *preSel = NULL);
	CTextStyle* GetSelStyle();


protected:
	CStyleList &List;
};

#define MAX_ITEM_NAME 15

class CVAlignCombo : public CComboBox
{
public:
	CVAlignCombo();
	void InitVAlignCombo(VER_ALIGN valign = VCENTER);
	VER_ALIGN GetSelVAlign();
	bool SelectAlign(VER_ALIGN align);
};



class CHAlignCombo : public CComboBox
{
public:
	CHAlignCombo();
	void InitHAlignCombo(HOR_ALIGN halign = CENTER);
	HOR_ALIGN GetSelHAlign();
	bool SelectAlign(HOR_ALIGN align);
private:

};

#endif

#endif