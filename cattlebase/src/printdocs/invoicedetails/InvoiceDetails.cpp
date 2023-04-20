#include "stdafx.h"
#include "InvoiceDetails.h"
#include "../../consts.h"
#include "datatypes/CompanyInfo.h"
#include "../../accessor/CowAccessor.h"
#include "../../Utils/StringFormat.h"
#include "../../classes/IntString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CInvoiceDetailsDoc::CInvoiceDetailsDoc() : m_curInv(NULL),m_starti(0),m_endi(0),m_nPages(0),
						m_totalWeight(0),m_totalSecWeight(0),m_totalNetto(ZEROMONEY)

{
     
}

int CInvoiceDetailsDoc::GetPrintPages() const
{
	return m_nPages;
}


int CInvoiceDetailsDoc::LoadInputData(CPtrArray &docInputData)
{
	
	SetPricePerKgFormat(FloatFormat(PrintCfg().GetNum(GetCfgItemID(),PRICEPERKGPRECISION),
			                        (char)PrintCfg().GetNum(GetCfgItemID(),PRICEPERKGDECIMALPT)));

	m_invs2print.RemoveAll();
	m_invs2print.Append((CInvoiceDataArray&)docInputData);

	int pages = 0,size,i,ret = RELOAD_OK_CONTINUE;
	size = (int)m_invs2print.GetSize();
	Invoice *pinv;
	m_invs.RemoveAll();
	

	for(i =0;i < size;i++)
	{
		pinv = (Invoice*)m_invs2print[i];
		if(!pinv->HasCorrect())
			m_invs.Add(pinv);
		else ret = RELOAD_CAN_CONTINUE;
	}

	
	size = m_invs.GetSize();
	if(size > 0)
	{
		for(i = 0;i < size;i++)
				pages += GetInvoicePageSize((Invoice*)m_invs[i]);
		m_nPages = pages;
	} else ret = RELOAD_NO_CONTINUE;

	return ret;
}

int CInvoiceDetailsDoc::GetInvoicePageSize(Invoice *pInv)
{

	int pages = 1,cowsize = pInv->GetCowQty();

	if(cowsize > SPECFAKTURYPAGE1SIZE)
	{
		pages ++;
		if(cowsize > SPECFAKTURYPAGE1SIZE)
		{
			cowsize -= SPECFAKTURYPAGE1SIZE;
			do
			{
				if(cowsize > SPECFAKTURYPAGE1SIZE)		pages++;
				cowsize -= SPECFAKTURYPAGE2SIZE;

			} while (cowsize > 0);
		}
	}
	return pages;	
}

void CInvoiceDetailsDoc::SetPricePerKgFormat(const FloatFormat& fmt)
{
	m_PricePerKgFormat = fmt;
}

const FloatFormat& CInvoiceDetailsDoc::GetPricePerKgFormat() const
{
	return m_PricePerKgFormat;
}

//void CInvoiceDetailsDoc::PrintBottom()
//{
//	Money vat,totalGros;
//
//	vat = m_totalNetto*m_curInv->VATrate;
//	totalGros = m_totalNetto+vat;
//
//	DrawText("TOTAL_PRIMARY_WEIGHT",CIntString(m_totalWeight));
//	DrawText("TOTAL_SECONDARY_WEIGHT",CIntString(m_totalSecWeight));
//	DrawText("TOTAL_COW_AMOUNT",CStringFormat("%d szt.",m_curInv->GetCowQty()));
//	DrawText("VAT_RATE",CStringFormat("%.2f%%",m_curInv->VATrate));
//	DrawText("TOTAL_NETTO",m_totalNetto.ToString());
//	DrawText("TOTAL_VAT",vat.ToString());
//	DrawText("TOTAL_BRUTTO",totalGros.ToString());
//}

void CInvoiceDetailsDoc::PrintHeader(bool bWithOutHents)
{
	CString Str;
	const Hent *seller,*buyer,*pHent = m_curInv->GetInvoiceHent();
	InvoiceTypes invtype = m_curInv->GetRealInvoiceType();
	Hent companyHent(NULL_HENTID);
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

	if(!bWithOutHents)
	{
		PrintSeller(seller);
		PrintBuyer(buyer);
	}
	
	Str = m_curInv->GetInvoiceNoStr();

	CString sCustomNumber = m_curInv->GetCustomNumber();
	if(sCustomNumber.IsEmpty())
		DrawText("INVOICENO",Str);
	else
	{
		DrawText("INVOICENO",sCustomNumber);
		DrawText("SMALL_INVOICENO",Str);
	}

		DrawText("INVOICE_DATE",m_curInv->GetInvoiceDate().GetDayDate());
			
		
}
CString CInvoiceDetailsDoc::GetHentInfoString(const Hent *pHent)
{
	CString Str;
	Str.Format("%s\n%s %s\n%s %s\nNr ARiMR: %s",
		(LPCSTR)pHent->GetName(),
		(LPCSTR)pHent->GetStreet(),
		(LPCSTR)pHent->GetPOBox(),
		(LPCSTR)pHent->GetZip(),
		(LPCSTR)pHent->GetCity(),
		(LPCSTR)pHent->GetFarmNo());

	if(pHent->HasNIP())
		Str.AppendFormat("\nNIP: %s",(LPCSTR)pHent->GetNIP());
	
	if(pHent->HasREGON())
		Str.AppendFormat(", REGON: %s",(LPCSTR)pHent->GetREGON());	

	return Str;
}


