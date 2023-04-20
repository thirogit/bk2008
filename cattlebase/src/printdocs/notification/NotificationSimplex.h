#ifndef __NOTIFICATIONSIMPLEX_H__
#define __NOTIFICATIONSIMPLEX_H__

#include "Notification.h"

class CNotificationSimplex : public CZgloszenieBase
{
public:
	CNotificationSimplex(NotificationRecordConverter* pConverter,DisplacementNotificationNavigator* pNavigator);	
protected:
	void ComputeIndexes();
	void PrintPage(int nPage);	
	virtual int GetPrintPages() const;
};

#endif

