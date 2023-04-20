#include "stdafx.h"
#include "SokolowDoc.h"
#include "PrintDocsCfg.h"
#include "../logo/Logo.h"
#include "../datatypes/CompanyInfo.h"
#include "../Utils/StringFormat.h"

IMPLEMENT_DYNCREATE(CSokolowDoc, CCowDrwDoc)


CSokolowDoc::CSokolowDoc() 
{    
}
int CSokolowDoc::LoadInputData(CPtrArray &docInputData)
{
	m_cows.RemoveAll();
	m_cows.Append((CCowArray&)docInputData);

	bool bRemoved = false;
	CCowArray::ArrayIterator cowIt = m_cows.iterator();
	while(cowIt.hasNext())
	{
		if(!((*cowIt)->HasFirstOwner()))
		{
			cowIt.remove();
			bRemoved = true;
		}
		else
			cowIt++;
	}
	
	return bRemoved ? RELOAD_CAN_CONTINUE : RELOAD_OK_CONTINUE;
}

void CSokolowDoc::PreparePage(int nPage)
{
	Cow *pCow = m_cows[nPage-1];	

	m_barcodeStream.Close();
	m_c39Barcode.LoadData(pCow->GetEAN(),CODE39_NARROW_BAR,CODE39_BARCODE_HEIGHT_INCH,CODE39_BARCODE_RATIO,CODE39_BARCODE_DPI);

	if(m_barcodeImage.DrawBarcode(m_c39Barcode))
		m_barcodeImage.GetImageStream(m_barcodeStream);

	if(!m_logoStream.m_lpStream)
		CLogoImage::GetLogo()->GetOleLogoImage(m_logoStream);
	

			
}

int CSokolowDoc::GetPrintPages() const
{
	return m_cows.GetSize();
}
void CSokolowDoc::PrintPage(int page)
{
	Cow *pCow = m_cows[page-1];	
	Hent* fstownr = pCow->GetFirstOwner();
			    		
	DrawText("NAME",(LPCSTR)fstownr->GetName());
	DrawText("ZIPCODE",(LPCSTR)fstownr->GetZip());
	DrawText("CITY",(LPCSTR)fstownr->GetCity());
	DrawText("ADDRESS",CStringFormat("%s %s",(LPCSTR)fstownr->GetStreet(),(LPCSTR)fstownr->GetPOBox()));
	DrawText("FARMNO",(LPCSTR)fstownr->GetFarmNo());
	DrawText("DELIVERNO","");
	DrawText("DATE",CDateTime::now().GetDayDate());
	DrawText("PLACE",(LPCSTR)Company::GetCompany()->GetCity());
	DrawText("COW_EAN",pCow->GetEAN());	
	DrawImage("BARCODE_IMAGE",m_barcodeStream);
	m_barcodeStream.Seek(0,STREAM_SEEK_SET);
	DrawImage("LOGO",m_logoStream);
	m_logoStream.Seek(0,STREAM_SEEK_SET);

}
