#ifndef __DATAMEMBERSEFS_H__
#define __DATAMEMBERSEFS_H__

#include "DataMember.h"
#include "../PtrFlushArray.h"
#include "sqlupdate/SQLColumnUpdate.h"
#include "../classes/types/CowSex.h"
#include "../classes/types/Money.h"
#include "../classes/types/DateTime.h"
#include "../classes/types/NullInt.h"
#include "../classes/types/NullDouble.h"
#include "../classes/types/Longitude.h"
#include "../classes/types/Latitude.h"
#include "../classes/types/VATRate.h"
#include "../classes/types/Time.h"
#include "../classes/types/Weight.h"


typedef DataMember<CString,LPCSTR> StringDataMember;
typedef DataMember<Money,double> MoneyDataMember;
typedef DataMember<NullInt,int> NullIntDataMember;
typedef DataMember<CDateTime,const CDateTime&> DateTimeDataMember;
typedef DataMember<CowSex,COWSEX> CowSexDataMember;
typedef DataMember<bool,bool> BooleanDataMember;
typedef DataMember<int,int> IntegerDataMember;
typedef DataMember<double,double> FloatDataMember;
typedef DataMember<NullDouble,double> NullDoubleDataMember;
typedef DataMember<Latitude,double> LatitudeDataMember;
typedef DataMember<Longitude,double> LongitudeDataMember;
typedef DataMember<VATRate,double> VATRateDataMember;
typedef DataMember<Time,const Time&> TimeDataMember;
typedef DataMember<Weight,double> WeightDataMember;

typedef PtrFlushArray<TransactedDataMember> TransactedDataMemberArray; 
typedef PtrFlushArray<SQLColumnUpdate> UpatableDataMembers;

#endif