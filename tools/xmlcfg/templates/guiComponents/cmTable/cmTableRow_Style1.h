// cmTableRow_Style1.h: interface for the cmTableRow_Style1 class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "cmTableRow.h"
#include "cmRendererFactory.h"

class cmTableRow_Style1  : public cmTableRow
{
public:
	cmTableRow_Style1();
	virtual ~cmTableRow_Style1();

	virtual void SetNumColumns(unsigned int numColumns);
	
	cmBaseCell* SetCellType(unsigned int index, enCmCellType cellType);

	void SetFixedColumnsNumber(unsigned int number);

protected:
	void InitializeCellLookAndFeel(cmBaseCell* cell);
	void InitializeHeaderLookAndFeel(cmBaseCell* cell);

	cmRendererFactory rendFactory_;
};
