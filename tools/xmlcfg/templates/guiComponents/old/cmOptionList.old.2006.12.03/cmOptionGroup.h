// CmOptionGroup.h: interface for the CmOptionGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CM_OPTIONGROUP_H_)
#define _CM_OPTIONGROUP_H_

#include <map>
#include "cmOptionRow.h"
#include "cmBaseCell.h"
#include "cmFontInfo.h"
#include "cmCellEnum.h"
#include "cmString.h"
#include "ICmOptionRowListener.h"

using namespace std;

typedef map<cmString, cmOptionRow*> OptionRowMap;

class ICmOptionGroupListener;

class cmOptionGroup : public ICmOptionRowListener
{
public:
	cmOptionGroup();
	virtual ~cmOptionGroup();

	enum enCmOptionGroupHitTest {
		enCmOptionGroupHitTest_None = 0,
		enCmOptionGroupHitTest_Expand,
		enCmOptionGroupHitTest_Splitter,
		enCmOptionGroupHitTest_Header,
		enCmOptionGroupHitTest_Rows
	};

	// implementation of ICmOptionRowListener
	//
	void OnRowSelection(cmOptionRow* selectedRow, bool redraw = true);

	void Draw(CDC* pDC);

	void SetCaption(const cmString& caption);
	cmString GetCaption() const;

	void SetDescription(const cmString& description);
	cmString GetDescription() const;

	OptionRowMap* GetRows();

	cmOptionRow* AddRow(const cmString& name, const cmString& caption, enCmCellType valueCellType);
	cmOptionRow* GetRow(const cmString& name);

	void SetDrawArea(const CRect& drawArea);
	CRect GetDrawArea() const;

	void SetTitleRowHeight(int height);
	int GetTitleRowHeight() const;

	void SetPropertyRowsHeight(int height);
	int GetPropertyRowsHeight() const;

	void SetVerticalStripWidth(int width);
	int GetVerticalStripWidth() const;

	void SetBorderColor(COLORREF borderColor);
	COLORREF GetBorderColor() const;

	void SetTitleRowBkColor(COLORREF bkColor);
	COLORREF GetTitleRowBkColor() const;

	void SetPropertyRowsBkColor(COLORREF bkColor);
	COLORREF GetPropertyRowsBkColor() const;

	void SetVerticalSplitterPos(int pos);
	int GetVerticalSplitterPos() const;

	enCmOptionGroupHitTest HitTest(const CPoint& pt);

	void RegisterListener(ICmOptionGroupListener* listener);

	void SetParentWindow(CWnd* parentWnd);

	bool ProcessMessage(DWORD msg, DWORD lParam, DWORD wParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	void SetFontInfo(const cmString& fontName, int fontSize = 10, bool fontBold = false);
	void SetFontInfo(const cmFontInfo& fontInfo);

	// row selection management
	//
	cmBaseCell* MoveSelection(	bool moveUp, 
								bool keepTopSelection, 
								bool keepBottomSelection,
								bool& selectionLost,
								bool redraw = true);
	void SelectBottomRow(bool redraw = true);
	void SelectHeader(bool select, bool redraw = true);
	void ClearSelection(bool redraw = true);
	cmBaseCell* GetHeaderCell();

	cmBaseCell* GetFocusedCell();

	bool IsSelected();

	void GetSelectionInfo(cmString& caption, 
						  cmString& description,
						  cmString& tooltipText,
						  UINT& tooltipIcon);
	cmOptionRow* GetSelectedRow();

	void SetHeaderFontInfo(const cmFontInfo& fontInfo);
	void SetHeaderFontInfo(const cmString& fontName, int fontSize, bool fontBold);

	void SetSorted(bool sorted);
	bool GetSorted() const;

protected:

	void OnExpandButtonPressed(bool redraw = true);


	cmBaseCell header_;
	
	OptionRowMap rows_;

	CRect drawArea_;
	CRect expandRect_;

	int titleRowHeight_;
	int propertyRowsHeight_;
	int verticalStripWidth_;
	int verticalSplitterPos_;

	cmString caption_;
	cmString description_;

	COLORREF borderColor_;
	COLORREF titleRowBkColor_;
	COLORREF propertyRowsBkColor_;
	COLORREF verticalStripBkColor_;

	bool expanded_;

	ICmOptionGroupListener* listener_;
	CWnd* parentWnd_;

	cmFontInfo fontInfo_;

	bool headerIsSelected_;

	bool sorted_;
};

#endif // !defined(_CM_OPTIONGROUP_H_)
