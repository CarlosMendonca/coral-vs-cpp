#include "win_utils.h"

namespace coral{

	std::string GetWorkingDir()
	{
		char path[MAX_PATH] = "";
		GetModuleFileNameA(NULL, path, MAX_PATH);
		PathRemoveFileSpecA(path);
		PathAddBackslashA(path);
		return path;
	}

}