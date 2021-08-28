#pragma once

#define GET_PROCESS_ID(pProcessInfo) *(UINT*)((UINT64)pProcessInfo + ProcessHelper::OffsetUniqueProcessId)
#define GET_PROCESS_LINKS(pProcessInfo) *(UINT*)((UINT64)pProcessInfo + ProcessHelper::OffsetActiveProcessLinks)
#define GET_PROCESS_NAME(pProcessInfo) (CHAR*)((UINT64)pProcessInfo + ProcessHelper::OffsetImageFileName)
#define GET_PROCESS_THREADS(pProcessInfo) *(UINT*)((UINT64)pProcessInfo + ProcessHelper::OffsetActiveThreads)

namespace ProcessHelper
{
	extern INL NTSTATUS Init();
	extern NTSTATUS GetInfo(IN const CHAR* szProcessName, OUT PEPROCESS* pProcessInfo);
	extern UINT GetID(IN const CHAR* szProcessName);

	extern UINT OffsetUniqueProcessId;
	extern UINT OffsetActiveProcessLinks;
	extern UINT OffsetImageFileName;
	extern UINT OffsetActiveThreads;
}