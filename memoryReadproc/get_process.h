#pragma once

#include "util.h"

typedef NTSTATUS(NTAPI* NtQueryInformationProcessPtr)(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL);

typedef ULONG(NTAPI* RtlNtStatusToDosErrorPtr)(NTSTATUS Status);

struct PEB_INTERNAL {
	BYTE							InheritedAddressSpace;
	BYTE							ReadImageFileExecOptions;
	BYTE							BeingDebugged;
	BYTE							Spare;
	PVOID							Mutant;
	PVOID							ImageBaseAddress;
	PPEB_LDR_DATA					Ldr;
	PRTL_USER_PROCESS_PARAMETERS	ProcessParameters;
	BYTE							Reserved4[104];
	PVOID							Reserved5[52];
	PPS_POST_PROCESS_INIT_ROUTINE	PostProcessInitRoutine;
	BYTE							Reserved6[128];
	PVOID							Reserved7[1];
	ULONG							SessionId;
};

HANDLE get_process_hundle(TCHAR* target_exe_file);

PVOID get_process_base_address(HANDLE hProcess);