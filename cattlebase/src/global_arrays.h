#ifndef __GLOBAL_DEFS_H__
#define __GLOBAL_DEFS_H__

#include "stdafx.h"

#include "datatypes/ReasonDataArray.h"
#include "datatypes/HentTypeArray.h"
#include "datatypes/CountryCodeArray.h"
#include "datatypes/HentArray.h"
#include "datatypes/StockDataArray.h"
#include "datatypes/DocDataArray.h"
#include "datatypes/CowDataArray.h"
#include "datatypes/UserDataArray.h"
#include "datatypes/RuleDataArray.h"
#include "datatypes/InvoiceDataArray.h"
#include "datatypes/ExpenseCategoryArray.h"
#include "datatypes/ExpenseDataArray.h"
#include "datatypes/AgentArray.h"


extern CReasonDataArray ary_reasons;
extern CHentTypeArray  ary_htypes;
extern CCountryCodeArray ary_countries;
extern CHentArray ary_hents;
extern CHentArray ary_companies;
extern CHentArray ary_farmers;
extern CStockDataArray ary_stocks;
extern CDocDataArray ary_indocs;
extern CDocDataArray ary_outdocs;
extern CCowArray ary_cows;
extern CUserDataArray ary_users;
extern CCowArray ary_incows;
extern CRuleDataArray ary_rules;
extern CInvoiceDataArray ary_buy_invs;
extern CInvoiceDataArray ary_sell_invs;
extern CInvoiceDataArray ary_rr_invs;
extern CExpenseCategoryArray ary_expensecategories;
extern CExpenseDataArray ary_expenses;
extern CAgentArray ary_agents;

class DataContext
{

};

#endif