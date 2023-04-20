#include "stdafx.h"
#include "InvoiceData.h"

#include "log.h"
#include "CattleSQL.h"
#include "consts.h"


InvoiceData::InvoiceData() : invoiceid(0),invtype(IN_INV_VAT),
					invoicevalue(ZEROPRICE),VATrate(0.0),
					payway(PAY_CASH),correct(NULL),
					correctfor(NULL),invoiceno(0)
{
}

void InvoiceData::CopyFrom(const InvoiceData& copyFrom)
{
	RawData::Copy(copyFrom);

	invoiceno = copyFrom.invoiceno;
	invoiceid = copyFrom.invoiceid;
	invtype = copyFrom.invtype;
	extras = copyFrom.extras;
	invoicedate = copyFrom.invoicedate;
	paid = copyFrom.paid;
	invoicevalue = copyFrom.invoicevalue;			
	VATrate = copyFrom.VATrate;
	payway = copyFrom.payway;
	correct = copyFrom.correct; 
	correctfor = copyFrom.correctfor; 
	customnumber = copyFrom.customnumber;
	SQLUpdate = copyFrom.SQLUpdate;

	invDocs.RemoveAll();
	invDocs.Append(copyFrom.invDocs);

}

	
bool InvoiceData::IsPaid()
{
	return paid.IsNull();
}

bool InvoiceData::CanEditThisInvoice()
{
	return paid.IsNull();
}
bool InvoiceData::CanDeleteThisInv()
{
	return  CAN_DO(CANDELETE,this) && paid.IsNull();
}


bool InvoiceData::InsertThisInvoice(bool bCommit/*=true*/)
{
	SQLQuery q;

	const char insStmtStr[] = "INSERT INTO invoices(invoicedate,paid,VATrate,invoicevalue,"
							  "correct,invtype,payway,extras,creator,creattime,lastmodifier,"
							  "lmodiftime,customnumber,invoiceno) "
							  "VALUES(%0n,%1n,%2,%3,%4,%5,%6q,%7n,%8,%9q,%10,%11q,%12n,%13);";

	const char nextInvoiceNoQuery[] = "SELECT MAX(invoiceno) FROM invoices WHERE invtype = %0";
	

	int nextinvno = 0;

	
	q << nextInvoiceNoQuery;
	q.parse();
	if((nextinvno = SQLDB.executeScalar(q.str(invtype).c_str(),0)) < 0)
	{
		return false;
	}
	nextinvno++;


	q.reset();

	q << insStmtStr;
	q.parse();
		
	SQLQueryParms p;
	char payWayStr[2] = {char(payway),0};



	SQLDB.SetCurrentRawData(*this);


		p  << SQLString(invoicedate,true)
		  << SQLString(paid,true)
		  << SQLString(VATrate)
		  << SQLString(invoicevalue);

		if(invtype == CORRCT_INV)
			p << SQLString(correctfor->invoiceid);
		else
			p << SQLStringNull();

		 p  << SQLString(int(invtype))
		  << SQLString(payWayStr)
		  << SQLString(extras)
		  << SQLString(creation.user->userid)
		  << SQLString(creation.eventDt)
		  << SQLString(modification.user->userid)
		  << SQLString(modification.eventDt)
		  << SQLString(customnumber)
		  << SQLString(nextinvno);
	  
		LOG("InsertThisInvoice() query: %s",q.str(p).c_str());

		if(!SQLDB.execute(q.str(p).c_str()))
		{
			LOG("InsertThisInvoice() error: %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}

		if((invoiceid = SQLDB.executeScalar("SELECT MAX(invoiceid) FROM invoices",1)) < 0)
		{
			SQLDB.Rollback();
			return false;
		}	

		invoiceno = nextinvno;

		SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::ADDED);

		if(bCommit) SQLDB.Commit();

		return true;
}
bool InvoiceData::DeleteThisInvoice(bool bCommit/*=true*/)
{
	SQLQuery q;
    q << "DELETE FROM invoices WHERE invoiceid = " << invoiceid;
	
	LOG("DeleteThisInvoice() stmt: %s",q.str().c_str());
	SQLDB.m_stats.AdjustStat(this,CattleSQL::DataDayStats::DELETED);
	if(!SQLDB.execute(q.str().c_str(),bCommit))
	{
		LOG("DeleteThisInvoice() error: %s",(LPCSTR)SQLDB.GetLastError());
		invoiceid = 0;		
		return false;
	}
	return true;
}


Money& InvoiceData::GetInvoiceValue(double *outVATrate/* = NULL*/)
{
	if(correct)
		return correct->GetInvoiceValue(outVATrate);

	if(outVATrate) *outVATrate = VATrate;
	return invoicevalue;
}
Money InvoiceData::GetBruttoValueAfterCorrect()
{
	double correctVATrate;
	Money bruttoVal;
	bruttoVal = GetInvoiceValue(&correctVATrate);
	bruttoVal += bruttoVal*correctVATrate;
	return bruttoVal;
}

int InvoiceData::GetCowQty()
{
	int cowqty = 0;
	for(int i = 0,s = invDocs.GetSize();i<s;i++)
		cowqty += invDocs[i]->cattle.GetSize();
	return cowqty;
}

CString InvoiceData::GetInvoiceNoStr()
{
	CString outNo;
	outNo.Format("FV%04d",invoiceno);
	return outNo;
}

InvoiceTypes InvoiceData::GetInvoiceType()
{
	InvoiceData *pInv = this;
	while(pInv->invtype == CORRCT_INV)
		pInv = pInv->correctfor;
	return pInv->invtype;
}
BuyOrSell InvoiceData::GetBuyOrSell()
{
	switch(GetInvoiceType())
	{
	case IN_INV_VAT:
        return BUY;
	case OUT_INV_VAT:
        return SELL;
	default:
		ASSERT(NULL && "Unknown VAT invoice type");
	}

	return BUY;
}

bool InvoiceData::UpdateThisInvoice(bool bCommit/*=true*/)
{
	if(SQLUpdate.NeedUpdate())
	{
		SQLQuery q;
		q << "UPDATE invoices SET "; 
		
		SQLEqualList eql;
		char payWayStr[2] = {char(payway),0};

		CHECKUPDATE2_0LEN2NULL(paid,eql); 
		CHECKUPDATE2(extras,eql);
		CHECKUPDATE2_0LEN2NULL(invoicedate,eql);
        CHECKUPDATE2(invoicevalue,eql);
		CHECKUPDATE2(VATrate,eql);
		CHECKUPDATE(payway,payWayStr,eql);
		CHECKUPDATE2(customnumber,eql);
		
		
		
		q << eql.equal_list();
		q << " WHERE invoiceid = " << invoiceid;

		LOG("UpdateThisInvoice() stmt %s",q.str().c_str());

		if(!SQLDB.execute(q.str().c_str(),bCommit))
		{
			LOG("UpdateThisInvoice() error %s",(LPCSTR)SQLDB.GetLastError());
			return false;
		}
	}
	return true;

}
HentData* InvoiceData::GetInvoiceHent()
{
	return invDocs[0]->hent;
}
