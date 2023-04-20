#ifndef __KEYMANAGER_H__
#define __KEYMANAGER_H__

#include "WinTimerThread.h"
#include <afxmt.h>
#include "../../../rockey_common/RockeyCommon.h"
#include "../VersionNo.h"
#include "RockeyWrapper.h"

class KeyManager : public CWinTimerThread
{
private:
	typedef struct tagKEY_CONTENT
	{
		ROCKEY_BKCLIENTINFO clientInfo;
		time_t	keyExpiryDt;
		time_t	keyClock;
		WORD	wRequiredVersion;
	} KEY_CONTENT,*PKEY_CONTENT;
public:
	enum KEY_STATE
	{
		NONE,
		NOT_FOUND,
		EXPIRED,
		REQUIRES_UPDATE,
		VALID,
		ABSENT_TOO_LONG,
		INVALID_CLOCK
	};

public:
    DECLARE_DYNCREATE(KeyManager)

    KeyManager();
    virtual ~KeyManager();

	void Shutdown();

	KEY_STATE GetKeyState();
	time_t GetAbsentLimitSecs() const;
	time_t GetAbsentTimeSecs() const;
	bool IsFeatureEnabled(UINT uFeature) const;
	time_t GetKeyClock() const;
	time_t GetKeyExpiryDt() const;
	UINT GetKeyRequiredVersion() const;

	//void UpdateKey();

    virtual BOOL InitInstance();
    virtual int ExitInstance();
private:
	CCriticalSection m_Working;
	CSingleLock m_Lock;
	UINT_PTR	m_TickTimerId;
	KEY_STATE	m_KeyState;
	time_t		m_KeyAbsenceTimeSecs;
	time_t		m_KeyExpiryDt;
	UINT		m_RequiredVersion;
	time_t		m_KeyLastSeenTime;
	WORD		m_wFeatures;
	time_t		m_KeyClock;



protected:
	bool ReadKey(PKEY_CONTENT pKeyContent,RockeyWrapper &key);
	bool TimerTick();
	void SettleCompany(const PROCKEY_BKCLIENTINFO companyInfo);
	//void SettleState(time_t currentTime);
	bool AdjustClock(time_t currentTime,RockeyWrapper &key);
    void OnTimer(WPARAM idEvent, LPARAM dwTime);
	void Lock();
	void Unlock();
	
    DECLARE_MESSAGE_MAP()
};

#endif 
