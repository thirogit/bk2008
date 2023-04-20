// cmNumericTextCell.h
//

#if !defined(_CASNUMERICTEXTCELL_H_)
#define _CASNUMERICTEXTCELL_H_

#include "cmBaseCell.h"
#include "cmString.h"

class cmNumericEdit;

class cmNumericTextCell: public cmBaseCell
{
public:
	cmNumericTextCell();
	~cmNumericTextCell();

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
	
	void SetMinValue(double minValue);
	double GetMinValue() const;

	void SetMaxValue(double maxValue);
	double GetMaxValue() const;

	void SetPrecision(int precision);
	int GetPrecision();

	void SetMinMaxCheckEnable(bool enable);
	bool GetMinMaxCheckEnable() const;

	void SetValue(double value, bool redraw = true);
	double GetValue();

	void SetReadOnly(bool readOnly);
	bool IsReadOnly() const;

protected:

	double ValidateValue(double value);
	cmString FormatValue(double value);
	CRect GetTextBoxArea();
	void StoreData();

	cmNumericEdit* editBox_;

	double minValue_;
	double maxValue_;
	int precision_;
	bool minMaxCheckEnable_;
	double value_;
	bool readOnly_;

	CFont font_;
};

#endif //!defined(_CASNUMERICTEXTCELL_H_)