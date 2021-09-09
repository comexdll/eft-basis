#pragma once
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include "menu.h"
#include "font.h"
#include "iconcpp.h"

#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

using namespace std;

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);