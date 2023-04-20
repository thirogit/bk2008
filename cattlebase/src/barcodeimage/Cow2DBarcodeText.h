#ifndef __COW2DBARCODETEXT_H__
#define __COW2DBARCODETEXT_H__

#include "../datatypes/Cow.h"

class CCow2DBarcodeText
{
public:
	CCow2DBarcodeText(const Cow* pCow);
	CString GetBarcodeText() const;
protected:
	CString GetString(const CString& s) const;
	CString GetDate(const CDateTime& dt) const;
	CString GetCowSex(const CowSex& cowsex) const;

private:
	const Cow* m_pCow;
};

#endif