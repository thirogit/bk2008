#include "stdafx.h"
#include "Notifications.h"
#include "../../datatypes/CompanyInfo.h"
#include "../../datatypes/OutDoc.h"

DisplacementDirection InDocDisplacementNotificationNavigator::GetDirection(void* pElement)
{
	return DisplacementDirection(((InDoc*)pElement)->GetHent(),Company::GetCompany()->GetAsHent());
}

DisplacementDirection OutDocDisplacementNotificationNavigator::GetDirection(void* pElement)
{
	return DisplacementDirection(Company::GetCompany()->GetAsHent(),((OutDoc*)pElement)->GetHent());
}

DisplacementDirection BuyInvoiceDisplacementNotificationNavigator::GetDirection(void* pElement)
{
	return DisplacementDirection(((Invoice*)pElement)->GetInvoiceHent(),Company::GetCompany()->GetAsHent());
}

DisplacementDirection SellInvoiceDisplacementNotificationNavigator::GetDirection(void* pElement)
{
	return DisplacementDirection(Company::GetCompany()->GetAsHent(),((Invoice*)pElement)->GetInvoiceHent());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
BaseNotificationRecordConverter::BaseNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator) : m_pNavigator(pNavigator)
{
}

DisplacementDirection BaseNotificationRecordConverter::GetDiretion(void *pElement)
{
	return m_pNavigator->GetDirection(pElement);
}

UINT BaseNotificationRecordConverter::GetExportEvents(const DisplacementDirection& direction)
{
	const Hent* pDestHent = direction.GetDestHent();
	const Hent* pSrcHent = direction.GetSrcHent();

	Country* pDestCountry = pDestHent->GetCountry();
	Country* pSrcCountry = pSrcHent->GetCountry();
	UINT uiEvents = CZgloszenieBase::EVENT_NONE;

	if(pDestCountry != pSrcCountry)
	{
		if(pDestCountry->IsCountryInEU())
		{
			uiEvents &= CZgloszenieBase::EVENT_EXPORT_TO_EU;
		}
		else
		{
			uiEvents &= CZgloszenieBase::EVENT_EXPORT_NOTTO_EU;
		}
	}
	return uiEvents;
}


InDocNotificationRecordConverter::InDocNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator) :
		BaseNotificationRecordConverter(pNavigator)
{
}

NotificationRecord* InDocNotificationRecordConverter::convert(void *pElement)
{
	CCowArray cows;
	
	InDoc *pDoc = (InDoc*)pElement;

	NotificationRecord *pRecord = new NotificationRecord();
	DisplacementDirection direction = GetDiretion(pElement);

	pRecord->SetEventDate(pDoc->GetLoadDate());
	
	pRecord->SetSrcHent(direction.GetSrcHent());
	pRecord->SeDstHent(direction.GetDestHent());
		
	pRecord->SetEvents(GetExportEvents(direction) | CZgloszenieBase::EVENT_BUY);
		

	cows.RemoveAll();
	cows.AppendPtr(pDoc->GetCows());
	pRecord->SetCows(cows);

	return pRecord;
}

OutDocNotificationRecordConverter::OutDocNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator) :
		BaseNotificationRecordConverter(pNavigator)
{
}

NotificationRecord* OutDocNotificationRecordConverter::convert(void *pElement)
{
	CCowArray cows;
	
	OutDoc *pDoc = (OutDoc*)pElement;

	NotificationRecord *pRecord = new NotificationRecord();
	DisplacementDirection direction = GetDiretion(pElement);

	pRecord->SetEventDate(pDoc->GetLoadDate());
	
	pRecord->SetSrcHent(direction.GetSrcHent());
	pRecord->SeDstHent(direction.GetDestHent());
		
	pRecord->SetEvents(GetExportEvents(direction) | CZgloszenieBase::EVENT_SELL);
		

	cows.RemoveAll();
	cows.AppendPtr(pDoc->GetCows());
	pRecord->SetCows(cows);

	return pRecord;
}

SellInvoiceNotificationRecordConverter::SellInvoiceNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator) :
		BaseNotificationRecordConverter(pNavigator)
{
}

NotificationRecord* SellInvoiceNotificationRecordConverter::convert(void *pElement)
{
		
	CCowArray invoiceCows;
	Invoice *pInvoice = (Invoice*)pElement;
	NotificationRecord *pRecord = new NotificationRecord();
	DisplacementDirection direction = GetDiretion(pElement);
	
	pRecord->SetSrcHent(direction.GetSrcHent());
	pRecord->SeDstHent(direction.GetDestHent());

	pRecord->SetEventDate(pInvoice->GetInvoiceDate());

	pRecord->SetEvents(GetExportEvents(direction) | CZgloszenieBase::EVENT_SELL);

	invoiceCows.RemoveAll();
	pInvoice->GetInvoiceCows(invoiceCows);
	pRecord->SetCows(invoiceCows);
	return pRecord;

}


BuyInvoiceNotificationRecordConverter::BuyInvoiceNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator) :
		BaseNotificationRecordConverter(pNavigator)
{
}

