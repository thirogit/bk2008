// cmCellContainer.h: interface for the cmCellContainer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class cmCellContainer  
{
public:
	cmCellContainer();
	virtual ~cmCellContainer();

	virtual void UpdateCells() = 0;
};
