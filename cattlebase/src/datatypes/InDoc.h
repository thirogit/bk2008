#ifndef __INDOC_H__
#define __INDOC_H__

#include "Doc.h"


class InDoc : public Doc
{
public:	
	InDoc(int docId);
	virtual bool InsertThisDoc(bool bCommit = true);
	virtual bool DeleteThisDoc(bool bCommit = true);

	bool HaveNoRRInvs() const;
	virtual bool CanMakeVATInvoiceOnThisDoc() const;
protected:
	virtual CString GetDocPrefix() const;
	virtual CString GetTableName() const;
};

#endif