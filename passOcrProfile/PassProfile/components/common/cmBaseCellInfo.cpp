// cmBaseCellInfo.cpp: implementation of the cmBaseCellInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmBaseCellInfo.h"
#include "cmCellContainer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cmBaseCellInfo::cmBaseCellInfo():
	extraType_(enCmCellExtraMsgType_None),
	container_(NULL)
{

}

cmBaseCellInfo::~cmBaseCellInfo()
{

}

void cmBaseCellInfo::SetTitle(const cmString& title)
{
	title_ = title;
}

cmString cmBaseCellInfo::GetTitle() const
{
	return title_;
}

void cmBaseCellInfo::SetDescription(const cmString& description)
{
	description_ = description;
}

cmString cmBaseCellInfo::GetDescription() const
{
	return description_;
}

void cmBaseCellInfo::SetExtraMsgType(enCmCellExtraMsgType type)
{
	extraType_ = type;
}

enCmCellExtraMsgType cmBaseCellInfo::GetExtraMsgType() const
{
	return extraType_;
}

void cmBaseCellInfo::SetExtraMsg(const cmString& msg)
{
	extraMsg_ = msg;
}

cmString cmBaseCellInfo::GetExtraMsg() const
{
	return extraMsg_;
}

void cmBaseCellInfo::SetCellContainer(cmCellContainer* container)
{
	container_ = container;
}

cmCellContainer* cmBaseCellInfo::GetCellContainer()
{
	return container_;
}