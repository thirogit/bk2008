#include "stdafx.h"
#include "InvoiceDetailsExternal.h"
#include "../../Utils/StringFormat.h"
#include "../../classes/IntString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CInvoiceDetailsExternalDoc, CInvoiceDetailsDoc)

CInvoiceDetailsExternalDoc::CInvoiceDetailsExternalDoc()
{
     
}
void CInvoiceDetailsExternalDoc::PrintBottom()
{
	Money vat,totalGros;

	vat = m_totalNetto*m_curInv->GetVATRate();
	totalGros = m_totalNetto+vat;	
	DrawText("TOTAL_WEIGHT",CStringFormat("%s kg",(LPCSTR)m_totalSecWeight.ToString(GetWeightFormat())));
	DrawText("TOTAL_COW_AMOUNT",CStringFormat("%d szt.",m_curInv->GetCowQty()));
	DrawText("VAT_RATE",CStringFormat("%.2f%%",m_curInv->GetVATRate()));
	DrawText("TOTAL_NETTO",m_totalNetto.ToString(NULL,GetMoneyFormat()));
	DrawText("TOTAL_VAT",vat.ToString(NULL,GetMoneyFormat()));
	DrawText("TOTAL_BRUTTO",totalGros.ToString(NULL,GetMoneyFormat()));
}

void CInvoiceDetailsExternalDoc::PrintRecord(int row,int iLP,CowAccessor* pAccessor,Cow *pCow)
{
		DrawText(CStringFormat("LP%d",row),CIntString(iLP));		
		DrawText(CStringFormat("SECONDSTOCK%d",row),(LPCSTR)pAccessor->GetStock(pCow)->GetStockCode());		
		DrawNullDouble(CStringFormat("SECONDWEIGHT%d",row),pAccessor->GetWeight(pCow),GetWeightFormat());


		double realWeight = pAccessor->GetWeight(pCow).GetDouble();
		Money unitPriceNet = pAccessor->GetPrice(pCow)/realWeight;

		DrawText(CStringFormat("UNITPRICENETTO%d",row),unitPriceNet.ToString(NULL,GetPricePerKgFormat()));
		
		DrawText(CStringFormat("COWNO%d",row),pCow->GetEAN());			
		DrawText(CStringFormat("NETTO%d",row),(pAccessor->GetPrice(pCow)).ToString(NULL,GetMoneyFormat()));
		Money vatValue =  pAccessor->GetPrice(pCow)*m_curInv->GetVATRate();
		DrawText(CStringFormat("VAT%d",row),vatValue.ToString(NULL,GetMoneyFormat()));
		DrawText(CStringFormat("BRUTTO%d",row),(pAccessor->GetPrice(pCow)+vatValue).ToString(NULL,GetMoneyFormat()));
		DrawText(CStringFormat("ALIAS%d",row),m_curInv->GetInvoiceHent()->GetAlias());
}
