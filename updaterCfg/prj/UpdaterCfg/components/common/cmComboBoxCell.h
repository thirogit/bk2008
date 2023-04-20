// cmComboBoxCell.h
//

#if !defined(_CASCOMBOBOXCELL_H_)
#define _CASCOMBOBOXCELL_H_

#include <map>
#include "cmBaseCell.h"
#include "cmString.h"

using namespace std;

class cmComboBox;

class cmComboBoxCell: public cmBaseCell
{
public:
	cmComboBoxCell();
	~cmComboBoxCell();

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
	
	virtual void AddItem(const cmString& str, int key);
	virtual void SelectItem(const cmString& caption);
	virtual void SelectItem(int key);

	virtual int GetSelectedItemKey();
	virtual cmString GetSelectedItemCaption();

	virtual cmString GetText();
	virtual void SetSelection(bool redraw = true);
	virtual void KillSelection(bool redraw = true);

	virtual void DeleteAllItems();
	
protected:
	
	void DrawComboButton(CDC* pDC);
	void StoreData();
	CRect GetComboBoxArea();
	void ReleaseCombo();

	cmComboBox* comboBox_;
	CFont font_;
	
	int selKey_;
	int oldSelKey_;
	cmString oldText_;
	map<int, cmString> items_;

	cmFontInfo fontInfo_;
};

#endif //!defined(_CASCOMBOBOXCELL_H_)