NotificationRecord* BuyInvoiceNotificationRecordConverter::convert(void *pElement)
{
	CCowArray invoiceCows;
	Invoice *pInvoice = (Invoice*)pElement;
	NotificationRecord *pRecord = new NotificationRecord();
	DisplacementDirection direction = GetDiretion(pElement);
	
	pRecord->SetSrcHent(direction.GetSrcHent());
	pRecord->SeDstHent(direction.GetDestHent());

	pRecord->SetEventDate(pInvoice->GetInvoiceDate());

	pRecord->SetEvents(GetExportEvents(direction) | CZgloszenieBase::EVENT_BUY);

	invoiceCows.RemoveAll();
	pInvoice->GetInvoiceCows(invoiceCows);
	pRecord->SetCows(invoiceCows);
	return pRecord;
}

CZgloszenie60::CZgloszenie60(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator) : 
	CNotificationDuplex(pConverter,pNavigator)
{

}

int CZgloszenie60::GetNumberOfRecordsPerSheet()
{
	return 60;
}

int CZgloszenie60::GetNumberOfRecordsOnFrontPage()
{
	return 40;
}


CZgloszenie30::CZgloszenie30(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator) : 
	CNotificationSimplex(pConverter,pNavigator)
{

}

int CZgloszenie30::GetNumberOfRecordsPerSheet()
{
	return 30;
}


CZgloszenie4::CZgloszenie4(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator) : 
	CNotificationSimplex(pConverter,pNavigator)
{

}

int CZgloszenie4::GetNumberOfRecordsPerSheet()
{
	return 4;
}

//// In Doc

IMPLEMENT_DYNCREATE(CZgloszenie60InDoc,CCowDrwDoc);
CZgloszenie60InDoc::CZgloszenie60InDoc() : m_Converter(&m_Navigator),CZgloszenie60(&m_Converter,&m_Navigator)
{
}

IMPLEMENT_DYNCREATE(CZgloszenie30InDoc,CCowDrwDoc);
CZgloszenie30InDoc::CZgloszenie30InDoc() : m_Converter(&m_Navigator),CZgloszenie30(&m_Converter,&m_Navigator)
{
}

IMPLEMENT_DYNCREATE(CZgloszenieCardInDoc,CCowDrwDoc);
CZgloszenieCardInDoc::CZgloszenieCardInDoc() : m_Converter(&m_Navigator),CZgloszenie4(&m_Converter,&m_Navigator)
{
}

//
//////Buy Inv

IMPLEMENT_DYNCREATE(CZgloszenie60BuyInv,CCowDrwDoc);
CZgloszenie60BuyInv::CZgloszenie60BuyInv() : m_Converter(&m_Navigator),CZgloszenie60(&m_Converter,&m_Navigator)
{
}

IMPLEMENT_DYNCREATE(CZgloszenie30BuyInv,CCowDrwDoc);
CZgloszenie30BuyInv::CZgloszenie30BuyInv() : m_Converter(&m_Navigator),CZgloszenie30(&m_Converter,&m_Navigator)
{
}

IMPLEMENT_DYNCREATE(CZgloszenieCardBuyInv,CCowDrwDoc);
CZgloszenieCardBuyInv::CZgloszenieCardBuyInv() : m_Converter(&m_Navigator),CZgloszenie4(&m_Converter,&m_Navigator)
{
}



////Out Doc

IMPLEMENT_DYNCREATE(CZgloszenie60OutDoc,CCowDrwDoc);
CZgloszenie60OutDoc::CZgloszenie60OutDoc() : m_Converter(&m_Navigator), CZgloszenie60(&m_Converter,&m_Navigator)
{
}

IMPLEMENT_DYNCREATE(CZgloszenie30OutDoc,CCowDrwDoc);
CZgloszenie30OutDoc::CZgloszenie30OutDoc() : m_Converter(&m_Navigator), CZgloszenie30(&m_Converter,&m_Navigator)
{
}

IMPLEMENT_DYNCREATE(CZgloszenieCardOutDoc,CCowDrwDoc);
CZgloszenieCardOutDoc::CZgloszenieCardOutDoc() : m_Converter(&m_Navigator), CZgloszenie4(&m_Converter,&m_Navigator)
{
}

//////Sell Inv
//
IMPLEMENT_DYNCREATE(CZgloszenie60SellInv,CCowDrwDoc);
CZgloszenie60SellInv::CZgloszenie60SellInv() : m_Converter(&m_Navigator), CZgloszenie60(&m_Converter,&m_Navigator)
{
}

IMPLEMENT_DYNCREATE(CZgloszenie30SellInv,CCowDrwDoc);
CZgloszenie30SellInv::CZgloszenie30SellInv() : m_Converter(&m_Navigator), CZgloszenie30(&m_Converter,&m_Navigator)
{
}

IMPLEMENT_DYNCREATE(CZgloszenieCardSellInv,CCowDrwDoc);
CZgloszenieCardSellInv::CZgloszenieCardSellInv() : m_Converter(&m_Navigator), CZgloszenie4(&m_Converter,&m_Navigator)
{
}







