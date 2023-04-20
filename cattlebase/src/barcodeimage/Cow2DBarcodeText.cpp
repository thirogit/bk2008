#include "stdafx.h"
#include "Cow2DBarcodeText.h"
#include "../datatypes/Hent.h"

CCow2DBarcodeText::CCow2DBarcodeText(const Cow* pCow) : m_pCow(pCow)
{
}

CString CCow2DBarcodeText::GetBarcodeText() const
{

	CString bcText;
	PtrFlushArray<CString> bcTexts;

	
	bcTexts.Add(new CString(m_pCow->GetEAN()));
	bcTexts.Add(new CString(GetCowSex(m_pCow->GetSex())));
	bcTexts.Add(new CString(GetString(m_pCow->GetStock()->GetStockCode())));
	bcTexts.Add(new CString());
	bcTexts.Add(new CString(m_pCow->GetPassNo()));
	bcTexts.Add(new CString(GetDate(m_pCow->GetPassDate())));
	bcTexts.Add(new CString(m_pCow->GetMotherEAN()));
	bcTexts.Add(new CString(GetDate(m_pCow->GetBirthDate())));
	bcTexts.Add(new CString(GetString(m_pCow->GetBirthplace())));
	Hent* pFisrtOwner = m_pCow->GetFirstOwner();
	if(pFisrtOwner)
	{
		bcTexts.Add(new CString(pFisrtOwner->GetFarmNo()));
		bcTexts.Add(new CString(GetString(pFisrtOwner->GetName())));
		bcTexts.Add(new CString(GetString(pFisrtOwner->GetStreet())));
		bcTexts.Add(new CString(GetString(pFisrtOwner->GetPOBox())));
		bcTexts.Add(new CString(GetString(pFisrtOwner->GetCity())));
		bcTexts.Add(new CString(GetString(pFisrtOwner->GetZip())));
	}

	PtrFlushArray<CString>::ArrayIterator bcTextsIt = bcTexts.iterator();
	while(bcTextsIt.hasNext())
	{
		CString* bcTextPart = *(bcTextsIt++);
		bcText.Append(*bcTextPart);
		if(bcTextsIt.hasNext())
			bcText.AppendChar(';');
	}

	bcTexts.FreeAndFlush();

	return bcText;

}

CString CCow2DBarcodeText::GetString(const CString& s) const
{
	if(s.IsEmpty())
		return s;
	CString _s(s);
	_s.Replace(";","\\;");
	_s.Replace("\n","\\n");
	_s.Replace("\r","\\r");
	return _s;
}
CString CCow2DBarcodeText::GetDate(const CDateTime& dt) const
{
	if(dt.IsNull())
		return CString();

	return GetString(dt.GetDateInFmt(CDateTime::YYYYMMDD));
}
CString CCow2DBarcodeText::GetCowSex(const CowSex& cowsex) const
{
	if(cowsex.IsUNK())
		return CString();

	return (LPCSTR)cowsex;
}
