#include "stdafx.h"
#include "InvoiceDoc.h"
#include "../Zapisslowny.h"
#include "../../logo/Logo.h"
#include "../../consts.h"
#include "../../datatypes/CompanyInfo.h"
#include "../../accessor/CowAccessor.h"
#include "../../Utils/StringFormat.h"
#include "../../classes/IntString.h"
#include "../../classes/FloatString.h"

#include <limits>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#######################################################################################
InvoiceItem::InvoiceItem() : m_qtyPiece(0),m_qtyWeight(0.0),m_qtyUnit(PIECE)
{
}

InvoiceItem::InvoiceItem(const InvoiceItem& src)
{
	CopyFrom(src);
}

InvoiceItem& InvoiceItem::operator=(const InvoiceItem& right)
{
	CopyFrom(right);
	return *this;
}

void InvoiceItem::CopyFrom(const InvoiceItem& src)
{
	m_sDesc = src.m_sDesc;
	m_qtyPiece = src.m_qtyPiece;
	m_qtyWeight = src.m_qtyWeight;
	m_qtyUnit = src.m_qtyUnit;
	m_Total = src.m_Total;
}
	
void InvoiceItem::SetDescription(const CString& sDesc)
{
	m_sDesc = sDesc;
}

CString InvoiceItem::GetDescription() const
{
	return m_sDesc;
}

void InvoiceItem::SetQtyWeight(double dWeight)
{
	m_qtyWeight = dWeight;
	m_qtyUnit = KG;
}

void InvoiceItem::SetQtyPiece(int iPieces)
{
	m_qtyPiece = iPieces;
	m_qtyUnit = PIECE;
}

InvoiceItem::QTYUNITT InvoiceItem::GetQtyUnit() const
{
	return m_qtyUnit;
}

NullDouble InvoiceItem::GetQtyWeight() const
{
	if(m_qtyUnit == KG)
		return NullDouble(m_qtyWeight);

	return NullDouble();
}
NullInt InvoiceItem::GetQtyPieces() const
{
	const int nullPieces = -1;
	if(m_qtyUnit == PIECE)
		return NullInt(m_qtyPiece,nullPieces);

	return NullInt(nullPieces);
}



void InvoiceItem::SetTotal(const Money& total)
{
	m_Total = total;
}

Money InvoiceItem::GetTotal() const
{
	return m_Total;
}

Money InvoiceItem::GetUnitPrice() const
{
	return GetTotal()/GetQty();
}

double InvoiceItem::GetQty() const
{
	switch(m_qtyUnit)
	{
		case PIECE:
			return (double)(m_qtyPiece);
		case KG:
			return m_qtyWeight;
	}

	return std::numeric_limits<double>::quiet_NaN();
}
//#######################################################################################
InvoiceItemGroup::InvoiceItemGroup() : m_CowQty(0),m_TotalGroupWeight(ZEROWEIGHT),m_TotalGroupPrice(ZEROMONEY)
{
}

void InvoiceItemGroup::AddCow(const CowInvoiceEntry *pCow)
{
	m_TotalGroupWeight += pCow->GetWeight().GetDouble();
	m_TotalGroupPrice += pCow->GetPrice();
	m_CowQty++;	
}

int InvoiceItemGroup::GetCowQty() const
{
	return m_CowQty;
}

const NullDouble& InvoiceItemGroup::GetTotalGroupWeight() const
{
	return m_TotalGroupWeight;
}

const Money& InvoiceItemGroup::GetTotalGroupPrice() const
{
	return m_TotalGroupPrice;
}

//#######################################################################################
PricePerKegItemGroup::PricePerKegItemGroup(double dGroupPrice) : m_dGroupPrice(dGroupPrice)
{
}

bool PricePerKegItemGroup::Matches(const CowInvoiceEntry *pCow)
{
	const double dHalfCent = 0.01/2;
	double dPricePerKg = (pCow->GetPrice()/pCow->GetWeight()).GetDouble();
	double dPriceLow = m_dGroupPrice-dHalfCent;
	double dPriceHi  = m_dGroupPrice+dHalfCent;

	return dPriceLow < dPricePerKg && dPriceHi > dPricePerKg;
}

double PricePerKegItemGroup::GetGroupPricePerKg() const
{
	return m_dGroupPrice;
}

//#######################################################################################
StockGroup::StockGroup() : qty(0),groupTotal(0.0),stock(NULL)
{
}
void StockGroup::Reset()
{	
	qty = 0;
	groupWeight = 0.0;
	groupTotal = ZEROMONEY;
	stock = NULL;
}

//#######################################################################################
CFakturaDoc::CFakturaDoc() : curInv(NULL),starti(0),endi(0),m_nPages(0)
{
}



