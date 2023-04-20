// cmCellDescriptor.h
//
#if !defined(_CELLDESCRIPTOR_H_)
#define _CELLDESCRIPTOR_H_

#include "cmString.h"

class cmCellDescriptor
{
public:

	cmCellDescriptor(const cmString& name);
	virtual ~cmCellDescriptor();

	void SetName(const cmString& name);
	cmString GetName() const;

protected:

	cmString name_;
};

#endif //!defined(_CELLDESCRIPTOR_H_)