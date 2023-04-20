// cmTreeNodeDescriptor.h: interface for the cmTreeNodeDescriptor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_CMTREENODEDESCRIPTOR_H_)
#define _CMTREENODEDESCRIPTOR_H_

#include "cmString.h"
#include "cmTreeEnum.h"
#include <map>

using namespace std;

typedef map<enCmTreeIcon, UINT> cmTreeIconMap;


class cmTreeNodeDescriptor  
{
public:
	cmTreeNodeDescriptor();
	virtual ~cmTreeNodeDescriptor();

	void SetNodeHeight(int h);
	int GetNodeHeight() const;

	void SetBkColor(COLORREF bkColor);
	COLORREF GetBkColor() const;

	void SetTextColor(COLORREF txtColor);
	COLORREF GetTextColor() const;

	void SetFontName(const cmString& name);
	cmString GetFontName() const;

	void SetFontSize(int size);
	int GetFontSize() const;

	void SetFontBold(bool bold);
	bool GetFontBold() const;

	void SetTransparent(bool transparent);
	bool GetTransparent() const;

	void SetTooltipDescription(const cmString& desc);
	cmString GetTooltipDescription() const;

	void SetTooltipDescriptionIcon(UINT icon);
	UINT GetTooltipDescriptionIcon() const;

	void SetTooltipExtraMsg(const cmString& msg);
	cmString GetTooltipExtraMsg() const;

	void SetTooltipExtraMsgIcon(UINT icon);
	UINT GetTooltipExtraMsgIcon() const;

	void SetTreeIcon(enCmTreeIcon iconType, UINT icon);
	UINT GetTreeIcon(enCmTreeIcon iconType) const;

	void SetTreeNode(HTREEITEM node);
	HTREEITEM GetTreeNode() const;

	void SetText(const cmString& text);
	cmString GetText() const;

protected:
	
	int nodeHeight_;

	COLORREF bkColor_;
	COLORREF txtColor_;
	bool transparent_;
	cmString text_;

	// font info
	//
	cmString fontName_;
	int fontSize_;
	bool fontBold_;

	// tooltip info
	//
	cmString tooltipDescription_;
	UINT tooltipDescriptionIcon_;
	cmString tooltipExtraMsg_;
	UINT tooltipExtraMsgIcon_;

	cmTreeIconMap treeIcons_;

	HTREEITEM hNode_;
};

#endif // !defined(_CMTREENODEDESCRIPTOR_H_)

