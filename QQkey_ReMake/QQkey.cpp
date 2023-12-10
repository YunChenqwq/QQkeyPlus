#include "qqkey.h"
BOOL QQKEY::AdjustPrivilege()
{
	HANDLE token;
	if ((OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &token)) == NULL)	//打开进程令牌
	{
		std::cout << GetLastError();
		std::cout << "打开进程令牌失败" << std::endl;
		return FALSE;
	}

	LUID luid;
	if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid) == NULL)		//查询权限
	{
		std::cout << GetLastError();
		std::cout << "权限查询失败" << std::endl;
		//OutputDebugString(L"权限查询失败");
		return FALSE;
	}

	TOKEN_PRIVILEGES tkp;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;		//固定操作,创建一个新特权，tkp即为新特权结构体;

	if (AdjustTokenPrivileges(token, FALSE, &tkp, sizeof(tkp), NULL, NULL) == NULL)			//特权调整
	{
		std::cout << GetLastError();
		std::cout << "调整自身权限失败" << std::endl;
		//OutputDebugString(L"调整自身权限失败");
		return FALSE;
	}

	return TRUE;
}
std::string QQKEY::exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
		result += buffer.data();
	}
	_pclose(pipe);
	return result;
}

BOOL QQKEY::IsProcessNameMatch(const std::string& processName, const std::string& matchName) {
	return processName.find(matchName) != std::string::npos;
}

std::string QQKEY::ProcessIdToName(DWORD processId)
{
	std::string ret;
	HANDLE handle = OpenProcess(
		PROCESS_QUERY_LIMITED_INFORMATION,
		FALSE,
		processId /* This is the PID, you can find one from windows task manager */
	);
	if (handle)
	{
		DWORD buffSize = 1024;
		CHAR buffer[1024];
		if (QueryFullProcessImageNameA(handle, 0, buffer, &buffSize))
		{
			ret = buffer;
		}
		else
		{
			std::cout << " GetModuleBaseNameA错误 代码:" << GetLastError()<<std::endl;
		}
		CloseHandle(handle);
	}
	else
	{
		std::cout << "打开进程错误 代码:" << GetLastError() << std::endl;
	}
	return ret;
}
