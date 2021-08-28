#include "Includes.h"

NTSTATUS INL Init()
{
	NTSTATUS Status = STATUS_SUCCESS;

	Status = ProcessHelper::Init(); // Before using need to parse offsets
	if (Status != STATUS_SUCCESS)
		return Status;

	return Status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	NTSTATUS Status = STATUS_SUCCESS;

	Status = Init();
	if (Status != STATUS_SUCCESS)
		return Status;


	// Method 1
	UINT ID1 = ProcessHelper::GetID("explorer.exe"); // returns 0 if not found
	DBG_LOG("explorer.exe PID (method 1) = %p\n", ID1); // Log


	// Method 2
	PEPROCESS ProcessInfo;
	if (NT_SUCCESS(ProcessHelper::GetInfo("explorer.exe", &ProcessInfo))) // returns STATUS_NOT_FOUND if not found
	{
		UINT ID2 = GET_PROCESS_ID(ProcessInfo);
		DBG_LOG("explorer.exe PID (method 2) = %p\n", ID2); // Log
	}
	// WARNING, if status is not STATUS_SUCCESS never try to use GET_PROCESS_ID with it, u will get BSOD


	return Status;
}
