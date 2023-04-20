#ifndef __NOTIFICATIONDUPLEX_H__
#define __NOTIFICATIONDUPLEX_H__

#include "Notification.h"

class CNotificationDuplex : public CZgloszenieBase
{
public:
	CNotificationDuplex(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator);
	
protected:
	virtual int Load();
	void ComputeIndexes();
	void PrintPage(int nPage);
	void PreparePage(int nPage);
	virtual int GetPrintPages() const;
	virtual int GetNumberOfRecordsOnFrontPage() = 0;
	virtual int GetNumberOfRecordsOnBackPage();
};
#endif
