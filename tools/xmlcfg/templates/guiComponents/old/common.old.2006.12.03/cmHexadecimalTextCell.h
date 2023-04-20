// cmHexadecimalTextCell.h
//

#if !defined(_CASHEXADECIMALTEXTCELL_H_)
#define _CASHEXADECIMALTEXTCELL_H_

#include "cmBaseCell.h"
#include "cmString.h"

class cmHexadecimalEdit;

class cmHexadecimalTextCell: public cmBaseCell
{
public:
	cmHexadecimalTextCell();
	~cmHexadecimalTextCell();

	virtual void SetDrawArea(const CRect& drawArea);
	virtual CSize GetMinSize();
	virtual void Draw(CDC* pDC);
	virtual bool HitTest(const CPoint& pt);
	virtual void SetVisible(bool visible);
	virtual void SetText(const cmString& text);
	
	virtual bool ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	virtual void SetFontInfo(const cmFontInfo& fontInfo);

	virtual void KillFocus(bool storeData = true, bool redraw = true);
	virtual void SetFocus(bool redraw = true);
	
	void SetBitCount(long bitCount);
	long GetBitCount();

	void SetValue(long value, bool redraw = true);
	long GetValue();

	void SetReadOnly(bool readOnly);
	bool IsReadOnly() const;

protected:

	long ValidateValue(long value);
	cmString FormatValue(long value);
	CRect GetTextBoxArea();
	void StoreData();

	cmHexadecimalEdit* editBox_;

	long bitCount_;
	long value_;
	bool readOnly_;

	CFont font_;
};

#endif //!defined(_CASHEXADECIMALTEXTCELL_H_)