int CFakturaDoc::GetPrintPages() const
{
	return m_nPages;
}
int CFakturaDoc::LoadInputData(CPtrArray &docInputData)
{
	m_invs2print.RemoveAll();
	m_invs2print.Append((CInvoiceDataArray&)docInputData);

	int pages = 0,size,i,ret = RELOAD_OK_CONTINUE;
	size = (int)m_invs2print.GetSize();
	Invoice *pinv;
	invs.RemoveAll();
	
	for(i =0;i < size;i++)
	{
		pinv = m_invs2print[i];
		if(IsInvoiceValidToPrint(pinv) && !pinv->HasCorrect())
			invs.Add(pinv);
		else ret = RELOAD_CAN_CONTINUE;
	}
	
	size = invs.GetSize();
	if(size > 0)
	{
		for(i = 0;i < size;i++)
				pages += GetInvoicePageSize((Invoice*)invs[i]);
		m_nPages = pages;
	} 
	else 
	{
		ret = RELOAD_NO_CONTINUE;
	}


	m_invoiceOptions.Load(GetCfgItemID());

	m_MustacheContext = Mustache::Parse(m_invoiceOptions.GetEntryFormat());

	m_numeralSyntax.Init(m_invoiceOptions.GetNumeralName1(),
						 m_invoiceOptions.GetNumeralName2_4(),
						 m_invoiceOptions.GetNumeralNameOther());
	
	return ret;
}
void CFakturaDoc::PrintBottom()
{
	const char szIrrelevant[] = "-----";
	CString Str;
	CZapisSlowny zapisSlowny;

	Hent *pHent = curInv->GetInvoiceHent();
	
	DrawText("VATRATE",CStringFormat("%.2f",curInv->GetVATRate()*100.0));	
	
	
	FloatFormat moneyFmt(GetMoneyFormat());

	DrawText("TOTAL_NETTO",curInv->GetInvoiceNetValue().ToString(NULL,moneyFmt));
	Money totalVAT = curInv->GetInvoiceNetValue()*curInv->GetVATRate();

	DrawText("TOTAL_VAT",totalVAT.ToString(NULL,moneyFmt));
	Money totalBrutto = curInv->GetInvoiceGrossValue();

	DrawText("TOTAL_1",totalBrutto.ToString(NULL,moneyFmt));
	DrawText("TOTAL_2",totalBrutto.ToString(NULL,moneyFmt));

	CString bankname,accountno;
	switch(curInv->GetPayWay())
	{
		case Invoice::PAY_CASH:
			Str = PayWayNames[1];
			bankname = szIrrelevant;
			accountno = szIrrelevant;
		break;
		case Invoice::PAY_TRANS:
			Str = PayWayNames[0];
			bankname = pHent->GetBankName();
			accountno = pHent->GetAccountNo();

		break;
	}

	DrawText("PAYWAY",Str);
	DrawText("PAYDUEDAYS",CIntString(curInv->GetPayDueDays()));

   	DrawText("BANKNAME",bankname);
	DrawText("ACCOUNTNO",accountno);

	CString sExtras = curInv->GetExtras();
	if(sExtras.IsEmpty())
		sExtras = GetExtrasTemplate();

	DrawText("EXTRAS",sExtras);

	zapisSlowny.ZapisSlowny(totalBrutto,Str,"z³","gr");
	DrawText("TOTAL_TEXT",Str);
	
}

CString CFakturaDoc::GetExtrasTemplate() const
{
	return m_invoiceOptions.GetExtrasTemplate();
}

void CFakturaDoc::SetLogo()
{
	COleStreamFile logoStream;
	if(CLogoImage::GetLogo()->GetOleLogoImage(logoStream))
		DrawImage("LOGO",logoStream);
}
void CFakturaDoc::PrintHeader(bool withDates,bool withHents)
{
	const Hent *seller,*buyer,*pHent = curInv->GetInvoiceHent();
	InvoiceTypes invtype = curInv->GetRealInvoiceType();
	const Hent *pCompanyHent = Company::GetCompany()->GetAsHent();

	switch(invtype)
	{
	case IN_INV_RR:
	case IN_INV_VAT:
		
		buyer = pCompanyHent;
		seller = pHent;
		break;
	case OUT_INV_VAT:
					
		seller = pCompanyHent;
		buyer = pHent;				
		break;
	}
	if(withHents) 
	{		

		PrintSeller(seller);
		PrintBuyer(buyer);
		
	}

	CString sInvoiceNo = curInv->GetInvoiceNoStr();
	CString sCustomNumber = curInv->GetCustomNumber();

	if(sCustomNumber.IsEmpty())
		DrawText("INVOICENO",sInvoiceNo);
	else
	{
		DrawText("INVOICENO",sCustomNumber);
		DrawText("SMALL_INVOICENO",sInvoiceNo);
	}
	
	if(withDates)
	{
		
		DrawText("INVOICE_DATE",curInv->GetInvoiceDate().GetDayDate());
	}

	CDateTime transactionDt = curInv->GetTransactionDate();
	if(!transactionDt.IsNull())
	{
		DrawText("TRANSACTION_DATE",transactionDt.GetDayDate());
	}

	DrawText("TRANSACTION_PLACE",curInv->GetTransactionPlace());
}


CString CFakturaDoc::GetQtyString(const InvoiceItem *pPrintRecord) const
{
	switch(pPrintRecord->GetQtyUnit())
	{
		case InvoiceItem::PIECE:
			return CIntString(pPrintRecord->GetQtyPieces().ToInt());
		case InvoiceItem::KG:
			return pPrintRecord->GetQtyWeight().ToString(NULL,GetWeightFormat());
	}
	return CString();
}

CString CFakturaDoc::GetQtyUnitString(const InvoiceItem *pPrintRecord) const
{
	const char szUnitShorts[][5] =
	{
		"kg",
		"szt."
	};
	return szUnitShorts[pPrintRecord->GetQtyUnit()];
}

void CFakturaDoc::PrintInvoiceRecord(int lp,int row,const InvoiceItem *pPrintRecord)
{
	FloatFormat moneyFmt(GetMoneyFormat());

	DrawText(CStringFormat("LP%d",row),CIntString(lp));
	DrawText(CStringFormat("NAME%d",row),pPrintRecord->GetDescription());
	DrawText(CStringFormat("QTY%d",row),GetQtyString(pPrintRecord));
	DrawText(CStringFormat("UNIT%d",row),GetQtyUnitString(pPrintRecord));
	DrawText(CStringFormat("UNITPRICENETTO%d",row),pPrintRecord->GetUnitPrice().ToString(NULL,m_invoiceOptions.GetPricePerKgFormat()));
	DrawText(CStringFormat("PKWIU%d",row),m_invoiceOptions.GetPKWIU());
	
	Money netTotal = pPrintRecord->GetTotal();
	Money vatTotal = netTotal*curInv->GetVATRate();
	Money grosTotal = netTotal+vatTotal;

	DrawText(CStringFormat("NETTO%d",row),netTotal.ToString(NULL,moneyFmt));	
	DrawText(CStringFormat("VAT%d",row),vatTotal.ToString(NULL,moneyFmt));
	DrawText(CStringFormat("BRUTTO%d",row),grosTotal.ToString(NULL,moneyFmt));

}

