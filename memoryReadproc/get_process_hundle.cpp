#include "get_process.h"

/* ターゲットプロセスのハンドルを取得 */
HANDLE get_process_hundle(TCHAR* target_exe_file) {
	// 起動している全プロセスの情報を取得
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		printf("Error: hSnapshot is INVALID_HANDLE_VALUE\n");
		return INVALID_HANDLE_VALUE;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	// プロセスの取得 (初回のみProcess32First)
	if (!Process32First(hSnapshot, &pe)) {
		CloseHandle(hSnapshot);
		printf("Error: couldn't get entry by Process32First\n");
		return INVALID_HANDLE_VALUE;
	}
	do {
		// 実行ファイル名で比較
		if (_tcscmp(pe.szExeFile, target_exe_file) == 0) {
			// 対象のプロセスを発見
			std::cout << "the process exsists" << "\n";
			CloseHandle(hSnapshot);
			HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, pe.th32ProcessID);
			if (hProcess) {
				return hProcess;
			}
			else {
				std::cerr << GetLastError() << "\n";
				return INVALID_HANDLE_VALUE;
			}
		}

	} while (Process32Next(hSnapshot, &pe));

	// ハンドルを閉じる
	CloseHandle(hSnapshot);

	return INVALID_HANDLE_VALUE;
}