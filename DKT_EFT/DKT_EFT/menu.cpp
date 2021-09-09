#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "cryptopp-static.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.Lib")
#pragma comment (lib, "Crypt32.Lib")

#include "api/KeyAuth.hpp"
#include "settings.h"

using namespace KeyAuth;
using namespace std;

#pragma region Variable Declarations
char uLicenseKeyReg[128] = "";

bool bKAAInitComplete = false;
#pragma endregion

void Menu::ChooseForm() {

	ShowMainForm();	
}

void Menu::ShowMainForm()
{
	
	static ImVec4 active = imguipp::to_vec4(41, 40, 41, 255);
	static ImVec4 inactive = imguipp::to_vec4(31, 30, 31, 255);
	
	ImGui::Columns(2);
	ImGui::SetColumnWidth(-1, 200);

	ImGui::PushItemWidth(200);
	ImGui::PushStyleColor(ImGuiCol_Button, gs->vars.MMTab == 1 ? active : inactive);
	if (ImGui::Button(" Main Menu", ImVec2(ImGui::GetWindowWidth() / 6 * 2 - 15, 41)))
		gs->vars.MMTab = 1;
	ImGui::PushItemWidth(200);
	ImGui::PushStyleColor(ImGuiCol_Button, gs->vars.MMTab == 2 ? active : inactive);
	if (ImGui::Button(" Options", ImVec2(ImGui::GetWindowWidth() / 6 * 2 - 15, 41)))
		gs->vars.MMTab = 2;
	ImGui::PushItemWidth(200);
	ImGui::PushStyleColor(ImGuiCol_Button, gs->vars.MMTab == 3 ? active : inactive);
	if (ImGui::Button(" Exit", ImVec2(ImGui::GetWindowWidth() / 6 * 2 - 15, 41)))
		gs->vars.MMTab = 3;

	ImGui::PopStyleColor(3);
	
	ImGui::NextColumn();
	{
		if (gs->vars.MMTab == 1 || gs->vars.MMTab == 0)
			Menu::fShowMainMenu();
		else if (gs->vars.MMTab == 2)
			Menu::fShowOptions();
		else if (gs->vars.MMTab == 3)
			exit(0);
	}
	
	ImGui::EndColumns();
}

void Menu::Theme()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowBorderSize = 5;
	style->WindowTitleAlign = ImVec2(0.5, 0.5);
	style->WindowMinSize = ImVec2(600, 300);

	style->FramePadding = ImVec2(8, 6);

	style->Colors[ImGuiCol_TitleBg] = ImColor(0, 0, 0, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(154, 3, 30, 255);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(154, 3, 30, 130);

	style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(154, 3, 30, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(154, 3, 30, 255);

	style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
	style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

	style->Colors[ImGuiCol_FrameBg] = ImColor(37, 36, 37, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(37, 36, 37, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(37, 36, 37, 255);

	style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(30, 39, 97, 255);

	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}

void Menu::fShowMainMenu() {
	static ImVec4 active = imguipp::to_vec4(41, 40, 41, 255);
	static ImVec4 inactive = imguipp::to_vec4(31, 30, 31, 255);

	ImGui::Text("EFT Main Menu:", 1, false);


	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

	ImGui::PushStyleColor(ImGuiCol_Button, gs->vars.MFTab == 1 ? active : inactive);
	if (ImGui::Button(" Get Base Address", ImVec2(200, 41)))
		gs->vars.MFTab = 1;

	ImGui::PopStyleColor(1);

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

	if (gs->vars.bProcessIsOpen) {
		ImGui::Text("Process Opened! Base Address is: %u\n", &gs->vars.baseAddress, ImGui::GetWindowWidth() - 15, 1, false);
	}
	else if (gs->vars.bErrorOpeningProcess) {
		ImGui::Text("Error opening process - game not running?", ImGui::GetWindowWidth() - 15, 1, false);
	}

	{
		if (gs->vars.MFTab == 1) {
			if (!gs->vars.bProcessIsOpen) {
				int iProcId = MyMem::GetProcId(L"EscapeFromTarkov.exe");
				if (!iProcId > 0) {
					gs->vars.bErrorOpeningProcess = true;
					return;
				}
				gs->vars.hProcess = MyMem::GetProcessHandle(iProcId);
				gs->vars.baseAddress = MyMem::GetModuleBaseAddress(iProcId, L"EscapeFromTarkov.exe");
				if (gs->vars.baseAddress == 0) {
					gs->vars.bProcessIsOpen = false;
					gs->vars.bErrorOpeningProcess = true;
				}
				else if (gs->vars.baseAddress > 0) {
					gs->vars.bProcessIsOpen = true;
					gs->vars.bErrorOpeningProcess = false;
				}
			}
			else if (gs->vars.bProcessIsOpen) {
				SIZE_T* iBytesWritten = 0;	
			}
		}

		if (gs->vars.MFTab == 2) {
			ExitProcess(NULL);
		}
	}
}

void Menu::fShowOptions() {
	static ImVec4 active = imguipp::to_vec4(41, 40, 41, 255);
	static ImVec4 inactive = imguipp::to_vec4(31, 30, 31, 255);

	
	imguipp::center_text("Options:", 1, false);

	ImGui::Text("No options at the moment..");
}
