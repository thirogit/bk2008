#include "stdafx.h"
#include "KsiegaStada.h"
#include "../../Utils/StringFormat.h"
#include "../../datatypes/CompanyInfo.h"

IMPLEMENT_DYNCREATE(CKsiegaStada, CHerdBook)



int CKsiegaStada::GetCowsOnPage() const
{
	return KSIEGASTADA_COWSONPAGE;
}

void CKsiegaStada::DrawCowRow(int iRow,int iSerialNum,const Cow* pCow)
{
				
	DrawText(CStringFormat("LP_%d",iRow),CStringFormat("%d",iSerialNum));
					
	DrawText(CStringFormat("COWNO_%d",iRow),pCow->GetEAN());
						
	DrawText(CStringFormat("BIRTHDT_%d",iRow),pCow->GetBirthDate().GetDayDate());

	DrawText(CStringFormat("INDT_%d",iRow),pCow->GetInDoc()->GetLoadDate().GetDayDate());

	
	Hent* pLastOwner = NULL;
	//if(pCow->HasFirstOwner())
	if(pCow->HasRRBuyInvoice())
		pLastOwner = pCow->GetFirstOwner();
	else
		pLastOwner = pCow->GetDeliver();
                     
					    			
    DrawText(CStringFormat("FROMPLACE_%d",iRow),FormatHent(pLastOwner));
	
	DrawText(CStringFormat("SEX_%d",iRow),CString((LPCSTR)pCow->GetSex())); 
	
	DrawText(CStringFormat("INREASON_%d",iRow),pCow->GetInDoc()->GetReason()->GetReasonCode());
	

	Stock* pCowStock = pCow->GetStock();
	CString sOfficialStockCode = pCowStock->GetOfficialCode();

	if(sOfficialStockCode.IsEmpty())
		sOfficialStockCode = pCowStock->GetStockCode();

	DrawText(CStringFormat("STOCK_%d",iRow),sOfficialStockCode);
						
	DrawText(CStringFormat("MOTHERNO_%d",iRow),pCow->GetMotherEAN());

	if(pCow->IsOut())
	{
		DrawText(CStringFormat("OUTDT_%d",iRow),pCow->GetOutDoc()->GetLoadDate().GetDayDate());
	
		DrawText(CStringFormat("TOPLACE_%d",iRow),FormatHent(pCow->GetBuyer()));
		
		DrawText(CStringFormat("OUTREASON_%d",iRow),pCow->GetOutDoc()->GetReason()->GetReasonCode());

		DrawText(CStringFormat("CARRIER_%d",iRow),FormatCompany());
	}

}

CString CKsiegaStada::FormatCompany()
{
	const char szAddressFmt[] = "%s, %s\n%s %s,%s %s";
	const Company* company = Company::GetCompany();
	return CStringFormat(szAddressFmt,
						(LPCSTR)company->GetFarmNo(),
						(LPCSTR)company->GetCompanyName(),						
						(LPCSTR)company->GetStreet(),
						(LPCSTR)company->GetPOBox(),
						(LPCSTR)company->GetZipCode(),
						(LPCSTR)company->GetCity());


}