// CmOptionGroup.h: interface for the CmOptionGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CM_OPTIONGROUP_H_)
#define _CM_OPTIONGROUP_H_

#include <map>
#include <vector>
#include "cmOptionRow.h"
#include "cmBaseCell.h"
#include "cmCellEnum.h"
#include "cmGroupHeaderCell.h"
#include "cmString.h"
#include "ICmOptionRowListener.h"

using namespace std;

typedef map<cmString, cmOptionRow*> OptionRowMap;

class ICmOptionGroupListener;
class cmRendererSet;

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
		enCmOptionGroupHitTest_Rows,
		enCmOptionGroupHitTest_Grid
	};

	// implementation of ICmOptionRowListener
	//
	void OnRowSelection(cmOptionRow* selectedRow, bool redraw = true);

	virtual void Draw(CDC* pDC, const CPoint& mousePos);

	void SetCaption(const cmString& caption);
	cmString GetCaption() const;

	void SetDescription(const cmString& description);
	cmString GetDescription() const;

	OptionRowMap* GetRows();

	virtual cmOptionRow* AddRow(const cmString& name, const cmString& caption, enCmCellType valueCellType);
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

	//void SetTitleRowBkColor(COLORREF bkColor);
	//COLORREF GetTitleRowBkColor() const;

	void SetPropertyRowsBkColor(COLORREF bkColor);
	COLORREF GetPropertyRowsBkColor() const;

	void SetVerticalSplitterPos(int pos);
	int GetVerticalSplitterPos() const;

	enCmOptionGroupHitTest HitTest(const CPoint& pt, int& itemIndex);

	void RegisterListener(ICmOptionGroupListener* listener);

	void SetParentWindow(CWnd* parentWnd);

	bool ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	// row selection management
	//
	cmBaseCell* MoveSelection(	int shift, 
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

/*
	void GetSelectionInfo(cmString& caption, 
						  cmString& description,
						  cmString& tooltipText,
						  UINT& tooltipIconId);
*/
	cmOptionRow* GetSelectedRow();

	void SetHeaderRendererSet(enCmCellStatus status, cmRendererSet* renderer);

	void SetSorted(bool sorted);
	bool GetSorted() const;

	void SelectRow(int index, bool redraw = false);

protected:

	void OnExpandButtonPressed(bool redraw = true);

	virtual void UpdateHeaderDrawArea(const CRect& drawArea);

	virtual void DrawExpandButton(CDC* pDC);
	virtual void DrawRows(CDC* pDC, const CPoint& mousePos);
	virtual void DrawGrid(CDC* pDC);

	cmGroupHeaderCell header_;
	
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
	//COLORREF titleRowBkColor_;
	COLORREF propertyRowsBkColor_;
	COLORREF verticalStripBkColor_;
	COLORREF cellBkColor_;

	bool expanded_;

	ICmOptionGroupListener* listener_;
	CWnd* parentWnd_;

	bool headerIsSelected_;

	bool sorted_;

	vector<cmRendererSet*> headerRenderers_[4];
};

#endif // !defined(_CM_OPTIONGROUP_H_)
