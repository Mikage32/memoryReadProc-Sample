#include "util.h"
#include "get_process.h"
#include "set_privilege.h"

#pragma comment(lib, "user32.lib")

using namespace std;

int main() {
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tokenPrivileges;
	BOOL bRet;

	//1.OpenProcessToken関数で、プロセストークンを取得する
	bRet = OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken);
	if (!bRet) {
		return -1;
	}

	SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);

	TCHAR name[] = L"smss.exe";

	HANDLE hProcess = get_process_hundle(name);
	if (hProcess == INVALID_HANDLE_VALUE) {
		cerr << "Invalid handle value" << "\n";
		return -1;
	}

	PVOID pBaseAddress = get_process_base_address(hProcess);
	if (pBaseAddress == NULL) {
		cerr << "geting process base address failed" << "\n";
		return -1;
	}

	cout << "process base address: " << pBaseAddress << "\n";

	// 整数値に変換
	uint64_t iBaseAddress = reinterpret_cast<uintptr_t>(pBaseAddress);

	// テスト読み込み
	int data = 0;
	SIZE_T bytesRead = 0;
	ReadProcessMemory(hProcess, (PVOID)(iBaseAddress + 0x0), &data, 4, &bytesRead);

	printf("data is %d\n", data);

	return 0;
}