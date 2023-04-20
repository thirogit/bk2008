// cmNumericTextCell.h
//

#if !defined(_CCMSMARTNUMERICTEXTCELL_H_)
#define _CCMSMARTNUMERICTEXTCELL_H_

#include "cmBaseCell.h"
#include "cmString.h"

class cmSmartNumericEdit;

class cmSmartNumericTextCell: public cmBaseCell
{
public:
	cmSmartNumericTextCell();
	~cmSmartNumericTextCell();

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
	
	void SetMinValue(long minValue);
	long GetMinValue() const;

	void SetMaxValue(long maxValue);
	long GetMaxValue() const;

	void SetMinMaxCheckEnable(bool enable);
	bool GetMinMaxCheckEnable() const;

	void SetValue(long value, bool redraw = true);
	long GetValue();

	void SetReadOnly(bool readOnly);
	bool IsReadOnly() const;

	void SetHexDisplayMode(bool hexDisplay);
	bool GetHexDisplayMode() const;

protected:

	long ValidateValue(long value);
	cmString ValidateText(const cmString& text);

	cmString FormatValue(long value, bool hexDisplay = false);
	CRect GetTextBoxArea();
	void StoreData();

	cmSmartNumericEdit* editBox_;

	long minValue_;
	long maxValue_;
	int precision_;
	bool minMaxCheckEnable_;
	long value_;
	bool readOnly_;
	bool hexDisplayMode_;
	CFont font_;
};

#endif //!defined(_CCMSMARTNUMERICTEXTCELL_H_)