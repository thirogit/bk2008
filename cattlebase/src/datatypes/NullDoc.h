#ifndef __NULLDOC_H__
#define __NULLDOC_H__

#include "Doc.h"


class NullDoc : public Doc
{
public:
	NullDoc(UINT Id = NULL_DOCID);
protected:
	virtual CString GetDocPrefix() const;	
	virtual CString GetTableName() const;

};

#endif