void CFakturaDoc::PrintRecords(int sindex,int eindex,int row)
{
	Cow *cow;
	InvoiceItem invoiceRecord;
	
	CowAccessor *pAccessor = CowAccessorFactory::createAccessor(curInv->GetBuyOrSell());


	MustacheVariables variables;
	variables.Put(GetProductNameVariable(1));
	variables.Put(GetProductQtyVariable(1));

	CString sDesc;

	for(int i = sindex;i <= eindex;i++,row++)
	{
		cow = cows[i];

		variables.Put(GetStockVariable(pAccessor->GetStock(cow)));	
		variables.Put(GetCowEANVariable(cow));
		variables.Put(GetCowSexVariable(cow));
	    
		sDesc = FormatInvoiceEntry(variables);
		if(sDesc.IsEmpty())
		{
			sDesc = (LPCSTR)CStringFormat("%s, %s",(LPCSTR)cow->GetEAN(),(LPCSTR)cow->GetSex());
		}		

		invoiceRecord.SetDescription(sDesc);
		invoiceRecord.SetTotal(pAccessor->GetPrice(cow));
		invoiceRecord.SetQtyWeight(pAccessor->GetWeight(cow).GetDouble());						
		PrintInvoiceRecord(i+1,row,&invoiceRecord);
	}
	delete pAccessor;

}

void CFakturaDoc::PrintPage(int page)
{
	int row;
		
	DrawText("PAGES",CStringFormat("%d/%d",pageOff+1,curInvPageSize));

		if(page == curInvLastPage)	PrintBottom();

    	if(pageOff > 0)
		{
			row = PAGE1SIZE;	//druga strona
			PrintHeader(false,false);
		}
		else
		{			
			PrintHeader(true,true);	
		
			row = 0;
		}
		PrintRecords(starti,endi,row);
		
			
}

MustacheVariable CFakturaDoc::GetProductNameVariable(int qty) const
{
	return MustacheVariable("TOWAR",GetProductNumeralName(qty));
}
CString CFakturaDoc::GetProductNumeralName(int iProductNumeral) const
{
	return m_numeralSyntax.Odmiana(iProductNumeral);
}
MustacheVariable CFakturaDoc::GetProductQtyVariable(int qty) const
{
	return MustacheVariable("ILOSC",CIntString(qty));
}
MustacheVariable CFakturaDoc::GetCowEANVariable(const Cow* pCow) const
{
	return MustacheVariable("KOLCZYK",pCow->GetEAN());
}
MustacheVariable CFakturaDoc::GetCowSexVariable(const Cow* pCow) const
{
	return MustacheVariable("PLEC",(LPCSTR)pCow->GetSex());
}

MustacheVariable CFakturaDoc::GetStockVariable(const Stock* pStock) const
{
	return MustacheVariable("RASA",pStock->GetStockCode());
}

CString CFakturaDoc::FormatInvoiceEntry(const MustacheVariables& variables)
{
	return Mustache::Format(m_MustacheContext,variables);
}

const FloatFormat& CFakturaDoc::GetPricePerKgFormat() const
{
	return m_invoiceOptions.GetPricePerKgFormat();
}

//#######################################################################################
void CFakturaA4::PreparePage(int nPage)
{
	int p = 0,i = 0,fp,size,grp,off;

	Invoice *newCurInv = NULL;
	int newCurInvPageSize,newCurInvLastPage;

	do
	{
		p++;
		fp  = p;
		newCurInv = (Invoice*)invs[i];
		newCurInvPageSize = GetInvoicePageSize(newCurInv);
		p += newCurInvPageSize-1;
		newCurInvLastPage = p;
		i++;
	}
	while(p < nPage);

	if(newCurInv != curInv)
	{
		curInv = newCurInv;
		curInvPageSize = newCurInvPageSize;
		curInvLastPage = newCurInvLastPage;
		cows.RemoveAll();

		curInv->GetInvoiceCows(cows);

		if(GetSortBy() != (COWDATA_MEMBERS)-1)
			cows.SortBy(GetSortBy(),GetSortOrder());
	}

	ASSERT(cows.GetSize() == curInv->GetCowQty());

	size = curInv->GetCowQty();

	ResetGroupsVisibility(false);
	CString grpname;
	
	if(nPage == fp) 
	{
		starti = 0;

		if(size <= PAGE1UPPER)
		{
		         SetGroupVisible("PAGEBOTTOM",true);
				 endi = size-1;
		}
		else endi = PAGE1SIZE; //could cause endi overflow

		SetTemplatePage(0);
		SetLogo();
		pageOff = 0;
		grp = 0;
	}
	else
	{
		SetGroupVisible("TABLEHEADER2",true);
		starti = PAGE1SIZE + (nPage-fp-1)*PAGE2SIZE;

		off = size-starti;
		if(off > 0)
		{
			if(off < PAGE2UPPER)
			{
				SetGroupVisible("PAGEBOTTOM2",true);
				endi = size-1;
			}
			else
			{
				endi = starti+PAGE2SIZE; //endi overflow
			}
		}
		else 
		{
			SetGroupVisible("TABLEHEADER2",false);
			SetGroupVisible("PAGEBOTTOM2",true);
			endi = starti;
		}

		SetTemplatePage(1);
		pageOff = nPage-fp;
		grp = PAGE1SIZE;
		
	}
				
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,grp++)
	{
		grpname.Format("%d",grp);
		SetGroupVisible(grpname,true);
	}

	
}

int CFakturaA4::_GetInvoicePageSize(int nRecords)
{
int pages = 1,cowsize = nRecords;

	if(cowsize > PAGE1UPPER)
	{
		pages ++;
		if(cowsize > PAGE1SIZE)
		{
			cowsize -= PAGE1SIZE;
			do
			{
				if(cowsize > PAGE2UPPER)		pages++;
				cowsize -= PAGE2SIZE;

			} while (cowsize > 0);
		}
	}
	return pages;


}

int CFakturaA4::GetInvoicePageSize(Invoice *pInv)
{
	return _GetInvoicePageSize((int)pInv->GetCowQty());
}
//#######################################################################################

IMPLEMENT_DYNCREATE(CFakturaRRA4, CFakturaA4)

bool CFakturaRRA4::IsInvoiceValidToPrint(Invoice *pInv)
{
	return pInv->GetRealInvoiceType() == IN_INV_RR;
}

void CFakturaRRA4::PrintBuyer(const Hent *buyer)
{
	CString Str;
	
	DrawText("BUYER_NAME",buyer->GetName());
	Str.Format("%s %s\n%s %s",(LPCSTR)buyer->GetStreet(),(LPCSTR)buyer->GetPOBox(),(LPCSTR)buyer->GetZip(),(LPCSTR)buyer->GetCity());
    DrawText("BUYER_ADDRESS",Str);
	DrawText("BUYER_FARMNO",buyer->GetFarmNo());
	DrawText("BUYER_WETNO",buyer->GetWetNo());

	Str.Empty();
	if(buyer->HasNIP())
		Str = buyer->GetNIP();
	if(buyer->GetREGON())
		Str.AppendFormat("(%s)",(LPCSTR)buyer->GetREGON());
	DrawText("BUYER_NIP_REGON",Str);
}

