#include "mmc.h"

MyMem::module TargetModule;
HANDLE TargetProcess;
DWORD TargetId;

DWORD MyMem::GetProcId(const wchar_t* procName) {
	DWORD procId = 0;
	HANDLE hSnap = (CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry)) {
			do {
				if (!_wcsicmp(procEntry.szExeFile, procName)) {
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t MyMem::GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
	uintptr_t modeBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					modeBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modeBaseAddr;
}

uintptr_t MyMem::FindMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;
	for (unsigned i = 0; i < offsets.size(); i++) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, 4, 0);
		addr += offsets[i];
	}
	return addr;
}

HANDLE MyMem::GetProcessHandle(int procId) {
	TargetProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
	return TargetProcess;
}

MyMem::module MyMem::GetModule(const wchar_t* modName) {
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, TargetId);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					CloseHandle(hSnap);
					TargetModule = { (DWORD)modEntry.hModule, (DWORD)modEntry.modBaseSize };
					break;
				}
			} while (Module32Next(hSnap, &modEntry));

		}
		return { (DWORD)false, (DWORD)false };
	}
}

template <typename var>
bool MyMem::WriteMemory(DWORD Address, var Value) {
	return WriteProcessMemory(TargetProcess, (LPVOID)Address, &Value, sizeof(var), 0);
}

template <typename var>
bool MyMem::ReadMemory(DWORD Address) {
	var Value;
	return ReadProcessMemory(TargetProcess, (LPVOID)Address, &Value, sizeof(var), 0);
}

bool MyMem::MemoryCompare(const byte* data, const byte* mask, const char* szMask) {
	for (; *szMask; ++szMask, ++data, ++mask) {
		if (*szMask == 'X' && *data != *mask) {
			return false;
		}
	}
}

DWORD MyMem::FindPattern(DWORD SigStart, DWORD SigSize, const char* Signature, const char* Mask) {
	byte* data = new byte[SigSize];
	SIZE_T BytesRead;
	ReadProcessMemory(TargetProcess, (LPVOID)SigStart, data, SigSize, &BytesRead);
	for (DWORD i = 0; i < SigSize; i++) {
		if (MemoryCompare((const byte*)(data + i), (const byte*)Signature, Mask)) {
			return SigStart + i;
		}
		delete[] data;
	}
}

void MyMem::FindAllAddressesContainingPattern(DWORD SigStart, DWORD SigSize, const char* Signature, const char* Mask, unsigned long addressList[100]) {
	byte* data = new byte[SigSize];
	SIZE_T BytesRead;
	int iAddressListIndex = 0;
	// Read all memory from the module at once and store it in data, which was declared above.
	ReadProcessMemory(TargetProcess, (LPVOID)SigStart, data, SigSize, &BytesRead);
	for (DWORD i = 0; i < SigSize; i++) {
		if (MemoryCompare((const byte*)(data + i), (const byte*)Signature, Mask)) {
			addressList[i] = SigStart + i;
			iAddressListIndex++;
			return;
		}
		delete[] data;
	}
}
