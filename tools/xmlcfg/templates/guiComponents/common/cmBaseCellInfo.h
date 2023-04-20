// cmBaseCellInfo.h: interface for the cmBaseCellInfo class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "cmString.h"
#include "cmCellEnum.h"
#include "cmCellContainer.h"

class cmBaseCellInfo  
{
public:
	cmBaseCellInfo();
	virtual ~cmBaseCellInfo();

	void SetTitle(const cmString& title);
	cmString GetTitle() const;

	void SetDescription(const cmString& description);
	cmString GetDescription() const;

	void SetExtraMsgType(enCmCellExtraMsgType type);
	enCmCellExtraMsgType GetExtraMsgType() const;

	void SetExtraMsg(const cmString& msg);
	cmString GetExtraMsg() const;

	void SetCellContainer(cmCellContainer* container);
	cmCellContainer* GetCellContainer();

protected:

	cmString title_;
	cmString description_;
	enCmCellExtraMsgType extraType_;
	cmString extraMsg_;

	cmCellContainer* container_;
};


