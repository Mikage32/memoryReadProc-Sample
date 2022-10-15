#include "get_process.h"


/* プロセスのベースアドレスを取得 */
PVOID get_process_base_address(HANDLE hProcess) {
	// ntdllのインスタンスを取得
	HINSTANCE hNtDll = GetModuleHandleW(L"ntdll.dll");
	if (hNtDll == NULL) {
		std::cerr << "Error: ntdll.dll is NULL" << "\n";
		return NULL;
	}

	// ntdllのモジュールを配置
	NtQueryInformationProcessPtr NtQueryInformationProcess = (NtQueryInformationProcessPtr)GetProcAddress(hNtDll, "NtQueryInformationProcess");
	RtlNtStatusToDosErrorPtr RtlNtStatusToDosError = (RtlNtStatusToDosErrorPtr)GetProcAddress(hNtDll, "RtlNtStatusToDosError");
	if (!NtQueryInformationProcess || !RtlNtStatusToDosError) {
		printf("Functions cannot be located.\n");
		return NULL;
	}

	// PROCESS_BASIC_INFORMATIONを取得
	PROCESS_BASIC_INFORMATION pbi;
	ULONG len;
	NTSTATUS status = NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &len);
	SetLastError(RtlNtStatusToDosError(status));
	if (NT_ERROR(status) || !pbi.PebBaseAddress) {
		printf("NtQueryInformationProcess(ProcessBasicInformation) failed.\n");
		return NULL;
	}

	// PBE(Process Environment Block)の読み込み
	SIZE_T bytesRead = 0;
	PEB_INTERNAL peb;
	if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), &bytesRead))
	{
		printf("Reading PEB failed.\n");
		return NULL;
	}

	return peb.ImageBaseAddress;
}