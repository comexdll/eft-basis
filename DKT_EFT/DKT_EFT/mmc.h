#pragma once
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <iostream>

class MyMem
{
public:

struct module {
	DWORD Base, Size;
};

static DWORD GetProcId(const wchar_t* procName);

static uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

static uintptr_t FindMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int>);

static HANDLE GetProcessHandle(int procId);

static module GetModule(const wchar_t* modName);

template <typename var>
static bool WriteMemory(DWORD address, var Value);

template <typename var>
static bool ReadMemory(DWORD address);

static bool MemoryCompare(const byte* data, const byte* mask, const char* szMask);

static DWORD FindPattern(DWORD SigStart, DWORD SigSize, const char* Signature, const char* Mask);

static void FindAllAddressesContainingPattern(DWORD SigStart, DWORD SigSize, const char* Signature, const char* Mask, unsigned long addressList[100]);

};

