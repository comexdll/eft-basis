#pragma once
#include <Windows.h>
#include "functions.h"
#include "imguipp.h"
#include "icons.h"
#include "mmc.h"
#include <thread>

typedef struct Variables
{
	std::wstring sRandStringOne = L"runforrestrun";
	int MFTab = 0;
	int MMTab = 0;
	bool bVerified = false;
	std::string sMyLicenseKey;
	bool bShowingWindow = true;

	uintptr_t baseAddress;

	BOOL bProcessIsOpen = false;
	BOOL bErrorOpeningProcess = false;

	HANDLE hProcess = 0;
	uintptr_t moduleBase = 0;
}myvars;

typedef struct GlobalStructs {
	myvars vars;
}gstructs;

gstructs* gs = new gstructs;

namespace Menu
{
	void ChooseForm();
	void Theme();
	void ShowMainForm();
	void fShowMainMenu();
	void fShowOptions();
}