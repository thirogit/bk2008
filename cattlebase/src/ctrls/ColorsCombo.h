#if !defined(_COLORSCOMBO_H__INCLUDED_)
#define _COLORSCOMBO_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CColorsCombo : public CComboBox
{

public:
	CColorsCombo();
	virtual ~CColorsCombo();
protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
	DECLARE_MESSAGE_MAP()
};


#endif 
