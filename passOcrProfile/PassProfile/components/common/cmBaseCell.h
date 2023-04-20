// AsBaseCell.h: interface for the cmBaseCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_ASBASECELL_H_)
#define _ASBASECELL_H_

#include "cmFontInfo.h"
#include "ICmCellListener.h"
#include "cmString.h"
#include "cmCellEnum.h"
#include "cmRendererSet.h"
#include "cmCellBaseRenderer.h"

class cmCellBaseRenderer;
class cmCellDescriptor;
class cmBaseCellInfo;

class cmBaseCell: public CObject  
{
public:
	cmBaseCell();
	cmBaseCell(bool createStockRenderers);
	virtual ~cmBaseCell();

	virtual void SetDrawArea(const CRect& drawArea);
	CRect GetDrawArea() const;

	virtual CSize GetMinSize();

	virtual void Draw(CDC* pDC, const CPoint& mousePos);
	
	virtual bool HitTest(const CPoint& pt);
	virtual bool IsPrintable(TCHAR c);
	virtual bool IsValidChar(TCHAR c, const cmString& text);

	//void OnMouseMove(const CPoint& pt);

	virtual void SetVisible(bool visible);
	bool IsVisible() const;

	bool IsHidden();

	virtual void SetText(const cmString& text, bool redraw = true);
	virtual cmString GetText();

	void SetParentWindow(CWnd* parentWnd);

	virtual bool ProcessMessage(DWORD msg, WPARAM wParam, LPARAM lParam, const CPoint& pt, const CRect& parentRect, bool& needRedraw);

	virtual void KillFocus(bool storeData = true, bool redraw = true);
	virtual void SetFocus(bool redraw);
	virtual bool HasFocus();

	virtual void SetSelection(bool redraw = true);
	virtual void KillSelection(bool redraw = true);
	virtual bool IsSelected();

	virtual bool IsMouseOver() const;

	void SetCellListener(ICmCellListener* listener);

	void SetDescriptor(cmCellDescriptor* desc);
	cmCellDescriptor* GetDescriptor();

	void SetType(enCmCellType type);
	enCmCellType GetType() const;

	virtual void SetRenderer(enCmCellStatus status, cmCellBaseRenderer* rend);
	cmCellBaseRenderer* GetRenderer(enCmCellStatus status);
	cmCellBaseRenderer* GetCurrentRenderer();

	void SetRenderingMode(enCmRenderingMode mode);
	enCmRenderingMode GetRenderingMode() const;

	static TCHAR VirtualKeyCode2Ascii(WPARAM wParam, LPARAM lParam);
	
	void SetUserId(DWORD id);
	DWORD GetUserId() const;

	void SetReadOnly(bool readOnly);
	bool GetReadOnly() const;

	void ReleaseCustomRenderers();

	void SetInfo(cmBaseCellInfo* info);
	cmBaseCellInfo* GetInfo();

protected:

	virtual void Initialize(bool createStockRenderers);
	void ReleaseStockRenderers();

	cmString text_;

	CRect drawArea_;
	bool visible_;

	CWnd* parentWnd_;

	bool hasFocus_;
	bool isSelected_;
	bool mouseOver_;
	cmCellDescriptor* descriptor_;

	ICmCellListener* cellListener_;
	enCmCellType type_;

	// renderers:
	//	0:Normal,
	//	1:Normal+Focused,
	//  2:MouseOver,
	//	3:MouseOver+Focused
	//
	cmCellBaseRenderer* stockRenderers_[4];
	cmCellBaseRenderer* customRenderers_[4];	
	
	enCmRenderingMode renderingMode_;
	UINT userId_;

	bool readOnly_;

	cmBaseCellInfo* info_;
};

#endif // !defined(_ASBASECELL_H_)