void CInvoiceDetailsDoc::PrintBuyer(const Hent *buyer)
{
	CString Str = GetHentInfoString(buyer);
    DrawText("BUYER_NAME",Str); 
}

void CInvoiceDetailsDoc::PrintSeller(const Hent *seller)
{
	CString Str = GetHentInfoString(seller);
	DrawText("SELLER_NAME",Str);
}

void CInvoiceDetailsDoc::PreparePage(int nPage)
{

	int p = 0,i = 0,fp,size,grp;

	Invoice *newCurInv = NULL;
	int newCurInvPageSize,newCurInvLastPage;
	do
	{
		p++;
		fp  = p;
		newCurInv = (Invoice*)m_invs[i];
		newCurInvPageSize = GetInvoicePageSize(newCurInv);
		p += newCurInvPageSize-1;
		newCurInvLastPage = p;
		i++;
	}
	while(p < nPage);

	
	Cow *pCow = NULL;
	Money Cow::*price = NULL;
	NullInt Cow::*secondWeight= NULL;
	CowAccessor *pAccessor;		

	if(newCurInv != m_curInv)
	{
		m_curInv = newCurInv;
		m_curInvPageSize = newCurInvPageSize;
		m_curInvLastPage = newCurInvLastPage;
		m_cows.RemoveAll();

		m_curInv->GetInvoiceCows(m_cows);

		if(GetSortBy() != (COWDATA_MEMBERS)-1)
			m_cows.SortBy(GetSortBy(),GetSortOrder());
		
		m_totalNetto = ZEROMONEY;
		m_totalWeight = 0;
		m_totalSecWeight = 0;
	
		pAccessor = CowAccessorFactory::createAccessor(m_curInv->GetBuyOrSell());		
		for(i = 0; i < m_cows.GetSize();i++)
		{
			pCow = m_cows[i];			
			m_totalWeight += pCow->GetWeight();
			m_totalSecWeight += pAccessor->GetWeight(pCow).GetDouble();				
			m_totalNetto += pAccessor->GetPrice(pCow);
		}
		delete pAccessor;
	}

	ASSERT(m_cows.GetSize() == m_curInv->GetCowQty());

	size = m_curInv->GetCowQty();

	ResetGroupsVisibility(false);
	CString grpname;
	
	if(nPage == fp) 
	{
		m_starti = 0;

		if(size <= SPECFAKTURYPAGE1SIZE)
		{
		         SetGroupVisible("PAGEBOTTOM",true);
				 m_endi = size-1;
		}
		else m_endi = SPECFAKTURYPAGE2SIZE; //could cause endi overflow

		SetTemplatePage(0);
		m_pageOff = 0;
		grp = 0;
	}
	else
	{
		//SetGroupPrintVis("TABLEHEADER2",true);
		m_starti = SPECFAKTURYPAGE1SIZE + (nPage-fp-1)*SPECFAKTURYPAGE2SIZE;

		m_endi = m_starti+SPECFAKTURYPAGE2SIZE; //endi overflow
		
		SetTemplatePage(1);
		m_pageOff = nPage-fp;
		grp = 32;

		if(m_curInvLastPage == nPage)
			SetGroupVisible("PAGEBOTTOM2",true);
		
	}
				
	if(m_endi >= size) m_endi = size-1;
	
	for(i=m_starti;i<=m_endi;i++,grp++)
	{
		grpname.Format("%d",grp);
		SetGroupVisible(grpname,true);
	}
		

};

void CInvoiceDetailsDoc::PrintRecords(int sindex,int eindex,int row)
{
	CowAccessor* pAccessor = CowAccessorFactory::createAccessor(m_curInv->GetBuyOrSell());
	for(int i = sindex;i <= eindex;i++,row++)
	{		
		PrintRecord(row,i+1,pAccessor,m_cows[i]);
	}
	delete pAccessor;
}

void CInvoiceDetailsDoc::DrawWeight(const CString& sLabel,const Weight &weight)
{
	DrawText(sLabel,weight.ToString(GetWeightFormat()));
}

void CInvoiceDetailsDoc::DrawNullDouble(const CString& sLabel,const NullDouble& nullDouble,const FloatFormat& fmt)
{

	if(nullDouble.IsNull())
	{
		DrawText(sLabel,"-");
	}
	else
	{
		DrawText(sLabel,nullDouble.ToString(NULL,fmt));
	}
}

void CInvoiceDetailsDoc::PrintPage(int page)
{
	int row = 0;
	
	DrawText("PAGES",CStringFormat("%d/%d",m_pageOff+1,m_curInvPageSize));
	if(page == m_curInvLastPage)	PrintBottom();

    if(m_pageOff > 0)
	{
		PrintHeader(true);
	}
	else
	{			
		PrintHeader(false);	
	
		row = 0;
	}
		
	PrintRecords(m_starti,m_endi,row);
				
}
