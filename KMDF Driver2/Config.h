#pragma once

#define ENABLE_DBG_LOGS
#define ENABLE_FORCEINLINE

//-----------------------------------------------------------------------------

#ifdef ENABLE_DBG_LOGS
	#define DBG_LOG(...) DbgPrintEx(DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, __VA_ARGS__)
#else
	#define DBG_LOG(...) (__VA_ARGS__)
#endif 

#ifdef ENABLE_FORCEINLINE
	#define INL __forceinline
#else
	#define INL
#endif