void CFakturaRRA4::PrintSeller(const Hent *seller)
{
	CString Str;

	DrawText("SELLER_NAME",seller->GetName());
	Str.Format("%s %s\n%s %s",(LPCSTR)seller->GetStreet(),(LPCSTR)seller->GetPOBox(),(LPCSTR)seller->GetZip(),(LPCSTR)seller->GetCity());
    DrawText("SELLER_ADDRESS",Str);
	DrawText("SELLER_FARMNO",seller->GetFarmNo());
	DrawText("SELLER_WETNO",seller->GetWetNo());

	Str.Empty();
	if(seller->HasPESEL())
		Str = seller->GetPESEL();
	if(seller->HasNIP())
		Str.AppendFormat(" (%s)",(LPCSTR)seller->GetNIP());

	DrawText("SELLER_PESEL_NIP",Str);
	DrawText("SELLER_IDNO",seller->GetIdNo());
	DrawText("SELLER_ISSUEDATE",seller->GetIssueDate().GetDayDate());
	DrawText("SELLER_ISSUEPOST",seller->GetIssuePost());

}
//#######################################################################################

IMPLEMENT_DYNCREATE(CFakturaRRA5, CFakturaDoc)

int CFakturaRRA5::GetInvoicePageSize(Invoice *pInv)
{
	return 1;// Reload() removed all invoices with cattle array size grather than FAKTURARRA5SIZE
}

bool CFakturaRRA5::IsInvoiceValidToPrint(Invoice *pInv)
{
	return pInv->GetCowQty() <= FAKTURARRA5SIZE && pInv->GetRealInvoiceType() == IN_INV_RR;
}

void CFakturaRRA5::PreparePage(int nPage)
{
	Invoice* newCurInv = NULL;
	newCurInv = invs[nPage-1];

	SetLogo();

	if(curInv != newCurInv)
	{
		curInv = newCurInv;
		curInv->GetInvoiceCows(cows);

		if(GetSortBy() != (COWDATA_MEMBERS)-1)
			cows.SortBy(GetSortBy(),GetSortOrder());

		starti = 0;
		endi = curInv->GetCowQty()-1;

		pageOff = 0;
		curInvPageSize = 1;
		curInvLastPage = nPage;
		
	}
}

void CFakturaRRA5::PrintBuyer(const Hent *buyer)
{
	CString Str;
	Str.Format("%s\n%s %s, %s %s\n%s",
		(LPCSTR)buyer->GetName(),
		(LPCSTR)buyer->GetStreet(),
		(LPCSTR)buyer->GetPOBox(),
		(LPCSTR)buyer->GetZip(),
		(LPCSTR)buyer->GetCity(),
		(LPCSTR)buyer->GetFarmNo());
	
  	if(buyer->HasWetNo())
		Str.AppendFormat(", %s",(LPCSTR)buyer->GetWetNo());
	
	if(buyer->HasNIP())
		Str.AppendFormat(", NIP: %s",(LPCSTR)buyer->GetNIP());
	if(buyer->HasREGON())
		Str.AppendFormat(", REGON: %s",(LPCSTR)buyer->GetREGON());

	Str += '\n';

	DrawText("BUYER_INFO",Str);


	
}

void CFakturaRRA5::PrintSeller(const Hent *seller)
{
	CString Str;

	Str.Format("%s\n%s %s, %s %s\n%s",(LPCSTR)seller->GetName(),(LPCSTR)seller->GetStreet(),(LPCSTR)seller->GetPOBox(),
						(LPCSTR)seller->GetZip(),(LPCSTR)seller->GetCity(),(LPCSTR)seller->GetFarmNo());
	
  
	if(seller->HasWetNo())
		Str.AppendFormat(", %s",(LPCSTR)seller->GetWetNo());

	if(seller->HasPESEL())
		Str.AppendFormat(", PESEL: %s",(LPCSTR)seller->GetPESEL());
	if(seller->HasNIP())
		Str.AppendFormat(", NIP: %s",(LPCSTR)seller->GetNIP());

	Str += '\n';


	Str.AppendFormat("%s %s",
		(LPCSTR)seller->GetIdNo(),
		seller->GetIssueDate().GetDayDate());
		
	CString sIssuePost = seller->GetIssuePost();
	if(!sIssuePost.IsEmpty())
	{
		Str += "\n";
		Str += sIssuePost;
	}

	DrawText("SELLER_INFO",Str);
	
}
//#######################################################################################

IMPLEMENT_DYNCREATE(CFakturaVATA4, CFakturaA4)

void CFakturaVATA4::PrintSeller(const Hent *seller)
{
	PrintHent("SELLER",seller);
}

void CFakturaVATA4::PrintHent(LPCSTR prefix,const Hent *hent)
{
	CString Str,fld;

	fld.Format("%s_NAME",prefix);
	DrawText(fld,hent->GetName());
	Str.Format("%s %s\n%s %s",
		(LPCSTR)hent->GetStreet(),
		(LPCSTR)hent->GetPOBox(),
		(LPCSTR)hent->GetZip(),
		(LPCSTR)hent->GetCity());

	fld.Format("%s_ADDRESS",prefix);
    DrawText(fld,Str);

	fld.Format("%s_FARMNO",prefix);
	DrawText(fld,hent->GetFarmNo());

	fld.Format("%s_WETNO",prefix);
	DrawText(fld,hent->GetWetNo());

	Str.Empty();
	if(hent->HasNIP())
		Str = hent->GetNIP();
	if(hent->HasREGON())
		Str.AppendFormat(" (%s)",(LPCSTR)hent->GetREGON());

	fld.Format("%s_NIP_REGON",prefix);
	DrawText(fld,Str);

	fld.Format("%s_BANKNAME",prefix);
	DrawText(fld,hent->GetBankName());

	fld.Format("%s_ACCOUNTNO",prefix);
	DrawText(fld,hent->GetAccountNo());


}
void CFakturaVATA4::PrintBuyer(const Hent *buyer)
{
	PrintHent("BUYER",buyer);
}

