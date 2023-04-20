// AsBinaryWildCardsCell.h: interface for the cmBinaryWildCardsCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMBINARYWILDCARDSCELL_H_)
#define _CMBINARYWILDCARDSCELL_H_

#include <vector>
#include "cmBaseCell.h"
#include "cmString.h"

using namespace std;

class cmBinaryWildCardsCell: public cmBaseCell 
{
public:
	cmBinaryWildCardsCell();
	virtual ~cmBinaryWildCardsCell();

	enum enCmBitStatus {
		enCmBitStatus_None = 0,
		enCmBitStatus_Checked,
		enCmBitStatus_Unchecked,
		enCmBitStatus_Unknown
	};

	virtual void SetDrawArea(const CRect& drawArea);

	virtual CSize GetMinSize();

	virtual void Draw(CDC* pDC, const CPoint& mousePos);

	virtual bool HitTest(const CPoint& pt);

	virtual void SetBinaryString(const cmString& checkedText, bool redraw = true);
	virtual cmString GetBinaryString() const;

	virtual void SetValue(long value, bool redraw = true);
	virtual long GetValue() const;

	virtual bool ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	virtual void KillFocus(bool storeData = true, bool redraw = true);
	virtual void SetFocus(bool redraw = true);

	virtual void SetBitCount(long bitCount);
	virtual long GetBitCount() const;

	virtual void SetReadOnly(bool readOnly);
	virtual bool IsReadOnly() const;

	virtual void SetBitStatus(long bitIndex, enCmBitStatus status, bool redraw = true);
	virtual enCmBitStatus GetBitStatus(long bitIndex) const;

protected:

	cmString binaryString_;
	long value_;

	vector<CRect> bitRect_;
	vector<enCmBitStatus> bitStatus_;
	bool readOnly_;
	long bitCount_;

};

#endif // !defined(_CMBINARYWILDCARDSCELL_H_)