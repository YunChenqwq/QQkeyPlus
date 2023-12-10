#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <array>
#define STRING_SIZE 255	
#define MAX_PROCESS	24				//最大获取QQ数量24
#define PUBLIC_BUFFER_SIZE 270		//共享内存缓冲区大小
#define BUF_SIZE 4096               //QQ结构体最大长度
struct QQinfo
{
	std::string pid;
	std::string port;
	std::string pt_locak_token;
	std::string account;
	std::string sex;
	std::string name;
	std::string qqkey;
	std::string skey;
	std::string p_skey;
};

class QQKEY
{
public:
	std::string exec(const char* cmd);//管道回显
	BOOL IsProcessNameMatch(const std::string& processName, const std::string& matchName);//是否为QQ进程
	std::string ProcessIdToName(DWORD processId);//PID取路径    
	BOOL AdjustPrivilege();	//提权

private:
	//变量区
	TCHAR pName[STRING_SIZE];		//目标进程名
	DWORD target_pid[MAX_PROCESS];	//目标进程pid
};