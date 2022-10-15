#include "get_process.h"

/* �^�[�Q�b�g�v���Z�X�̃n���h�����擾 */
HANDLE get_process_hundle(TCHAR* target_exe_file) {
	// �N�����Ă���S�v���Z�X�̏����擾
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		printf("Error: hSnapshot is INVALID_HANDLE_VALUE\n");
		return INVALID_HANDLE_VALUE;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	// �v���Z�X�̎擾 (����̂�Process32First)
	if (!Process32First(hSnapshot, &pe)) {
		CloseHandle(hSnapshot);
		printf("Error: couldn't get entry by Process32First\n");
		return INVALID_HANDLE_VALUE;
	}
	do {
		// ���s�t�@�C�����Ŕ�r
		if (_tcscmp(pe.szExeFile, target_exe_file) == 0) {
			// �Ώۂ̃v���Z�X�𔭌�
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

	// �n���h�������
	CloseHandle(hSnapshot);

	return INVALID_HANDLE_VALUE;
}