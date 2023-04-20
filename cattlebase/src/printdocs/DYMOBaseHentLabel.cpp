#include "stdafx.h"
#include "DYMOBaseHentLabel.h"

IMPLEMENT_DYNCREATE(CDYMOBaseHentLabel,CDYMOBaseDoc)

CDYMOBaseHentLabel::CDYMOBaseHentLabel()
{
}

int CDYMOBaseHentLabel::LoadInputData(CPtrArray &docInputData)
{
	m_hents.RemoveAll();
	m_hents.Append((CHentArray&)docInputData);
	return RELOAD_OK_CONTINUE;
}



int CDYMOBaseHentLabel::GetPrintPages() const
{
	return m_hents.GetCount();
}

void CDYMOBaseHentLabel::SetFields(int nLabelPage)
{
	Hent* pHent = m_hents[nLabelPage-1];
	CString hentData;
	hentData.Format("%s %s\n%s %s\nTel. %-12s",
		(LPCSTR)pHent->GetStreet(),
		(LPCSTR)pHent->GetPOBox(),
		(LPCSTR)pHent->GetCity(),
		(LPCSTR)pHent->GetZip(),
		(LPCSTR)pHent->GetPhone());

	
	DrawText("NAME",pHent->GetName());
	DrawText("DATA",hentData);
	DrawBarcode("BARCODE",pHent->GetFarmNo());
}

