#include "stdafx.h"
#include "raportARiMR.h"
//#include "../classes/types/CowSex.h"
//#include "PrintDocsCfg.h"
//#include "../consts.h"
//#include "datatypes/CompanyInfo.h"
#include "../../Utils/StringFormat.h"

IMPLEMENT_DYNCREATE(CRaportARiMR, CHerdBook)



int CRaportARiMR::GetCowsOnPage() const
{
	return RAPORTPAGESIZE;
}

void CRaportARiMR::DrawCowRow(int iRow,int iSerialNum,const Cow* pCow)
{
				
	DrawText(CStringFormat("NUMBER%d",iRow),CStringFormat("%d",iSerialNum));
					
	PrintEAN(pCow->GetEAN(),CStringFormat("COW%d_%%d",iRow));
						
	DrawText(CStringFormat("BIRTHDATE%d",iRow),pCow->GetBirthDate().GetDayDate());

	DrawText(CStringFormat("INDATE%d",iRow),pCow->GetInDoc()->GetLoadDate().GetDayDate());

	
	Hent* pLastOwner = NULL;
	if(pCow->HasFirstOwner())//if(pCow->HasRRBuyInvoice())
		pLastOwner = pCow->GetFirstOwner();
	else
		pLastOwner = pCow->GetDeliver();
                     
					    			
    DrawText(CStringFormat("LASTOWNER%d",iRow),FormatHent(pLastOwner));

	CowSex sex = pCow->GetSex();
	if(sex.IsXX())
		DrawText(CStringFormat("XX_BIG%d",iRow),CString((LPCSTR)sex)); 
	else
		DrawText(CStringFormat("XY_BIG%d",iRow),CString((LPCSTR)sex));
						
	Stock* pCowStock = pCow->GetStock();
	CString sOfficialStockCode = pCowStock->GetOfficialCode();

	if(sOfficialStockCode.IsEmpty())
		sOfficialStockCode = pCowStock->GetStockCode();

	DrawText(CStringFormat("STOCK%d",iRow),sOfficialStockCode);
						
	PrintEAN(pCow->GetMotherEAN(),CStringFormat("MOTHERNO%d_%%d",iRow));

	if(pCow->IsOut())
	{
		DrawText(CStringFormat("OUTDATE%d",iRow),pCow->GetOutDoc()->GetLoadDate().GetDayDate());
	
		DrawText(CStringFormat("DESTPLACE%d",iRow),FormatHent(pCow->GetBuyer()));
		
		DrawText(CStringFormat("OUTREASON%d",iRow),pCow->GetOutDoc()->GetReason()->GetReasonCode());
	}

}

