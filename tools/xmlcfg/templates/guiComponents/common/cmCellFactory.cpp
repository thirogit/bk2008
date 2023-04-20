// cmCellFactory.cpp: implementation of the cmCellFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmCellFactory.h"
#include "cmBaseCell.h"
#include "cmGenericTextCell.h"
#include "cmCheckBoxCell.h"
#include "cmComboBoxCell.h"
#include "cmNumericTextCell.h"
#include "cmHexadecimalTextCell.h"
#include "cmSmartNumericTextCell.h"
#include "cmHeaderCell.h"
#include "cmBaseCellInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
set<cmBaseCellInfo*> cmCellFactory::infoList_;

cmBaseCell* cmCellFactory::CreateCell(enCmCellType type)
{
	cmBaseCell* newCell = NULL;

	switch (type) {
		case enCmCellType_Caption:
			newCell = new cmBaseCell;
			break;

		case enCmCellType_TextBox:
			newCell = new cmGenericTextCell;
			break;

		case enCmCellType_NumTextBox:
			newCell = new cmNumericTextCell;
			break;

		case enCmCellType_CheckBox:
			newCell = new cmCheckBoxCell;
			break;

		case enCmCellType_ComboBox:
			newCell = new cmComboBoxCell;
			break;

		case enCmCellType_HexTextBox:
			newCell = new cmHexadecimalTextCell;
			break;

		case enCmCellType_SmartNumTextBox:
			newCell = new cmSmartNumericTextCell;
			break;

		case enCmCellType_Header:
			newCell = new cmHeaderCell;
			break;
	}

	return newCell;
}

cmBaseCellInfo* cmCellFactory::CreateCellInfo()
{
	cmBaseCellInfo* newCellInfo;

	newCellInfo = new cmBaseCellInfo;

	infoList_.insert(newCellInfo);

	return newCellInfo;
}

void cmCellFactory::ReleaseCellInfo(cmBaseCellInfo* info)
{
	set<cmBaseCellInfo*>::iterator it;

	it = infoList_.find(info);
	if (it != infoList_.end()) {
		delete *it;
		infoList_.erase(it);
	}

}
	