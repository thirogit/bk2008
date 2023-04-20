#ifndef __CATTLEBASE_LOG_H__
#define __CATTLEBASE_LOG_H__

#ifdef DO_LOGGING
#include "log\AmLog.h"
#else
#define AMLOGINFO __noop
#endif

#define LOG AMLOGINFO

#endif
