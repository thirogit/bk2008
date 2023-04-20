#ifndef __REGISTRATIONDUPLEX_H__
#define __REGISTRATIONDUPLEX_H__

#include "RegistrationDoc.h"

class RegistrationDuplex : public RegistrationDoc
{
	DECLARE_DYNCREATE(RegistrationDuplex)	
public:
	RegistrationDuplex();
	
protected:
	void ComputeIndexes();
	void PrintPage(int nPage);
	void PreparePage(int nPage);
	virtual int GetPrintPages() const;
	virtual int GetNumberOfRecordsOnFrontPage();
	virtual int GetNumberOfRecordsOnBackPage();
	virtual int GetNumberOfRecordsPerSheet();
};
#endif
