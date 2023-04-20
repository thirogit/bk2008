// cmCellDescriptor.h
//	

#include "stdafx.h"
#include "cmCellDescriptor.h"
	
cmCellDescriptor::cmCellDescriptor(const cmString& name)
{
	name_ = name;
}

cmCellDescriptor::~cmCellDescriptor()
{}

void cmCellDescriptor::SetName(const cmString& name)
{
	name_ = name;
}

cmString cmCellDescriptor::GetName() const
{
	return name_;
}