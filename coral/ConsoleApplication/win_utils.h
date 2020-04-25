#pragma once
#pragma comment(lib, "shlwapi.lib")

#include <shlwapi.h>
#include <windows.h>

#include <iostream>

namespace coral {

	std::string GetWorkingDir();
};