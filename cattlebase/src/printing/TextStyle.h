#ifndef __TEXTSTYLE_H__
#define __TEXTSTYLE_H__

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

#define USERDEF_STYLENAME "USERDEF"
#define USERDEF_STYLEID (-1)

#define MAX_STYLE_NAME 32


class CTextStyle : public CObject
{
public:
	CTextStyle();
	virtual ~CTextStyle();

	CTextStyle(const CString &sStyleName,UINT styleId);
	CTextStyle(const CTextStyle &copy);
	CTextStyle& operator=(const CTextStyle &right);
	
	void SetStyleFont(const CString& sFaceName,int iHeight);
	void SetDefaultStyle();		
	void SetDefaultFont();
	void Serialize(CArchive &ar);

	void ResetId();
	UINT GetId() const;
	void SetStyleName(const CString& sStyleName);
	COLORREF GetTextColor() const;
	VER_ALIGN GetVerAlign() const;
	HOR_ALIGN GetHorAlign() const;
	HFONT	GetFont();
	bool	IsUserDefined() const;
protected:
	void RealizeStyle();
protected:
	int m_Id;
	CString m_stylename;
	LOGFONT m_lf;
	COLORREF m_txtclr;
	HOR_ALIGN m_halign;
	VER_ALIGN m_valign;
	UINT m_TextAngle;
	HFONT m_font;
};

#endif