bool CFakturaVATA4::IsInvoiceValidToPrint(Invoice *pInv)
{
	InvoiceTypes invType = pInv->GetRealInvoiceType();
	return invType == IN_INV_VAT || invType == OUT_INV_VAT;
}

void CFakturaVATA4::PrintHeader(bool withDates,bool withHents)
{
	CFakturaA4::PrintHeader(withDates,withHents);
	DrawText("INVOICE_TITLE",m_invoiceOptions.GetTitle());
}
//#######################################################################################


CFakturaVATGroupA4::~CFakturaVATGroupA4()
{
	FreePrintMap();
}

void CFakturaVATGroupA4::FreePrintMap()
{
	POSITION pos;
	InvoiceItems *printRecords;
	Invoice *pInv;

	pos = printMap.GetStartPosition();
	while(pos)
	{
		printMap.GetNextAssoc(pos,pInv,printRecords);
		printRecords->FreeAndFlush();
		delete printRecords;
	}
	printMap.RemoveAll();

	//printCube.FreeAndFlush();

}



int CFakturaVATGroupA4::LoadInputData(CPtrArray &docInputData)
{
	int reloadReturn;
	FreePrintMap();
	reloadReturn = CFakturaVATA4::LoadInputData(docInputData);
		
	CMap<Stock*,Stock*,StockGroup,StockGroup&> stock2qty;

	int i,si,j,sj;
	Cow *cow;
	StockGroup stockGroup;
	POSITION pos;	
	InvoiceItems *recArray;
	InvoiceItem *invRec;
	Invoice *pInv = NULL;	

	NullDouble weight;
	Stock *pStock;
	
	for(i=0,si=invs.GetSize();i<si;i++)
	{
		pInv = invs[i];
		pInv->GetInvoiceCows(cows);

		CowAccessor *pAccessor = CowAccessorFactory::createAccessor(pInv->GetBuyOrSell());
		
		for(j=0,sj = cows.GetSize();j<sj;j++)
		{
			cow  = cows[j];		

			pStock = pAccessor->GetStock(cow);
			weight = pAccessor->GetWeight(cow);


			if(!stock2qty.Lookup(pStock,stockGroup))	
				stockGroup.Reset();

			stockGroup.qty++;		
			stockGroup.groupWeight += weight;		
			stockGroup.groupTotal += pAccessor->GetPrice(cow);			
			stockGroup.stock = pStock;

            stock2qty.SetAt(pStock,stockGroup);
		}
		delete pAccessor;

		recArray = new InvoiceItems();
		 
		pos = stock2qty.GetStartPosition();
		while(pos)
		{
			stock2qty.GetNextAssoc(pos,pStock,stockGroup);
			invRec = new InvoiceItem();
			SetInvRecord(invRec,stockGroup);
			recArray->Add(invRec);
		}

		printMap.SetAt(pInv,recArray);
	}

		return reloadReturn;

}


void CFakturaVATGroupA4::PrintRecords(int sindex,int eindex,int row)
{
	InvoiceItems *printRecord;
	printMap.Lookup(curInv,printRecord);
	
	for(int i = starti;i <= eindex;i++)
	{
		PrintInvoiceRecord(i+1,row,((*printRecord)[i]));
		row++;
	}

}

int CFakturaVATGroupA4::GetInvoicePageSize(Invoice *pInv)
{
	InvoiceItems *recArray;

	if(!printMap.Lookup(pInv,recArray)) return 0;

	return _GetInvoicePageSize(recArray->GetSize());
}

void CFakturaVATGroupA4::PreparePage(int nPage)
{
	int p = 0,i = 0,fp,size,grp,off;	

	InvoiceItems *printRecords;
	POSITION pos = printMap.GetStartPosition();
	while(pos && p < nPage)
	{
		p++;
		fp  = p;
		printMap.GetNextAssoc(pos,curInv,printRecords);
		curInvPageSize = _GetInvoicePageSize(printRecords->GetSize());
		p += curInvPageSize-1;
		curInvLastPage = p;
		//i++;
	}
	
	size = printRecords->GetSize();

	ResetGroupsVisibility(false);
	CString grpname;
	
	if(nPage == fp) 
	{
		starti = 0;

		if(size <= PAGE1UPPER)
		{
		         SetGroupVisible("PAGEBOTTOM",true);
				 endi = size-1;
		}
		else endi = PAGE1SIZE; //could cause endi overflow

		SetTemplatePage(0);
		SetLogo();
		pageOff = 0;
		grp = 0;
	}
	else
	{
		SetGroupVisible("TABLEHEADER2",true);
		starti = PAGE1SIZE + (nPage-fp-1)*PAGE2SIZE;

		off = size-starti;
		if(off > 0)
		{
			if(off < PAGE2UPPER)
			{
				SetGroupVisible("PAGEBOTTOM2",true);
				endi = size-1;
			}
			else
			{
				endi = starti+PAGE2SIZE; //endi overflow
			}
		}
		else 
		{
			SetGroupVisible("TABLEHEADER2",false);
			SetGroupVisible("PAGEBOTTOM2",true);
			endi = starti;
		}

		SetTemplatePage(1);
		pageOff = nPage-fp;
		grp = PAGE1SIZE;
		
	}
				
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,grp++)
	{
		grpname.Format("%d",grp);
		SetGroupVisible(grpname,true);
	}

	
}

//#######################################################################################
IMPLEMENT_DYNCREATE(CFakturaVATGroupWeightA4, CFakturaVATGroupA4)

void CFakturaVATGroupWeightA4::SetInvRecord(InvoiceItem *pInvRec,const StockGroup &stockGroup)
{
	
	MustacheVariables variables;
	variables.Put(GetProductNameVariable(stockGroup.qty));
	variables.Put(GetProductQtyVariable(stockGroup.qty));
	variables.Put(GetStockVariable(stockGroup.stock));

	CString sDesc = FormatInvoiceEntry(variables);
	if(sDesc.IsEmpty())
	{
		sDesc += CIntString(stockGroup.qty);
		sDesc += ' ';
		sDesc += GetProductNumeralName(stockGroup.qty);
		sDesc += ' ';
		sDesc += stockGroup.stock->GetStockCode();
	}
	
	pInvRec->SetDescription(sDesc);
	pInvRec->SetQtyWeight(stockGroup.groupWeight.GetDouble());
	pInvRec->SetTotal(stockGroup.groupTotal);
	
}

