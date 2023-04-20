#ifndef __ZGLOSZENIE_H__
#define __ZGLOSZENIE_H__

#include "NotificationSimplex.h"
#include "NotificationDuplex.h"
#include "Notification.h"

//-------NAVIGATORS--------------------------------------------------------------------------------
class InDocDisplacementNotificationNavigator : public DisplacementNotificationNavigator
{
public:
	virtual DisplacementDirection GetDirection(void* pElement);
};

class OutDocDisplacementNotificationNavigator : public DisplacementNotificationNavigator
{
public:
	virtual DisplacementDirection GetDirection(void* pElement);
};

class BuyInvoiceDisplacementNotificationNavigator : public DisplacementNotificationNavigator
{
public:
	virtual DisplacementDirection GetDirection(void* pElement);
};

class SellInvoiceDisplacementNotificationNavigator : public DisplacementNotificationNavigator
{
public:
	virtual DisplacementDirection GetDirection(void* pElement);
};
//------CONVERTERS---------------------------------------------------------------------------------
class BaseNotificationRecordConverter : public NotificationRecordConverter
{
public:
	BaseNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator);
	virtual NotificationRecord* convert(void *pElement) = 0;
protected:
	DisplacementDirection GetDiretion(void *pElement);
	UINT GetExportEvents(const DisplacementDirection& direction);
private:
	DisplacementNotificationNavigator* m_pNavigator;
};


class InDocNotificationRecordConverter : public BaseNotificationRecordConverter
{
public:
	InDocNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator);
	virtual NotificationRecord* convert(void *pElement);
};

class OutDocNotificationRecordConverter : public BaseNotificationRecordConverter
{
public:
	OutDocNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator);
	virtual NotificationRecord* convert(void *pElement);
};

class SellInvoiceNotificationRecordConverter : public BaseNotificationRecordConverter
{
public:
	SellInvoiceNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator);
	virtual NotificationRecord* convert(void *pElement);
};

class BuyInvoiceNotificationRecordConverter : public BaseNotificationRecordConverter
{
public:
	BuyInvoiceNotificationRecordConverter(DisplacementNotificationNavigator* pNavigator);
	virtual NotificationRecord* convert(void *pElement);
};


class CZgloszenie60 : public CNotificationDuplex
{
public:
	CZgloszenie60(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator);
protected:
	virtual int GetNumberOfRecordsPerSheet();
	virtual int GetNumberOfRecordsOnFrontPage();
};

class CZgloszenie30 : public CNotificationSimplex
{
public:
	CZgloszenie30(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator);
protected:
	virtual int GetNumberOfRecordsPerSheet();	
};


class CZgloszenie4 : public CNotificationSimplex
{
public:
	CZgloszenie4(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator);
protected:
	virtual int GetNumberOfRecordsPerSheet();	
};


class CZgloszenie60InDoc : public CZgloszenie60
{
	DECLARE_DYNCREATE(CZgloszenie60InDoc);
public:
	CZgloszenie60InDoc();
private:
	InDocNotificationRecordConverter m_Converter;
	InDocDisplacementNotificationNavigator m_Navigator;
};

class CZgloszenieCardInDoc : public CZgloszenie4
{
	DECLARE_DYNCREATE(CZgloszenieCardInDoc);
public:
	CZgloszenieCardInDoc();
private:
	InDocNotificationRecordConverter m_Converter;
	InDocDisplacementNotificationNavigator m_Navigator;
};

class CZgloszenie30InDoc : public CZgloszenie30
{
	DECLARE_DYNCREATE(CZgloszenie30InDoc);
public:
	CZgloszenie30InDoc();
private:
	InDocNotificationRecordConverter m_Converter;
	InDocDisplacementNotificationNavigator m_Navigator;
};





class CZgloszenie60BuyInv : public CZgloszenie60
{
	DECLARE_DYNCREATE(CZgloszenie60BuyInv);
public:
	CZgloszenie60BuyInv();
private:
	BuyInvoiceNotificationRecordConverter m_Converter;
	BuyInvoiceDisplacementNotificationNavigator m_Navigator;
};

class CZgloszenie30BuyInv :  public CZgloszenie30
{
	DECLARE_DYNCREATE(CZgloszenie30BuyInv);
public:
	CZgloszenie30BuyInv();
private:
	BuyInvoiceNotificationRecordConverter m_Converter;
	BuyInvoiceDisplacementNotificationNavigator m_Navigator;
};

class CZgloszenieCardBuyInv : public CZgloszenie4
{
	DECLARE_DYNCREATE(CZgloszenieCardBuyInv);
public:
	CZgloszenieCardBuyInv();
private:
	BuyInvoiceNotificationRecordConverter m_Converter;
	BuyInvoiceDisplacementNotificationNavigator m_Navigator;
};



class CZgloszenie60OutDoc : public CZgloszenie60
{
	DECLARE_DYNCREATE(CZgloszenie60OutDoc);
public:
	CZgloszenie60OutDoc();
private:
	OutDocNotificationRecordConverter m_Converter;
	OutDocDisplacementNotificationNavigator m_Navigator;
};

class CZgloszenie30OutDoc :  public CZgloszenie30
{
	DECLARE_DYNCREATE(CZgloszenie30OutDoc);
public:
	CZgloszenie30OutDoc();
private:
	OutDocNotificationRecordConverter m_Converter;
	OutDocDisplacementNotificationNavigator m_Navigator;
};

class CZgloszenieCardOutDoc : public CZgloszenie4
{
	DECLARE_DYNCREATE(CZgloszenieCardOutDoc);
public:
	CZgloszenieCardOutDoc();
private:
	OutDocNotificationRecordConverter m_Converter;
	OutDocDisplacementNotificationNavigator m_Navigator;
};


class CZgloszenie60SellInv : public CZgloszenie60
{
	DECLARE_DYNCREATE(CZgloszenie60SellInv);
public:
	CZgloszenie60SellInv();	
private:
	SellInvoiceNotificationRecordConverter m_Converter;
	SellInvoiceDisplacementNotificationNavigator m_Navigator;
};

class CZgloszenie30SellInv :  public CZgloszenie30
{
	DECLARE_DYNCREATE(CZgloszenie30SellInv);
public:
	CZgloszenie30SellInv();	
private:
	SellInvoiceNotificationRecordConverter m_Converter;
	SellInvoiceDisplacementNotificationNavigator m_Navigator;
};

class CZgloszenieCardSellInv : public CZgloszenie4
{
	DECLARE_DYNCREATE(CZgloszenieCardSellInv);
public:
	CZgloszenieCardSellInv();
private:
	SellInvoiceNotificationRecordConverter m_Converter;
	SellInvoiceDisplacementNotificationNavigator m_Navigator;
};


#endif