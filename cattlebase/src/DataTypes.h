#ifndef __CATTLEBASE_DATATYPES_H__
#define __CATTLEBASE_DATATYPES_H__

#include "datatypes/Stock.h"
#include "datatypes/Cow.h"
#include "datatypes/Doc.h"
#include "datatypes/Invoice.h"
#include "datatypes/RRInvoice.h"
#include "datatypes/VATInvoice.h"

#include "datatypes/DocDataArray.h"
#include "datatypes/HentArray.h"
#include "datatypes/StockDataArray.h"
#include "datatypes/InvoiceDataArray.h"

#include "datatypes/CowDataComparator.h"


#define DATATYPE_COWS 0x01
#define DATATYPE_INDOCS 0x02
#define DATATYPE_OUTDOCS 0x04
#define DATATYPE_HENTS 0x08
#define DATATYPE_INVS 0x10
#define DATATYPE_BUYVATINVS 0x20
#define DATATYPE_SELLVATINVS 0x40
#define DATATYPE_RRINVS 0x80
#define DATATYPE_VATINVS 0x100
#define DATATYPE_BUYINVS 0x200
#define DATATYPE_SELLINVS 0x400
#define DATATYPE_EXPENSES 0x800


#endif