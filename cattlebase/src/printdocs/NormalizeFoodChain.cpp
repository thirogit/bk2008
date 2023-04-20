#include "stdafx.h"
#include "NormalizeFoodChain.h"
#include "PrintDocsCfg.h"
#include "../Utils/StringFormat.h"
#include "../classes/IntString.h"
#include "zapisslowny.h"



IMPLEMENT_DYNCREATE(CNormalizeFoodChain, CCowDrwDoc)

CNormalizeFoodChain::CNormalizeFoodChain() 
{    
}

CNormalizeFoodChain::~CNormalizeFoodChain()
{
	m_content.FreeAndFlush();
}

int CNormalizeFoodChain::LoadInputData(CPtrArray &invInputData)
{
	m_content.FreeAndFlush();
	for(int iInvoice = 0,sizeInvoice = invInputData.GetSize();iInvoice < sizeInvoice;iInvoice++)
	{
		CCowArray cows;
		Invoice* pInvoice = ((Invoice*)invInputData.GetAt(iInvoice));
		pInvoice->GetInvoiceCows(cows);
		
		for(int iCow = 0,sizeCows = cows.GetSize(); iCow < sizeCows;iCow+=NORM_FOOD_CHAIN_COWS_ON_PAGE)
		{
			OnePageContent* pOnePage = new OnePageContent();
			pOnePage->firstOwner = pInvoice->GetInvoiceHent();

			for(int iCowOnPage = 0;
				iCowOnPage < NORM_FOOD_CHAIN_COWS_ON_PAGE && (iCow+iCowOnPage) < sizeCows;
				iCowOnPage++)
			{
				pOnePage->cows.Add(cows[iCow+iCowOnPage]);
			}
			m_content.Add(pOnePage);
		}
	}	
	return RELOAD_OK_CONTINUE;
}

int CNormalizeFoodChain::GetPrintPages() const
{	
	return m_content.GetSize();		
}
CString CNormalizeFoodChain::FormatCowNo(Cow *pCow)
{
	CString result = pCow->GetEAN();
	CString sPassNo = pCow->GetPassNo();
	CString sHealthCertNo = pCow->GetHealthCertNo();

	if(!sPassNo.IsEmpty())
	{
		result += ", (";
		result += sPassNo;
		result += ')';
	}

	if(!sHealthCertNo.IsEmpty())
	{
		result += ", [";
		result += sHealthCertNo;
		result += ']';
	}
	return result;
}

void CNormalizeFoodChain::PrintPage(int page)
{
	OnePageContent* pOnePage = m_content[(page-1)];

	Hent* pFstOwner = pOnePage->firstOwner;
	DrawText("FIRST_OWNER_NAME",pFstOwner->GetName());

	CString fstOwnerAddress = pFstOwner->GetStreet() + ' ' + 
		                      pFstOwner->GetPOBox() + ", " +
							  pFstOwner->GetZip() + ' ' + 
							  pFstOwner->GetCity();

	DrawText("FIRST_OWNER_ADDRESS",fstOwnerAddress);
	DrawText("FIRST_OWNER_PHONENO",pFstOwner->GetPhone());
	DrawText("FIRST_OWNER_HENTNO",pFstOwner->GetFarmNo());

	int s = pOnePage->cows.GetSize();
	DrawText("COW_AMOUNT_NUM",CIntString(s));

	CString sCowAmountText;
	CZapisSlowny cowAmount;
	cowAmount.ZapisSlowny(s,sCowAmountText);
	DrawText("COW_AMOUNT_TXT",sCowAmountText);
		
	for(int i=0;i<s;i++)	
	{				
		DrawText(CStringFormat("COW_%d",i),FormatCowNo(pOnePage->cows[i]));
	}
}
