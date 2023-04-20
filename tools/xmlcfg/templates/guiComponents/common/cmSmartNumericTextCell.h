// cmNumericTextCell.h
//

#if !defined(_CCMSMARTNUMERICTEXTCELL_H_)
#define _CCMSMARTNUMERICTEXTCELL_H_

#include "cmBaseCell.h"
#include "cmString.h"
#include "cmGenericTextCell.h"

class cmSmartNumericEdit;

class cmSmartNumericTextCell: public cmGenericTextCell
{
public:
	cmSmartNumericTextCell();
	~cmSmartNumericTextCell();

	virtual void SetDrawArea(const CRect& drawArea);
	virtual CSize GetMinSize();
	virtual void Draw(CDC* pDC, const CPoint& mousePos);
	virtual bool HitTest(const CPoint& pt);
	virtual void SetVisible(bool visible);
	virtual void SetText(const cmString& text);
	
	virtual bool ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	virtual void SetFontInfo(const cmFontInfo& fontInfo);

	virtual void KillFocus(bool storeData = true, bool redraw = true);
	virtual void SetFocus(bool redraw = true);
	
	void SetMinValue(long minValue);
	long GetMinValue() const;

	void SetMaxValue(long maxValue);
	long GetMaxValue() const;

	void SetMinMaxCheckEnable(bool enable);
	bool GetMinMaxCheckEnable() const;

	void SetValue(long value, bool redraw = true);
	long GetValue();

	void SetHexDisplayMode(bool hexDisplay);
	bool GetHexDisplayMode() const;

	bool IsValidChar(TCHAR c, const cmString& text, int cursorPos);

	void SetDigits(int digits);
	int GetDigits() const;

	void SetHexPrefix(const cmString& prefix);
	cmString GetHexPrefix() const;

protected:

	long ValidateValue(long value);
	cmString ValidateText(const cmString& text);
	bool HexPrefixFound(const cmString& text);

	cmString FormatValue(long value, bool hexDisplay = false);
	CRect GetTextBoxArea();
	void StoreData();

	cmSmartNumericEdit* editBox_;

	long minValue_;
	long maxValue_;
	int precision_;
	int digits_;
	bool minMaxCheckEnable_;
	long value_;
	bool hexDisplayMode_;
	cmString hexPrefix_;

	CFont font_;
	cmFontInfo fontInfo_;
};

#endif //!defined(_CCMSMARTNUMERICTEXTCELL_H_)