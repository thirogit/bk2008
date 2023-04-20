// cmTreeNodeDescriptor.cpp: implementation of the cmTreeNodeDescriptor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmTreeNodeDescriptor.h"

/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmTreeNodeDescriptor::cmTreeNodeDescriptor():
	bkColor_(::GetSysColor(COLOR_WINDOW)),
	txtColor_(::GetSysColor(COLOR_WINDOWTEXT)),
	fontName_(_T("tahoma")),
	fontSize_(12),
	fontBold_(false),
	transparent_(true),
	tooltipDescriptionIcon_(0),
	tooltipExtraMsgIcon_(0),
	hNode_(NULL)
{
	
}

cmTreeNodeDescriptor::~cmTreeNodeDescriptor()
{

}

void cmTreeNodeDescriptor::SetNodeHeight(int h)
{
	nodeHeight_ = h;
}

int cmTreeNodeDescriptor::GetNodeHeight() const
{
	return nodeHeight_;
}

void cmTreeNodeDescriptor::SetBkColor(COLORREF bkColor)
{
	bkColor_ = bkColor;
}

COLORREF cmTreeNodeDescriptor::GetBkColor() const
{
	return bkColor_;
}

void cmTreeNodeDescriptor::SetTextColor(COLORREF txtColor)
{
	txtColor_ = txtColor;
}

COLORREF cmTreeNodeDescriptor::GetTextColor() const
{
	return txtColor_;
}

void cmTreeNodeDescriptor::SetFontName(const cmString& name)
{
	fontName_ = name;
}

cmString cmTreeNodeDescriptor::GetFontName() const
{
	return fontName_;
}

void cmTreeNodeDescriptor::SetFontSize(int size)
{
	fontSize_ = size;
}

int cmTreeNodeDescriptor::GetFontSize() const
{
	return fontSize_;
}


void cmTreeNodeDescriptor::SetFontBold(bool bold)
{
	fontBold_ = bold;
}

bool cmTreeNodeDescriptor::GetFontBold() const
{
	return fontBold_;
}

void cmTreeNodeDescriptor::SetTransparent(bool transparent)
{
	transparent_ = transparent;
}

bool cmTreeNodeDescriptor::GetTransparent() const
{
	return transparent_;
}
	
void cmTreeNodeDescriptor::SetTooltipDescription(const cmString& desc)
{
	tooltipDescription_ = desc;
}

cmString cmTreeNodeDescriptor::GetTooltipDescription() const
{
	return tooltipDescription_;
}

void cmTreeNodeDescriptor::SetTooltipDescriptionIcon(UINT icon)
{
	tooltipDescriptionIcon_ = icon;
}

UINT cmTreeNodeDescriptor::GetTooltipDescriptionIcon() const
{
	return tooltipDescriptionIcon_;
}

void cmTreeNodeDescriptor::SetTooltipExtraMsg(const cmString& msg)
{
	tooltipExtraMsg_ = msg;
}

cmString cmTreeNodeDescriptor::GetTooltipExtraMsg() const
{
	return tooltipExtraMsg_;
}

void cmTreeNodeDescriptor::SetTooltipExtraMsgIcon(UINT icon)
{
	tooltipExtraMsgIcon_ = icon;
}

UINT cmTreeNodeDescriptor::GetTooltipExtraMsgIcon() const
{
	return tooltipExtraMsgIcon_;
}

void cmTreeNodeDescriptor::SetTreeIcon(enCmTreeIcon iconType, UINT icon)
{
	treeIcons_[iconType] = icon;
}

UINT cmTreeNodeDescriptor::GetTreeIcon(enCmTreeIcon iconType) const
{
	cmTreeIconMap::const_iterator it;
	UINT ret = 0;

	it = treeIcons_.find(iconType);
	if (it != treeIcons_.end())
		ret = it->second;
	
	return ret;
}

void cmTreeNodeDescriptor::SetTreeNode(HTREEITEM node)
{
	hNode_ = node;
}

HTREEITEM cmTreeNodeDescriptor::GetTreeNode() const
{
	return hNode_;
}

void cmTreeNodeDescriptor::SetText(const cmString& text)
{
	text_ = text;
}

cmString cmTreeNodeDescriptor::GetText() const
{
	return text_;
}