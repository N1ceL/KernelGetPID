#include "Includes.h"

namespace ProcessHelper
{
	UINT OffsetUniqueProcessId = 0x0;
	UINT OffsetActiveProcessLinks = 0x0;
	UINT OffsetImageFileName = 0x0;
	UINT OffsetActiveThreads = 0x0;
}


//-----------------------------------------------------------------------------
// Init
//-----------------------------------------------------------------------------

INL NTSTATUS ProcessHelper::Init()
{
	PEPROCESS SystemProcess = PsInitialSystemProcess;

	for (int i = 0; i < 0xFFF; i++) // 0xFFF larger than the size of full struct
	{
		if (!OffsetUniqueProcessId && !OffsetActiveProcessLinks)
		{
			if (*(UINT64*)((UINT64)SystemProcess + i) == 4 && // 4 always, pid of system process
				*(UINT64*)((UINT64)SystemProcess + i + 0x8) > 0xFFFF000000000000)  // > 0xFFFF000000000000 always
			{
				OffsetUniqueProcessId = i;
				OffsetActiveProcessLinks = i + 0x8;
			}
		}
		if (!OffsetImageFileName && !OffsetActiveThreads)
		{
			if (*(UINT64*)((UINT64)SystemProcess + i) > 0x0000400000000000 && *(UINT64*)((UINT64)SystemProcess + i) < 0x0000800000000000 && // 0x00006D6574737953 always, but better to make range
				*(UINT64*)((UINT64)SystemProcess + i + 0x48) > 0 && *(UINT64*)((UINT64)SystemProcess + i + 0x48) < 0xFFF) // 80 ~ 300 in general
			{
				OffsetImageFileName = i;
				OffsetActiveThreads = i + 0x48;
			}
		}

		if (OffsetUniqueProcessId && OffsetActiveProcessLinks && OffsetImageFileName && OffsetActiveThreads)
			return STATUS_SUCCESS;
	}

	return STATUS_NOT_FOUND;
}


/-----------------------------------------------------------------------------
// Get
//-----------------------------------------------------------------------------

NTSTATUS ProcessHelper::GetInfo(IN const CHAR* szProcessName, OUT PEPROCESS* pProcessInfo)
{
	PEPROCESS SystemProcess = PsInitialSystemProcess;
	PEPROCESS CurrentProcess = SystemProcess;

	do
	{
		if (strstr((CHAR*)((UINT64)CurrentProcess + OffsetImageFileName), szProcessName))
		{
			if (*(UINT*)((UINT64)CurrentProcess + OffsetActiveThreads))
			{
				*pProcessInfo = CurrentProcess;
				return STATUS_SUCCESS;
			}
		}

		PLIST_ENTRY List = (PLIST_ENTRY)((UINT64)(CurrentProcess)+OffsetActiveProcessLinks);
		CurrentProcess = (PEPROCESS)((UINT64)List->Flink - OffsetActiveProcessLinks);

	} while (CurrentProcess != SystemProcess);

	return STATUS_NOT_FOUND;
}

UINT ProcessHelper::GetID(IN const CHAR* szProcessName)
{
	PEPROCESS SystemProcess = PsInitialSystemProcess;
	PEPROCESS CurrentProcess = SystemProcess;

	do
	{
		if (strstr((CHAR*)((UINT64)CurrentProcess + OffsetImageFileName), szProcessName))
		{
			if (*(UINT*)((UINT64)CurrentProcess + OffsetActiveThreads))
			{
				return *(UINT*)((UINT64)CurrentProcess + OffsetUniqueProcessId);
			}
		}

		PLIST_ENTRY List = (PLIST_ENTRY)((UINT64)(CurrentProcess)+OffsetActiveProcessLinks);
		CurrentProcess = (PEPROCESS)((UINT64)List->Flink - OffsetActiveProcessLinks);

	} while (CurrentProcess != SystemProcess);

	return 0;
}