//#######################################################################################
IMPLEMENT_DYNCREATE(CFakturaVATGroupPieceA4, CFakturaVATGroupA4)

void CFakturaVATGroupPieceA4::SetInvRecord(InvoiceItem *pInvRec,const StockGroup &stockGroup)
{
	MustacheVariables variables;
	variables.Put(GetProductNameVariable(stockGroup.qty));
	variables.Put(GetProductQtyVariable(stockGroup.qty));
	variables.Put(GetStockVariable(stockGroup.stock));
	
	CString sDesc = FormatInvoiceEntry(variables);
	if(sDesc.IsEmpty())
	{
		sDesc = GetProductNumeralName(1) + ' ' + stockGroup.stock->GetStockCode();
	}

	pInvRec->SetDescription(sDesc);
	pInvRec->SetQtyPiece(stockGroup.qty);
	pInvRec->SetTotal(stockGroup.groupTotal);	

}

//#######################################################################################

int CFakturaVATCompactA4::LoadInputData(CPtrArray &docInputData)
{
	int reloadReturn;
	FreePrintMap();
	reloadReturn = CFakturaVATA4::LoadInputData(docInputData);
		
	int i,si;
		
	InvoiceItems *recArray;
	InvoiceItem *invRec;
	Invoice *pInv = NULL;	

	for(i=0,si=invs.GetSize();i<si;i++)
	{
		pInv = invs[i];
				
		recArray = new InvoiceItems();
		invRec = new InvoiceItem();

		SetInvRecord(invRec,pInv);
		recArray->Add(invRec);
		 
		printMap.SetAt(pInv,recArray);
	}

	return reloadReturn;
}


void CFakturaVATCompactA4::PreparePage(int nPage)
{
	int p = 0,i = 0,fp,size,grp,off;	

	InvoiceItems *printRecords;
	POSITION pos = printMap.GetStartPosition();
	while(pos && p < nPage)
	{
		p++;
		fp  = p;
		printMap.GetNextAssoc(pos,curInv,printRecords);
		curInvPageSize = _GetInvoicePageSize(printRecords->GetSize());
		p += curInvPageSize-1;
		curInvLastPage = p;
		//i++;
	}
	
	size = printRecords->GetSize();

	ResetGroupsVisibility(false);
	CString grpname;
	
	if(nPage == fp) 
	{
		starti = 0;

		if(size <= PAGE1UPPER)
		{
		         SetGroupVisible("PAGEBOTTOM",true);
				 endi = size-1;
		}
		else endi = PAGE1SIZE; //could cause endi overflow

		SetTemplatePage(0);
		SetLogo();
		pageOff = 0;
		grp = 0;
	}
	else
	{
		SetGroupVisible("TABLEHEADER2",true);
		starti = PAGE1SIZE + (nPage-fp-1)*PAGE2SIZE;

		off = size-starti;
		if(off > 0)
		{
			if(off < PAGE2UPPER)
			{
				SetGroupVisible("PAGEBOTTOM2",true);
				endi = size-1;
			}
			else
			{
				endi = starti+PAGE2SIZE; //endi overflow
			}
		}
		else 
		{
			SetGroupVisible("TABLEHEADER2",false);
			SetGroupVisible("PAGEBOTTOM2",true);
			endi = starti;
		}

		SetTemplatePage(1);
		pageOff = nPage-fp;
		grp = PAGE1SIZE;
		
	}
				
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,grp++)
	{
		grpname.Format("%d",grp);
		SetGroupVisible(grpname,true);
	}

}
int CFakturaVATCompactA4::GetInvoicePageSize(Invoice *pInv)
{
	return 1;
}

CFakturaVATCompactA4::~CFakturaVATCompactA4()
{
	FreePrintMap();
}

void CFakturaVATCompactA4::FreePrintMap()
{
	POSITION pos;
	InvoiceItems *printRecords;
	Invoice *pInv;

	pos = printMap.GetStartPosition();
	while(pos)
	{
		printMap.GetNextAssoc(pos,pInv,printRecords);
		printRecords->FreeAndFlush();
		delete printRecords;
	}
	printMap.RemoveAll();

	//printCube.FreeAndFlush();

}

void CFakturaVATCompactA4::PrintRecords(int sindex,int eindex,int row)
{
	InvoiceItems *printRecord;
	printMap.Lookup(curInv,printRecord);
	
	for(int i = starti;i <= eindex;i++)
	{
		PrintInvoiceRecord(i+1,row,((*printRecord)[i]));
		row++;
	}

}

//#######################################################################################
IMPLEMENT_DYNCREATE(CFakturaVATCompactWeightA4, CFakturaVATCompactA4)

void CFakturaVATCompactWeightA4::SetInvRecord(InvoiceItem *pInvRec,const Invoice *pInv)
{
	Money totalNetto(ZEROMONEY);
	double totalWeight = 0.0;
	CCowArray cows;
	int j,sj;
	CowAccessor *pAccessor = CowAccessorFactory::createAccessor(pInv->GetBuyOrSell());

	pInv->GetInvoiceCows(cows);
	
	Cow *pCow;
	for(j=0,sj = cows.GetSize();j<sj;j++)
	{
		pCow = cows[j];
		totalNetto += pAccessor->GetPrice(pCow);
		totalWeight += pAccessor->GetWeight(pCow).GetDouble();
	}
	delete pAccessor;
	 
	MustacheVariables variables;
	variables.Put(GetProductNameVariable(sj));
	variables.Put(GetProductQtyVariable(sj));

	CString sDesc = FormatInvoiceEntry(variables);
	if(sDesc.IsEmpty())
	{
		sDesc = (LPCSTR)CIntString(sj);
		sDesc += ' ';
		sDesc += GetProductNumeralName(sj);
	}
			
	pInvRec->SetDescription(sDesc);
	pInvRec->SetQtyWeight(totalWeight);
	pInvRec->SetTotal(totalNetto);
}

