#ifndef __ROCKEY_H__
#define __ROCKEY_H__

#include "../../../rockey_common/RockeyCommon.h"
#include <time.h>

#define INVALID_RY_HANDLE ((WORD)-1)
#define INVALID_TIME ((time_t)(-1))

class RockeyWrapper
{
public:
	RockeyWrapper();
	bool Open(WORD wBasicPwd1,WORD wBasicPwd2);
	bool Close();
	bool AdjustClock(time_t t);
	bool IsOpen() const;

	DWORD GetKeySerialNo() const;

	bool Read(BYTE* pOutBuffer,UINT uMaxSize,UINT uOffset);
	bool Write(const BYTE* pBuffer,UINT uSize,UINT uOffset);


private:
	void ConvertTime(SYSTEMTIME* pOutSystemTime, const struct tm* t);


	WORD m_wHandle;
	DWORD m_dwKeySerialNo;
};

#endif