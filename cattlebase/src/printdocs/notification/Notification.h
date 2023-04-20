#ifndef __DISPLACEMENTNOTIFICATEIONDOC_H__
#define __DISPLACEMENTNOTIFICATEIONDOC_H__

#include "../../DataTypes.h"
#include "../../printing/CowDrwDoc.h"
#include "../CustomPrintDocOptDlg.h"
#include "../PrintDocsCfg.h"
#include "../../ctrls/checkabegroup/CheckableGroupBox.h"
#include <boost/shared_ptr.hpp>


class DisplacementDirection
{
public:
	DisplacementDirection(const Hent* pSrcHent,const Hent* pDestHent);
	DisplacementDirection(const DisplacementDirection& src);
	DisplacementDirection& operator=(const DisplacementDirection& src);
	void CopyFrom(const DisplacementDirection& src);

	const Hent* GetDestHent() const;
	const Hent* GetSrcHent() const;
private:
	const Hent* m_pDestHent;
	const Hent* m_pSrcHent;
};

class DisplacementNotificationNavigator
{
public:
	virtual DisplacementDirection GetDirection(void* pElement) = 0;
};

class NotificationRecord
{
public:
	NotificationRecord();

	void SetSrcHent(const Hent* pSrcHent);
	void SeDstHent(const Hent* pDstHent);
	void SetEvents(UINT uiEvents);
	void SetEventDate(const CDateTime& eventDt);
	void SetCows(CCowArray& cows);
	
	const Hent* GetSrcHent() const;
	const Hent* GetDstHent() const;
	const CDateTime& GetEventDate() const;
	UINT GetEvents() const;

	int GetCowCount() const;
	CowArrayPtr GetCows();

private:
	CCowArray m_Cows;
	CDateTime m_EventDate;
	UINT m_Events;
	const Hent* m_pSrcHent;
	const Hent* m_pDstHent;
};


class NotificationRecordConverter
{
public:
	virtual NotificationRecord* convert(void *pElement) = 0;
};


class CZgloszenieBase : public CCowDrwDoc
{
public:
	enum EVENT_TYPE
	{
		EVENT_NONE = 0,
		EVENT_BUY = 1,
		EVENT_SELL = 2,
		EVENT_BUY_AND_SELL = 4,
		EVENT_EXPORT_TO_EU = 8,
		EVENT_EXPORT_NOTTO_EU = 16,
		EVENT_OTHER = 32
	};

protected:
	CZgloszenieBase(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator);
public:
	enum CFG_ITEM_INDEX
	{
		CFG_USE_CUSTOM_EVENTS = CPrintCfg::CFG_PRINT_OPTION_MAX,
		CFG_CUSTOM_EVENTS,
		CFG_DONT_FILL_DEST_HENTNO
	};

	virtual void PreparePage(int nPage);
	virtual int Load() ;
	virtual void FillEANs(int iStart,int iEnd,int iEANLblStart);
	virtual void FillOneEAN(const CString &strEAN,LPCSTR EANLblRowFmt);
	virtual void FillCowEANRecord(const CString &strEAN,int iRow,LPCSTR EANLblRowFmt);
	virtual int GetPrintPages() const = 0;
	virtual ~CZgloszenieBase();
	
protected:
	void FillBottomOfPage(NotificationRecord *pRecord);
	void FillHeaderOfPage();

	void PutChar(const CString& sLabel,char c);
	void PutEventType(UINT uiEvents);
	void PutEventDate(const CDateTime& eventDt);
	void PutPrintDate(const CDateTime& printDt);
	void PutEventCountry(Country* pEventCountry);

	virtual void ComputeIndexes() = 0;
	
	virtual int GetNumberOfRecordsPerSheet() = 0;
	virtual int LoadInputData(CPtrArray &inputElements);
	
	int m_nSheets;
	//BOOL m_bPutBuyerName;
	BOOL m_bUseCustomEvents;
	UINT m_CustomEvents;
	BOOL m_bDontFillDstHentNo;
	PtrFlushArray<NotificationRecord> m_records;
	
	CCowArray m_currCows;	
	NotificationRecord *m_pCurrentRecord;
	int m_currStartIdx;
	int m_currEndIdx;
	int m_currDocPage;	

	NotificationRecordConverter* m_pConverter;
	DisplacementNotificationNavigator* m_pNavigator;
};



class CZgloszenieOptDlg : public  CCustomPrintDocOptDlg 
{
	DECLARE_DYNCREATE(CZgloszenieOptDlg)
public:
	CZgloszenieOptDlg (CWnd *pParent = NULL);
	enum { IDD = IDD_ZGLOSZENIEOPTDLG };
	virtual UINT GetTemplateID() { return IDD; };
	virtual void Save();

protected:

	void EnableCustomEvents(BOOL bEnable);
	
	BOOL  m_bUseCustomEvents;
	BOOL  m_bEventType_Buy;
	BOOL  m_bEventType_Sell;
	BOOL  m_bEventType_Buy_And_Sell;
	BOOL  m_bEventType_Export_To_EU;
	BOOL  m_bEventType_Export_NotTo_EU;
	BOOL  m_bEventType_Other;
	BOOL m_bDontFillDestHentNo;

	CCheckableGroupBox m_CustomEventTypeGroup;
	    	
protected:			
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	afx_msg void OnCheckUseCustomEvents();
	
	DECLARE_MESSAGE_MAP()

};

#endif