//#######################################################################################
IMPLEMENT_DYNCREATE(CFakturaVATCompactPieceA4, CFakturaVATCompactA4)

void CFakturaVATCompactPieceA4::SetInvRecord(InvoiceItem *pInvRec,const Invoice *pInv)
{
	Money totalNetto(ZEROMONEY);
	CCowArray cows;
	int j,sj;
	CowAccessor *pAccessor = CowAccessorFactory::createAccessor(pInv->GetBuyOrSell());

	pInv->GetInvoiceCows(cows);
	
	for(j=0,sj = cows.GetSize();j<sj;j++)
	{
		totalNetto += pAccessor->GetPrice(cows[j]);
	}
	delete pAccessor;
	 

	MustacheVariables variables;
	variables.Put(GetProductNameVariable(sj));
	variables.Put(GetProductQtyVariable(sj));

	CString sDesc = FormatInvoiceEntry(variables);
	if(sDesc.IsEmpty())
	{
		sDesc = GetProductNumeralName(1);
	}
			
	pInvRec->SetDescription(sDesc);
	pInvRec->SetQtyPiece(sj);
	pInvRec->SetTotal(totalNetto);
}


//#######################################################################################
IMPLEMENT_DYNCREATE(CFakturaRRA5Piece, CFakturaRRA5)

void CFakturaRRA5Piece::PrintRecords(int sindex,int eindex,int row)
{
	Cow *cow;
	InvoiceItem invoiceRecord;
	
	CowAccessor *pAccessor = CowAccessorFactory::createAccessor(curInv->GetBuyOrSell());

	MustacheVariables variables;
	variables.Put(GetProductNameVariable(1));
	variables.Put(GetProductQtyVariable(1));

	CString sDesc;

	for(int i = sindex;i <= eindex;i++,row++)
	{
		cow = cows[i];
		variables.Put(GetStockVariable(pAccessor->GetStock(cow)));
		variables.Put(GetCowEANVariable(cow));
		variables.Put(GetCowSexVariable(cow));
	    		
		sDesc = FormatInvoiceEntry(variables);
		if(sDesc.IsEmpty())
		{
			sDesc = (LPCSTR)CStringFormat("%s, %s",(LPCSTR)cow->GetEAN(),(LPCSTR)cow->GetSex());
		}

		invoiceRecord.SetDescription(sDesc);
		invoiceRecord.SetTotal(pAccessor->GetPrice(cow));
		invoiceRecord.SetQtyPiece(1);
		PrintInvoiceRecord(i+1,row,&invoiceRecord);
	}
	delete pAccessor;

}
//#######################################################################################
IMPLEMENT_DYNCREATE(CFakturaRRA4Piece, CFakturaRRA4)

void CFakturaRRA4Piece::PrintRecords(int sindex,int eindex,int row)
{
	Cow *cow;
	InvoiceItem invoiceRecord;

	CowAccessor *pAccessor = CowAccessorFactory::createAccessor(curInv->GetBuyOrSell());

	MustacheVariables variables;
	variables.Put(GetProductNameVariable(1));
	variables.Put(GetProductQtyVariable(1));	

	CString sDesc;

	for(int i = sindex;i <= eindex;i++,row++)
	{
		cow = cows[i];
		
		variables.Put(GetStockVariable(pAccessor->GetStock(cow)));
		variables.Put(GetCowEANVariable(cow));
		variables.Put(GetCowSexVariable(cow));
				
		sDesc = FormatInvoiceEntry(variables);
		if(sDesc.IsEmpty())
		{
			sDesc = (LPCSTR)CStringFormat("%s, %s",(LPCSTR)cow->GetEAN(),(LPCSTR)cow->GetSex());
		}

		invoiceRecord.SetDescription(sDesc);
		invoiceRecord.SetTotal(pAccessor->GetPrice(cow));
		invoiceRecord.SetQtyPiece(1);
		PrintInvoiceRecord(i+1,row,&invoiceRecord);
	}
	delete pAccessor;

}


//#######################################################################################

CFakturaVATGroup::~CFakturaVATGroup()
{
	FreeItemsMap();
}

void CFakturaVATGroup::FreeItemsMap()
{
	POSITION pos;
	InvoiceItems *pItems;
	Invoice *pInvoice;

	pos = m_InvoiceItemsMap.GetStartPosition();
	while(pos)
	{
		m_InvoiceItemsMap.GetNextAssoc(pos,pInvoice,pItems);
		pItems->FreeAndFlush();
		delete pItems;
	}
	m_InvoiceItemsMap.RemoveAll();
}


int CFakturaVATGroup::LoadInputData(CPtrArray &docInputData)
{
	int result;
	FreeItemsMap();
	result = CFakturaVATA4::LoadInputData(docInputData);
	int i,si,j,sj;
	Invoice *pInvoice = NULL;
		
	for(i=0,si=invs.GetSize();i<si;i++)
	{
		
		pInvoice = invs[i];
		pInvoice->GetInvoiceCows(cows);
		
		PtrFlushArray<InvoiceItemGroup> groups;
		Cow *pCow = NULL;
		CowInvoiceEntry* pEntry = NULL;
		InvoiceItemGroup *pItemGroup = NULL;

		for(j=0,sj = cows.GetSize();j<sj;j++)
		{
			pCow  = cows[j];		
			pEntry = pInvoice->GetEntryForCow(pCow);
			pItemGroup = NULL;

			InvoiceItemGroup *pGroup = NULL;
			PtrFlushArray<InvoiceItemGroup>::ArrayIterator itemGroupIt = groups.iterator();
			while(itemGroupIt.hasNext())
			{
				pGroup = *(itemGroupIt++);
				if(pGroup->Matches(pEntry))
				{
					pItemGroup = pGroup;
					break;
				}
			}

			if(!pItemGroup)
			{
				pItemGroup = CreateItemGroupForEntry(pEntry);
				groups.Add(pItemGroup);
			}

			pItemGroup->AddCow(pEntry);			
		}
		
		InvoiceItems *pInvoiceItems = new InvoiceItems();		

		InvoiceItemGroup *pGroup = NULL;
		PtrFlushArray<InvoiceItemGroup>::ArrayIterator itemGroupIt = groups.iterator();
		while(itemGroupIt.hasNext())
		{
			pGroup = *(itemGroupIt++);
			pInvoiceItems->Add(CreateIvoiceItemForGroup(pGroup));
		}
		groups.FreeAndFlush();

		m_InvoiceItemsMap.SetAt(pInvoice,pInvoiceItems);
	}

	return result;

}


