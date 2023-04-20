#ifndef __PRINTERSCACHE_H__
#define __PRINTERSCACHE_H__

#include "printers/Printer.h"
#include "../PtrFlushArray.h"


class CPrintersCache 
{
public:
	CPrintersCache();
	~CPrintersCache();
	void Refresh();
	int PrintersCount() const;
	PtrFlushArray<CPrinter>::ArrayIterator GetIterator();
protected:
	void EnumLocalPrinters();
	void EnumBTPrinters();
protected:
	PtrFlushArray<CPrinter> m_cache;
	
};



#endif