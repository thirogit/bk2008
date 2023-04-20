#ifndef __OUTDOC_H__
#define __OUTDOC_H__

#include "Doc.h"


class OutDoc : public Doc
{
public:
	OutDoc(int docId);
	virtual bool InsertThisDoc(bool bCommit = true);
	virtual bool DeleteThisDoc(bool bCommit = true);
protected:
	virtual CString GetDocPrefix() const;
	virtual CString GetTableName() const;
};

#endif