void CFakturaVATGroup::PrintRecords(int sindex,int eindex,int row)
{
	InvoiceItems *printRecord;
	m_InvoiceItemsMap.Lookup(curInv,printRecord);
	
	for(int i = starti;i <= eindex;i++)
	{
		PrintInvoiceRecord(i+1,row,((*printRecord)[i]));
		row++;
	}

}

int CFakturaVATGroup::GetInvoicePageSize(Invoice *pInv)
{
	InvoiceItems *recArray;

	if(!m_InvoiceItemsMap.Lookup(pInv,recArray)) return 0;

	return _GetInvoicePageSize(recArray->GetSize());
}

void CFakturaVATGroup::PreparePage(int nPage)
{
	int p = 0,i = 0,fp,size,grp,off;	

	InvoiceItems *printRecords;
	POSITION pos = m_InvoiceItemsMap.GetStartPosition();
	while(pos && p < nPage)
	{
		p++;
		fp  = p;
		m_InvoiceItemsMap.GetNextAssoc(pos,curInv,printRecords);
		curInvPageSize = _GetInvoicePageSize(printRecords->GetSize());
		p += curInvPageSize-1;
		curInvLastPage = p;
		//i++;
	}
	
	size = printRecords->GetSize();

	ResetGroupsVisibility(false);
	CString grpname;
	
	if(nPage == fp) 
	{
		starti = 0;

		if(size <= PAGE1UPPER)
		{
		         SetGroupVisible("PAGEBOTTOM",true);
				 endi = size-1;
		}
		else endi = PAGE1SIZE; //could cause endi overflow

		SetTemplatePage(0);
		SetLogo();
		pageOff = 0;
		grp = 0;
	}
	else
	{
		SetGroupVisible("TABLEHEADER2",true);
		starti = PAGE1SIZE + (nPage-fp-1)*PAGE2SIZE;

		off = size-starti;
		if(off > 0)
		{
			if(off < PAGE2UPPER)
			{
				SetGroupVisible("PAGEBOTTOM2",true);
				endi = size-1;
			}
			else
			{
				endi = starti+PAGE2SIZE; //endi overflow
			}
		}
		else 
		{
			SetGroupVisible("TABLEHEADER2",false);
			SetGroupVisible("PAGEBOTTOM2",true);
			endi = starti;
		}

		SetTemplatePage(1);
		pageOff = nPage-fp;
		grp = PAGE1SIZE;
		
	}
				
	if(endi >= size) endi = size-1;
	
	for(i=starti;i<=endi;i++,grp++)
	{
		grpname.Format("%d",grp);
		SetGroupVisible(grpname,true);
	}
}
//#################################################################################################
IMPLEMENT_DYNCREATE(CFakturaVATGroupPricePerKgPieceA4, CFakturaVATGroup)

InvoiceItemGroup* CFakturaVATGroupPricePerKgPieceA4::CreateItemGroupForEntry(const CowInvoiceEntry *pCow)
{
	return new PricePerKegItemGroup((pCow->GetPrice()/pCow->GetWeight()).GetDouble());
}

InvoiceItem* CFakturaVATGroupPricePerKgPieceA4::CreateIvoiceItemForGroup(const InvoiceItemGroup* pInvoiceGroup)
{

	PricePerKegItemGroup* pPriceGroup = (PricePerKegItemGroup*)pInvoiceGroup;
	FloatFormat pricePerKgFmt = GetPricePerKgFormat();


	MustacheVariables variables;
	variables.Put(GetProductNameVariable(pPriceGroup->GetCowQty()));
	variables.Put(GetProductQtyVariable(pPriceGroup->GetCowQty()));
	variables.Put(MustacheVariable("ZAKG",pricePerKgFmt.Format(pPriceGroup->GetGroupPricePerKg())));
	
	CString sDesc = FormatInvoiceEntry(variables);
	if(sDesc.IsEmpty())
	{
		sDesc = GetProductNumeralName(1) + ' ' + pricePerKgFmt.Format(pPriceGroup->GetGroupPricePerKg());
	}

	InvoiceItem* pItem = new InvoiceItem();
	pItem->SetDescription(sDesc);
	pItem->SetQtyPiece(pPriceGroup->GetCowQty());
	pItem->SetTotal(pPriceGroup->GetTotalGroupPrice());
	return pItem;
}
//#################################################################################################
IMPLEMENT_DYNCREATE(CFakturaVATGroupPricePerKgWeightA4, CFakturaVATGroup)

InvoiceItemGroup* CFakturaVATGroupPricePerKgWeightA4::CreateItemGroupForEntry(const CowInvoiceEntry *pCow)
{
	return new PricePerKegItemGroup((pCow->GetPrice()/pCow->GetWeight()).GetDouble());
}

InvoiceItem* CFakturaVATGroupPricePerKgWeightA4::CreateIvoiceItemForGroup(const InvoiceItemGroup* pInvoiceGroup)
{

	PricePerKegItemGroup* pPriceGroup = (PricePerKegItemGroup*)pInvoiceGroup;
	FloatFormat pricePerKgFmt = GetPricePerKgFormat();


	MustacheVariables variables;
	variables.Put(GetProductNameVariable(pPriceGroup->GetCowQty()));
	variables.Put(GetProductQtyVariable(pPriceGroup->GetCowQty()));
	variables.Put(MustacheVariable("ZAKG",pricePerKgFmt.Format(pPriceGroup->GetGroupPricePerKg())));
	
	CString sDesc = FormatInvoiceEntry(variables);
	if(sDesc.IsEmpty())
	{
		sDesc = GetProductNumeralName(1) + ' ' + pricePerKgFmt.Format(pPriceGroup->GetGroupPricePerKg());
	}

	InvoiceItem* pItem = new InvoiceItem();
	pItem->SetDescription(sDesc);
	pItem->SetQtyWeight(pPriceGroup->GetTotalGroupWeight().GetDouble());
	pItem->SetTotal(pPriceGroup->GetTotalGroupPrice());
	return pItem;
}